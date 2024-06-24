#include <qmessagebox.h>

#include "../ConnectionController.h"
#include "../MainController.h"

ConnectionController::ConnectionController( const QModelIndex& index, QWidget* parent ) :
    QWidget( parent ),
    m_pModel( MainController::GetModel() ),
    m_index( index )
{}

void ConnectionController::SetIndexFromID( const unsigned int& connectionID )
{
    if ( m_index == QModelIndex() )
    {
        if ( m_pModel->hasChildren() )
        {
            int numberOfChildren = m_pModel->GetRootItem()->childCount();
            for ( int i = 0; i < numberOfChildren;
                  i++ )          // alternative : unique identifiers, data(), match(), custom item data role
            {
                // TreeItem* treeItem = m_pModel->GetItem(QModelIndex(), 0)->child(i);
                TreeItem* treeItem = m_pModel->GetRootItem()->child( i );
                ConnectionItem* pConnectionItem = static_cast<ConnectionItem*>( treeItem );
                if ( pConnectionItem->GetTrackerConn().id == connectionID )
                    m_index = MainController::GetModel()->index( i, 0, QModelIndex() );
            }
        }
        else
            return;
    }
    TreeItem* treeItem = m_pModel->GetItem( m_index, 0 );
    m_pConnectionItem = static_cast<ConnectionItem*>( treeItem );
}

void ConnectionController::SaveChanges( const TrackerConn& trackerConn, const int& connectionID ) 
{
    SetIndexFromID( connectionID );
    int idOfExistingItem = CheckIfConnectionExists( trackerConn );
    if ( idOfExistingItem == -1 )
    {
        m_pConnectionItem->GetTrackerConn() = trackerConn;
        m_pConnectionItem->SetConnectionLabel();
        std::cout << "\n save changes in controller: " << m_pConnectionItem->GetTrackerConn().jiProj << "\n";
        emit RequestUpdateView();

        auto IDisEqual = [ & ]( TrackerConn& connection )
        {
            return connection.id == m_pConnectionItem->GetTrackerConn().id;
        };
        auto pMatchedTrackerConn = std::find_if( MainController::GetTables().trackerConns.begin(),
                                                 MainController::GetTables().trackerConns.end(),
                                                 IDisEqual );
        *pMatchedTrackerConn = trackerConn;
    }
    else
        emit RequestConnectionExists( idOfExistingItem );
}

void ConnectionController::SaveAsNewConnection( TrackerConn& newTrackerConn, const int& connectionID )
{
    SetIndexFromID(
        connectionID ); // ToDo : Block/get correct index when no connection is selected, although at least one exists
    int idOfExistingItem = CheckIfConnectionExists( newTrackerConn );
    if ( idOfExistingItem == -1 || !m_pModel->hasChildren() )
    {
        const int numberOfRows = m_pModel->rowCount( m_index.parent() );
        newTrackerConn.id = CreateUniqueID();
        m_pModel->insertRows( numberOfRows, 1, m_index.parent() );
        TreeItem* insertedTreeItem;
        if ( m_index == QModelIndex() || !m_pModel->hasChildren() )
            insertedTreeItem = m_pModel->GetRootItem()->child( 0 );
        else
            insertedTreeItem = m_pModel->GetRootItem()->child( m_pModel->GetRootItem()->childCount() - 1 );
        // insertedTreeItem = m_pModel->GetItem(m_index.siblingAtRow(m_pModel->rowCount(m_index.parent())), 0);
        ConnectionItem* insertedConnectionItem = static_cast<ConnectionItem*>( insertedTreeItem );
        insertedConnectionItem->GetTrackerConn() = newTrackerConn;
        std::cout << "\n in savenew conn" << insertedConnectionItem->GetTrackerConn().jiProj << "\n";
        insertedConnectionItem->SetConnectionLabel();

        MainController::GetTables().trackerConns.push_back( newTrackerConn );
    }
    else
        emit RequestConnectionExists( idOfExistingItem );
}

void ConnectionController::DeleteConnection( const int& connectionID ) 
{
    SetIndexFromID( connectionID );
    std::cout << "\n row: " << m_index.row() << "\n";
    std::cout << "\n childCount: " << m_pModel->GetRootItem()->childCount() << "\n";
    m_pModel->removeRows( m_index.row(), 1, m_index.parent() );
    emit RequestUpdateView();

    auto IDisEqual = [ & ]( TrackerConn& connection )
    {
        return connection.id == m_pConnectionItem->GetTrackerConn().id;
    };
    std::_Erase_remove_if( MainController::GetTables().trackerConns, IDisEqual );
}

const int ConnectionController::CheckIfConnectionExists( const TrackerConn& newTrackerConn ) const
{
    for ( int i = 0; i < m_pModel->rowCount( m_index.parent() ); i++ )
    {
        ConnectionItem* siblingItem = static_cast<ConnectionItem*>( m_pModel->GetItem( m_index.siblingAtRow( i ), 0 ) );
        TrackerConn trackerConn = siblingItem->GetTrackerConn();
        if ( trackerConn.jiProj == newTrackerConn.jiProj && trackerConn.jiEpic == newTrackerConn.jiEpic &&
             trackerConn.bbRepo == newTrackerConn.bbRepo &&
             trackerConn.jiraLogIssueID == newTrackerConn.jiraLogIssueID &&
             int( trackerConn.commentsToBitbucket ) == newTrackerConn.commentsToBitbucket &&
             int( trackerConn.commentsToJira ) == newTrackerConn.commentsToJira &&
             int( trackerConn.destructiveOnBitbucket ) == newTrackerConn.destructiveOnBitbucket &&
             int( trackerConn.destructiveOnJira ) == newTrackerConn.destructiveOnJira &&
             int( trackerConn.newIssuesToBitbucket ) == newTrackerConn.newIssuesToBitbucket &&
             int( trackerConn.newIssuesToJira ) == newTrackerConn.newIssuesToJira )
            return trackerConn.id;
    }
    return -1;
}

const unsigned int ConnectionController::CreateUniqueID() const
{
    if ( m_pModel->hasChildren( QModelIndex() ) )
    {
        ConnectionItem* lastSiblingItem = static_cast<ConnectionItem*>(
            m_pModel->GetItem( m_index.siblingAtRow( m_pModel->rowCount( m_index.parent() ) - 1 ), 0 ) );
        unsigned int id = lastSiblingItem->GetTrackerConn().id + 1;
        while ( true )
        {
            for ( int i = 0; i < m_pModel->rowCount( m_index.parent() ); i++ )
            {
                ConnectionItem* siblingItem = static_cast<ConnectionItem*>(
                    m_pModel->GetItem( m_index.siblingAtRow( i ), 0 ) );
                if ( siblingItem->GetTrackerConn().id == id )
                {
                    id++;
                    break;
                }
                else if ( siblingItem == lastSiblingItem )
                    return id;
            }
        }
    }
    return 0;
}
