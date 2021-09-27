#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "glwidget.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->resize(500,500);

    auto glwidget = new GLWidget(this);
    this->setCentralWidget(glwidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}

