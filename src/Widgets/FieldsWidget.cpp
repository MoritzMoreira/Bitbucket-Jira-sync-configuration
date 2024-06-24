#include <qmessagebox.h>

#include "../Data/MainController.h"
#include "FieldsWidget.h"
#include "../ui_fields.h"

FieldsWidget::FieldsWidget( FieldItem* pFieldItem, FieldsController* pFieldsController, QWidget* parent ) :
    QWidget( parent ),
    ui( new Ui::FieldsForm ),
    m_pFieldItem( pFieldItem ),
    m_fields( MainController::GetTables().fields ),
    m_fieldMaps( MainController::GetTables().fieldMaps ),
    m_pFieldsController( pFieldsController )
{
    ui->setupUi( this );
    SetData();
    connect( ui->pushButtonSaveChanges, &QPushButton::clicked, this, &FieldsWidget::SaveChanges );
    connect( ui->pushButtonReset, &QPushButton::clicked, this, &FieldsWidget::SetData );
    connect( ui->comboBoxFromSys, &QComboBox::currentTextChanged, this, &FieldsWidget::SwapSystem );
}

FieldsWidget::FieldsWidget() :
    m_fields( MainController::GetTables().fields ),
    m_fieldMaps( MainController::GetTables().fieldMaps )
{}

FieldsWidget::~FieldsWidget()
{
    delete ui;
}

void FieldsWidget::SwapSystem() const
{
    if ( ui->comboBoxFromSys->currentText() == "Jira" )
        ui->labelToSysVar->setText( "Bitbucket" );
    else
        ui->labelToSysVar->setText( "Jira" );
    ui->comboBoxFromPath->clear();
    ui->comboBoxToPath->clear();
    FillPathComboboxes();
}

unsigned int FieldsWidget::CreateUniqueID() const
{
    unsigned int ID = m_fields.size() == 0 ? 0 : m_fields.back().id + 1;
    while ( true )
    {
        if ( ui->comboBoxFromPath->findData( ID ) == -1 )
            return ID;
        ID++;
    }
}

void FieldsWidget::FillPathComboboxes() const
{
    if ( ui->comboBoxFromSys->currentText() == "Jira" )
    {
        for ( Field pField : m_fields )
        {
            if ( pField.system == "ji" )
                ui->comboBoxFromPath->addItem( QString::fromStdString( pField.path ), pField.id );
            else
                ui->comboBoxToPath->addItem( QString::fromStdString( pField.path ), pField.id );
        }
    }
    else
    {
        for ( Field pField : m_fields )
        {
            if ( pField.system == "bb" )
                ui->comboBoxFromPath->addItem( QString::fromStdString( pField.path ), pField.id );
            else
                ui->comboBoxToPath->addItem( QString::fromStdString( pField.path ), pField.id );
        }
    }
}

void FieldsWidget::SetData() const
{
    if ( m_pFieldItem->GetFieldFrom().system == "bb" )
    {
        ui->comboBoxFromSys->setCurrentText( "Bitbucket" );
        ui->labelToSysVar->setText( "Jira" );
    }
    FillPathComboboxes();
    ui->comboBoxFromPath->setCurrentText( QString::fromStdString( m_pFieldItem->GetFieldFrom().path ) );
    ui->comboBoxToPath->setCurrentText( QString::fromStdString( m_pFieldItem->GetFieldTo().path ) );
}

void FieldsWidget::MappingExists( int& id )
{
    QMessageBox::information( this, "existing field mapping", QString::fromStdString( "field mapping already exists (ID = " + std::to_string( id ) + ")" ) );
}

void FieldsWidget::SaveChanges() 
{
    SaveNewFields();    // Fields needs to be saved first to create the IDs as Data of ComboBoxItem
    Field fromField = Field( { "0", ui->comboBoxFromSys->currentText().toStdString(), ui->comboBoxFromPath->currentText().toStdString() } );
    Field toField = Field( { "0", ui->labelToSysVar->text().toStdString(), ui->comboBoxToPath->currentText().toStdString() } );
    std::vector<int> resultOfSaveChanges = m_pFieldsController->SaveChanges( fromField, toField, m_pFieldItem->GetFieldMap()->id );
    if ( resultOfSaveChanges.size() == 1 )
    {
        if ( resultOfSaveChanges.front() >= 0 )
            MappingExists( resultOfSaveChanges.front() );
    }
    else
    {
        m_pSaveOptionDia = new SaveOptionDia( this );
        m_pSaveOptionDia->SetConnectionIDLabel( resultOfSaveChanges );
        ui->SaveOptionLayout->addWidget( m_pSaveOptionDia );
        connect( m_pSaveOptionDia->GetUi()->pushButtonNo, &QPushButton::clicked, this, [ & ]
                 {
                     m_pFieldsController->SaveOnlyToCurrentConn();
                     ui->SaveOptionLayout->removeWidget( m_pSaveOptionDia );
                     m_pSaveOptionDia->deleteLater();
                 } );
        connect( m_pSaveOptionDia->GetUi()->pushButtonYes, &QPushButton::clicked, this, [ & ]
                 {
                     m_pFieldsController->SaveChangesToAll();
                     ui->SaveOptionLayout->removeWidget( m_pSaveOptionDia );
                     m_pSaveOptionDia->deleteLater();
                 } );
    }
}

void FieldsWidget::SaveNewFields() const
{
    QString currentText = ui->comboBoxFromPath->currentText();
    int idField = ui->comboBoxFromPath->findText( currentText );
    int ID = CreateUniqueID();
    if ( idField == -1 )
    {
        ui->comboBoxFromPath->addItem( currentText, ID );
        ui->comboBoxFromPath->setCurrentIndex( ui->comboBoxFromPath->count() - 1 );
        m_fields.emplace_back( Field( { std::to_string( ID ),
                                        ui->comboBoxFromSys->currentText() == "Jira" ? "ji" : "bb",
                                        ui->comboBoxFromPath->currentText().toStdString() } ) );
    }

    ID = CreateUniqueID();
    currentText = ui->comboBoxToPath->currentText();
    idField = ui->comboBoxToPath->findText( currentText );
    if ( idField == -1 )
    {
        ui->comboBoxToPath->addItem( currentText, ID );
        ui->comboBoxToPath->setCurrentIndex( ui->comboBoxToPath->count() - 1 );
        m_fields.emplace_back( Field( { std::to_string( ID ),
                                        ui->labelToSysVar->text() == "Jira" ? "ji" : "bb",
                                        ui->comboBoxToPath->currentText().toStdString() } ) );
    }
}

void FieldsWidget::SaveAsNewMapping() const
{
    SaveNewFields();
    Field fromField = Field( { "0", ui->comboBoxFromSys->currentText().toStdString(), ui->comboBoxFromPath->currentText().toStdString() } );
    Field toField = Field( { "0", ui->labelToSysVar->text().toStdString(), ui->comboBoxToPath->currentText().toStdString() } );
    m_pFieldsController->SaveAsNewMapping( fromField, toField, m_pFieldItem->GetFieldMap()->id );
}

// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

SaveOptionDia::SaveOptionDia( QWidget* parent ) :
    QDialog( parent ),
    ui( new Ui::SaveDialog )
{
    ui->setupUi( this );
}

SaveOptionDia::~SaveOptionDia()
{
    delete ui;
}

const Ui::SaveDialog* SaveOptionDia::GetUi() const
{
    return ui;
}

void SaveOptionDia::SetConnectionIDLabel( const std::vector<int>& connIDsWithMapping ) const
{
    std::string pluralS = connIDsWithMapping.size() > 2 ? "s" : "";
    std::string idList;
    for ( int id : connIDsWithMapping )
    {
        if ( id != connIDsWithMapping.front() )
        {
            if ( id == connIDsWithMapping.back() )
            {
                std::cout << "\n id: " << id << "\n";
                idList += std::to_string( id );
            }
            else
                idList += std::to_string( id ) + ", ";
        }
    }
    std::cout << "\n id: " << idList << "\n";
    std::string connectionIDsLabel = std::to_string( connIDsWithMapping.size() - 1 ) + " connection" + pluralS +
                                     " (ID" + pluralS + " " + idList + ").";
    ui->label_ConnectionIDs->setText( QString::fromStdString( connectionIDsLabel ) );
    std::cout << connIDsWithMapping.front();
}
