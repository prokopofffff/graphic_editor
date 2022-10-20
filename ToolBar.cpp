#include "ToolBar.h"

ToolBar::ToolBar(QMainWindow *parent) : QToolBar(parent) {
    isSelectedFigure1 = false;
    isSelectedFigure2 = false;

    buttonFigure1 = new QPushButton("Фигура №1");
    buttonFigure2 = new QPushButton("Фигура №2");
    buttonAdd = new QPushButton("Добавить");
    buttonDelete = new QPushButton("Удалить");
    buttonDeleteAll = new QPushButton("Удалить все");

    buttonFigure1->setCheckable(true);
    buttonFigure2->setCheckable(true);

    buttonAdd->setDisabled(true);
    buttonDelete->setDisabled(true);
    buttonDeleteAll->setDisabled(true);

    connect(buttonFigure1, SIGNAL(clicked()), this, SLOT(chooseFigure1()));
    connect(buttonFigure2, SIGNAL(clicked()), this, SLOT(chooseFigure2()));
    connect(buttonAdd, SIGNAL(clicked()), this, SLOT(addFigure()));
    connect(buttonDelete, SIGNAL(clicked()), this, SLOT(deleteFigure()));
    connect(buttonDeleteAll, SIGNAL(clicked()), this, SLOT(deleteAllFigures()));

    this->resize(800,30);
    this->setMovable(false);
    this->addWidget(buttonFigure1);
    this->addWidget(buttonFigure2);
    this->addSeparator();
    this->addWidget(buttonAdd);
    this->addWidget(buttonDelete);
    this->addWidget(buttonDeleteAll);

    this->show();
}

void ToolBar::chooseFigure1() {
    isSelectedFigure2 = false;
    if (isSelectedFigure1) {
        buttonFigure1->setChecked(false);
        isSelectedFigure1 = false;
    } else {
        buttonFigure2->setChecked(false);
        isSelectedFigure1 = true;
    }
    if (!isSelectedFigure1 && !isSelectedFigure2) {
        buttonAdd->setDisabled(true);
    } else {
        buttonAdd->setDisabled(false);
    }
}
void ToolBar::chooseFigure2() {
    isSelectedFigure1 = false;
    if (isSelectedFigure2) {
        buttonFigure2->setChecked(false);
        isSelectedFigure2 = false;
    } else {
        buttonFigure1->setChecked(false);
        isSelectedFigure2 = true;
    }
    if (!isSelectedFigure1 && !isSelectedFigure2) {
        buttonAdd->setDisabled(true);
    } else {
        buttonAdd->setDisabled(false);
    }
}

void ToolBar::addFigure() {
    if (isSelectedFigure1) {
        Figure1* newFigure = new Figure1(parentWidget());
        newFigure->lower();
        newFigure->show();
        vec.push_back(newFigure);
        buttonDeleteAll->setDisabled(false);
        return;
    }
    if (isSelectedFigure2) {
        Figure2* newFigure = new Figure2(parentWidget());
        newFigure->lower();
        newFigure->show();
        vec.push_back(newFigure);
        buttonDeleteAll->setDisabled(false);
        return;
    }
}

void ToolBar::deleteFigure() {
    vec.erase(std::find(vec.begin(), vec.end(), SELECTED_FIGURE));

    delete SELECTED_FIGURE;
    SELECTED_FIGURE = nullptr;
    buttonDelete->setDisabled(true);
    if(vec.size() == 0) buttonDeleteAll->setDisabled(true);
}

void ToolBar::deleteAllFigures(){
    for(int i = 0; i < vec.size(); i++){
        delete vec[i];
    }
    vec.clear();
    buttonDelete->setDisabled(true);
    buttonDeleteAll->setDisabled(true);
}
