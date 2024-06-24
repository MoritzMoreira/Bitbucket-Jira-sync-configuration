#pragma once

#include "../Data/FieldsController.h"

namespace Ui
{
    class DetailsContainerForm;
}

class DetailsContainer : public QWidget
{
    Q_OBJECT

public:
    explicit DetailsContainer( const QModelIndex& index, QWidget* parent = 0 );
    ~DetailsContainer();

    const FieldsController* GetFieldsController() const;
signals:
    void RequestUpdateView() const;
    void RequestSaveOption() const;

private:
    Ui::DetailsContainerForm* ui;
    Tables& m_tables;
    TreeItem* m_pTreeItem;
    FieldsController* m_pFieldsController;
};

const int exists = -1;
const int doesNotExist = -2;
