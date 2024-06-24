#include "../DerivedItems.h"

ConnectionItem::ConnectionItem( TrackerConn& trackerConn, TreeItem* parent ) :
    TreeItem( {}, parent ),
    m_trackerConn( trackerConn )
{
    SetConnectionLabel();
}

const EitemType ConnectionItem::GetItemType() const
{
    return EitemType::connection;
}

void ConnectionItem::SetConnectionLabel() 
{
    std::string jira = m_trackerConn.jiProj == "" ? m_trackerConn.jiEpic : m_trackerConn.jiProj;
    m_itemData = { m_trackerConn.bbRepo + " - " + jira };
}

TrackerConn& ConnectionItem::GetTrackerConn() 
{
    return m_trackerConn;
}
