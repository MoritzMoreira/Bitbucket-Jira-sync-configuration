#include <fstream>
#include <regex>

#include "Parser.h"
#include "tables.h"
#include "treemodel.h"

Parser::Parser( std::string path ) :
    m_path( path )
{}

const Tables Parser::ParseTables() const
{
    std::ifstream stream( m_path );
    constexpr std::size_t readSize = 4096;
    std::string outString;
    auto buf = std::string( readSize, '\0' );
    while ( stream.read( &buf[ 0 ], readSize ) )
        outString.append( buf, 0, stream.gcount() );
    outString.append( buf, 0, stream.gcount() );

    std::string tableContent, tableName;
    std::vector<std::string> unsplitTabEntries;
    Tables tables;
    outString = RemoveOneLineComments( outString );
    std::size_t positionOfExpConst = outString.find( "export const" );
    while ( positionOfExpConst != std::string::npos )
    {
  // table name
        for ( size_t i = positionOfExpConst + 13; outString[ i ] != ' '; i++ )
            tableName += outString[ i ];
  // table content
        size_t beginVarPos = positionOfExpConst + 22 + tableName.size(), endOfContent( 0 );
        std::regex r(
            "\\][\\,]*([ ]*(\\n)[ ]*)*\\]" );  // A closing square bracket followed by n commas,
                                              // followed by n line breaks, surrounded by n spaces (can be "]" or "],")
        std::smatch sm;
        std::string outStringSub = outString.substr( beginVarPos,
                                                     outString.size() - beginVarPos ); // get substring from begin of
                                                                                       // content to end of entire file
        regex_search( outStringSub, sm, r );
        endOfContent = sm.position();

        tableContent = outStringSub.substr( 0, endOfContent + 1 );
        // split table content into arrays
        std::string brackCom = "],";
        unsplitTabEntries = SplitDbLinesOrEntries( tableContent, brackCom );

        for ( std::string unsplitTabEntry : unsplitTabEntries )
        {
            auto IsQuote = []( char c )
            {
                return c == '\"' || c == '[' || c == ']' || c == '{' || c == '}';
            };
            unsplitTabEntry.erase( remove_if( unsplitTabEntry.begin(), unsplitTabEntry.end(), IsQuote ),
                                   unsplitTabEntry.end() );
            std::string comma = ",";
            std::vector<std::string> splitTableEntry = SplitDbLinesOrEntries( unsplitTabEntry, comma );
            tables = GenerateTableObjects( splitTableEntry, tableName, tables );
        }
        positionOfExpConst = outString.find( "export const", positionOfExpConst + 1 );
        tableContent = tableName = "";
    }
    return tables;
}

void Parser::DeleteSpaces( std::string& string ) const
{
    // Delete spaces, line breaks and tabs at beginning and end of string, keep spaces sourrounded by letters (for
    // example in [Not Required]).
    std::smatch m;
    while ( true )
    {
        std::regex expForBegin( "\\s[^a-zA-Z]|\\t[.]|\\r\\n[.]" ); // A space followed by any character except letters
                                                                   // OR a tab OR a newline followed by any character
        std::regex expForEnd( "[^a-zA-Z]\\s|[.]\\t|[.]\\r\\n" ); // the same expression inverted / swapped
        std::string firstTwoChars = string.substr( 0, 2 );
        std::string lastTwoChars = string.substr( string.size() - 2, 2 );
        if ( !( std::regex_match( firstTwoChars, m, expForBegin ) ) &&
             !( std::regex_match( lastTwoChars, m, expForEnd ) ) )
            break; // continue if first or last character is not a space, new line or tab not followed or preceded by a
                   // letter
        if ( std::regex_match( firstTwoChars, m, expForBegin ) )
            string.erase( 0, 1 );
        if ( std::regex_match( lastTwoChars, m, expForEnd ) )
            string.erase( string.size() - 1, 1 );
    }
    // delete spaces, line breaks and tabs inside the string
    std::regex expForInside(
        "([^\\w]\\s[^\\w])|[.]\\t[.]|[.]\\r\\n[.]|\\s*" ); // A space sourrounded by non-letter characters OR a tab OR a
                                                           // newline sourrounded by any two characters
    for ( int i = 1; i < string.size() - 1; i++ )
    {
        std::string threeChars = string.substr( i - 1, 3 );
        if ( std::regex_match( threeChars, m, expForInside ) )
        {
            string.erase( i, 1 );
            i--;
        }
    }
}

const std::vector<std::string> Parser::SplitDbLinesOrEntries( std::string input, const std::string& delimiter ) const
{
    DeleteSpaces( input );
    std::vector<std::string> result;
    const int splitDBlines = 1;
    const int splitSingleEntries = 0;

    size_t beginAt = delimiter == "]," ? splitDBlines : splitSingleEntries;
    size_t beginning = beginAt;
    std::size_t foundComma = input.find( delimiter );

    while ( 1 )
    {
        result.push_back( input.substr( beginning, foundComma - beginning ) );
        beginning = foundComma + ( beginAt ? 3 : 1 );
        foundComma = input.find( delimiter, foundComma + 1 );
        if ( foundComma == std::string::npos )
        {
            if ( beginning >= input.size() )
                break;
            result.push_back( input.substr( beginning, input.size() - beginning - beginAt ) );
            break;
        }
    }
    return result;
}

const std::string& Parser::RemoveOneLineComments( std::string& string ) const
{
    std::size_t positionOfDashes = string.find( "//" );
    std::size_t positionOfNewLine;
    while ( positionOfDashes != std::string::npos )
    {
        positionOfNewLine = string.find( '\n', positionOfDashes );
        ;
        if ( positionOfNewLine == std::string::npos )
            positionOfNewLine = string.size(); // ToDo: size() really correct or rather -1?
        string.erase( positionOfDashes, positionOfNewLine - positionOfDashes );
        positionOfDashes = string.find( "//", positionOfDashes + 1 );
    }
    return string;
}

const Tables& Parser::GenerateTableObjects( const std::vector<std::string>& tableEntry, const std::string& tableName, Tables& tables ) const
{
    if ( tableName == "fields" )
        tables.fields.emplace_back( Field( tableEntry ) );
    else if ( tableName == "field_maps" )
        tables.fieldMaps.emplace_back( FieldMap( tableEntry ) );
    else if ( tableName == "fopts" )
        tables.fOpts.emplace_back( FOpt( tableEntry ) );
    else if ( tableName == "fopt_maps" )
        tables.fOptMaps.emplace_back( FOptMap( tableEntry ) );
    else if ( tableName == "fopt_additionals" )
        tables.fOptAdditionals.emplace_back( FOptAdditional( tableEntry ) );
    else if ( tableName == "tracker_conns" )
        tables.trackerConns.emplace_back( TrackerConn( tableEntry ) );
    else if ( tableName == "tracker_conns_field_maps" )
        tables.trackerConnFieldMaps.emplace_back( TrackerConnFieldMap( tableEntry ) );
    else if ( tableName == "tracker_conns_fopt_maps" )
        tables.trackerConnFOptMaps.emplace_back( TrackerConnFOptMap( tableEntry ) );
    else if ( tableName == "tracker_conns_fopt_adds" )
        tables.trackerConnFOptAdds.emplace_back( TrackerConnFOptAdd( tableEntry ) );
    return tables;
}

