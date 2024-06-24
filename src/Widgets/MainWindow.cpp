#include "../Data/MainController.h"
#include "../ui_MainWindow.h"
#include "MainWindow.h"

MainWindow::MainWindow( QWidget* parent ) :
    QMainWindow( parent ),
    ui( new Ui::MainWindow ),
    m_tables( MainController::GetTables() )
{
    ui->setupUi( this );
    UpdateView();
    ui->treeView->setModel( MainController::GetModel() );
    ui->treeView->setModel( MainController::GetModel() );
    for ( int i = 0; i < MainController::GetModel()->columnCount( MainController::GetModel()->index( 0, 0 ) ); i++ )
        ui->treeView->resizeColumnToContents( i );
    ui->treeView->setColumnWidth( 2, 200 );
    ui->treeView->setColumnWidth( 1, 100 );
    for ( int i = 1; i < 12; i++ )
        ui->treeView->setColumnHidden( i, 1 );
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::RemoveWidget() const
{
    if ( ui->detailWidgetsLayout->count() != 0 )
    {
        ui->detailWidgetsLayout->itemAt( 0 )->widget()->setVisible( false );
        ui->detailWidgetsLayout->removeWidget( ui->detailWidgetsLayout->itemAt( 0 )->widget() );
    }
}

void MainWindow::UpdateView()
{
    if ( m_tables.trackerConns.size() == 0 )
    {
        RemoveWidget();
        m_pDetailsContainer = new DetailsContainer( QModelIndex(), this );
        connect( m_pDetailsContainer, &DetailsContainer::RequestUpdateView, this, &MainWindow::UpdateView );
        ui->detailWidgetsLayout->addWidget( m_pDetailsContainer );
    }
    ui->treeView->setModel( MainController::GetModel() );
    if ( m_selectedIndex.isValid() )
    {
        EitemType depth = MainController::GetModel()->GetItem( m_selectedIndex, 0 )->GetItemType();
        ui->treeView->expandToDepth(
            depth == EitemType::connection ?
                0 :
                int( depth ) - 2 );  // ToDo: get the same expansions as from old view (maybe with isExpanded?)
    }
    connect( ui->treeView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &MainWindow::ShowDetails );
}

void MainWindow::ShowDetails()
{
    RemoveWidget();
    m_selectedIndex = ui->treeView->selectionModel()->currentIndex();
    m_pDetailsContainer = new DetailsContainer(
        m_selectedIndex,
        this );  // alternative: pass QModelIndex directly to DetailsWidget. Do differentiation of itemtype here or in
 // other method. Get and cast item directly in DetailWidget via MainController::GetModel()->GetItem()
    ui->detailWidgetsLayout->addWidget( m_pDetailsContainer );
    connect( m_pDetailsContainer, &DetailsContainer::RequestUpdateView, this, &MainWindow::UpdateView );
}

