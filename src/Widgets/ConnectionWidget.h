#pragma once

#include "../Data/ConnectionController.h"

namespace Ui
{
    class ConnectionsForm;
}

class ConnectionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ConnectionWidget( ConnectionItem* pConnectionItem,
                               const QModelIndex& index,
                               ConnectionController* pConnectionController,
                               QWidget* parent = 0 );
    ~ConnectionWidget();

    void SetData() const;
    const TrackerConn CreateTrackerConn() const;
    void ConnectionExists( int& id );

public slots:
    void SaveChanges() const;
    void SaveAsNewConnection() const;
    void DeleteConnection() const;
    void CheckIfNumber();

signals:
    void RequestUpdateView() const;

private:
    Ui::ConnectionsForm* ui;
    ConnectionItem* m_pConnectionItem;
    std::vector<TrackerConn>& m_trackerConns;
    QModelIndex m_index;
    TreeModel* m_pModel;
    ConnectionController* m_pConnectionController;
};
