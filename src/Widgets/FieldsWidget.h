#pragma once

#include "../Data/DerivedItems.h"
#include "../Data/FieldsController.h"
#include "../ui_saveOption.h"
#include <QWidget>

namespace Ui
{
    class FieldsForm;
}

class SaveOptionDia;

class FieldsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FieldsWidget( FieldItem*, FieldsController*, QWidget* parent = 0 );
    FieldsWidget();
    ~FieldsWidget();

    void SetData() const;
    void FillPathComboboxes() const;
    unsigned int CreateUniqueID() const;
    void SaveNewFields() const;
    void SaveAsNewMapping() const;
    void MappingExists( int& id );

public slots:
    void SaveChanges();
    void SwapSystem() const;
signals:
    void RequestUpdateView() const;

private:
    Ui::FieldsForm* ui;
    FieldItem* m_pFieldItem;
    std::vector<Field>& m_fields;
    std::vector<FieldMap>& m_fieldMaps;
    int fromPathEditedFlag = 0;
    int toPathEditedFlag = 0;
    FieldsController* m_pFieldsController;
    SaveOptionDia* m_pSaveOptionDia;
};

//----------------------------------------------------------------------------------------------------------------------------------------------

class SaveOptionDia : public QDialog
{
    Q_OBJECT

public:
    explicit SaveOptionDia( QWidget* parent = 0 );
    ~SaveOptionDia();

    const Ui::SaveDialog* GetUi() const;
    void SetConnectionIDLabel( const std::vector<int>& connIDsWithMapping ) const;

private:
    Ui::SaveDialog* ui;
};
