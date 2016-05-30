#include "TipWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    TipWindow w;
    w.init();
    w.show();

    return a.exec();
}
