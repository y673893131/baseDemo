#include "tableviewmodel.h"
#include <QDebug>
#include <QIcon>

#define TEST_DATA_

TableViewModel::TableViewModel(QObject *parent)
    : QAbstractTableModel(parent)
{
#ifdef TEST_DATA_
    m_heads << "text" << "icon" << "combobox" << "checkBox" << "lineedit" << "edit" << "button" << "hyperLink" << "slider" << "spinbox" << "progress";
    CellType type[] = {Type_Text, Type_Icon, Type_Combobox, Type_CheckBox, Type_LineEdit, Type_Edit, Type_Button, Type_HyperLink, Type_Slider, Type_SpinBox, Type_Progress};
    for(int i = 0; i < 100; ++i)
    {
        RowItem row;
        QStringList data;
        for(int n = 0; n < m_heads.size(); ++n)
        {
            CellItem* cell;

            if(type[n] == Type_CheckBox)
                cell = new CellItem_CheckBox();
            else if(isRangeType(type[n]))
            {
                cell = new CellItem_Range();
                reinterpret_cast<CellItem_Range*>(cell)->value = (i + 1);
            }
            else if(type[n] == Type_HyperLink)
                cell = new CellItem_HyperLink(QString("www.test%1.com").arg(i));
            else
                cell = new CellItem();

            cell->type = type[n];
            if(cell->type == Type_Icon)
                cell->text = ":/icon/ac" + QString::number(i % 2);
            else if(isRangeType(type[n])) cell->text = QString::number(reinterpret_cast<CellItem_Range*>(cell)->value);
            else
                cell->text = m_heads[n] + ":" + QString::number(i);

            row.push_back(cell);
        }

        m_datas.push_back(row);
    }
    qDebug() << m_heads.size() << sizeof(type)/sizeof(type[0]);

#endif
}

QVariant TableViewModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    // FIXME: Implement me!
    if(role == Qt::DisplayRole && orientation == Qt::Orientation::Horizontal)
    {
        if(section < m_heads.size())
            return m_heads[section];
    }

    return QVariant();
}

int TableViewModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    // FIXME: Implement me!
    return m_datas.size();
}

int TableViewModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    // FIXME: Implement me!
    return m_heads.size();
}

QVariant TableViewModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    if(index.row() >= m_datas.size() || index.column() >= m_datas[index.row()].size())
        return QVariant();
    int type = 0;
    if(role != UserRole_Type )
        type = index.data(UserRole_Type).toInt();
    switch (role) {
    case Qt::DecorationRole:
        if(type == Type_Icon)
            return QIcon(m_datas[index.row()][index.column()]->text);
        break;
    case Qt::DisplayRole:
        return m_datas[index.row()][index.column()]->text;
    case Qt::UserRole:
        break;
    case Qt::CheckStateRole:
        if(type == Type_CheckBox)
            return reinterpret_cast<CellItem_CheckBox*>(m_datas[index.row()][index.column()])->checkState;
        break;
    case Qt::TextAlignmentRole:
        return m_datas[index.row()][index.column()]->align;
    case UserRole_Type:
        return m_datas[index.row()][index.column()]->type;
    case UserRole_Range_Min:
        if(isRangeType(type))
            return reinterpret_cast<CellItem_Range*>(m_datas[index.row()][index.column()])->min;
        break;
    case UserRole_Range_Max:
        if(isRangeType(type))
            return reinterpret_cast<CellItem_Range*>(m_datas[index.row()][index.column()])->max;
        break;
    case UserRole_Range_Value:
        if(isRangeType(type))
            return reinterpret_cast<CellItem_Range*>(m_datas[index.row()][index.column()])->value;
        break;
    case UserRole_HyperLink_Value:
        if(type == Type_HyperLink)
            return reinterpret_cast<CellItem_HyperLink*>(m_datas[index.row()][index.column()])->sLinkUrl;
        break;
    }

    return QVariant();
}

QStringList TableViewModel::heads()
{
    return m_heads;
}

void TableViewModel::setHeads(const QStringList &heads)
{
    m_heads = heads;
}

void TableViewModel::setDatas(QVector<RowItem> &data)
{
    for(auto it : m_datas)
        for(auto cell : it)
            delete cell;
    m_datas.swap(data);
}

bool TableViewModel::isRangeType(int type) const
{
    return type >= Type_Slider && type <= Type_Progress;
}

Qt::ItemFlags TableViewModel::flags(const QModelIndex &index) const
{
    auto type = index.data(UserRole_Type).toInt();
    switch(type)
    {
    case Type_Default:
        return QAbstractTableModel::flags(index);
    }

    return (QAbstractTableModel::flags(index) | Qt::ItemIsEditable);
}


bool TableViewModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    int type = index.data(UserRole_Type).toInt();
    switch(role)
    {
    case Qt::DisplayRole:
        if(index.row() < m_datas.size() && index.column() < m_datas[index.row()].size())
        {
            m_datas[index.row()][index.column()]->text = value.toString();
            return true;
        }
        break;
    case Qt::CheckStateRole:
        if(index.row() < m_datas.size() && index.column() < m_datas[index.row()].size())
        {
            reinterpret_cast<CellItem_CheckBox*>(m_datas[index.row()][index.column()])->checkState = value.value<Qt::CheckState>();
            return true;
        }
        break;
    case UserRole_Range_Value:
        if(index.row() < m_datas.size() && index.column() < m_datas[index.row()].size() && isRangeType(type))
        {
            reinterpret_cast<CellItem_Range*>(m_datas[index.row()][index.column()])->value = value.toInt();
#ifdef TEST_DATA_
            for(int n = 1; n <= 2; ++n)
            {
                QModelIndex i = this->index(index.row(), index.column() + n);
                if(!i.isValid()) break;
                reinterpret_cast<CellItem_Range*>(m_datas[i.row()][i.column()])->value = value.toInt();
                reinterpret_cast<CellItem_Range*>(m_datas[i.row()][i.column()])->text = value.toString();
                emit dataChanged(i,i);
            }

#endif
        }
        break;
    }
    return QAbstractTableModel::setData(index, value, role);
}
