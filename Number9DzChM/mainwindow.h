/*
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
*/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QTabWidget>
#include <QTableView>
#include <QRadioButton>
#include <QGraphicsScene>
#include <QLabel>
#include <qcustomplot.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_pressed();

    void on_pushButton_2_pressed();

    void on_infoButton_pressed();

    void on_tableButton_pressed();

    void on_clearButton_pressed();

private:
    Ui::MainWindow *ui;

    QWidget *solutionInfo,
            *progInfo,
            *tableInfo;
    QLabel  *solutionInfoLabel,
            *progInfoLabel,
            *tableInfoLabel;

    bool isSolvedOnce = false;
    size_t graphCount = 0;

    void initializeOutputInfoWidget(QWidget *parent, QLabel *label); //форматирование справки о приложении
    void initializePlotWidget(QCustomPlot *plotWidget); //форматирование виджетов с графиками
    void initializeTable(QTableView *table, size_t rows); //инициализация таблиц (передача моделей в них)
    bool checkInput(const QString &, const QString &, const QString &, const QString &, const QString &, const QString &, const QString &, const QString &,
                    double &, double &, double &, double &, double &, int&, double &, double &);
};

#endif // MAINWINDOW_H
