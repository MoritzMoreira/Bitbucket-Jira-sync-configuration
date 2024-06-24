#include "OptionsWidget.h"
#include "../Data/MainController.h"
#include "../ui_options.h"

OptionsWidget::OptionsWidget( OptionItem* pOptionItem, const std::vector<FOpt>& options, QWidget* parent ) :
    QWidget( parent ),
    ui( new Ui::OptionsForm ),
    m_pOptionItem( pOptionItem ),
    m_options( options )
{
    ui->setupUi( this );
    SetData();
    connect( ui->comboBoxFromSysOption, SIGNAL( activated( const int& ) ), this, SLOT( EditFromSys( const int& ) ) );
    connect( ui->pushButtonSaveChanges, &QPushButton::clicked, this, &OptionsWidget::SaveChanges );
    ui->pushButtonSaveChanges->setDisabled( true );
}

OptionsWidget::~OptionsWidget()
{
    delete ui;
}

void OptionsWidget::SetData()
{
    ui->labelFromSys->setText(
        QString::fromStdString( m_pOptionItem->GetFieldFrom().system == "ji" ? "Jira" : "Bitbucket" ) );
    ui->labelFromPath->setText( QString::fromStdString( m_pOptionItem->GetFieldFrom().path ) );
    ui->labelToSys->setText(
        QString::fromStdString( m_pOptionItem->GetFieldTo().system == "ji" ? "Jira" : "Bitbucket" ) );
    ui->labelToPath->setText( QString::fromStdString( m_pOptionItem->GetFieldTo().path ) );

    QStringList fromOptions;
    QStringList toOptions;
    QStringList conditionOptions = { "" };
    for ( FOpt pOption : m_options )
    {
        if ( pOption.fieldID == m_pOptionItem->GetFieldFrom().id )
            fromOptions.append( QString::fromStdString( pOption.opt ) );
        if ( pOption.fieldID == m_pOptionItem->GetFieldTo().id )
            toOptions.append( QString::fromStdString( pOption.opt ) );
        const int conditionalOptID = 4;
        if ( pOption.fieldID == conditionalOptID )
            conditionOptions.append( QString::fromStdString( pOption.opt ) );
    }
    ui->comboBoxFromSysOption->addItems( fromOptions );
    ui->comboBoxFromSysOption->setCurrentText( QString::fromStdString( m_pOptionItem->GetFOptFrom().opt ) );
    ui->comboBoxToSysOption->addItems( toOptions );
    ui->comboBoxToSysOption->setCurrentText( QString::fromStdString( m_pOptionItem->GetFOptTo().opt ) );
    ui->comboBoxConditionalOption->addItems( conditionOptions );
    if ( m_pOptionItem->GetFOptCondition().opt != "" )
        ui->comboBoxConditionalOption->setCurrentText(
            QString::fromStdString( m_pOptionItem->GetFOptCondition().opt ) );
    else
        ui->comboBoxConditionalOption->setCurrentText( "" );

    /*for (FOpt* pOption : m_options)
    {
        if (pField->system == "ji")
            jiraFields.append(QString::fromStdString(pField->path));
        else
            bitbucketFields.append(QString::fromStdString(pField->path));
    }*/
}

void OptionsWidget::SaveChanges()
{
    MainController::UpdateModel();
    emit RequestUpdateView();
}

void OptionsWidget::EditFromSys( const int& )
{
    // m_pFieldItem->GetFieldMap()->fieldIDfrom = ui->comboBoxFromSys->currentIndex;
}
