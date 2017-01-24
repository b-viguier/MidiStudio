#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "statuswidget.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    auto *statusWidget = new StatusWidget(this);
    addDockWidget(Qt::LeftDockWidgetArea, statusWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}
