#include "handwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    HandWindow w;
    w.show();

    return a.exec();
}
