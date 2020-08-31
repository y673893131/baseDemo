#include "widget.h"
#include "tableview.h"
#include <QBoxLayout>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    resize(1200, 600);
    auto table = new TableView(this);
    auto layout = new QHBoxLayout(this);
    layout->setMargin(10);
    layout->addWidget(table);
}

Widget::~Widget()
{
}

