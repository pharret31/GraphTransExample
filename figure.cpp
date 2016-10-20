#include "figure.h"

Figure::Figure(QVector<QPoint> _points, QPoint _beginPoint,
               float _lFi, float _rFi, float _deltaFi,
               float _deltaResizeCoeffX, float _deltaResizeCoeffY,
               float _deltaHeight, float _Ax, float _By,
               float _parentHeight, QObject *parent) :
    QObject(parent)
{
    // Установка начальных значений
    m_points = _points;
    m_beginPoint = _beginPoint;
    m_lFi = _lFi;
    m_rFi = _rFi;
    m_deltaFi = _deltaFi;
    m_deltaResizeCoeffX = _deltaResizeCoeffX;
    m_deltaResizeCoeffY = _deltaResizeCoeffY;
    m_deltaHeight = _deltaHeight;
    m_Ax = _Ax;
    m_By = _By;
    m_parentHeight = _parentHeight;

    m_Fi = 0.00F;
    m_fHeight = m_beginPoint.y();
    m_resizeCoeffX = 1;
    m_resizeCoeffY = 1;
    m_flag = true;

    m_isDeleted = false;

    // Смещение координатной оси
    for (int i = 0; i < m_points.count(); i++)
    {
        QPoint temp = m_points.at(i);
        temp.setX(temp.x() + m_Ax);
        temp.setY(temp.y() + m_By);
        m_points.replace(i, temp);
    }
}

// Реализация функции поддержки динамической установки значений
void Figure::setHeight(int _height)
{
    m_parentHeight = _height;
}

void Figure::setLeftAngle(float _lFi)
{
    m_lFi = _lFi;
}

void Figure::setRightAngle(float _rFi)
{
    m_rFi = _rFi;
}

void Figure::setDeltaAngle(float _deltaFi)
{
    m_deltaFi = _deltaFi;
}

void Figure::setDeltaResizeCoeffX(float _deltaResizeCoeffX)
{
    m_deltaResizeCoeffX = _deltaResizeCoeffX;
}

void Figure::setDeltaResizeCoeffY(float _deltaResizeCoeffY)
{
    m_deltaResizeCoeffY = _deltaResizeCoeffY;
}

void Figure::setDeltaHeight(float _deltaHeight)
{
    m_deltaHeight = _deltaHeight;
}

float Figure::getLeftAngle()
{
    return m_lFi;
}

float Figure::getRightAngle()
{
    return m_rFi;
}

float Figure::getDeltaAngle()
{
    return m_deltaFi;
}

float Figure::getDeltaResizeCoeffX()
{
    return m_deltaResizeCoeffX;
}

float Figure::getDeltaResizeCoeffY()
{
    return m_deltaResizeCoeffY;
}

float Figure::getDeltaHeight()
{
    return m_deltaHeight;
}

bool Figure::isDeleted()
{
    if (m_isDeleted)
    {
        return true;
    }

    return false;
}

// Установка рабочих матриц
void Figure::setRotationMatrix(float angle)
{
    RotationMatrix[0][0] = cos(angle);
    RotationMatrix[0][1] = -sin(angle);
    RotationMatrix[1][0] = sin(angle);
    RotationMatrix[1][1] = cos(angle);
}

void Figure::setResizeMatrix(float Rx, float Ry)
{
    ResizeMatrix[0][0] = Rx;
    ResizeMatrix[0][1] = 0.00;
    ResizeMatrix[1][0] = 0.00;
    ResizeMatrix[1][1] = Ry;
}

void Figure::setTranslationMatrix(float Tx, float Ty)
{
    TranslationMatrix[0][0] = 1.00;
    TranslationMatrix[0][1] = 0.00;
    TranslationMatrix[0][2] = 0.00;

    TranslationMatrix[1][0] = 0.00;
    TranslationMatrix[1][1] = 1.00;
    TranslationMatrix[1][2] = 0.00;

    TranslationMatrix[2][0] = Tx;
    TranslationMatrix[2][1] = Ty;
    TranslationMatrix[2][2] = 1.00;
}

// Реализация функций изменения координат точки по матрицам
QPoint Figure::rotatePoint(const QPoint point)
{
    QPoint result;
    result.setX(point.x()*RotationMatrix[0][0] + point.y()*RotationMatrix[1][0]);
    result.setY(point.x()*RotationMatrix[0][1] + point.y()*RotationMatrix[1][1]);
    return result;
}

QPoint Figure::resizePoint(const QPoint point)
{
    QPoint result;
    result.setX(point.x()*ResizeMatrix[0][0] + point.y()*ResizeMatrix[1][0]);
    result.setY(point.x()*ResizeMatrix[0][1] + point.y()*ResizeMatrix[1][1]);
    return result;
}

QPoint Figure::translatePoint(const QPoint point)
{
    QPoint result;
    result.setX(point.x()*TranslationMatrix[0][0] + point.y()*TranslationMatrix[1][0] + 1.00*TranslationMatrix[2][0]);
    result.setY(point.x()*TranslationMatrix[0][1] + point.y()*TranslationMatrix[1][1] + 1.00*TranslationMatrix[2][1]);
    return result;
}

// Слот изменения координат точек
void Figure::update()
{
    // Если данная фигура удалена, то ничего прорисовывать не нужно
    if (m_isDeleted)
    {
        return;
    }

    int i = 0;

    // Увеличение фигуры на соответствующее delta значение
    m_resizeCoeffX += m_deltaResizeCoeffX;
    m_resizeCoeffY += m_deltaResizeCoeffY;

    // Очистка вектора линий (чтобы не перерисовывалось заново
    // предыдущее состояние фигуры)
    lines.clear();

    // Если флаг установлен (фигура движется в одном направлении)
    if (m_flag)
    {
        // Увеличиваем угол смещения
        m_Fi += m_deltaFi;
        // Ненамного роняем фигуру
        m_fHeight += m_deltaHeight * 2;

        // При достижении ограничивающего значения угла
        if (m_Fi > m_lFi)
        {
            // Меняем флаг на противоположный
            m_flag = false;
        }
    }
    else
    {
        // Аналогично тому, когда флаг установлен
        m_Fi -= m_deltaFi;
        m_fHeight += m_deltaHeight;

        if (m_Fi < m_rFi)
        {
            m_flag = true;
        }
    }

    // Устанавливаем рабочие матрицы по вычисленным ранее значениям
    setRotationMatrix(m_Fi);
    setResizeMatrix(m_resizeCoeffX, m_resizeCoeffY);
    setTranslationMatrix(m_beginPoint.x(), m_fHeight);

    // Временный массив точек
    QVector<QPoint> ps;

    // Для каждой точки
    for (i = 0; i < m_points.count(); i++)
    {
        // Устанавливаем новые значения её координат
        QPoint temp = m_points.at(i);
        temp = rotatePoint(temp);
        temp = resizePoint(temp);
        temp = translatePoint(temp);
        ps.push_back(temp);
    }

    // Так же для каждой точки
    for (i = 0; i < m_points.count(); i += 2)
    {
        // Создаём отрезки (по заданию)
        QLine temp(ps.at(i), ps.at(i+1));
        // И записываем их как линии для рисования
        lines.push_back(temp);
    }

    int k = 0;

    // Для каждой линии в фигуре
    for (i = 0; i < lines.count(); i++)
    {
        QLine temp = lines.at(i);
        // Вычисляем самое верхнее значение Y
        int maxY = temp.y1() > temp.y2() ? temp.y1() : temp.y2();

        // Если оно больше, чем высота родительского окна
        if (maxY > m_parentHeight)
        {
            // Увеличиваем количество линий, которые прошли границу на 1
            k++;
        }
    }

    // k == i эквивалентно k == m_points.count() - 1
    // т.к. i не менялось после выполнения цикла и
    // равно m_points.count() - 1
    // Если все линии фигуры достигли границы окна
    if (k == i)
    {
        // Очищаем массив линий
        lines.clear();
        // Уничтожаем все точки, по которым они рисовались
        m_points.clear();
        // Фигура удалена
        m_isDeleted = true;
    }
}
