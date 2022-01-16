#include "mainwindow.hh"
#include <QObject>
#include "gameengine.hh"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    GameEngine *engine = new GameEngine;
    MainWindow w(engine);

    w.show();
    return a.exec();
}
