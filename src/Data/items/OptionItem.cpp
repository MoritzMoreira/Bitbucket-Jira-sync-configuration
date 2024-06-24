#include "../DerivedItems.h"

OptionItem::OptionItem( FieldItem* pFieldItem,
                        FOptMap& fOptMap,
                        FOpt& fOptFrom,
                        FOpt& fOptTo,
                        Field& fieldFrom,
                        Field& fieldTo,
                        FOpt& fOptCondition )
  : TreeItem( {}, pFieldItem ),
    m_fOptMap( fOptMap ),
    m_fOptFrom( fOptFrom ),
    m_fOptTo( fOptTo ),
    m_fOptCondition( fOptCondition ),
    m_fieldFrom( fieldFrom ),
    m_fieldTo( fieldTo )
{
    std::string condition = "";
    if ( m_fOptCondition.opt != "" )
    {
        condition = " (" + m_fOptCondition.opt + ")";
    }
    m_itemData = { m_fOptFrom.opt + " -> " + m_fOptTo.opt + condition };
}

const EitemType OptionItem::GetItemType() const
{
    return EitemType::option;
}

Field& OptionItem::GetFieldFrom() const
{
    return m_fieldFrom;
}

Field& OptionItem::GetFieldTo() const
{
    return m_fieldTo;
}

FOpt& OptionItem::GetFOptFrom() const
{
    return m_fOptFrom;
}

FOpt& OptionItem::GetFOptTo() const
{
    return m_fOptTo;
}

FOpt& OptionItem::GetFOptCondition() 
{
    return m_fOptCondition;
}

FOptMap& OptionItem::GetFOptMap() const
{
    return m_fOptMap;
}
