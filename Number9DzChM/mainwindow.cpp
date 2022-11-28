#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDebug>
#include <Methods.h>
#include <qcustomplot.h>
#include <limits>
#include <QtMath>
using namespace std;

void MainWindow::initializeOutputInfoWidget(QWidget *parent, QLabel *label)
{
    parent->setWindowTitle("Выходные данные");
    parent->setGeometry(300, 300, 300, 300);
    QHBoxLayout *layout = new QHBoxLayout(parent);
    layout->setAlignment(parent, Qt::AlignCenter);
    layout->addWidget(label);
    parent->setLayout(layout);
}

void MainWindow::initializePlotWidget(QCustomPlot *plotWidget, QWidget *backgroundWidget)
{
    QPalette pal(palette());
    pal.setColor(QPalette::Window, Qt::gray);
    backgroundWidget->setAutoFillBackground(true);
    backgroundWidget->setPalette(pal);
    backgroundWidget->setGeometry(plotWidget->x() - 1, plotWidget->y() - 1, plotWidget->width() + 2, plotWidget->height() + 2);
    plotWidget->setInteraction(QCP::iRangeZoom,true);
    plotWidget->setInteraction(QCP::iRangeDrag,true);
}

void MainWindow::initializeTestTaskTable(QTableView *table, size_t rows)
{
    QStandardItemModel *model = new QStandardItemModel(rows,10,this);

    model->setHeaderData(0, Qt::Horizontal, "x_i");
    model->setHeaderData(1, Qt::Horizontal, "v_i");
    model->setHeaderData(2, Qt::Horizontal, "v_2i");
    model->setHeaderData(3, Qt::Horizontal, "v_i - v_2i");
    model->setHeaderData(4, Qt::Horizontal, "ОЛП");
    model->setHeaderData(5, Qt::Horizontal, "u_i");
    model->setHeaderData(6, Qt::Horizontal, "|u_i - v_i|");
    model->setHeaderData(7, Qt::Horizontal, "h_i");
    model->setHeaderData(8, Qt::Horizontal, "C+");
    model->setHeaderData(9, Qt::Horizontal, "C-");

    table->setModel(model);
}

void MainWindow::initializeMainTaskTable(QTableView *table, size_t rows)
{
    QStandardItemModel *model = new QStandardItemModel(rows,8,this);

    model->setHeaderData(0, Qt::Horizontal, "x_i");
    model->setHeaderData(1, Qt::Horizontal, "v_i");
    model->setHeaderData(2, Qt::Horizontal, "v_2i");
    model->setHeaderData(3, Qt::Horizontal, "v_i - v_2i");
    model->setHeaderData(4, Qt::Horizontal, "ОЛП");
    model->setHeaderData(5, Qt::Horizontal, "h_i");
    model->setHeaderData(6, Qt::Horizontal, "C+");
    model->setHeaderData(7, Qt::Horizontal, "C-");

    table->setModel(model);
}

//void MainWindow::on_introTextButton_clicked()
//{
//    if (introReference != nullptr && !introReference->isHidden())
//        introReference->hide();
//
//    introReference->show();
//}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("Numerical Analysis Laboratory Work");

 //   initializeTestTaskTable(ui->testTable, 0);
//    ui->testTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
 //   initializeMainTaskTable(ui->main1Table, 0);
 //   ui->main1Table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    initializeMainTaskTable(ui->Table, 0);
    ui->Table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    introReference    = new QWidget;
  //  testSolutionInfo  = new QWidget;
 //   main1SolutionInfo = new QWidget;
    main2SolutionInfo = new QWidget;

    introReferenceLabel    = new QLabel(introReference);
 //   testSolutionInfoLabel  = new QLabel(testSolutionInfo);
   // main1SolutionInfoLabel = new QLabel(main1SolutionInfo);
    main2SolutionInfoLabel = new QLabel(main2SolutionInfo);

    //testSolutionInfoLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    //main1SolutionInfoLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    main2SolutionInfoLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);

    initializeOutputInfoWidget(introReference, introReferenceLabel);
   // initializeOutputInfoWidget(testSolutionInfo , testSolutionInfoLabel);
   // initializeOutputInfoWidget(main1SolutionInfo, main1SolutionInfoLabel);
    initializeOutputInfoWidget(main2SolutionInfo, main2SolutionInfoLabel);

    introReferenceLabel->setText(
    "Это приложение выполнено в рамках лабораторной работы №1 по численным методам.\n\
Оно содержит 3 вкладки, каждая из них посвящена численному решению задачи Коши для конкретного ОДУ.\n\n\
Пользователь может задать начальные условия (точку и значение функции в ней) и параметры численного решения\n\
(правая граница отрезка 'x_max', точность выхода на правую границу 'eps_x', начальная величина шага 'h', \n\
для методов с регулировкой шага - максимальное число итераций 'N_max' и параметр контроля 'ε').\n\n\
После введения всех значений и нажатия кнопки 'Решить!' заполняется таблица с данными о решении и строится \n\
график численного решения (в тестовой задаче - ещё и истинное решение).\n\n\
Если введённые значения некорректны, выведется окно с сообщением об ошибке.");
    introReference->setWindowTitle("О программе");

 //   initializePlotWidget(ui->testPlot,  ui->testPlotBackground);
 //   initializePlotWidget(ui->main1Plot, ui->main1PlotBackground);
 //   initializePlotWidget(ui->main2Plot1, ui->main2Plot1Background);
 //   initializePlotWidget(ui->main2Plot2, ui->main2Plot2Background);
 //   initializePlotWidget(ui->main2Plot3, ui->main2Plot3Background);
 //   phase = new QCPCurve(ui->main2Plot3->xAxis, ui->main2Plot3->yAxis);
 //   phase->setName("Фазовая траектория");

 //   ui->testRadioButton ->click();
 //   ui->main1RadioButton->click();
 //   ui->main2RadioButton->click();

 //   ui->tabWidget->setCurrentIndex(0);
 //   ui->introTextButton->click(); // кнопка справки
    introReference->activateWindow();
}


MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::checkInput(const QString &str_u0, const QString &str_x_max, const QString &str_h,
                            const QString &str_x_eps, const QString &str_v_eps, const QString &str_a, const QString &str_b,
                            double &u0, double &x_max, double &h, double &x_eps, double &v_eps, double &a, double &b)
{

    bool result = false;

         if (str_u0.isEmpty())
        QMessageBox::critical(this, "Ошибка!", "Поле 'u0' должно быть непустым");
    else if (str_x_max.isEmpty())
        QMessageBox::critical(this, "Ошибка!", "Поле 'x_max' должно быть непустым");
    else if (str_h.isEmpty())
        QMessageBox::critical(this, "Ошибка!", "Поле 'h' должно быть непустым");
    else if (str_x_eps.isEmpty())
        QMessageBox::critical(this, "Ошибка!", "Поле 'eps_x' должно быть непустым");
    else if (str_v_eps.isEmpty())
        QMessageBox::critical(this, "Ошибка!", "Поле 'eps_v' должно быть непустым");
    else if (str_a.isEmpty())
        QMessageBox::critical(this, "Ошибка!", "Поле 'a' должно быть непустым");
    else if (str_b.isEmpty())
        QMessageBox::critical(this, "Ошибка!", "Поле 'ϑ' должно быть непустым");
    else
        result = true;



    u0    = str_u0.toDouble(&result);
    if (result == false)
    {
        QMessageBox::critical(this, "Ошибка!", "Поле 'u0' имеет некорректное значение");
        return false;
    }
    x_max = str_x_max.toDouble(&result);
    if (result == false)
    {
        QMessageBox::critical(this, "Ошибка!", "Поле 'x_max' имеет некорректное значение");
        return false;
    }
    h     = str_h.toDouble(&result);
    if (result == false)
    {
        QMessageBox::critical(this, "Ошибка!", "Поле 'h' имеет некорректное значение");
        return false;
    }
    x_eps = str_x_eps.toDouble(&result);
    if (result == false)
    {
        QMessageBox::critical(this, "Ошибка!", "Поле 'eps_x' имеет некорректное значение");
        return false;
    }
    v_eps = str_v_eps.toDouble(&result);
    if (result == false)
    {
        QMessageBox::critical(this, "Ошибка!", "Поле 'eps_v' имеет некорректное значение");
        return false;
    }
    a = str_a.toDouble(&result);
    if (result == false)
    {
        QMessageBox::critical(this, "Ошибка!", "Поле 'a' имеет некорректное значение");
        return false;
    }
    b = str_b.toDouble(&result);
    if (result == false)
    {
        QMessageBox::critical(this, "Ошибка!", "Поле 'b' имеет некорректное значение");
        return false;
    }


    result = false;
    if (x_max < 0.0)
        QMessageBox::critical(this, "Ошибка!", "Правая граница отрезка 'x_max' должна быть не меньше чем левая - 'x0' = 0");
    else if (h <= 0.0)
        QMessageBox::critical(this, "Ошибка!", "Шаг интегрирования 'h' должен быть положительным");
    else if (x_eps <= 0.0)
        QMessageBox::critical(this, "Ошибка!", "Точность выхода на правую границу 'eps_x' должна быть положительной");
    else if (v_eps <= 0.0)
        QMessageBox::critical(this, "Ошибка!", "Параметр контроля 'eps_v' должен быть положительным");
    else
        result = true;


    return result;
}

template <typename value_t, size_t dim>
void fillSolutionInfoLabel(QLabel *label, const vector<Entry<value_t, dim>> &solution, double x_max)
{
    size_t n = solution.size();
    double h_min = solution[0].h, h_max = h_min;
    double s_star_max = 0.0;
    size_t h_min_idx = 0, h_max_idx = 0;
    size_t plus_count = 0, minus_count = 0;

    for (size_t i = 0; i < n; ++i)
    {
        plus_count += solution[i].c_plus;
        minus_count += solution[i].c_minus;
        if (solution[i].h < h_min)
        {
            h_min_idx = i;
            h_min = solution[i].h;
        }
        if (solution[i].h > h_max)
        {
            h_max_idx = i;
            h_max = solution[i].h;
        }
        if (abs(solution[i].s_star) > s_star_max)
            s_star_max = abs(solution[i].s_star);
    }

    QString solutionInfoString;
    //solutionInfoString += "Число шагов метода (N): " + QString::number(n-1);
    solutionInfoString += "\nx_max  -  x_N: " + QString::number(x_max - solution.back().x);
    solutionInfoString += "\n\nМаксимум модуля ОЛП: " + QString::number(s_star_max);

    solutionInfoString += "\n\nОбщее число удвоений шага: " + QString::number(plus_count);
    solutionInfoString += "\nОбщее число делений шага: " + QString::number(minus_count);

    solutionInfoString += "\n\nМинимальный шаг: " + QString::number(h_min) + " при x = " + QString::number(solution[h_min_idx].x);
    solutionInfoString += "\nМаксимальный шаг: " + QString::number(h_max) + " при x = " + QString::number(solution[h_max_idx].x);

    label->setText(solutionInfoString);
}







/*// График производной численного решения, его, возможно не надо
    QCustomPlot *plot2 = ui->main2Plot2;
    plot2->clearGraphs();

    y_min = *min_element(v_dash.begin(), v_dash.end()),
    y_max = *max_element(v_dash.begin(), v_dash.end());

    plot2->xAxis->setRange(0,x_max);
    plot2->yAxis->setRange(y_min, y_max);

    plot2->legend->setVisible(true);
    plot2->xAxis->setLabel("x");
    plot2->yAxis->setLabel("u'");

    plot2->addGraph();
    plot2->graph(0)->addData(x, v_dash);
    plot2->graph(0)->setPen(QPen(Qt::blue));
    plot2->graph(0)->setName("Производная численного решения v(x)");

    plot2->addGraph();
    plot2->graph(1)->addData(x, v2_dash);
    plot2->graph(1)->setPen(QPen(Qt::green));
    plot2->graph(1)->setName("Производная численного решения с половинным шагом v2(x)");

    plot2->replot();


// Фазовый портрет //его, возможно, не надо
    QCustomPlot *plot3 = ui->main2Plot3;
    plot3->clearGraphs();

    double xmin = *min_element(v.begin(), v.end()),
           xmax = *max_element(v.begin(), v.end());

    plot3->xAxis->setRange(u0 - (xmax - xmin)/2.0, u0 + (xmax - xmin)/2.0);
    plot3->yAxis->setRange(u0_dash - (xmax - xmin)/2.0, u0_dash + (xmax - xmin)/2.0);

    plot3->legend->setVisible(true);
    plot3->xAxis->setLabel("u");
    plot3->yAxis->setLabel("u'");

    phase->setData(v, v_dash);

    plot3->replot();
*/

// Создание справки и её отображение
  /*  main2IsSolvedOnce = true;

    fillSolutionInfoLabel(main2SolutionInfoLabel, solution, x_max);
    ui->pushButton_2->click();
}*/

void MainWindow::on_main2ShowSolnInfoButton_clicked()
{
    if (!main2IsSolvedOnce)
        return;

    if (main2SolutionInfo != nullptr && !main2SolutionInfo->isHidden())
        main2SolutionInfo->hide();

    main2SolutionInfo->show();
}


void MainWindow::on_pushButton_pressed()
{
    // Ввод данных и проверка их корректности
         QString
                str_u0    = ui->lineEdit_u0      ->text(),
                str_x_max = ui->lineEdit_x_max   ->text(),
                str_h     = ui->lineEdit_h       ->text(),
                str_x_eps = ui->lineEdit_E_x     ->text(),
                str_a     = ui->lineEdit_a       ->text(),
                str_b     = ui->lineEdit_teta    ->text(),
                str_v_eps = ui->lineEdit_E_nu    ->text();


        double u0, x_max, h, x_eps, v_eps, a, b;
        if (!checkInput(str_u0, str_x_max, str_h, str_x_eps,  str_v_eps, str_a, str_b,
                            u0,     x_max,     h,     x_eps,      v_eps,     a,     b)) //пока убрал x0, потому что по заданию у нас  x0 = 0

        {
            return;
        }

    // Численное решение
        auto f = [&](double x, double u)
        {
            return -a*(u-b);
        };
        auto solution = ivp_step_adjust(rk4, Diff_equation<double,1>{f}, 0.0, u0, h, x_max, x_eps, v_eps);

    // Заполнение таблицы
        size_t n = solution.size();
        auto model = (QStandardItemModel*)ui->Table->model();
        model->setRowCount(n);
        for (size_t i = 0; i < n; ++i) {
            model->setData(model->index(i,0), solution[i].x);
            model->setData(model->index(i,1), solution[i].v);
            model->setData(model->index(i,2), solution[i].v2);
            model->setData(model->index(i,3), solution[i].v - solution[i].v2);
            model->setData(model->index(i,4), solution[i].s_star);
            model->setData(model->index(i,5), solution[i].h);
            model->setData(model->index(i,6), solution[i].c_plus);
            model->setData(model->index(i,7), solution[i].c_minus);
        }
        ui->Table->setModel(model);

    // Построение графиков

        QVector<double> x(n), v(n), v2(n), v_dash(n), v2_dash(n);
        for (size_t i = 0; i < n; ++i)
        {
            x[i]        = solution[i].x;
            v[i]        = solution[i].v;
            v2[i]       = solution[i].v2;
            v_dash[i]   = solution[i].v;
            v2_dash[i]  = solution[i].v2;
        }

    // График численного решения
        QCustomPlot *plot1 = ui->mainPlot;
        plot1->clearGraphs();

        double y_min = *min_element(v.begin(), v.end()),
               y_max = *max_element(v.begin(), v.end());

        plot1->xAxis->setRange(0,x_max);
        plot1->yAxis->setRange(y_min, y_max);

        plot1->legend->setVisible(true);
        plot1->xAxis->setLabel("x");
        plot1->yAxis->setLabel("u");

        plot1->addGraph();
        plot1->graph(0)->addData(x, v);
        plot1->graph(0)->setPen(QPen(Qt::blue));
        plot1->graph(0)->setName("Численное решение v(x)");

        plot1->addGraph();
        plot1->graph(1)->addData(x, v2);
        plot1->graph(1)->setPen(QPen(Qt::green));
        plot1->graph(1)->setName("Численное решение с половинным шагом v2(x)");

        plot1->replot();

}
