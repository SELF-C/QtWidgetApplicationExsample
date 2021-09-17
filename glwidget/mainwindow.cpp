#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "glwidget_1_1.h"
#include "glwidget_2_0.h"

#define GL_VER_1_1

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->resize(500,500);

#ifndef GL_VER_1_1
    auto glwidget = new GLWidget_2_0(this);
#else
    auto glwidget = new GLWidget_1_1(this);
#endif

    this->setCentralWidget(glwidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}

