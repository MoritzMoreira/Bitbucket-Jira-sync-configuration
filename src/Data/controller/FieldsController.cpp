#include <qmessagebox.h>
#include <qdebug.h>

#include "../../Widgets/DetailsContainer.h"
#include "../FieldsController.h"
#include "../ConnectionController.h"
#include "../MainController.h"

FieldsController::FieldsController( const QModelIndex& index, QWidget* parent ) :
    QWidget( parent ),
    m_pModel( MainController::GetModel() ),
    m_index( index )
{}

const QModelIndex FieldsController::SetIndexFromID( const unsigned int& trackerConnID, const unsigned int& fieldMappingID )
{
    int rowCount = m_pModel->rowCount( QModelIndex() );
    QModelIndex connectionIndex;
    for ( int i = 0; i < rowCount; ++i )
    {
        connectionIndex = m_pModel->index( i, 0, QModelIndex() );
        TreeItem* pTreeItem = m_pModel->GetItem( connectionIndex, 0 );
        ConnectionItem* pConnectionItem = static_cast<ConnectionItem*>( pTreeItem );
        if ( pConnectionItem->GetTrackerConn().id == trackerConnID )
        {
            int fieldsRowCount = m_pModel->rowCount( connectionIndex );
            for ( int j = 0; j < fieldsRowCount; ++j )
            {
                QModelIndex fieldMappingIndex = m_pModel->index( j, 0, connectionIndex );
                pTreeItem = m_pModel->GetItem( fieldMappingIndex, 0 );
                FieldItem* pFieldItem = static_cast<FieldItem*>( pTreeItem );
                if ( pFieldItem->GetFieldMap()->id == fieldMappingID )
                    return fieldMappingIndex;
            }
        }
    }
    return QModelIndex();
}

void FieldsController::SetSaveOption( const bool& value )
{
    m_saveOption = value;
}

void FieldsController::SetFieldItem( const int& trackerConnID, const int& fieldMappingID )
{
    if ( m_index == QModelIndex() )
        m_index = SetIndexFromID( trackerConnID, fieldMappingID );
    TreeItem* treeItem = m_pModel->GetItem( m_index, 0 );
    m_pFieldItem = static_cast<FieldItem*>( treeItem );
}

const std::vector<int> FieldsController::SaveChanges( Field& newFromField, Field& newToField, const int& trackerConnID, const int& fieldMappingID )
{
    SetFieldItem( trackerConnID, fieldMappingID );
    CheckIfFieldExists( newFromField );
    CheckIfFieldExists( newToField );

    int idOfExistingItem = CheckIfMappingExistsAsSibling( newFromField, newToField );

    if ( idOfExistingItem != -1 )
        return { idOfExistingItem };
    else
    {
        m_pFieldItem->GetFieldFrom() = newFromField;
        m_pFieldItem->GetFieldTo() = newToField;

        auto IDisEqual = [ & ]( FieldMap& fieldMapping ) // get field mapping object from tables as iterator
        {
            return fieldMapping.id == m_pFieldItem->GetFieldMap()->id;
        };
        m_pMatchedMapping = std::find_if( MainController::GetTables().fieldMaps.begin(), MainController::GetTables().fieldMaps.end(), IDisEqual );

        std::vector<TrackerConnFieldMap>::iterator searchRangeBeginning = MainController::GetTables().trackerConnFieldMaps.begin();
        m_connsWithMapping.clear();
        while ( true )
        {
            auto MappingIDisEqual = [ & ]( TrackerConnFieldMap& trackerConnFieldMapping )
            {
                return trackerConnFieldMapping.fieldMapID == m_pFieldItem->GetFieldMap()->id;
            };
            auto pMatchedConnectionMapping = std::find_if( searchRangeBeginning,
                                                           MainController::GetTables().trackerConnFieldMaps.end(),
                                                           MappingIDisEqual );
            ConnectionItem* parentConnection = static_cast<ConnectionItem*>( m_pFieldItem->parentItem() );
            if ( pMatchedConnectionMapping != MainController::GetTables().trackerConnFieldMaps.end() )
            {
                if ( pMatchedConnectionMapping->trackerConnID == parentConnection->GetTrackerConn().id )
                    m_connsWithMapping.insert( m_connsWithMapping.begin(), pMatchedConnectionMapping );
                else
                    m_connsWithMapping.emplace_back( pMatchedConnectionMapping );
                searchRangeBeginning = pMatchedConnectionMapping + 1;
            }
            else
                break;
        }
        if ( m_connsWithMapping.size() > 1 )  // field mapping exists under at least 1 other connection
        {
            std::vector<int> connsWithMappingIDs;
            for ( std::vector<TrackerConnFieldMap>::iterator pTrackerConnFieldMap : m_connsWithMapping )
            {
                connsWithMappingIDs.emplace_back( pTrackerConnFieldMap->trackerConnID );
            }
            return connsWithMappingIDs;
        }
        else  // field mapping does not exist under other connections
        {
            SaveChangesToAll();
            return { -1 };
        }
    }
}

void FieldsController::SaveChangesToAll()
{
    *m_pMatchedMapping = *m_pFieldItem->GetFieldMap();
    for ( std::vector<TrackerConnFieldMap>::iterator trackerConnFieldMap : m_connsWithMapping )
    {
        if ( trackerConnFieldMap != m_connsWithMapping.front() )
        {
            QModelIndex idx = SetIndexFromID( trackerConnFieldMap->trackerConnID, trackerConnFieldMap->fieldMapID );

            TreeItem* treeItem = m_pModel->GetItem( idx, 0 );
            FieldItem* fieldItem = static_cast<FieldItem*>( treeItem );

            CopyFieldProperties( m_pFieldItem, fieldItem );

            fieldItem->SetFieldsLabel();
        }
    }
    m_pFieldItem->SetFieldsLabel();
    emit RequestUpdateView();
}

void FieldsController::SaveOnlyToCurrentConn()
{
    MainController::GetTables().trackerConnFieldMaps.erase( m_connsWithMapping.front() );
    SaveAsNewMapping( m_pFieldItem->GetFieldFrom(),
                      m_pFieldItem->GetFieldTo(),
                      static_cast<ConnectionItem*>( m_pFieldItem->parentItem() )->GetTrackerConn().id,
                      m_pFieldItem->GetFieldMap()->id );
    m_pFieldItem->SetFieldsLabel();
    emit RequestUpdateView();
}

void FieldsController::CopyFieldProperties( FieldItem* inFieldItem, FieldItem* outFieldItem )
{
    outFieldItem->GetFieldFrom().path = inFieldItem->GetFieldFrom().path;
    outFieldItem->GetFieldFrom().system = inFieldItem->GetFieldFrom().system;
    outFieldItem->GetFieldTo().system = inFieldItem->GetFieldTo().system;
    outFieldItem->GetFieldTo().path = inFieldItem->GetFieldTo().path;
}

void FieldsController::SaveAsNewMapping( Field& newFromField, Field& newToField,  int trackerConnID, const int& fieldMappingID )
{
    int numberOfRows = 0;
    bool resultFromInsertRows;
    TreeItem* insertedTreeItem;
    FieldMap newFieldMap;
    newFieldMap.id = CreateUniqueID();
    newFieldMap.fieldIDfrom = newFromField.id;
    newFieldMap.fieldIDto = newToField.id;

    if ( fieldMappingID == -1 )
    {
        ConnectionController connectionController;
        m_index = connectionController.SetIndexFromID( trackerConnID );

        m_pModel->insertRows( numberOfRows, 1, m_index );
        insertedTreeItem = m_pModel->GetItem( m_index, 0 )->child( 0 );
    }
    else
    {
        numberOfRows = m_pModel->rowCount( m_index.parent() );
        m_pModel->insertRows( numberOfRows, 1, m_index.parent() );
        insertedTreeItem = m_pModel->GetItem( m_index.siblingAtRow( numberOfRows ), 0 );
    }

    CheckIfFieldExists( newFromField );
    CheckIfFieldExists( newToField );
    int idOfExistingItem = CheckIfMappingExistsAsSibling( newFromField, newToField );
  
    if ( idOfExistingItem == -1 )
    {
        std::vector<FieldMap>::iterator matchedFieldMapping = CheckIfMappingExists( newFromField, newToField );
        unsigned int matchedFieldMappingID;
        if ( matchedFieldMapping != MainController::GetTables().fieldMaps.end() )  // Field mapping exists in other connection?
            matchedFieldMappingID = matchedFieldMapping->id;
        else  
        {
            MainController::GetTables().fieldMaps.push_back( newFieldMap );
            matchedFieldMappingID = CreateUniqueID();
        }
        
        FieldItem* insertedFieldItem = static_cast<FieldItem*>( insertedTreeItem );
        *insertedFieldItem->GetFieldMap() = newFieldMap;
        insertedFieldItem->GetFieldFrom() = newFromField;
        insertedFieldItem->GetFieldTo() = newToField;
        insertedFieldItem->SetFieldsLabel();
        
        MainController::GetTables().trackerConnFieldMaps.push_back(TrackerConnFieldMap( { std::to_string( trackerConnID ), std::to_string( matchedFieldMappingID ) } ) );

        /*auto IDisEqual = [ & ]( FieldMap& fieldMapping )
        {
            return fieldMapping.id == m_pFieldItem->GetFieldMap()->id;
        };
        auto pMatchedMapping = std::find_if( MainController::GetTables().fieldMaps.begin(), MainController::GetTables().fieldMaps.end(), IDisEqual );
        *pMatchedMapping = *m_pFieldItem->GetFieldMap();*/
    }
    emit RequestMappingExists( idOfExistingItem );
}

void FieldsController::DeleteMapping( const int& trackerConnID, const int& fieldMappingID )
{
    SetFieldItem( trackerConnID, fieldMappingID );
    auto IDisEqual = [ & ]( FieldMap& fieldMapping )
    {
        return fieldMapping.id == m_pFieldItem->GetFieldMap()->id;
    };
    std::_Erase_remove_if( MainController::GetTables().fieldMaps, IDisEqual );
    MainController::UpdateModel();
    emit RequestUpdateView();
}

const int FieldsController::CheckIfMappingExistsAsSibling( const Field& newFromField, const Field& newToField )
{
    for ( int i = 0; i < m_pModel->rowCount( m_index.parent() ); i++ )
    {
        QModelIndex siblingIndex = m_index.siblingAtRow( i );
        TreeItem* treeItem = m_pModel->GetItem( siblingIndex, 0 );
        FieldItem* siblingItem = static_cast<FieldItem*>( treeItem );
        Field fromField( siblingItem->GetFieldFrom() );
        Field toField( siblingItem->GetFieldTo() );

        if ( fromField.system == newFromField.system && fromField.path == newFromField.path &&
             toField.system == newToField.system && toField.path == newToField.path )
            return siblingItem->GetFieldMap()->id;
    }
    return -1;
}

const std::vector<FieldMap>::iterator FieldsController::CheckIfMappingExists( const Field& newFromField, const Field& newToField )
{
    auto mappingIsEqual = [ &newFromField, &newToField ]( FieldMap fieldMap )
    {
        return fieldMap.fieldIDfrom == newFromField.id &&
               fieldMap.fieldIDto == newToField.id;    // check if Mapping exists for any connection
    };
    auto pMatchedMapping = std::find_if( MainController::GetTables().fieldMaps.begin(),
                                         MainController::GetTables().fieldMaps.end(),
                                         mappingIsEqual );

    return pMatchedMapping;
}

void FieldsController::CheckIfFieldExists( Field& newField )
{
    auto stringsAreEqual = [ &newField ]( Field field )
    {
        return field.system == newField.system && field.path == newField.path;
    };
    auto pMatchedField = std::find_if( MainController::GetTables().fields.begin(),
                                       MainController::GetTables().fields.end(),
                                       stringsAreEqual );
    if ( pMatchedField == MainController::GetTables().fields.end() )
    {
        newField.id = CreateUniqueFieldID();
        MainController::GetTables().fields.push_back( newField );
    }
    else
        newField.id = pMatchedField->id;
}

const int FieldsController::CreateUniqueFieldID()
{
    if ( MainController::GetTables().fields.size() != 0 )
    {
        unsigned int newID = MainController::GetTables().fields.back().id + 1;
        while ( true )
        {
            auto idIsEqual = [ &newID ]( Field field )
            {
                return field.id == newID;
            };
            auto pMatchedField = std::find_if( MainController::GetTables().fields.begin(),
                                               MainController::GetTables().fields.end(),
                                               idIsEqual );
            if ( pMatchedField == MainController::GetTables().fields.end() )
                return newID;
            else
                newID++;
        }
    }
    return 0;
}

const unsigned int FieldsController::CreateUniqueID()
{
    if ( m_pModel->hasChildren( QModelIndex() ) )
    {
        QModelIndex indexOfLastSibling = m_index.siblingAtRow( m_pModel->rowCount( m_index.parent() ) - 1 );
        FieldItem* lastSiblingItem = static_cast<FieldItem*>( m_pModel->GetItem( indexOfLastSibling, 0 ) );
        qDebug() << "FieldMapID : " << QString::number(lastSiblingItem->GetFieldMap()->id);
        unsigned int id = lastSiblingItem->GetFieldMap()->id + 1;
        while ( true )
        {
            for ( int i = 0; i < m_pModel->rowCount( m_index.parent() ); i++ )
            {
                FieldItem* siblingItem = static_cast<FieldItem*>( m_pModel->GetItem( m_index.siblingAtRow( i ), 0 ) );
                if ( siblingItem->GetFieldMap()->id == id )
                {
                    id++;
                    break;
                }
                else if ( siblingItem == lastSiblingItem )
                    return 13;
            }
        }
    }
    return 0;
}
