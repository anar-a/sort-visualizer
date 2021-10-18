#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPainter>
#include <QTimer>
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    populateData();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::populateData(){
    srand(time(0));
    this->data = QVector<int>(100);

    for (int i = 0; i < this->data.size(); i++){
        this->data[i] = rand()%100;
    }

}

// Classic bubble sort that calls update to the painter
void MainWindow::bubbleSort(QVector<int> &toSort){
    for (int i = 0; i < toSort.size() - 1; i++){
        for (int j = 0; j < toSort.size() - i - 1; j++){
            if (toSort[j] > toSort[j+1]) {
                int temp = toSort[j+1];
                toSort[j+1] = toSort[j];
                toSort[j] = temp;

                update(); // redraw the array
                pauseLoop(20); // pause in MS before next swap
            }
        }
    }
}

void MainWindow::pauseLoop(int mSecPause)
{
    QEventLoop loop;
    QTimer::singleShot(mSecPause, &loop, &QEventLoop::quit);
    loop.exec();
}

void MainWindow::drawAllData(QPainter& painter){
    int rectWidth = this->width() / this->data.size();

    for (int i = 0; i < this->data.size(); i++){
        int posX = rectWidth * i;
        QRect rect(posX, this->height() - this->data.at(i)*4 - 10, rectWidth, this->data.at(i) * 4);
        painter.drawRect(rect);

    }
}

void MainWindow::paintEvent(QPaintEvent *event){
    QPainter painter(this);

    this->drawAllData(painter);
}


void MainWindow::on_sort_released()
{
    bubbleSort(this->data);


}

