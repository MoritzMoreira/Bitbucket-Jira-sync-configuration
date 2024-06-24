#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeView>

#include "DetailsContainer.h"

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow( QWidget* parent = 0 );
    ~MainWindow();

    void RemoveWidget() const;

public slots:
    void UpdateView();
    void ShowDetails();

private:
    Ui::MainWindow* ui;
    QTreeView treeView;
    QModelIndex m_selectedIndex = QModelIndex();
    Tables& m_tables;
    DetailsContainer* m_pDetailsContainer;
};

#endif // MAINWINDOW_H Done")
