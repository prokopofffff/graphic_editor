#include <QApplication>
#include <vector>

class Figure;
Figure* SELECTED_FIGURE;
std::vector<Figure*> vec;

#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.resize(800,800);
    w.show();
    return a.exec();
}
