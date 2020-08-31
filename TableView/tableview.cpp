#include "tableview.h"
#include "tableviewmodel.h"
#include "tableviewdelagate.h"
#include <QHeaderView>
#include <QMouseEvent>
#include <QItemSelectionModel>
#include <QDebug>
TableView::TableView(QWidget* parent)
    :QTableView(parent)
{
    setMouseTracking(true);
    setEditTriggers(CurrentChanged);

    setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
    setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);

    horizontalHeader()->setStretchLastSection(true);
    verticalHeader()->hide();
    auto model = new TableViewModel(this);
    setModel(model);
    auto delegate = new TableViewDelagate(this);
    setItemDelegate(delegate);

    connect(this, &TableView::clicked, this, [this](const QModelIndex &index){
        if(index.data(TableViewModel::UserRole_Type) != Type_Default)
            edit(index);
    });
}


void TableView::mouseMoveEvent(QMouseEvent *event)
{
    auto index = indexAt(event->pos());
    if(index.isValid())
    {
        auto type = index.data(TableViewModel::UserRole_Type).toInt();
        if(type == Type_Button || type == Type_HyperLink)
            setCursor(Qt::PointingHandCursor);
        else
            setCursor(Qt::ArrowCursor);
    }else
        setCursor(Qt::ArrowCursor);
    QTableView::mouseMoveEvent(event);
}


void TableView::mousePressEvent(QMouseEvent *event)
{
    QTableView::mousePressEvent(event);
}
