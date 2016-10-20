#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

#include <QPainter>
#include <QVector>
#include <QTimer>
#include <QMouseEvent>

#include <cstdlib>
#include <ctime>

// Подключение класса фигуры
#include "figure.h"

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);

private:
    QPainter *m_canvas;
    QBrush *m_brush;

    QTimer *m_timer;

    // Вектор точек фигуры
    QVector<QPoint> m_points;
    // Вектор фигур
    QVector<Figure*> m_figures;

    // События формы
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *me);
    void mouseReleaseEvent(QMouseEvent *me);
    void resizeEvent(QResizeEvent *re);

    // Функция генерации случайного числа
    float randomNumber(float min, float max);
    
signals:
    
public slots:

};

#endif // MAINWINDOW_H
