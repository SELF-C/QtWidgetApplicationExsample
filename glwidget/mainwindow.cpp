#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "glwidget_1_1.h"
#include "glwidget_2_0.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->resize(500,500);

    auto glwidget = new GLWidget_1_1(this);
    //auto glwidget = new GLWidget_2_0(this);


    this->setCentralWidget(glwidget);

}

MainWindow::~MainWindow()
{
    delete ui;
}

