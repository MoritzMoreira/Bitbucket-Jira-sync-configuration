#include <qmessagebox.h>
#include <regex>

#include "../Data/MainController.h"
#include "../ui_connection.h"
#include "ConnectionWidget.h"

ConnectionWidget::ConnectionWidget( ConnectionItem* pConnectionItem,
                                    const QModelIndex& index,
                                    ConnectionController* pConnectionController,
                                    QWidget* parent ) :
    QWidget( parent ),
    ui( new Ui::ConnectionsForm ),
    m_pConnectionItem( pConnectionItem ),
    m_index( index ),
    m_trackerConns( MainController::GetTables().trackerConns ),
    m_pModel( MainController::GetModel() ),
    m_pConnectionController( pConnectionController )

{
    ui->setupUi( this );
    SetData();

    connect( ui->pushButtonSave, &QPushButton::clicked, this, &ConnectionWidget::SaveChanges );
    connect( ui->pushButtonSaveAsNewConn, &QPushButton::clicked, this, &ConnectionWidget::SaveAsNewConnection );
    connect( ui->pushButtonDeleteConn, &QPushButton::clicked, this, &ConnectionWidget::DeleteConnection );
    connect( ui->pushButtonReset, &QPushButton::clicked, this, &ConnectionWidget::SetData );
    connect( ui->lineEditLogIssue, &QLineEdit::textChanged, this, &ConnectionWidget::CheckIfNumber );
    connect( m_pConnectionController, &ConnectionController::RequestConnectionExists, this, &ConnectionWidget::ConnectionExists );
    ui->pushButtonAddFieldMapping->setDisabled( true );
    if ( m_index == QModelIndex() )
    {
        ui->pushButtonSave->setDisabled( true );
        // ui->pushButtonSaveAsNewConn->setDisabled(true);
        ui->pushButtonDeleteConn->setDisabled( true );
    }
}

ConnectionWidget::~ConnectionWidget()
{
    delete ui;
    delete m_pConnectionController;
}

void ConnectionWidget::CheckIfNumber()     // ToDo: replace with  inputMask() and/or validator()
{
    std::regex numbers( "[0-9]*" );
    if ( std::regex_match( ui->lineEditLogIssue->text().toStdString(), numbers ) )
        m_pConnectionItem->GetTrackerConn().jiraLogIssueID = ui->lineEditLogIssue->text().toInt();
    else
        QMessageBox::information( this, "Log-ID entry", "only integers" );
}

void ConnectionWidget::SetData() const
{
    std::cout << "\n in setdata: " << m_pConnectionItem->GetTrackerConn().jiProj << "\n";
    ui->lineEditJiProj->setText( QString::fromStdString( m_pConnectionItem->GetTrackerConn().jiProj ) );
    ui->lineEditJiEpic->setText( QString::fromStdString( m_pConnectionItem->GetTrackerConn().jiEpic ) );
    ui->lineEditBbRepo->setText( QString::fromStdString( m_pConnectionItem->GetTrackerConn().bbRepo ) );
    ui->comboBoxIssToJi->setCurrentIndex( m_pConnectionItem->GetTrackerConn().newIssuesToJira );
    ui->comboBoxIssToBitb->setCurrentIndex( m_pConnectionItem->GetTrackerConn().newIssuesToBitbucket );
    ui->comboBoxCommToJi->setCurrentIndex( m_pConnectionItem->GetTrackerConn().commentsToJira );
    ui->comboBoxCommToBitb->setCurrentIndex( m_pConnectionItem->GetTrackerConn().commentsToBitbucket );
    ui->comboBoxDestrOnJi->setCurrentIndex( m_pConnectionItem->GetTrackerConn().destructiveOnJira );
    ui->comboBoxDestrOnBitb->setCurrentIndex( m_pConnectionItem->GetTrackerConn().destructiveOnBitbucket );
    ui->lineEditLogIssue->setText( QString::number( m_pConnectionItem->GetTrackerConn().jiraLogIssueID ) );
}

const TrackerConn ConnectionWidget::CreateTrackerConn() const
{
    int id = -1;
    if ( m_pConnectionItem != nullptr )
        id = m_pConnectionItem->GetTrackerConn().id;
    return TrackerConn( {
        std::to_string( id ),
        ui->lineEditJiProj->text().toStdString(),
        ui->lineEditJiEpic->text().toStdString(),
        ui->lineEditBbRepo->text().toStdString(),
        std::to_string( ui->comboBoxCommToJi->currentIndex() ),
        std::to_string( ui->comboBoxCommToBitb->currentIndex() ),
        std::to_string( ui->comboBoxIssToJi->currentIndex() ),
        std::to_string( ui->comboBoxIssToBitb->currentIndex() ),
        std::to_string( ui->comboBoxDestrOnJi->currentIndex() ),
        std::to_string( ui->comboBoxDestrOnBitb->currentIndex() ),
        ui->lineEditLogIssue->text().toStdString(),
    } );
}

void ConnectionWidget::SaveChanges() const
{
    TrackerConn trackerConn = CreateTrackerConn();
    m_pConnectionController->SaveChanges( trackerConn, m_pConnectionItem->GetTrackerConn().id );
}

void ConnectionWidget::SaveAsNewConnection() const
{
    TrackerConn trackerConn = CreateTrackerConn();
    m_pConnectionController->SaveAsNewConnection( trackerConn, m_pConnectionItem->GetTrackerConn().id );
}

void ConnectionWidget::DeleteConnection() const
{
    m_pConnectionController->DeleteConnection( m_pConnectionItem->GetTrackerConn().id );
    if ( !m_pModel->hasChildren() )
        ui->pushButtonSave->setDisabled( true );
}

void ConnectionWidget::ConnectionExists( int& id )
{
    QMessageBox::information( this, "existing connection", QString::fromStdString( "connection already exists (ID = " + std::to_string( id ) + ")" ) );
}
