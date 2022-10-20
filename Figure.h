#ifndef FIGURE_H
#define FIGURE_H

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QPoint>
#include <QDialog>
#include <QLabel>
#include <QSpinBox>
#include <QPushButton>
#include <QRadioButton>
#include <QBoxLayout>
#include <QMainWindow>

#include <cmath>
#include <stdexcept>
#include <random>
#include <vector>

class Figure;
extern Figure* SELECTED_FIGURE;
extern std::vector<Figure*> vec;

enum pointType {
    none = 0,
    stair = 1,
    slope = 2,
    innerRound = 3,
    outerRound = 4,
    pit = 5,
    roundPit = 6
};

class Point {
public:
    pointType type;
    int value;
};

class Figure : public QWidget
{
    Q_OBJECT
public:
    explicit Figure(QWidget *parent = nullptr);

    void setPoints(const std::vector<pointType>& points);

    void deselect();

protected:
    void mousePressEvent(QMouseEvent *event);

    void mouseMoveEvent(QMouseEvent *event);

    void paintEvent(QPaintEvent *event);

private slots:
    void infoAboutFigure();

    void deleteFigure();

private:
    void paintPoint(QPainter& painter, const Point& point, const int posW, const int posH, int modifierW, int modifierH) const;

    bool isPointOfFigure(QPoint point) const;

    bool isPointOfPoint(QPoint point, const Point& pointOfFigure, int modifierW, int modifierH) const;

    void updatePerimeter();

    double perimeterOfPoint(const Point& point, bool EorF = false) const;

    void updateArea();

    double areaOfPoint(const Point& point, bool EorF = false) const;

    std::pair<int, int> newCoordinates (const std::pair<int, int>& oldCoordinates) const;

    void createDialog(QBoxLayout *boxLayout, const QString& message);

    void createDialog(QBoxLayout *boxLayout, const QString& message1, const QString& message2);

    bool isRotationChanged;
    int rotationValue;
    int mousePosX;
    int mousePosY;

    Point pointA;
    Point pointB;
    Point pointC;
    Point pointD;
    Point pointE;
    Point pointF;

    int figWidth;
    int figHeight;
    double perimeter;
    double area;

    bool isSelected;
    bool isResized;
    bool isClosed;
};

class Figure1 : public Figure {
public:
    explicit Figure1(QWidget *parent = nullptr):Figure(parent){
        pointType pointA = pointType::none;
        pointType pointB = pointType::innerRound;
        pointType pointC = pointType::outerRound;
        pointType pointD = pointType::none;
        pointType pointE = pointType::pit;
        pointType pointF = pointType::roundPit;
        setPoints({pointA, pointB, pointC, pointD, pointE, pointF});
    }
};

class Figure2 : public Figure {
public:
    explicit Figure2(QWidget *parent = nullptr):Figure(parent){
        pointType pointA = pointType::none;
        pointType pointB = pointType::innerRound;
        pointType pointC = pointType::stair;
        pointType pointD = pointType::outerRound;
        pointType pointE = pointType::none;
        pointType pointF = pointType::roundPit;
        setPoints({pointA, pointB, pointC, pointD, pointE, pointF});
    }
};

#endif // BASEFIGURE_H

