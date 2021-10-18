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
private slots:
    void on_sort_clicked();

    void on_comboBox_currentTextChanged(const QString &arg1);

    void on_reset_clicked();

    void on_delay_valueChanged(int delayValue);

    void on_values_valueChanged(int value);

private:
    Ui::MainWindow *ui;

    QVector<int> data;
    QVector<QRect> rectangles;

    bool currentlySorting = false;

    // defaulted in constructor to UI defaults
    int iterationDelay;
    int totalValues;

    QString selectedAlgorithm = "None";

    void drawAllData(QPainter &painter);
    void pauseLoop(int mSecPause);
    void populateData();

    void bubbleSort(QVector<int>& toSort);
    void selectionSort(QVector<int>& toSort);

    virtual void paintEvent(QPaintEvent *event);

};
#endif // MAINWINDOW_H
