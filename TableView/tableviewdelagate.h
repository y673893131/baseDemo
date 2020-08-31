#ifndef TABLEVIEWDELAGATE_H
#define TABLEVIEWDELAGATE_H

#include <QObject>
#include <QStyledItemDelegate>

class TableViewDelagate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit TableViewDelagate(QObject* parent = nullptr);

    // QAbstractItemDelegate interface
public:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
private:
    void paintButton(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void paintHyperLink(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void paintProgress(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    // QAbstractItemDelegate interface
public:
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
private slots:
    void commit();
};

#endif // TABLEVIEWDELAGATE_H
