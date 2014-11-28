#include "mostrador.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Mostrador w;
    w.show();

    return a.exec();
}
