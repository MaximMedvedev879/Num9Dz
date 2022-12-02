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

void MainWindow::initializePlotWidget(QCustomPlot *plotWidget)
{
    plotWidget->setInteraction(QCP::iRangeZoom,true);
    plotWidget->setInteraction(QCP::iRangeDrag,true);
}

void MainWindow::initializeTable(QTableView *table, size_t rows)
{
    QStandardItemModel *model = new QStandardItemModel(rows,11,this);

    model->setHeaderData(0, Qt::Horizontal, "i");
    model->setHeaderData(1, Qt::Horizontal, "x_i");
    model->setHeaderData(2, Qt::Horizontal, "u_i");
    model->setHeaderData(3, Qt::Horizontal, "v_i");
    model->setHeaderData(4, Qt::Horizontal, "v_2i");
    model->setHeaderData(5, Qt::Horizontal, "u_i - v_i");
    model->setHeaderData(6, Qt::Horizontal, "v_i - v_2i");
    model->setHeaderData(7, Qt::Horizontal, "ОЛП");
    model->setHeaderData(8, Qt::Horizontal, "h_i");
    model->setHeaderData(9, Qt::Horizontal, "C+");
    model->setHeaderData(10, Qt::Horizontal, "C-");

    table->setModel(model);
}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("Приложение по задаче 9. Медведев Максим, 382003_3");

    initializeTable(ui->Table, 0);
    ui->Table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    solutionInfo = new QWidget;
    progInfo     = new QWidget;
    tableInfo    = new QWidget;
    solutionInfoLabel = new QLabel(solutionInfo);
    progInfoLabel     = new QLabel(progInfo);
    tableInfoLabel    = new QLabel(tableInfo);

    progInfoLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    tableInfoLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    solutionInfoLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);

    initializeOutputInfoWidget(solutionInfo, solutionInfoLabel);
    initializeOutputInfoWidget(progInfo, progInfoLabel);
    initializeOutputInfoWidget(tableInfo, tableInfoLabel);
    progInfo->setWindowTitle("О задаче");
    tableInfo->setWindowTitle("О столбцах таблицы");

    progInfoLabel->setText(
"Это приложение выполнено в рамках задания 9 по численным методам.\n\
Суть задания - решить задачу Коши для ОДУ 1 порядка, используя метод Рунге-Кутты 4 порядка,\n\
показать зависимость решения от параметров модели, начальных условий и параметров численного метода,\n\
указать зависимость свойств решения, наиболее интересных с прикладной точки зрения, от параметров.\n\n\
Уравнение описывает остывание нагретого тела, помещённого для охлаждения в поток жидкости или газа постоянной температуры 'ϑ'.\n\
'x'     - время.\n\
'u(x)'  - температура тела в момент времени 'x'.\n\
'a'     - постоянный коэффициент пропорциональности (a > 0).\n\
'ϑ'     - температура потока воздуха или жидкости.\n\n\
Параметры задачи Коши:\n\
'u0'    - температура в момент времени 'x = 0'.\n\
Параметры численного метода:\n\
'x_max' - правая граница отрезка численного решения (по времени).\n\
'ε_x'   - точность выхода на правую границу отрезка численного решения (по времени)\n\
(счёт прекращается, если для текущего времени 'x' верно: 'x > x_max - ε_x').\n\
'h'     - начальное значение шага интегрирования (по времени).\n\
'N_max' - максимальное число шагов метода (счёт прекращается при достижении шага с этим номером).\n\
'ε_ν'   - параметр контроля (участвует в оценке контрольного слагаемого и принятии решения об изменении шага).\n\n\
Критерии остановки счёта: выход на правую границу с заданной точностью, достижение заданного числа шагов метода.\n\
Метод контроля погрешности: счёт с половинным шагом и оценка контрольного слагаемого 'S = (v_n - v_2n)/15'.\n\
'|S| > ε_ν'            : полученные на этом шаге значения не принимаются; шаг уменьшается вдвое, .\n\
'ε_ν/32 <= |S| <= ε_ν': принимается значение, полученное с половинным шагом; шаг не меняется.\n\
'|S| < ε_ν/32'        : принимается значение, полученное с половинным шагом; шаг удваивается.\n\
");
    tableInfoLabel->setText("\
Смысл столбцов таблицы:\n\
'i' - номер шага метода.\n\
'x_i'  - значение 'x' текущей точки численного решения.\n\
'u_i'  - значение 'u' (точного решения) в момент 'x_i'.\n\
'v_i'  - значение 'v' в момент 'x_i', которое было принято методом.\n\
'v_2i' - значение 'v', полученное из точки, предшествующей 'x_i', двойным счётом с половинным шагом.\n\
'u_i - v_i'  - глобальная погрешность в момент 'x_i'.\n\
'v_i - v_2i' - разность 'v_i' и 'v_2i'.\n\
'ОЛП' - оценка локальной погрешности в точке в точке, предшествующей 'x'.\n\
'h_i' - шаг, с которым ПРИШЛИ в точку 'x_i' из предыдущей точки.\n\
'C+' - количество удвоений шага интегрирования, выполненных на предыдущем шаге метода.\n\
'C-' - количество делений вдвое шага интегрирования, выполненных на предыдущем шаге метода.\n\
");
    ui->Table->verticalHeader()->setVisible(false);
    initializePlotWidget(ui->mainPlot);
}


MainWindow::~MainWindow()
{
    progInfo->close();
    tableInfo->close();
    solutionInfo->close();
    delete progInfo;
    delete tableInfo;
    delete solutionInfo;
    delete ui;
}

bool MainWindow::checkInput(const QString &str_u0, const QString &str_x_max, const QString &str_h,
                            const QString &str_x_eps, const QString &str_v_eps, const QString &str_n_max, const QString &str_a, const QString &str_b,
                            double &u0, double &x_max, double &h, double &x_eps, double &v_eps, int &n_max, double &a, double &b)
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
    else if (str_n_max.isEmpty())
        QMessageBox::critical(this, "Ошибка!", "Поле 'N_max' должно быть непустым");
    else if (str_a.isEmpty())
        QMessageBox::critical(this, "Ошибка!", "Поле 'a' должно быть непустым");
    else if (str_b.isEmpty())
        QMessageBox::critical(this, "Ошибка!", "Поле 'ϑ' должно быть непустым");
    else
        result = true;
    if (result == false)
        return false;


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
    n_max = str_n_max.toInt(&result);
    if (result == false)
    {
        QMessageBox::critical(this, "Ошибка!", "Поле 'N_max' имеет некорректное значение");
        return false;
    }
    a     = str_a.toDouble(&result);
    if (result == false)
    {
        QMessageBox::critical(this, "Ошибка!", "Поле 'a' имеет некорректное значение");
        return false;
    }
    b     = str_b.toDouble(&result);
    if (result == false)
    {
        QMessageBox::critical(this, "Ошибка!", "Поле 'ϑ' имеет некорректное значение");
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
    else if (n_max <= 0)
        QMessageBox::critical(this, "Ошибка!", "Максимальное число шагов 'N_max' должно быть положительным");
    else if (a <= 0.0)
        QMessageBox::critical(this, "Ошибка!", "Параметр 'a' должен быть положительным");
    else
        result = true;


    return result;
}

void fillSolutionInfoLabel(QLabel *label, const vector<Entry<double, 1>> &solution, double x_max, QString &solutionInfoString, int numOfIterations)
{
    size_t n = solution.size();
    double h_min = solution[0].h, h_max = h_min;
    double s_star_max = 0.0, s_star_min = numeric_limits<double>::max();
    size_t h_min_idx = 0, h_max_idx = 0;
    size_t s_star_max_idx = 0, s_star_min_idx = 0;
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
        if (abs(solution[i].s_star) > s_star_max) {
            s_star_max = abs(solution[i].s_star);
            s_star_max_idx = i;
        }
        if (i && abs(solution[i].s_star) < s_star_min) {
            s_star_min = abs(solution[i].s_star);
            s_star_min_idx = i;
        }
    }

    solutionInfoString += "\n\nРезультат расчёта:";
    solutionInfoString += "\nЧисло шагов метода: N = " + QString::number(numOfIterations);
    solutionInfoString += "\nТочность выхода на правую границу: " + QString::number(x_max - solution.back().x);
    solutionInfoString += "\nx_N = " + QString::number(solution.back().x) + ", v_N = " + QString::number(solution.back().v);
    solutionInfoString += "\n\nМаксимум модуля ОЛП: " + QString::number(s_star_max) + " при x = "
                                                      + QString::number(solution[s_star_max_idx].x);
    solutionInfoString += "\nМинимум модуля ОЛП: "  + QString::number(s_star_min) + " при x = "
                                                      + QString::number(solution[s_star_min_idx].x);

    solutionInfoString += "\n\nОбщее число удвоений шага: " + QString::number(plus_count);
    solutionInfoString += "\nОбщее число делений шага: " + QString::number(minus_count);

    solutionInfoString += "\n\nМинимальный шаг: " + QString::number(h_min) + " при x = " + QString::number(solution[h_min_idx].x);
    solutionInfoString += "\nМаксимальный шаг: " + QString::number(h_max) + " при x = " + QString::number(solution[h_max_idx].x);

    label->setText(solutionInfoString);
}

void MainWindow::on_pushButton_pressed()
{
    // Ввод данных и проверка их корректности
        QString str_u0    = ui->lineEdit_u0   ->text(),
                str_x_max = ui->lineEdit_x_max->text(),
                str_h     = ui->lineEdit_h    ->text(),
                str_x_eps = ui->lineEdit_E_x  ->text(),
                str_a     = ui->lineEdit_a    ->text(),
                str_b     = ui->lineEdit_teta ->text(),
                str_v_eps = ui->lineEdit_E_nu ->text(),
                str_n_max = ui->lineEdit_N_max->text();

        double u0, x_max, h, x_eps, v_eps, a, b;
        int n_max;
        if (!checkInput(str_u0, str_x_max, str_h, str_x_eps,  str_v_eps, str_n_max, str_a, str_b,
                            u0,     x_max,     h,     x_eps,      v_eps,     n_max,     a,     b)) //пока убрал x0, потому что по заданию у нас  x0 = 0
        {
            return;
        }

    // Численное решение
        auto f = [&](double x, double u)
        {
            return -a*(u-b);
        };
        auto soln = ivp_step_adjust(rk4, Diff_equation<double,1>{f}, 0.0, u0, h, x_max, x_eps, v_eps, n_max);
        auto solution = soln.first;
        int numOfIterations = soln.second;

    // Заполнение таблицы
        size_t n = solution.size();
        auto model = (QStandardItemModel*)ui->Table->model();
        QVector<double> u(n);
        for (int i = 0; i < n; ++i)
            u[i] = (u0 - b)*exp(-a*solution[i].x) + b;
        model->setRowCount(n);
        for (size_t i = 0; i < n; ++i) {
            model->setData(model->index(i,0), i);
            model->setData(model->index(i,1), solution[i].x);
            model->setData(model->index(i,2), u[i]);
            model->setData(model->index(i,3), solution[i].v);
            model->setData(model->index(i,4), solution[i].v2);
            model->setData(model->index(i,5), u[i] - solution[i].v);
            model->setData(model->index(i,6), solution[i].v - solution[i].v2);
            model->setData(model->index(i,7), solution[i].s_star);
            model->setData(model->index(i,8), solution[i].h);
            model->setData(model->index(i,9), solution[i].c_plus);
            model->setData(model->index(i,10), solution[i].c_minus);
        }
        ui->Table->resizeColumnsToContents();

    // Построение графиков

        QVector<double> x(n), v(n), v2(n);
        for (size_t i = 0; i < n; ++i)
        {
            x[i]        = solution[i].x;
            v[i]        = solution[i].v;
            v2[i]       = solution[i].v2;
        }

    // График численного решения
        double y_min = *min_element(v.begin(), v.end()),
               y_max = *max_element(v.begin(), v.end());

        QCustomPlot *plot = ui->mainPlot;
        plot->clearGraphs();

        plot->xAxis->setRange(0,x_max);
        plot->yAxis->setRange(y_min, y_min + x_max);

        plot->legend->setVisible(true);
        plot->xAxis->setLabel("x");
        plot->yAxis->setLabel("u");

        plot->addGraph();
        plot->graph(0)->addData(x, v);
        plot->graph(0)->setPen(QPen(Qt::blue));
        plot->graph(0)->setName("Численное решение v(x)");

        plot->addGraph();
        plot->graph(1)->addData(x, v2);
        plot->graph(1)->setPen(QPen(Qt::green));
        plot->graph(1)->setName("Численное решение с половинным шагом v2(x)");

        plot->addGraph();
        plot->graph(2)->addData(x, u);
        plot->graph(2)->setPen(QPen(Qt::red));
        plot->graph(2)->setName("Точное решение u(x)");

        plot->replot();

        isSolvedOnce = true;
        QString solutionInfoString;
        solutionInfoString += "№ варианта задания: 7";
        solutionInfoString += "\nТип задачи: тестовая";
        solutionInfoString += "\nМетод Рунге-Кутта порядка 'p = 4'";
        solutionInfoString += "\nСпособы счёта - выход на правую границу ИЛИ выполнение заданного числа шагов метода";
        solutionInfoString += "\n\nx0 = " + QString::number(0) + ", u0 = " + QString::number(u0);
        solutionInfoString += "\nx_max = " + QString::number(x_max) + ", ε_x = " + QString::number(x_eps);
        solutionInfoString += "\nh0 = " + QString::number(h) + ", N_max = " + QString::number(n_max);
        solutionInfoString += "\nКонтроль локальной погрешности включён";
        solutionInfoString += "\nε = " + QString::number(16.0*v_eps) + ", ε_min = " + QString::number(0.5*v_eps);
        fillSolutionInfoLabel(solutionInfoLabel, solution, x_max, solutionInfoString, numOfIterations);
        ui->pushButton_2->click();
}

void MainWindow::on_pushButton_2_pressed()
{
    if (!isSolvedOnce)
        return;

    if (solutionInfo != nullptr && !solutionInfo->isHidden())
        solutionInfo->hide();

    solutionInfo->show();
}


void MainWindow::on_infoButton_pressed()
{
    if (progInfo != nullptr && !progInfo->isHidden())
        progInfo->hide();

    progInfo->show();
}


void MainWindow::on_tableButton_pressed()
{
    if (tableInfo != nullptr && !tableInfo->isHidden())
        tableInfo->hide();

    tableInfo->show();
}

