#pragma once

#include <QVariant>

enum class EitemType
{
    tree, connection, field, option, addoption
};

class TreeItem
{
public:
    explicit TreeItem( const std::vector<std::string>& data = {}, TreeItem* parentItem = nullptr );
    virtual ~TreeItem();

    void appendChild( TreeItem* child );
    TreeItem* child( int row );
    int childCount() const;
    int columnCount() const;
    QVariant data( int column ) const;
    int row() const;
    TreeItem* parentItem();
    virtual const EitemType GetItemType() const = 0;
    bool removeChildren( int position, int count );

protected:
    std::vector<std::string> m_itemData;
    QVector<TreeItem*> m_childItems;

private:
    TreeItem* m_parentItem;
};
