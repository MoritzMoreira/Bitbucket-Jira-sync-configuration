#pragma once
#include <QVariant>
#include <QVector>

#include "Tables.h"
#include "TreeItem.h"

class ConnectionItem;

class RootItem : public TreeItem
{
public:
    RootItem( const std::vector<std::string>& data = {} );

    virtual const EitemType GetItemType() const override;
    void appendChild( ConnectionItem* item );
};

// -------------------------------------------------------------------------------------------------------------------------------------------------

class ConnectionItem : public TreeItem
{
public:
    ConnectionItem( TrackerConn& trackerConn, TreeItem* parent );

    TrackerConn& GetTrackerConn();
    void SetConnectionLabel();
    virtual const EitemType GetItemType() const override;

private:
    QString m_itemLabel;
    TrackerConn m_trackerConn;
};

// -------------------------------------------------------------------------------------------------------------------------------------------------

class FieldItem : public TreeItem
{
public:
    FieldItem( ConnectionItem* pConnectionItem, FieldMap& fieldMap, Field& pFieldFrom, Field& pFieldTo );

    FieldMap* GetFieldMap();
    Field& GetFieldFrom();
    Field& GetFieldTo();
    virtual const EitemType GetItemType() const override;
    void SetFieldsLabel();

private:
    Field m_fieldFrom;
    Field m_fieldTo;
    FieldMap m_fieldMap;
};

// -------------------------------------------------------------------------------------------------------------------------------------------------

class OptionItem : public TreeItem
{
public:
    OptionItem( FieldItem* pFieldItem,
                FOptMap& fOptMap,
                FOpt& fOptFrom,
                FOpt& fOptTo,
                Field& fieldFrom,
                Field& fieldTo,
                FOpt& fOptCondition );

    FOptMap& GetFOptMap() const;
    Field& GetFieldFrom() const;
    Field& GetFieldTo() const;
    FOpt& GetFOptFrom() const;
    FOpt& GetFOptTo() const;
    FOpt& GetFOptCondition();
    virtual const EitemType GetItemType() const override;

private:
    FOpt& m_fOptFrom;
    FOpt& m_fOptTo;
    FOpt m_fOptCondition;
    FOptMap& m_fOptMap;
    Field& m_fieldTo;
    Field& m_fieldFrom;
};

// -------------------------------------------------------------------------------------------------------------------------------------------------

class AdditionalOptionItem : public TreeItem
{
public:
    AdditionalOptionItem( OptionItem* pOptionItem, FOptAdditional& pFOptAdditional, FOpt& fOptAdd );

    const FOpt& GetAddOpt() const;
    virtual const EitemType GetItemType() const override;

private:
    FOpt& m_fOptAdd;
    FOptAdditional& m_pFOptAdditional;
};
