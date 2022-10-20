#include "mainwindow.h"

#include <QLayout>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent){
    std::srand(time(NULL));

    toolBar = new ToolBar(this);
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    if (event->spontaneous() && SELECTED_FIGURE != nullptr) {
        SELECTED_FIGURE->deselect();
    }

    if (SELECTED_FIGURE == nullptr) {
        toolBar->setDisabledOfButtonDelete(true);
    } else {
        toolBar->setDisabledOfButtonDelete(false);
    }
}

void MainWindow::resizeEvent(QResizeEvent *event) {
    Q_UNUSED(event)
    toolBar->resize(this->width(),30);
    toolBar->repaint();
}
