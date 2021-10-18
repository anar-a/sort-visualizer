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
    ui->comboBox->addItems({"Bubble sort", "Selection sort"});

    // set default iteration delay and value amount
    this->iterationDelay = ui->delay->value();
    this->totalValues = ui->values->value();


    // generate new seed
    srand(time(0));
    this->data = QVector<int>(this->totalValues);
    populateData();     //fill the data array randomly

    this->rectangles = QVector<QRect>(100);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::populateData(){
    if (this->data.size() != this->totalValues) {
        this->data = QVector<int>(this->totalValues);
    }

    int valueHeight = this->height() - 100;
    for (int i = 0; i < this->data.size(); i++){
        this->data[i] = rand() % valueHeight;
    }

}

void MainWindow::pauseLoop(int mSecPause)
{
    QEventLoop loop;
    QTimer::singleShot(mSecPause, &loop, &QEventLoop::quit);
    loop.exec();
}

void MainWindow::drawAllData(QPainter &painter){
    int rectWidth = this->width() / this->data.size();

    for (int i = 0; i < this->data.size(); i++){
        int posX = rectWidth * i;
        QRect rect(posX, this->height() - this->data.at(i) - 10, rectWidth, this->data.at(i));
        painter.drawRect(rect);

    }
}

// Classic bubble sort that calls update for QPainter redraw
void MainWindow::bubbleSort(QVector<int> &toSort){
    bool breakVar = true; // variable to stop the iterating if cancel clicked

    // set break condition on cancel button click
    connect(ui->sort, &QAbstractButton::released, this, [&breakVar](){breakVar = false;});

    for (int i = 0; i < toSort.size() - 1 && breakVar; i++){
        for (int j = 0; j < toSort.size() - i - 1 && breakVar; j++){
            if (toSort[j] > toSort[j+1]) {
                int temp = toSort[j+1];
                toSort[j+1] = toSort[j];
                toSort[j] = temp;

                update(); // redraw the array
                pauseLoop(this->iterationDelay); // pause in MS before next swap
            }
        }

    }
}

void MainWindow::selectionSort(QVector<int> &toSort)
{

}

void MainWindow::paintEvent(QPaintEvent *){
    QPainter painter(this);
    drawAllData(painter);
}



void MainWindow::on_sort_clicked()
{
    if (!this->currentlySorting){
        this->currentlySorting = true; // Can't click sort again while still sorting
        ui->sort->setText("Cancel");

        // Sort according to selected algorithm from dropdown menu
        if (this->selectedAlgorithm == "Bubble sort"){
            bubbleSort(this->data);
        }
        else if (this->selectedAlgorithm == "Selection sort"){
            selectionSort(this->data);
        }

        ui->sort->setText("Sort");
        this->currentlySorting = false;
    }

}


void MainWindow::on_comboBox_currentTextChanged(const QString &commandText)
{
    this->selectedAlgorithm = commandText;
}


void MainWindow::on_reset_clicked()
{
    if (!this->currentlySorting){
        populateData();
        update();
    }
}


void MainWindow::on_delay_valueChanged(int delayValue)
{
    this->iterationDelay = delayValue;
}


void MainWindow::on_values_valueChanged(int value)
{
    this->totalValues = value;
    populateData();
    update();
}

