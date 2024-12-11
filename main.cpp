#include "screen.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    //qDebug() << "Launched";
    QApplication a(argc, argv);
    Screen w;
    w.show();

    return a.exec();
}
