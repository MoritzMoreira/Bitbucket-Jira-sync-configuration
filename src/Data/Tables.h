#pragma once

#include <iostream>
#include <string>
#include <vector>

struct Field
{
    Field( const std::vector<std::string>& field = { "0", "", "" } );

    unsigned int id;
    std::string system;
    std::string path;
};

struct FieldMap
{
    FieldMap( const std::vector<std::string>& fieldMap = { "0", "0", "0" } );
    unsigned int id;
    unsigned int fieldIDfrom;
    unsigned int fieldIDto;
};

struct FOpt
{
    FOpt( std::vector<std::string> fOpt );
    FOpt();

    unsigned int id;
    unsigned int fieldID;
    std::string opt;
};

struct FOptMap
{
    FOptMap( const std::vector<std::string>& fOptMap );

    unsigned int id;
    unsigned int fOptIDfrom;
    unsigned int fOptIDto;
    unsigned int fOptIDcondition;
};

struct FOptAdditional
{
    FOptAdditional( const std::vector<std::string>& fOptAdditional );
    unsigned int id;
    unsigned int fOptMapID;
    unsigned int fOptIDadd;
    ;
    bool onlySetWhenNotPresent;
    bool onlySetOnCreate;
};

struct TrackerConn
{
    TrackerConn( const std::vector<std::string>& trackerConn = { "0", "", "", "", "0", "0", "0", "0", "0", "0", "0" } );
    unsigned int id;
    std::string jiProj;
    std::string jiEpic;
    std::string bbRepo;
    bool commentsToJira;
    bool commentsToBitbucket;
    bool newIssuesToJira;
    bool newIssuesToBitbucket;
    bool destructiveOnJira;
    bool destructiveOnBitbucket;
    unsigned int jiraLogIssueID;
};

struct TrackerConnFieldMap
{
    TrackerConnFieldMap( const std::vector<std::string>& trackerConnFieldMap );
    unsigned int trackerConnID;
    unsigned int fieldMapID;
};

struct TrackerConnFOptMap
{
    TrackerConnFOptMap( const std::vector<std::string>& trackerConnFOptMap );
    unsigned int trackerConnID;
    unsigned int fOptMapID;
};

struct TrackerConnFOptAdd
{
    TrackerConnFOptAdd( const std::vector<std::string>& trackerConnFOptAdd );
    unsigned int trackerConnID;
    unsigned int fOptAddID;
};

struct Tables
{
    std::vector<Field> fields;
    std::vector<FieldMap> fieldMaps;
    std::vector<FOpt> fOpts;
    std::vector<FOptMap> fOptMaps;
    std::vector<FOptAdditional> fOptAdditionals;
    std::vector<TrackerConn> trackerConns;
    std::vector<TrackerConnFieldMap> trackerConnFieldMaps;
    std::vector<TrackerConnFOptMap> trackerConnFOptMaps;
    std::vector<TrackerConnFOptAdd> trackerConnFOptAdds;
};

