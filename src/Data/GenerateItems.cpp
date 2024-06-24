#include "DerivedItems.h"
#include "TreeModel.h"

void TreeModel::GenerateConnectionItems( TreeItem* pParent ) const
{
    for ( TrackerConn trackerConn : m_data->trackerConns )
    {
        ConnectionItem* pTrackerConnItem = new ConnectionItem( trackerConn, pParent );
        pParent->appendChild( pTrackerConnItem );
        GenerateFieldItems( pTrackerConnItem );
    }
}

void TreeModel::GenerateFieldItems( ConnectionItem* pConnectionItem ) const
{
    for ( TrackerConnFieldMap pTrackerConnFieldMap : m_data->trackerConnFieldMaps )
    {
        if ( pTrackerConnFieldMap.trackerConnID != pConnectionItem->GetTrackerConn().id )
        {
            continue;
        }
        for ( FieldMap& fieldMap : m_data->fieldMaps )
        {
            if ( pTrackerConnFieldMap.fieldMapID != fieldMap.id )
                continue;

            auto IDfromIsEqual = [ fieldMap ]( Field& pField )
            {
                return pField.id == fieldMap.fieldIDfrom;
            };
            auto pFieldFrom = find_if( m_data->fields.begin(), m_data->fields.end(), IDfromIsEqual );

            auto IDtoIsEqual = [ fieldMap ]( Field& pField )
            {
                return pField.id == fieldMap.fieldIDto;
            };
            auto pFieldTo = find_if( m_data->fields.begin(), m_data->fields.end(), IDtoIsEqual );
            if ( pFieldFrom == m_data->fields.end() || pFieldTo == m_data->fields.end() )
            {
                std::cout << "\n no field found \n";
                continue;
            }
            FieldItem* pFieldItem = new FieldItem( pConnectionItem, fieldMap, *pFieldFrom, *pFieldTo );
            pConnectionItem->appendChild( pFieldItem );
            GenerateOptionItems( pConnectionItem, pFieldItem );
        }
    }
}

void TreeModel::GenerateOptionItems( ConnectionItem* pConnectionItem, FieldItem* pFieldItem ) const
{
    for ( TrackerConnFOptMap pTrackerConnFOptMap : m_data->trackerConnFOptMaps )
    {
        if ( pTrackerConnFOptMap.trackerConnID != pConnectionItem->GetTrackerConn().id )
            continue;
        for ( FOptMap pFOptMap : m_data->fOptMaps )
        {
            if ( pTrackerConnFOptMap.fOptMapID != pFOptMap.id )
                continue;

            auto IDfromIsEqual = [ pFOptMap, pFieldItem ]( FOpt pFOpt )
            {
                return pFOpt.id == pFOptMap.fOptIDfrom && pFOpt.fieldID == pFieldItem->GetFieldMap()->fieldIDfrom;
            };
            auto fOptFrom = find_if( m_data->fOpts.begin(), m_data->fOpts.end(), IDfromIsEqual );
            if ( fOptFrom == m_data->fOpts.end() )
                continue;

            auto IDtoIsEqual = [ pFOptMap, pFieldItem ]( FOpt pFOpt )
            {
                return pFOpt.id == pFOptMap.fOptIDto && pFOpt.fieldID == pFieldItem->GetFieldMap()->fieldIDto;
            };
            auto fOptTo = find_if( m_data->fOpts.begin(), m_data->fOpts.end(), IDtoIsEqual );
            if ( fOptTo == m_data->fOpts.end() )
                continue;

            FOpt fOptCondition;
            if ( pFOptMap.fOptIDcondition != 0 )
            {
                auto IDisEqual = [ pFOptMap ]( FOpt pFOpt )
                {
                    return pFOpt.id == pFOptMap.fOptIDcondition;
                };
                fOptCondition = *( find_if( m_data->fOpts.begin(), m_data->fOpts.end(), IDisEqual ) );
            }
            else
                fOptCondition = FOpt();
            OptionItem* pOptionItem = new OptionItem( pFieldItem,
                                                      pFOptMap,
                                                      *fOptFrom,
                                                      *fOptTo,
                                                      pFieldItem->GetFieldFrom(),
                                                      pFieldItem->GetFieldTo(),
                                                      fOptCondition );
            pFieldItem->appendChild( pOptionItem );
            GenerateAdditionalOptionItems( pConnectionItem, pOptionItem );
        }
    }
}

void TreeModel::GenerateAdditionalOptionItems( ConnectionItem* pConnectionItem, OptionItem* pOptionItem ) const
{
    for ( TrackerConnFOptAdd trackerConnFOptAdd : m_data->trackerConnFOptAdds )
    {
        if ( pConnectionItem->GetTrackerConn().id != trackerConnFOptAdd.trackerConnID )
            continue;

        for ( FOptAdditional fOptAdditional : m_data->fOptAdditionals )
        {
            if ( fOptAdditional.fOptMapID != pOptionItem->GetFOptMap().id || fOptAdditional.id != trackerConnFOptAdd.fOptAddID )
                continue;

            auto IDisEqual = [ fOptAdditional ]( FOpt pFOpt )
            {
                return pFOpt.id == fOptAdditional.fOptIDadd;
            };
            auto fOptAdd = std::find_if( m_data->fOpts.begin(), m_data->fOpts.end(), IDisEqual );
            pOptionItem->appendChild( new AdditionalOptionItem( pOptionItem, fOptAdditional, *fOptAdd ) );
        }
    }
}

TreeItem* TreeModel::GetRootItem() const
{
    return m_pRootItem;
}
