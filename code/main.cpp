#include "gui.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Gui w;
    w.showFullScreen();
    return a.exec();
}
