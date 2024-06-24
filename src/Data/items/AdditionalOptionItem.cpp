#include "../DerivedItems.h"

AdditionalOptionItem::AdditionalOptionItem( OptionItem* pOptionItem, FOptAdditional& pFOptAdditional, FOpt& fOptAdd ) 
  : TreeItem( { fOptAdd.opt }, pOptionItem ),
    m_pFOptAdditional( pFOptAdditional ),
    m_fOptAdd( fOptAdd )
{}

const EitemType AdditionalOptionItem::GetItemType() const
{
    return EitemType::addoption;
}

const FOpt& AdditionalOptionItem::GetAddOpt() const
{
    return m_fOptAdd;
}
