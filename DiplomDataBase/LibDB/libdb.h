#ifndef LIBDB_H
#define LIBDB_H

#include "LibDB_global.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QImage>
#include <QTime>
#include <QSqlError>
#include <QByteArray>
#include <QFile>
#include <iostream>
#include <QVariant>
#include <QList>
#include <QBuffer>
#include <QPixmap>

struct info{
    int id;
    QImage img;
    std::string type;
    timeval time;
    std::pair <double, double> coordinate;
};

struct outputInfo{
    int id;
    QByteArray img;
    std::string type;
    std::string time;
    std::pair <double, double> coordinate;
};

class LIBDB_EXPORT LibDB
{
public:
    LibDB();
~LibDB();
    bool insertIntoTable(const info & information);
    bool connectToDataBase(QString databaseName);
    QList <outputInfo> searchInfoId(QString id);
    QList <outputInfo> searchInfoType(QString type);
    QList <outputInfo> searchInfoCoord(QString coord);
    QList <outputInfo> searchInfoTime(QString time);
    void setAccuracy(double accuracy);

private:
    bool createTable();
    bool openDataBase(QString databaseName);        // Открытие базы данных
    void closeDataBase();       // Закрытие базы данных
    bool insertIntoTable(const QVariantList &data);
    bool restoreDataBase(QString databaseName);

private:
    QSqlDatabase db;
    double accuracy = 0;
};

#endif // LIBDB_H
