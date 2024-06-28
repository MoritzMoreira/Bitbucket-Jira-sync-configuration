#include "../DerivedItems.h"

FieldItem::FieldItem( ConnectionItem* pConnectionItem, FieldMap& fieldMap, Field& pFieldFrom, Field& pFieldTo ) 
  : TreeItem( { pFieldFrom.system }, pConnectionItem ),
    m_fieldMap( fieldMap ),
    m_fieldFrom( pFieldFrom ),
    m_fieldTo( pFieldTo )
{
    SetFieldsLabel();
}

const EitemType FieldItem::GetItemType() const
{
    return EitemType::field;
}

FieldMap* FieldItem::GetFieldMap()
{
    return &m_fieldMap;
}

Field& FieldItem::GetFieldFrom() 
{
    return m_fieldFrom;
}

Field& FieldItem::GetFieldTo()
{
    return m_fieldTo;
}

void FieldItem::SetFieldsLabel()
{
    m_itemData = { m_fieldFrom.system + " - " + m_fieldFrom.path + " -> " + m_fieldTo.system + " - " + m_fieldTo.path };
}
