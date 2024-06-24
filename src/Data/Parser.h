#pragma once

#include <iostream>
#include <vector>

struct Tables;

class Parser
{
public:
    Parser( std::string path );

    const Tables ParseTables() const;
    void DeleteSpaces( std::string& string ) const;
    const std::vector<std::string> SplitDbLinesOrEntries( std::string& s, std::string& searched ) const;
    const std::string& RemoveOneLineComments( std::string& string ) const;
    const Tables& GenerateTableObjects( const std::vector<std::string>& tableEntry, const std::string&, Tables& tables ) const;

private:
    std::string m_path;
};
