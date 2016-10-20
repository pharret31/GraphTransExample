#ifndef FIGURE_H
#define FIGURE_H

#include <QObject>

#include <QVector>
#include <QPoint>
#include <QLine>

#include <cmath>

class Figure : public QObject
{
    Q_OBJECT
public:
    explicit Figure(QVector<QPoint> _points, QPoint _beginPoint,
                    float _lFi, float _rFi, float _deltaFi,
                    float _deltaResizeCoeffX, float _deltaResizeCoeffY,
                    float _deltaHeight, float _Ax, float _By,
                    float _parentHeight, QObject *parent = 0);

    // Линии, которые необходимо отрисовать
    QVector<QLine> lines;

    // Поддержка динамической установки значений
    void setHeight(int _height);
    void setLeftAngle(float _lFi);
    void setRightAngle(float _rFi);
    void setDeltaAngle(float _deltaFi);
    void setDeltaResizeCoeffX(float _deltaResizeCoeffX);
    void setDeltaResizeCoeffY(float _deltaResizeCoeffY);
    void setDeltaHeight(float _deltaHeight);

    // Считывание значений
    float getLeftAngle();
    float getRightAngle();
    float getDeltaAngle();
    float getDeltaResizeCoeffX();
    float getDeltaResizeCoeffY();
    float getDeltaHeight();

    // Проверка: удалена-ли линия?
    bool isDeleted();
    
private:

    // Точки, с которыми работает класс
    QVector<QPoint> m_points;
    // Начальная точка, относительно которой производится вращение
    QPoint m_beginPoint;
    // Угол левого(l) и правого(r) отклонения от центра
    float m_lFi, m_rFi;
    // Изменение угла в каждый момент времени
    float m_deltaFi;
    // Изменение размера по X и Y в каждый момент времени
    float m_deltaResizeCoeffX, m_deltaResizeCoeffY;
    // Изменение высоты в каждый момент времени (падение)
    float m_deltaHeight;
    // Величины смещения осей координат
    float m_Ax, m_By;
    // Высота родительсой области (области окна в котором
    // производится падение фигуры)
    float m_parentHeight;

    // Текущий угол отклонения
    float m_Fi;
    // Первичная высота
    float m_fHeight;
    // Текущие величины увеличения
    float m_resizeCoeffX, m_resizeCoeffY;
    // Рабочий флаг
    bool m_flag;

    // Переменная для установки значения того, что фигура уничтожена
    bool m_isDeleted;

    // Матрицы вращения, изменения размера и трансляции
    float RotationMatrix[2][2];
    float ResizeMatrix[2][2];
    float TranslationMatrix[3][3];

    // Установка рабочих матриц
    void setRotationMatrix(float angle);
    void setResizeMatrix(float Rx, float Ry);
    void setTranslationMatrix(float Tx, float Ty);

    // Функции изменения координат точки по матрицам
    QPoint rotatePoint(const QPoint point);
    QPoint resizePoint(const QPoint point);
    QPoint translatePoint(const QPoint point);
    
signals:
    
public slots:
    // Слот изменения координат точек
    void update();
    
};

#endif // FIGURE_H
