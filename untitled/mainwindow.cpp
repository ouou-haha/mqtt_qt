#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    client = new healthmsg("123.207.9.26", "1883", "/mqtt", ui->pushButton, ui->label, ui->label_2);
    client->connect();
}

MainWindow::~MainWindow()
{
    delete ui;
}

