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
  //  void on_testRadioButton_clicked();
  //  void on_testRadioButton2_clicked();

  //  void on_testSolveButton_pressed();

  //  void on_testShowSolnInfoButton_clicked();

  //  void on_main1SolveButton_clicked();

  //  void on_main1RadioButton_clicked();

  //  void on_main1RadioButton2_clicked();

  //  void on_main1ShowSolnInfoButton_clicked();

  //  void on_introTextButton_clicked(); //справка

  //  void on_main2SolveButton_clicked();

  //  void on_main2RadioButton_clicked();

  //  void on_main2RadioButton2_clicked();

    void on_main2ShowSolnInfoButton_clicked();

    void on_pushButton_pressed();

private:
    Ui::MainWindow *ui;

    QWidget //*testSolutionInfo,
//            *main1SolutionInfo,
           *main2SolutionInfo,
            *introReference;
    QLabel // *testSolutionInfoLabel,
//           *main1SolutionInfoLabel,
           *main2SolutionInfoLabel,
           *introReferenceLabel;
    QCPCurve *phase;
 //   bool testIsSolvedOnce = false;
 //   bool main1IsSolvedOnce = false;
    bool main2IsSolvedOnce = false;

    void initializeOutputInfoWidget(QWidget *parent, QLabel *label); //форматирование справки о приложении
    void initializePlotWidget(QCustomPlot *plotWidget, QWidget *backgroundWidget); //форматирование виджетов с графиками
    void initializeTestTaskTable(QTableView *table, size_t rows); //инициализация таблиц (передача моделей в них)
    void initializeMainTaskTable(QTableView *table, size_t rows);
    bool checkInput(const QString &, const QString &, const QString &, const QString &, const QString &, const QString &, const QString &,
                    double &, double &, double &, double &, double &, double &, double &);
    bool checkMain2Input(const QString &, const QString &, const QString &, double &, double &, double &);
};

#endif // MAINWINDOW_H
