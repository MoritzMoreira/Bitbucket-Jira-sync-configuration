#include "../DerivedItems.h"

RootItem::RootItem( const std::vector<std::string>& data ) :
    TreeItem( data )
{}

const EitemType RootItem::GetItemType() const
{
    return EitemType::tree;
}

void RootItem::appendChild( ConnectionItem* item )
{
    m_childItems.append( item );
}
