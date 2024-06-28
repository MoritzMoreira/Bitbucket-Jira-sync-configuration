#include <iterator>
#include <QStringList>
#include <sstream>
#include <string>

#include "DerivedItems.h"
#include "TreeItem.h"
#include "TreeModel.h"

TreeModel::TreeModel( Tables* data, QObject* parent ) :
    QAbstractItemModel( parent ),
    m_data( data )
{
    std::vector<std::string> maxColumnNumber( 11 );
    m_pRootItem = new RootItem( maxColumnNumber );
    setupModelData( m_pRootItem );
}

TreeModel::~TreeModel()
{
    delete m_pRootItem;
}

QVariant TreeModel::data( const QModelIndex& index, int role ) const
{
    if ( !index.isValid() )
        return QVariant();
    if ( role != Qt::DisplayRole )
        return QVariant();
    TreeItem* item = static_cast<TreeItem*>( index.internalPointer() );
    return item->data( index.column() );
}

TreeItem* TreeModel::GetItem( const QModelIndex& index, int role ) const
{
    if ( !index.isValid() )
        return nullptr;
    if ( role != Qt::DisplayRole )
        return nullptr;
    TreeItem* item = static_cast<TreeItem*>( index.internalPointer() );

    std::cout << "\n begin getItem\n";
    return item;
}

Qt::ItemFlags TreeModel::flags( const QModelIndex& index ) const
{
    if ( !index.isValid() )
    {
        return Qt::NoItemFlags;
    }
    return QAbstractItemModel::flags( index );
}

QVariant TreeModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
    if ( orientation == Qt::Horizontal && role == Qt::DisplayRole )
    {
        return m_pRootItem->data( section );
    }
    return QVariant();
}

QModelIndex TreeModel::index( int row, int column, const QModelIndex& parent ) const
{
    if ( !hasIndex( row, column, parent ) )
        return QModelIndex();
    TreeItem* parentItem;
    if ( !parent.isValid() )
        parentItem = m_pRootItem;
    else
        parentItem = static_cast<TreeItem*>( parent.internalPointer() );
    TreeItem* childItem = parentItem->child( row );
    if ( childItem )
        return createIndex( row, column, childItem );
    return QModelIndex();
}

QModelIndex TreeModel::parent( const QModelIndex& index ) const
{
    if ( !index.isValid() )
        return QModelIndex();
    TreeItem* childItem = static_cast<TreeItem*>( index.internalPointer() );
    TreeItem* parentItem = childItem->parentItem();
    if ( parentItem == m_pRootItem )
        return QModelIndex();
    return createIndex( parentItem->row(), 0, parentItem );
}

int TreeModel::rowCount( const QModelIndex& parent ) const
{
    TreeItem* parentItem;
    if ( parent.column() > 0 )
    {
        return 0;
    }
    if ( !parent.isValid() )
        parentItem = m_pRootItem;
    else
        parentItem = static_cast<TreeItem*>( parent.internalPointer() );
    return parentItem->childCount();
}

int TreeModel::columnCount( const QModelIndex& parent ) const
{
    if ( parent.isValid() )
    {
        return static_cast<TreeItem*>( parent.internalPointer() )->columnCount();
    }
    return m_pRootItem->columnCount();
}


bool TreeModel::insertRows( int position, int rows, const QModelIndex& parent )
{
    rows = 1;
    TreeItem* parentItem = GetItem( parent, 0 );
    beginInsertRows( parent, position, position );
    if ( parentItem == nullptr )
    {
        ConnectionItem* emptyConnectionItem = new ConnectionItem( TrackerConn(), GetRootItem() );
        GetRootItem()->appendChild( emptyConnectionItem );
    }
    else if ( GetItem( parent, 0 )->GetItemType() == EitemType::connection )
    {        
        ConnectionItem* connectionItem = static_cast<ConnectionItem*>( parentItem );
        FieldItem* emptyFieldItem = new FieldItem( connectionItem, FieldMap(), Field(), Field() );
        parentItem->appendChild( emptyFieldItem );
    }
    endInsertRows();

    return true;
}

bool TreeModel::removeRows( int position, int rows, const QModelIndex& parent )
{
    TreeItem* parentItem = GetItem( parent, 0 );
    if ( !parentItem )
        // return false;

        beginRemoveRows( parent, position, position + rows - 1 );
    const bool success = GetRootItem()->removeChildren( position, rows );
    endRemoveRows();

    return success;
}

void TreeModel::setupModelData( TreeItem* parent )
{
    GenerateConnectionItems( parent );
}

std::string& operator+( std::string& sL, std::string& sR )
{
    return sL.append( sR );
}
