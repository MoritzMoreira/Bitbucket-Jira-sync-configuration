#pragma once

#include <QModelIndex>
#include <QWidget>

#include "TreeModel.h"

class ConnectionController : public QWidget
{
    Q_OBJECT

public:
    ConnectionController( const QModelIndex& index = QModelIndex(), QWidget* parent = 0 );

    void SetConnectionItem();
    QModelIndex SetIndexFromID( const unsigned int& connectionID );
    void SaveChanges( const TrackerConn& trackerConn, const int& connectionID );
    void SaveAsNewConnection( TrackerConn& newTrackerConn, const int& connectionID );
    void DeleteConnection( const int& connectionID );
    const int CheckIfConnectionExists( const TrackerConn& newTrackerConn ) const;
    const unsigned int CreateUniqueID() const;
signals:
    void RequestUpdateView() const;
    void RequestConnectionExists( int& ) const;

private:
    QModelIndex m_index;
    ConnectionItem* m_pConnectionItem;
    TreeModel* m_pModel;
    int m_connectionID;
};

