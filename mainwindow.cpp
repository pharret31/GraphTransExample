#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent)
{
    m_canvas = new QPainter();
    m_brush = new QBrush(Qt::red);

    this->setPalette(QPalette(Qt::white));

    // Установка точек фигуры
    // В данном случае рисование листа
    QPoint point1(0, 30);
    QPoint point2(10, 10);

    QPoint point3(10, 10);
    QPoint point4(30, 0);

    QPoint point5(30, 0);
    QPoint point6(20, 20);

    QPoint point7(20, 20);
    QPoint point8(0, 30);

    QPoint point9(15, 15);
    QPoint point10(40, -10);

    m_points << point1 << point2 << point3 << point4 << point5
             << point6 << point7 << point8 << point9 << point10;

    // Установка таймера
    m_timer = new QTimer(this);

    // Соединение сигнала истечения времени и слота перерисовки формы
    connect(m_timer, SIGNAL(timeout()), this, SLOT(repaint()));

    // Запуск таймера
    m_timer->start(10);
}

void MainWindow::paintEvent(QPaintEvent *)
{
    m_canvas->begin(this);

    for (int i = 0; i < m_figures.count(); i++)
    {
        for (int j = 0; j < m_figures.at(i)->lines.count(); j++)
        {
            // Если фигура не удалена,
            if (!m_figures.at(i)->isDeleted())
            {
                // то отрисовываем её линии
                m_canvas->drawLine(m_figures.at(i)->lines.at(j));
            }
        }
    }

    m_canvas->end();
}

void MainWindow::mousePressEvent(QMouseEvent *me)
{
    // При нажатии левой кнопкой мыши по форме
    if (me->button() == Qt::LeftButton)
    {
        // Останавливаем таймер
        m_timer->stop();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *me)
{
    // Задаём случайные значения параметров фигуры
    float randomLeftAngle = randomNumber(-0.9, -0.1);
    float randomRightAngle = randomNumber(-1.5, -0.5);
    float randomDeltaAngle = randomNumber(0.001, 0.009);
    float randomResizeCoeffs = randomNumber(0.0006, 0.0014);
    float randomDeltaHeight = randomNumber(0.05, 0.15);

    // Стандартная фигура
//    Figure *qFigure = new Figure(m_points, me->pos(), -0.5, -1,
//                                0.005, 0.001, 0.001,
//                                0.1, 30, 30,
//                                this->height());

    // Создание фигуры по созданным параметрам
    Figure *qFigure = new Figure(m_points, me->pos(),
                                 randomLeftAngle, randomRightAngle,
                                 randomDeltaAngle,
                                 randomResizeCoeffs, randomResizeCoeffs,
                                 randomDeltaHeight, 30, 30,
                                 this->height());

    // Добление фигуры в вектор фигур
    m_figures << qFigure;

    // Содеинение сигнала истечения времени со слотом
    // update последней созданной фигуры
    connect(m_timer, SIGNAL(timeout()), m_figures.last(), SLOT(update()));

    // Запускаем таймер
    m_timer->start(10);
}

void MainWindow::resizeEvent(QResizeEvent *re)
{
    // При изменении размеров формы для каждой фигуры
    for (int i = 0; i < m_figures.count(); i++)
    {
        // Изменяем значение высоты родительского окна
        m_figures.at(i)->setHeight(re->size().height());
    }
}

// Реализация функции генерации случайного числа
float MainWindow::randomNumber(float min, float max)
{
    srand(time(NULL));

    return ((float(rand()) / float(RAND_MAX)) * (max - min)) + min;
}
