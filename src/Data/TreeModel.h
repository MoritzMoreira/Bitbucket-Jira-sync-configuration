#pragma once

#include <QModelIndex>

#include "DerivedItems.h"

class TreeItem;

class TreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit TreeModel( Tables*, QObject* parent = nullptr );
    ~TreeModel();

    QVariant data( const QModelIndex& index, int role ) const override;
    TreeItem* GetItem( const QModelIndex& index, int role ) const;
    Qt::ItemFlags flags( const QModelIndex& index ) const override;
    QVariant headerData( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const override;
    QModelIndex index( int row, int column, const QModelIndex& parent = QModelIndex() ) const override;
    QModelIndex parent( const QModelIndex& index ) const override;
    int rowCount( const QModelIndex& parent = QModelIndex() ) const override;
    int columnCount( const QModelIndex& parent = QModelIndex() ) const override;
    bool insertRows( int position, int rows, const QModelIndex& parent = {} ) override;
    bool removeRows( int position, int rows, const QModelIndex& parent = {} ) override;

    void GenerateConnectionItems( TreeItem* pParent ) const;
    void GenerateFieldItems( ConnectionItem* pConnectionItem ) const;
    void GenerateOptionItems( ConnectionItem* pConnectionItem, FieldItem* pFieldItem ) const;
    void GenerateAdditionalOptionItems( ConnectionItem* pConnectionItem, OptionItem* pOptionItem ) const;
    TreeItem* GetRootItem() const;

    

protected:
    TreeItem* m_pRootItem;

private:
    void setupModelData( TreeItem* parent );
    Tables* m_data;
    static std::map<std::string, std::string> m_addOpts;
    std::vector<std::string> m_fieldVecFrom;
    std::vector<std::string> m_fieldVecTo;
};

std::string& operator+( std::string& sL, std::string& sR );

