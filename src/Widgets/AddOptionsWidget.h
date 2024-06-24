#pragma once

#include <QWidget>

#include "../Data/DerivedItems.h"

namespace Ui
{
    class AddOptionsForm;
}

class AddOptionsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AddOptionsWidget( AdditionalOptionItem*, const std::vector<FOpt>&, QWidget* parent = 0 );
    ~AddOptionsWidget();

    void SetData() const;

public slots:
    void EditAddOpt( const int& ) const;

signals:
    void RequestUpdateView() const;

private:
    Ui::AddOptionsForm* ui;
    AdditionalOptionItem* m_pAddOptionItem;
    std::vector<FOpt> m_options;
};
