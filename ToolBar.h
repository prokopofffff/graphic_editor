#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QMainWindow>
#include <QToolBar>
#include <QPushButton>

#include "Figure.h"

class ToolBar : public QToolBar {
    Q_OBJECT

public:
    explicit ToolBar(QMainWindow *parent = nullptr);

    void setDisabledOfButtonDelete (bool isDisabled) {
        buttonDelete->setDisabled(isDisabled);
    }

private slots:
    void chooseFigure1();
    void chooseFigure2();
    void addFigure();
    void deleteFigure();
    void deleteAllFigures();

private:
    bool isSelectedFigure1;
    bool isSelectedFigure2;
    QPushButton* buttonFigure1;
    QPushButton* buttonFigure2;
    QPushButton* buttonAdd;
    QPushButton* buttonDelete;
    QPushButton* buttonDeleteAll;
};

#endif // TOOLBAR_H

