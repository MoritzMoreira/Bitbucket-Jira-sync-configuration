#include "Tables.h"

Field::Field( const std::vector<std::string>& field ) :
    id( std::stoi( field.front() ) ),
    system( field.at( 1 ) ),
    path( field.back() )
{}

FieldMap::FieldMap( const std::vector<std::string>& fieldMap ) :
    id( std::stoi( fieldMap.front() ) ),
    fieldIDfrom( std::stoi( fieldMap.at( 1 ) ) ),
    fieldIDto( std::stoi( fieldMap.at( 2 ) ) )
{}

FOpt::FOpt( std::vector<std::string> fOpt ) :
    id( std::stoi( fOpt.front() ) ),
    fieldID( std::stoi( fOpt.at( 1 ) ) )
{
    if ( fOpt.back().find( ':' ) != std::string::npos )
    {
        std::size_t positionOfColon = fOpt.back().find( ':' );
        fOpt.back().insert( fOpt.back().begin() + positionOfColon, ' ' );
        fOpt.back().insert( fOpt.back().begin() + positionOfColon + 2, ' ' );
    }
    opt = fOpt.back();
}

FOpt::FOpt() :
    id( 0 ),
    fieldID( 0 ),
    opt( "" )
{}

FOptMap::FOptMap( const std::vector<std::string>& fOptMap ) :
    id( std::stoi( fOptMap.front() ) ),
    fOptIDfrom( std::stoi( fOptMap.at( 1 ) ) ),
    fOptIDto( std::stoi( fOptMap.at( 2 ) ) )
{
    if ( fOptMap.size() == 4 )
        fOptIDcondition = std::stoi( fOptMap.at( 3 ) );
    else
        fOptIDcondition = 0;
}

FOptAdditional::FOptAdditional( const std::vector<std::string>& fOptAdditional ) :
    id( std::stoi( fOptAdditional.front() ) ),
    fOptMapID( std::stoi( fOptAdditional.at( 1 ) ) ),
    fOptIDadd( std::stoi( fOptAdditional.at( 2 ) ) ),
    onlySetWhenNotPresent( std::stoi( fOptAdditional.at( 3 ) ) ),
    onlySetOnCreate( std::stoi( fOptAdditional.at( 4 ) ) )
{}

TrackerConn::TrackerConn( const std::vector<std::string>& trackerConn ) :
    id( std::stoi( trackerConn.front() ) ),
    jiProj( trackerConn.at( 1 ) ),
    jiEpic( trackerConn.at( 2 ) ),
    bbRepo( trackerConn.at( 3 ) ),
    commentsToJira( std::stoi( trackerConn.at( 4 ) ) ),
    commentsToBitbucket( std::stoi( trackerConn.at( 5 ) ) ),
    newIssuesToJira( std::stoi( trackerConn.at( 6 ) ) ),
    newIssuesToBitbucket( std::stoi( trackerConn.at( 7 ) ) ),
    destructiveOnJira( std::stoi( trackerConn.at( 8 ) ) ),
    destructiveOnBitbucket( std::stoi( trackerConn.at( 9 ) ) ),
    jiraLogIssueID( std::stoi( trackerConn.at( 10 ) ) )
{}

TrackerConnFieldMap::TrackerConnFieldMap( const std::vector<std::string>& trackerConnFieldMap ) :
    trackerConnID( std::stoi( trackerConnFieldMap.front() ) ),
    fieldMapID( std::stoi( trackerConnFieldMap.at( 1 ) ) )
{}

TrackerConnFOptMap::TrackerConnFOptMap( const std::vector<std::string>& trackerConnFOptMap ) :
    trackerConnID( std::stoi( trackerConnFOptMap.front() ) ),
    fOptMapID( std::stoi( trackerConnFOptMap.at( 1 ) ) )
{}

TrackerConnFOptAdd::TrackerConnFOptAdd( const std::vector<std::string>& trackerConnFOptAdd ) :
    trackerConnID( std::stoi( trackerConnFOptAdd.front() ) ),
    fOptAddID( std::stoi( trackerConnFOptAdd.at( 1 ) ) )
{}

