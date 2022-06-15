#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QDebug>
#include <sys/time.h>
#include <QtEndian>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    db.connectToDataBase("./testDB.db");

    info nform;
    nform.id = 2;
    nform.img = QImage("./imaga.jpg");
    gettimeofday(&nform.time, NULL);
    nform.type = "imaga";
    nform.coordinate = std::pair<double, double>(1.1, 6.5);
    db.insertIntoTable(nform);


//    ui->label->setPixmap(QPixmap::fromImage(db.searchInfoType("imaga").last().img.rgbSwapped()));
    qDebug() << db.searchInfoType("imaga").last().img.length();
    QPixmap qpix = QPixmap();
    qpix.loadFromData(db.searchInfoType("imaga").last().img, "JPG", Qt::ColorOnly);
//    qpix.load("./imaga.jpg");
    ui->label->setPixmap(qpix);

}

MainWindow::~MainWindow()
{
    delete ui;
}

