#ifndef TABLEVIEWMODEL_H
#define TABLEVIEWMODEL_H

#include <QAbstractTableModel>

enum CellType
{
    Type_Default = 0,
    Type_Text = Type_Default,
    Type_LineEdit,
    Type_Edit,
    Type_DateEdit,
    Type_TimeEdit,
    Type_DataTimeEdit,
    Type_Combobox,
    Type_CheckBox,
    Type_Button,
    Type_Slider,
    Type_SpinBox,
    Type_Progress,
    Type_Icon,
    Type_HyperLink,
};

struct CellItem
{
    CellItem()
        :type(Type_Default),  align(Qt::AlignCenter)
    {

    }
    QString text;
    CellType type;

    Qt::AlignmentFlag align;
};

struct CellItem_CheckBox : public CellItem
{
    CellItem_CheckBox()
        :checkState(Qt::Unchecked)
    {
    }

    Qt::CheckState checkState;
};

struct CellItem_Range : public CellItem
{
    CellItem_Range()
        :min(0), max(100), value(50)
    {
    }

    int min;
    int max;
    int value;
};

struct CellItem_HyperLink : public CellItem
{
    CellItem_HyperLink(){}
    CellItem_HyperLink(const QString& sUrl)
        :sLinkUrl((sUrl))
    {

    }
    QString sLinkUrl;
};

typedef QVector<CellItem*> RowItem;

class TableViewModel : public QAbstractTableModel
{
    Q_OBJECT

    Q_PROPERTY(QStringList heads READ heads WRITE setHeads NOTIFY headsChanged)

signals:
    void headsChanged(const QStringList&);
public:
    enum UserRoleType
    {
        UserRole_Type = Qt::UserRole + 1,
        UserRole_Range_Min,
        UserRole_Range_Max,
        UserRole_Range_Value,
        UserRole_HyperLink_Value
    };

public:
    explicit TableViewModel(QObject *parent = nullptr);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    Qt::ItemFlags flags(const QModelIndex &index) const;

    QStringList heads();
    void setHeads(const QStringList&);

    void setDatas(QVector<RowItem>& data);

private:
    bool isRangeType(int type) const;

private:
    QStringList m_heads;
    QVector<RowItem> m_datas;

    // QAbstractItemModel interface
public:

};

#endif // TABLEVIEWMODEL_H
