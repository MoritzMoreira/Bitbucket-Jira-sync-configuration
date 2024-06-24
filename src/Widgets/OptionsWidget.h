#pragma once

#include "../Data/DerivedItems.h"
#include <QWidget>

namespace Ui
{
    class OptionsForm;
}

class OptionsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit OptionsWidget( OptionItem* pOptionItem, const std::vector<FOpt>& options, QWidget* parent = 0 );
    ~OptionsWidget();

    void SetData();

public slots:
    void EditFromSys( const int& );
    void SaveChanges();

signals:
    void RequestUpdateView() const;

private:
    Ui::OptionsForm* ui;
    OptionItem* m_pOptionItem;
    std::vector<FOpt> m_options;
};
