#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "font.h"

#include <QLabel>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // font load
    auto current_path = QCoreApplication::applicationDirPath();
    auto font_path = current_path + "/font/NotoSansCJKjp-Regular.otf";

    Font().family(font_path);

    auto widget = new QWidget(this);
    this->setCentralWidget(widget);

    auto layout = new QVBoxLayout(this);
    widget->setLayout(layout);

    for (int i = 0; i < 10; i+=2) {
        auto label = new QLabel(QString("font text %1").arg(12+i), this);
        layout->addWidget(label);

        auto font = label->font();
        qDebug() << font.pointSize();
        font.setPointSize(12 + i);
        label->setFont(font);
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}

