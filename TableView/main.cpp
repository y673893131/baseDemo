#include "widget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("tableview delegate");
    Widget w;
    w.show();
    return a.exec();
}
