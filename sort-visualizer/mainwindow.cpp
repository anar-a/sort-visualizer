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
    ui->comboBox->addItems({"Bubble sort", "Selection sort", "Insertion sort"});

    // set default iteration delay and value amount
    this->iterationDelay = ui->delay->value();
    this->totalValues = ui->values->value();


    // generate new seed
    srand(time(0));
    this->data = QVector<int>(this->totalValues);
    populateData();     //fill the data array randomly

    this->redRectangles = QVector<QRect>();
    this->blueRectangles = QVector<QRect>();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::populateData(){
    // if the data vector size isnt equal to the ticker value amount
    if (this->data.size() != this->totalValues) {
        this->data = QVector<int>(this->totalValues);
    }

    // picks the value based on the window height plus 100 pixel padding
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
    for (int i = 0; i < this->data.size(); i++){
        QRect rect = drawIndicator(this->data.at(i), i);
        painter.fillRect(rect, Qt::transparent);
        painter.drawRect(rect);

    }
}

// return a rectangle given a height value and an index position
QRect MainWindow::drawIndicator(int value, int position)
{
    int rectWidth = this->width() / this->data.size();
    return QRect(rectWidth * position, this->height() - value - 10, rectWidth, value);
}

void MainWindow::drawRectangles(QPainter& painter, QVector<QRect> &rectangles, Qt::GlobalColor color)
{
    for (unsigned int i = 0; i < rectangles.size(); i++){
        QRect rect = rectangles.at(i);
        painter.fillRect(rect, color);
        painter.drawRect(rect);
    }

    rectangles.clear();
}

// Classic bubble sort that calls update for QPainter redraw
void MainWindow::bubbleSort(QVector<int> &toSort){
    bool breakVar = true; // variable to stop the iterating if cancel clicked

    // set break condition on cancel button click
    connect(ui->sort, &QAbstractButton::released, this, [&breakVar](){breakVar = false;});

    for (int i = 0; i < toSort.size() - 1 && breakVar; i++){
        for (int j = 0; j < toSort.size() - i - 1 && breakVar; j++){\
            this->blueRectangles.append(drawIndicator(this->data.at(j), j)); // hightlight current value
            this->redRectangles.append(drawIndicator(this->data.at(j+1), j+1)); // hightlight next value

            // update called before swap, or lag behind
            update(); // redraw the array
            pauseLoop(this->iterationDelay); // pause in MS before next swap

            if (toSort[j] > toSort[j+1]) {
                int temp = toSort[j+1];
                toSort[j+1] = toSort[j];
                toSort[j] = temp;

            }

        }

    }
    update();
}

void MainWindow::selectionSort(QVector<int> &toSort)
{
    bool breakVar = true; // variable to stop the iterating if cancel clicked

    // set break condition on cancel button click
    connect(ui->sort, &QAbstractButton::released, this, [&breakVar](){breakVar = false;});

    for (int i = 0; i < toSort.size() && breakVar; i++){
        int minIndex = i;

        for (int j = i+1; j < toSort.size() && breakVar; j++){
            this->redRectangles.append(drawIndicator(this->data.at(j), j));

            if (toSort.at(j) < toSort.at(minIndex)){
                minIndex = j;

            }
            this->blueRectangles.append(drawIndicator(this->data.at(minIndex), minIndex));

            pauseLoop(this->iterationDelay); // pause in MS before next swap
            update(); // redraw the array

        }

        int temp = toSort[i];
        toSort[i] = toSort[minIndex];
        toSort[minIndex] = temp;
    }
    update();
}

void MainWindow::insertionSort(QVector<int> &toSort)
{
    bool breakVar = true;

    // set break condition on cancel button click
    connect(ui->sort, &QAbstractButton::released, this, [&breakVar](){breakVar = false;});

    for (unsigned int i = 1; i < this->data.size() && breakVar; i++){
        int val = this->data[i];
        int j = i-1;

        while (j >= 0 && this->data[j] > val && breakVar) {
            this->data[j+1] = this->data[j];

            // draw indicators and update painter
            redRectangles.append(drawIndicator(this->data[j+1], j));
            blueRectangles.append(drawIndicator(val, j));
            update();
            pauseLoop(this->iterationDelay);

            j--;
        }

        this->data[j+1] = val;
        update(); // final painter update to clear old indicators

    }

}

void MainWindow::paintEvent(QPaintEvent *){
    QPainter painter(this);
    drawAllData(painter);

    // draw the leftover rectangles if any
    drawRectangles(painter, redRectangles, Qt::red);
    drawRectangles(painter, blueRectangles, Qt::blue);
}



// Button bindings below

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
        else if (this->selectedAlgorithm == "Insertion sort"){
            insertionSort(this->data);
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

