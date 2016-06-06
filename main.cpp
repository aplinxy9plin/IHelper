#include "ihelper.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    IHelper w;
    w.show();
    w.setWindowIcon(QIcon(":/ico.ico"));
    return a.exec();
}
