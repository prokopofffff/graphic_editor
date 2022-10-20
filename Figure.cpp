#include "Figure.h"

Figure::Figure(QWidget *parent) : QWidget(parent) {
    isRotationChanged = false;
    rotationValue = rand() % 360;
    mousePosX = rand() % parentWidget()->width()/2;
    mousePosY = rand() % parentWidget()->height()/2;
    this->move(mousePosX, mousePosY);
    figWidth = rand() % parentWidget()->width() + 9;
    figHeight = ( rand() % parentWidget()->height() + 4 ) % figWidth;
    pointA = {pointType::none, rand() % ( figHeight/3 )};
    pointB = {pointType::none, rand() % ( figHeight/3 )};
    pointC = {pointType::none, rand() % ( figHeight/3 )};
    pointD = {pointType::none, rand() % ( figHeight/3 )};
    pointE = {pointType::none, ( rand() % ( figWidth/4 ) ) % figHeight};
    pointF = {pointType::none, ( rand() % ( figWidth/4 ) ) % figHeight};
    isResized = false;
    isSelected = false;
    isClosed = true;

    updatePerimeter();
    updateArea();
}

void Figure::setPoints(const std::vector<pointType>& points) {
    if (points.size() < 6) {
        throw std::invalid_argument{"Необходимо задать 6 точек!"};
    }
    pointA.type = points[0];
    pointB.type = points[1];
    pointC.type = points[2];
    pointD.type = points[3];
    pointE.type = points[4];
    pointF.type = points[5];
}

void Figure::deselect() {
    isSelected = false;
    SELECTED_FIGURE = nullptr;
    this->repaint();
}

void Figure::mousePressEvent(QMouseEvent *event) {

    if (SELECTED_FIGURE != nullptr) {
        SELECTED_FIGURE->deselect();
    }

    switch(event->button()) {
    case Qt::LeftButton:
        if (isPointOfFigure(event->pos())) {
            mousePosX = event->globalX() - this->x();
            mousePosY = event->globalY() - this->y();

            SELECTED_FIGURE = this;
            isSelected = true;
            this->repaint();
        }
        break;
    case Qt::RightButton:
        if (isPointOfFigure(event->pos())) {
            SELECTED_FIGURE = this;
            isSelected = true;
            //QBoxLayout* layout = new QBoxLayout(QBoxLayout::TopToBottom);
            //createDialog(layout, "Удалить", "Изменить");
            infoAboutFigure();

        }
        break;
    default:
        QWidget::mousePressEvent(event);
        break;
    }

    QWidget::mousePressEvent(event);
};

void Figure::mouseMoveEvent(QMouseEvent *event) {
    if (isSelected) {
        if ( event->globalX() < parentWidget()->x() ||
             event->globalX() > parentWidget()->x() + parentWidget()->width() ||
             event->globalY() < parentWidget()->y() ||
             event->globalY() > parentWidget()->y() + parentWidget()->height()
           ) {
            return;
        }

        if (isPointOfFigure(event->pos())) {
            this->move(event->globalX() - mousePosX,
                       event->globalY() - mousePosY);
            this->repaint();
        }
    }
}

void Figure::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event)

    if (!isResized) {
        updatePerimeter();
        updateArea();

        int newWidth = std::max( std::abs(newCoordinates({0, 0}).first - newCoordinates({figWidth, figHeight}).first),
                                 std::abs(newCoordinates({0, figHeight}).first - newCoordinates({figWidth, 0}).first));
        int newHeight = std::max(std::abs(newCoordinates({0, 0}).second - newCoordinates({figWidth, figHeight }).second),
                                 std::abs(newCoordinates({0, figHeight}).second - newCoordinates({figWidth, 0}).second));
        isResized = true;

        this->move(this->x() + ( this->width() - newWidth + 4) / 2, this->y() + ( this->height() - newHeight + 4) / 2);

        this->resize(newWidth + 4, newHeight + 4);
    }

    QPainter painter(this);

    painter.translate((width()-1)/2, (height()-1)/2);

    if (isRotationChanged) {
        painter.rotate(+rotationValue);
    } else {
        painter.rotate(-rotationValue);
    }

    if (isSelected) painter.setPen(QPen(Qt::blue, 3));
    else painter.setPen(QPen(Qt::black, 3));

    painter.drawLine(pointD.value - figWidth/2, - figHeight/2, - pointE.value/2, - figHeight/2);
    painter.drawLine(pointE.value/2, - figHeight/2, figWidth/2 - pointA.value, - figHeight/2);

    painter.drawLine(figWidth/2, pointA.value - figHeight/2, figWidth/2, figHeight/2 - pointB.value);

    painter.drawLine(pointC.value - figWidth/2, figHeight/2, - pointF.value/2, figHeight/2);
    painter.drawLine(pointF.value/2, figHeight/2, figWidth/2 - pointB.value, figHeight/2);

    painter.drawLine(- figWidth/2, pointD.value - figHeight/2, - figWidth/2, figHeight/2 - pointC.value);

    paintPoint(painter, pointA,  figWidth/2 - pointA.value, -figHeight/2 + pointA.value,   +1, -1);
    paintPoint(painter, pointB,  figWidth/2 - pointB.value,  figHeight/2 - pointB.value,   +1, +1);
    paintPoint(painter, pointC, -figWidth/2 + pointC.value,  figHeight/2 - pointC.value,   -1, +1);
    paintPoint(painter, pointD, -figWidth/2 + pointD.value, -figHeight/2 + pointD.value,   -1, -1);
    paintPoint(painter, pointE, 0, -figHeight/2 + pointE.value/2,  0, -1);
    paintPoint(painter, pointF, 0, figHeight/2 - pointF.value/2,  0, +1);
}

void Figure::paintPoint(QPainter& painter, const Point& point, const int posW, const int posH, int modifierW, int modifierH) const {

    switch(point.type) {
    case pointType::none:

        if (modifierW != 0) {
            painter.drawLine(posW + modifierW * point.value, posH + modifierH * point.value, posW + modifierW * point.value, posH);
            painter.drawLine(posW + modifierW * point.value, posH + modifierH * point.value, posW, posH + modifierH * point.value);
        } else {
            painter.drawLine(posW + point.value/2, posH + modifierH * point.value/2, posW - point.value/2, posH + modifierH * point.value/2);
        }
        break;
    case pointType::stair:
        painter.drawLine(posW, posH, posW + modifierW * point.value, posH);
        painter.drawLine(posW, posH, posW, posH + modifierH * point.value);
        break;
    case pointType::slope:
        painter.drawLine(posW + modifierW * point.value, posH, posW, posH + modifierH * point.value);
        break;
    case pointType::innerRound:
        painter.drawArc(posW, posH, modifierW * point.value * 2, modifierH * point.value * 2, modifierH * 90*16, modifierW * modifierH * 90*16);
        break;
    case pointType::outerRound:
        painter.drawArc(posW + modifierW * point.value, posH + modifierH * point.value, - modifierW * point.value * 2, - modifierH * point.value * 2, - modifierH * 90*16, modifierW * modifierH * 90*16);
        break;
    case pointType::pit:

        painter.drawLine(posW, posH, posW + point.value/2, posH);
        painter.drawLine(posW, posH, posW - point.value/2, posH);

        painter.drawLine(posW + point.value/2, posH, posW + point.value/2, posH + modifierH * point.value/2);
        painter.drawLine(posW - point.value/2, posH, posW - point.value/2, posH + modifierH * point.value/2);
        break;
    case pointType::roundPit:
        painter.drawArc(posW - point.value/2, posH, point.value, modifierH * point.value, 0*16, modifierH * 180*16);
        break;
    default: break;
    }
}

void Figure::updatePerimeter() {
    perimeter = 0;

    perimeter += figWidth * 2 + figHeight * 2;

    perimeter -= pointA.value * 2 +
                 pointB.value * 2 +
                 pointC.value * 2 +
                 pointD.value * 2 +
                 pointE.value     +
                 pointF.value;

    perimeter += perimeterOfPoint(pointA) +
                 perimeterOfPoint(pointB) +
                 perimeterOfPoint(pointC) +
                 perimeterOfPoint(pointD) +
                 perimeterOfPoint(pointE, true) +
                 perimeterOfPoint(pointF, true);
}

double Figure::perimeterOfPoint(const Point& point, bool EorF) const {

    switch(point.type) {
    case pointType::none :
        if (!EorF) {
            return point.value * 2;
        } else {
            return point.value;
        }
        break;
    case pointType::stair :
        return point.value * 2;
        break;
    case pointType::slope :
        return point.value * std::sqrt(2);
        break;
    case pointType::innerRound :
        return point.value * 3.14159 / 2;
        break;
    case pointType::outerRound :
        return point.value * 3.14159 / 2;
        break;
    case pointType::pit :
        return point.value * 2;
        break;
    case pointType::roundPit :
        return point.value * 3.14159 / 2;
        break;
    }
    return 0;
}

void Figure::updateArea() {
    area = 0;

    area += figWidth * figHeight;

    area -= pointA.value * pointA.value   +
            pointB.value * pointB.value   +
            pointC.value * pointC.value   +
            pointD.value * pointD.value   +
            pointE.value * pointE.value/2 +
            pointF.value * pointF.value/2;

    area += areaOfPoint(pointA) +
            areaOfPoint(pointB) +
            areaOfPoint(pointC) +
            areaOfPoint(pointD) +
            areaOfPoint(pointE, true) +
            areaOfPoint(pointF, true);
}

double Figure::areaOfPoint(const Point& point, bool EorF) const {

    switch(point.type) {
    case pointType::none :
        if (!EorF) {
            return point.value * point.value;
        } else {
            return point.value * point.value/2;
        }
        break;
    case pointType::stair :
        return 0;
        break;
    case pointType::slope :
        return point.value * point.value / 2;
        break;
    case pointType::innerRound :

        return point.value * point.value * ( 1 - 3.14159 / 4 );
        break;
    case pointType::outerRound :
        return point.value * point.value * 3.14159 / 4;
        break;
    case pointType::pit :
        return 0;
        break;
    case pointType::roundPit :

        return point.value * point.value * ( 1/2 - 3.14159 / 8 );
        break;
    }
    return 0;
}

bool Figure::isPointOfFigure(QPoint point) const {

    point.rx() -= (width()-1)/2;
    point.ry() -= (height()-1)/2;

    std::pair<int, int> newPoint = newCoordinates({point.x(), point.y()});
    point.rx() = newPoint.first;
    point.ry() = newPoint.second;

    if (std::abs(point.x()) > figWidth/2 || std::abs(point.y()) > figHeight/2) {
        return false;
    }

    if (point.x() >= (figWidth/2 - pointA.value) && point.y() <= (-figHeight/2 + pointA.value)) {
        return isPointOfPoint(point, pointA, +1, -1);
    }

    if (point.x() >= (figWidth/2 - pointB.value) && point.y() >= (figHeight/2 - pointB.value)) {
        return isPointOfPoint(point, pointB, +1, +1);;
    }

    if (point.x() <= (-figWidth/2 + pointC.value) && point.y() >= (figHeight/2 - pointC.value)) {
        return isPointOfPoint(point, pointC, -1, +1);;
    }

    if (point.x() <= (-figWidth/2 + pointD.value) && point.y() <= (-figHeight/2 + pointD.value)) {
        return isPointOfPoint(point, pointD, -1, -1);;
    }

    if (std::abs(point.x()) <= pointE.value/2 && point.y() <= (-figHeight/2 + pointE.value/2)) {
        return isPointOfPoint(point, pointE, 0, -1);;
    }

    if (std::abs(point.x()) <= pointF.value/2 && point.y() >= (figHeight/2 - pointF.value/2)) {
        return isPointOfPoint(point, pointF, 0, 1);;
    }
    return true;
}

bool Figure::isPointOfPoint(QPoint point, const Point& pointOfFigure, int modifierW, int modifierH) const {

    switch(pointOfFigure.type) {
    case pointType::none:
        return true;
        break;
    case pointType::stair:
        return false;
        break;
    case pointType::slope:
        if ( std::abs( point.x() + ( modifierH * modifierW ) * point.y() ) <
             std::abs( modifierW * figWidth/2 + ( modifierH * modifierW ) * ( modifierH * figHeight/2 - modifierH * pointOfFigure.value ) ) ) {
            return true;
        } else {
            return false;
        }
        break;
    case pointType::innerRound:
        point.rx() -= modifierW * figWidth/2;
        point.ry() -= modifierH * figHeight/2;
        if ( ( point.x() * point.x() + point.y() * point.y() ) >= ( pointOfFigure.value * pointOfFigure.value ) ) {
            return true;
        } else {
            return false;
        }
        break;
    case pointType::outerRound:
        point.rx() -= modifierW * ( figWidth/2 - pointOfFigure.value );
        point.ry() -= modifierH * ( figHeight/2 - pointOfFigure.value );
        if ( ( point.x() * point.x() + point.y() * point.y() ) <= ( pointOfFigure.value * pointOfFigure.value ) ) {
            return true;
        } else {
            return false;
        }
        break;
    case pointType::pit:
        return false;
        break;
    case pointType::roundPit:
        point.ry() -= modifierH * figHeight/2;
        if ( ( point.x() * point.x() + point.y() * point.y() ) >= ( ( pointOfFigure.value/2 ) * ( pointOfFigure.value/2 ) ) ) {
            return true;
        } else {
            return false;
        }
        break;
    }
    return false;
}

std::pair<int, int> Figure::newCoordinates (const std::pair<int, int>& oldCoordinates) const {
    double radians = 3.14159 / 180.0;

    if (isRotationChanged) {
        return { ( std::cos(rotationValue * radians) * oldCoordinates.first + std::sin(rotationValue * radians) * oldCoordinates.second ),
                 (-std::sin(rotationValue * radians) * oldCoordinates.first + std::cos(rotationValue * radians) * oldCoordinates.second ) };
    } else {
        return { ( std::cos(rotationValue * radians) * oldCoordinates.first - std::sin(rotationValue * radians) * oldCoordinates.second ),
                 ( std::sin(rotationValue * radians) * oldCoordinates.first + std::cos(rotationValue * radians) * oldCoordinates.second ) };
    }
}

void Figure::infoAboutFigure() {

    int newfigWidth = figWidth;
    int newfigHeight = figHeight;
    int newRotationValue = rotationValue;
    int newPointA = pointA.value;
    int newPointB = pointB.value;
    int newPointC = pointC.value;
    int newPointD = pointD.value;
    int newPointE = pointE.value;
    int newPointF = pointF.value;
    bool newRotationChanged = isRotationChanged;
    isClosed = false;

    while(1) {
        QBoxLayout* boxLayout = new QBoxLayout(QBoxLayout::TopToBottom);

        QSpinBox *spinWidth  = new QSpinBox();
        spinWidth->setRange(9, INT_MAX);
        spinWidth->setValue(newfigWidth);
        spinWidth->setSingleStep(5);

        QSpinBox *spinHeight = new QSpinBox();
        spinHeight->setRange(4, INT_MAX);
        spinHeight->setValue(newfigHeight);
        spinHeight->setSingleStep(5);

        QSpinBox *spinRotate = new QSpinBox();
        spinRotate->setRange(INT_MIN, INT_MAX);
        spinRotate->setValue(newRotationValue);
        spinRotate->setSingleStep(5);

        QRadioButton *rotateR = new QRadioButton("По часовой стрелке");
        QRadioButton *rotateL = new QRadioButton("Против часовой стрелки");
        if (newRotationChanged) {
            rotateR->setChecked(true);
        } else {
            rotateL->setChecked(true);
        }

        QSpinBox *spinPointA = new QSpinBox();
        spinPointA->setRange(1, INT_MAX);
        spinPointA->setValue(newPointA);
        spinPointA->setSingleStep(5);

        QSpinBox *spinPointB = new QSpinBox();
        spinPointB->setRange(1, INT_MAX);
        spinPointB->setValue(newPointB);
        spinPointB->setSingleStep(5);

        QSpinBox *spinPointC = new QSpinBox();
        spinPointC->setRange(1, INT_MAX);
        spinPointC->setValue(newPointC);
        spinPointC->setSingleStep(5);

        QSpinBox *spinPointD = new QSpinBox();
        spinPointD->setRange(1, INT_MAX);
        spinPointD->setValue(newPointD);
        spinPointD->setSingleStep(5);

        QSpinBox *spinPointE = new QSpinBox();
        spinPointE->setRange(2, INT_MAX);
        spinPointE->setValue(newPointE);
        spinPointE->setSingleStep(5);

        QSpinBox *spinPointF = new QSpinBox();
        spinPointF->setRange(2, INT_MAX);
        spinPointF->setValue(newPointF);
        spinPointF->setSingleStep(5);

        boxLayout->addWidget(new QLabel("Информация о фигуре:"));
        boxLayout->addWidget(new QLabel("Периметр фигуры: " + QString::number(perimeter)));
        boxLayout->addWidget(new QLabel("Площадь фигуры: " + QString::number(area)));
        boxLayout->addWidget(new QLabel("Ширина:"));
        boxLayout->addWidget(spinWidth);
        boxLayout->addWidget(new QLabel("Высота:"));
        boxLayout->addWidget(spinHeight);
        boxLayout->addWidget(new QLabel("Угол поворота:"));
        boxLayout->addWidget(spinRotate);
        boxLayout->addWidget(rotateR);
        boxLayout->addWidget(rotateL);
        if (pointA.type != pointType::none) {
            boxLayout->addWidget(new QLabel("Параметры точки А:"));
            boxLayout->addWidget(spinPointA);
        }
        if (pointB.type != pointType::none) {
            boxLayout->addWidget(new QLabel("Параметры точки B:"));
            boxLayout->addWidget(spinPointB);
        }
        if (pointC.type != pointType::none) {
            boxLayout->addWidget(new QLabel("Параметры точки C:"));
            boxLayout->addWidget(spinPointC);
        }
        if (pointD.type != pointType::none) {
            boxLayout->addWidget(new QLabel("Параметры точки D:"));
            boxLayout->addWidget(spinPointD);
        }
        if (pointE.type != pointType::none) {
            boxLayout->addWidget(new QLabel("Параметры точки E:"));
            boxLayout->addWidget(spinPointE);
        }
        if (pointF.type != pointType::none) {
            boxLayout->addWidget(new QLabel("Параметры точки F:"));
            boxLayout->addWidget(spinPointF);
        }

        createDialog(boxLayout, "Сохранить изменения", "Отменить изменения");

        if (isClosed) {
            return;
        }

        if (spinWidth->value() < 9 || spinHeight->value() < 4 || spinHeight->value() >= spinWidth->value() ||
            spinPointA->value()*3 >= spinHeight->value() ||
            spinPointB->value()*3 >= spinHeight->value() ||
            spinPointC->value()*3 >= spinHeight->value() ||
            spinPointD->value()*3 >= spinHeight->value() ||
            spinPointE->value()*4 >= spinWidth->value()  ||
            spinPointF->value()*4 >= spinWidth->value()  ||
            spinPointE->value() >= spinHeight->value()   ||
            spinPointF->value() >= spinHeight->value())
        {
            newfigWidth = spinWidth->value();
            newfigHeight = spinHeight->value();
            newRotationValue = spinRotate->value();
            newPointA = spinPointA->value();
            newPointB = spinPointB->value();
            newPointC = spinPointC->value();
            newPointD = spinPointD->value();
            newPointE = spinPointE->value();
            newPointF = spinPointF->value();
            if (rotateR->isChecked()) newRotationChanged = true;
            else newRotationChanged = false;

            QBoxLayout* errorLayout = new QBoxLayout(QBoxLayout::TopToBottom);

            errorLayout->addWidget(new QLabel("Введенные данные некорректны"));
            createDialog(errorLayout, "Изменить");

        } else {
            figWidth = spinWidth->value();
            figHeight = spinHeight->value();
            rotationValue = spinRotate->value();
            pointA.value = spinPointA->value();
            pointB.value = spinPointB->value();
            pointC.value = spinPointC->value();
            pointD.value = spinPointD->value();
            pointE.value = spinPointE->value();
            pointF.value = spinPointF->value();
            if (rotateR->isChecked()) {
                isRotationChanged = true;
            } else {
                isRotationChanged = false;
            }

            isResized = false;
            this->repaint();

            QBoxLayout* layout = new QBoxLayout(QBoxLayout::TopToBottom);

            layout->addWidget(new QLabel("Введенные данные сохранены"));
            createDialog(layout, "Закрыть");

            isClosed = true;
            return;
        }
    }
}

void Figure::deleteFigure(){
    //vec.erase(std::find(vec.begin(), vec.end(), this));
    //SELECTED_FIGURE = nullptr;
    //this->deleteLater();
    //delete SELECTED_FIGURE;
    //this->repaint();
}

void Figure::createDialog(QBoxLayout *boxLayout, const QString& message) {
    QDialog *modalDialog = new QDialog(this);

    modalDialog->setModal(true);

    QPushButton *button = new QPushButton(message);
    boxLayout->addWidget(button);
    connect(button, SIGNAL(clicked()), modalDialog, SLOT(accept()));

    modalDialog->setLayout(boxLayout);
    modalDialog->exec();

    delete modalDialog;
}

void Figure::createDialog(QBoxLayout *boxLayout, const QString& message1, const QString& message2) {
    QDialog *modalDialog = new QDialog(this);

    modalDialog->setModal(true);

    QPushButton *deleteButton = new QPushButton(message1);
    boxLayout->addWidget(deleteButton);
    connect(deleteButton, SIGNAL(clicked()), this, SLOT(deleteFigure()));

    QPushButton *infoButton = new QPushButton(message2);
    boxLayout->addWidget(infoButton);
    //connect(cancelButton, SIGNAL(clicked()), modalDialog, SLOT(accept()));
    connect(infoButton, SIGNAL(clicked()), this, SLOT(infoAboutFigure()));

    modalDialog->setLayout(boxLayout);
    modalDialog->exec();

    delete modalDialog;
}
