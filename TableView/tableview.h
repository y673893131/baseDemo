#ifndef TABLEVIEW_H
#define TABLEVIEW_H

#include <QTableView>

class TableView : public QTableView
{
public:
    explicit TableView(QWidget* parent = nullptr);

    // QWidget interface
protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
};

#endif // TABLEVIEW_H
