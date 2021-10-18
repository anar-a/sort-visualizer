#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    virtual void paintEvent(QPaintEvent *event);
    void populateData();

private slots:
    void on_sort_released();

private:
    Ui::MainWindow *ui;
    QVector<int> data;

    void drawAllData(QPainter& painter);
    void bubbleSort(QVector<int>& toSort);
    void pauseLoop(int mSecPause);

};
#endif // MAINWINDOW_H
