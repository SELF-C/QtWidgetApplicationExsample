#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "floatingwidget.h"
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    auto fwidget = new FloatingWidget(this);
    fwidget->setFixedSize(150, 200);
    fwidget->setObjectName("FWidget");
    fwidget->setStyleSheet("#FWidget{ background-color: red }");
    auto btn = new QPushButton("Floating Widget", fwidget);


}

MainWindow::~MainWindow()
{
    delete ui;
}

