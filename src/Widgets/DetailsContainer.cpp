#include "DetailsContainer.h"
#include "../Data/ConnectionController.h"
#include "../Data/MainController.h"
#include "../ui_detailscontainer.h"
#include "AddOptionsWidget.h"
#include "ConnectionWidget.h"
#include "FieldsWidget.h"
#include "OptionsWidget.h"

DetailsContainer::DetailsContainer( const QModelIndex& index, QWidget* parent ) :
    QWidget( parent ),
    ui( new Ui::DetailsContainerForm ),
    m_tables( MainController::GetTables() ),
    m_pTreeItem( MainController::GetModel()->GetItem( index, 0 ) )
{
    ui->setupUi( this );
    if ( m_pTreeItem == NULL )
    {
        ConnectionController* pConnectionController = new ConnectionController( index );
        TrackerConn trackerConn = TrackerConn( { "0", "", "", "", "0", "0", "0", "0", "0", "0", "0" } );
        ConnectionItem connectionItem( trackerConn, m_pTreeItem );
        ConnectionWidget* connectionWidget = new ConnectionWidget( &connectionItem, index, pConnectionController );

        connect( pConnectionController, &ConnectionController::RequestUpdateView, this, &DetailsContainer::RequestUpdateView );
        ui->detailWidgetsLayout->addWidget( connectionWidget );
    }
    else if ( m_pTreeItem->GetItemType() == EitemType::connection )
    {
        ConnectionController* pConnectionController = new ConnectionController( index );

        ConnectionItem* pConnectionItem = static_cast<ConnectionItem*>( m_pTreeItem );
        ConnectionWidget* connectionWidget = new ConnectionWidget( pConnectionItem,
                                                                   index,
                                                                   pConnectionController,
                                                                   this );
        connect( pConnectionController, &ConnectionController::RequestUpdateView, this, &DetailsContainer::RequestUpdateView );
        ui->detailWidgetsLayout->addWidget( connectionWidget );
    }
    else if ( m_pTreeItem->GetItemType() == EitemType::field )
    {
        FieldItem* pFieldItem = static_cast<FieldItem*>( m_pTreeItem );
        m_pFieldsController = new FieldsController( index, this );
        FieldsWidget* fieldsWidget = new FieldsWidget( pFieldItem, m_pFieldsController, this );

        connect( m_pFieldsController, &FieldsController::RequestUpdateView, this, &DetailsContainer::RequestUpdateView );
        connect( m_pFieldsController, &FieldsController::RequestSaveOption, this, &DetailsContainer::RequestSaveOption );
        ui->detailWidgetsLayout->addWidget( fieldsWidget );
    }
    else if ( m_pTreeItem->GetItemType() == EitemType::option )
    {
        OptionItem* pOptionItem = static_cast<OptionItem*>( m_pTreeItem );
        OptionsWidget* optionsWidget = new OptionsWidget( pOptionItem, m_tables.fOpts, this );

        connect( optionsWidget, &OptionsWidget::RequestUpdateView, this, &DetailsContainer::RequestUpdateView );
        ui->detailWidgetsLayout->addWidget( optionsWidget );
    }
    else if ( m_pTreeItem->GetItemType() == EitemType::addoption )
    {
        AdditionalOptionItem* pAddOptionItem = static_cast<AdditionalOptionItem*>( m_pTreeItem );
        AddOptionsWidget* addOptionsWidget = new AddOptionsWidget( pAddOptionItem, m_tables.fOpts, this );

        connect( addOptionsWidget, &AddOptionsWidget::RequestUpdateView, this, &DetailsContainer::RequestUpdateView );
        ui->detailWidgetsLayout->addWidget( addOptionsWidget );
    }
}

DetailsContainer::~DetailsContainer()
{
    delete ui;
}

const FieldsController* DetailsContainer::GetFieldsController() const
{
    return m_pFieldsController;
}

