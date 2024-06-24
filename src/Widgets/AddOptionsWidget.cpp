#include "AddOptionsWidget.h"
#include "../ui_addoptions.h"

AddOptionsWidget::AddOptionsWidget( AdditionalOptionItem* pAddOptionItem,
                                    const std::vector<FOpt>& options,
                                    QWidget* parent ) :
    QWidget( parent ),
    ui( new Ui::AddOptionsForm ),
    m_pAddOptionItem( pAddOptionItem ),
    m_options( options )
{
    ui->setupUi( this );
    SetData();
    connect( ui->comboBoxAddOpt, SIGNAL( activated( const int& ) ), this, SLOT( EditAddOpt( const int& ) ) );
}

AddOptionsWidget::~AddOptionsWidget()
{
    delete ui;
}

void AddOptionsWidget::SetData() const
{
    QStringList addOptions;
    for ( FOpt pOption : m_options )
        addOptions.append( QString::fromStdString( pOption.opt ) );
    ui->comboBoxAddOpt->addItems( addOptions );
    ui->comboBoxAddOpt->setCurrentText( QString::fromStdString( m_pAddOptionItem->GetAddOpt().opt ) );
}

void AddOptionsWidget::EditAddOpt( const int& ) const
{
    // m_pFieldItem->GetFieldMap()->fieldIDfrom = ui->comboBoxFromSys->currentIndex;
}

