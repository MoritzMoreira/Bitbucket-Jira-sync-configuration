#include "TreeItem.h"
#include "DerivedItems.h"

TreeItem::TreeItem( const std::vector<std::string>& data, TreeItem* parent ) :
    m_itemData( data ),
    m_parentItem( parent )
{}

TreeItem::~TreeItem()
{
    qDeleteAll( m_childItems );
}

void TreeItem::appendChild( TreeItem* item )
{
    m_childItems.append( item );
}

TreeItem* TreeItem::child( int row )
{
    if ( row < 0 || row >= m_childItems.size() )
        return nullptr;
    return m_childItems.at( row );
}

int TreeItem::childCount() const
{
    return m_childItems.count();
}

int TreeItem::columnCount() const
{
    return int( m_itemData.size() );
}

QVariant TreeItem::data( int column ) const
{
    if ( column < 0 || column >= m_itemData.size() )
        return QVariant();
    return QString::fromStdString( m_itemData.at( column ) );
}

TreeItem* TreeItem::parentItem()
{
    return m_parentItem;
}

int TreeItem::row() const
{
    if ( m_parentItem )
        return m_parentItem->m_childItems.indexOf( const_cast<TreeItem*>( this ) );
    return 0;
}

// bool TreeItem::insertChildren(int position, int count, int columns)
//{
//	if (position < 0 || position > qsizetype(m_childItems.size()))
//		return false;
//
//	for (int row = 0; row < count; ++row) {
//		QVariantList data(columns);
//		m_childItems.insert(m_childItems.cbegin() + position,
//			std::make_unique<TreeItem>(data, this));
//	}
//
//	return true;
// }

bool TreeItem::removeChildren( int position, int count )
{
    if ( position < 0 || position + count > qsizetype( m_childItems.size() ) )
    {
        return false;
    }

    for ( int row = 0; row < count; ++row )
        m_childItems.erase( m_childItems.begin() + position );

    return true;
}
