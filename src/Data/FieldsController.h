#pragma once

#include <QModelIndex>
#include <QWidget>

#include "TreeModel.h"

class FieldsController : public QWidget
{
    Q_OBJECT

public:
    FieldsController( const QModelIndex& index = QModelIndex(), QWidget* parent = 0 );

    const QModelIndex SetIndexFromID( const unsigned int& trackerConnID, const unsigned int& fieldMappingID );
    void SetSaveOption( const bool& value );
    void SetFieldItem( const int& trackerConnID, const int& fieldMappingID );
    const std::vector<int> SaveChanges( Field& newFromField, Field& newToField, const int& trackerConnID = 0, const int& fieldMappingID = 0 );
    void SaveChangesToAll();
    void SaveOnlyToCurrentConn();
    void CopyFieldProperties( FieldItem* inFieldItem, FieldItem* outFieldItem );
    void SaveAsNewMapping( Field& newFromField, Field& newToField, int trackerConnID = -1, const int& fieldMappingID = -1 );
    void DeleteMapping( const int& trackerConnID = 0, const int& fieldMappingID = 0 );
    const int CheckIfMappingExistsAsSibling( const Field& newFromField, const Field& newToField );
    const std::vector<FieldMap>::iterator CheckIfMappingExists( const Field& newFromField, const Field& newToField );
    void CheckIfFieldExists( Field& newField );
    const int CreateUniqueFieldID();
    const unsigned int CreateUniqueID();

signals:
    void RequestUpdateView() const;
    void RequestMappingExists( int& ) const;
    void RequestSaveOption() const;

private:
    QModelIndex m_index;
    FieldItem* m_pFieldItem;
    TreeModel* m_pModel;
    int m_fieldID;
    bool m_saveOption;
    std::vector<FieldMap>::iterator m_pMatchedMapping;
    std::vector<std::vector<TrackerConnFieldMap>::iterator> m_connsWithMapping;
};

