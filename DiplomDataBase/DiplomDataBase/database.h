#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlTableModel>

#include <QImage>

#include <QDataStream>

#include <QTime>

#include <QSqlError>

/* Директивы имен таблицы, полей таблицы и базы данных */
#define DATABASE_HOSTNAME   "DataBase"
#define DATABASE_NAME       "testDB.db"

#define TABLE                   "ListTable"       // Название таблицы
#define TABLE_ID                "track"              // Вторая колонка
#define TABLE_IMAGE             "img"               // Третья колонка
#define TABLE_TYPE              "type"               // Четвертая колонка
#define TABLE_TIME              "time"               // Пятая колонка
#define TABLE_COORDINATES       "coord"               // Шестая колонка

struct info{
    int id;
    QImage img;
    std::string type;
    timeval time;
    std::pair <double, double> coordinate;
};

class database {
public:
    database();
~database();
    //void add(const info & information);
    bool openDataBase();        // Открытие базы данных
    void closeDataBase();       // Закрытие базы данных
    bool insertIntoTable(const QVariantList &data);
    bool insertIntoTable(const info & information);
    bool createTable();
    bool restoreDataBase();
    bool connectToDataBase();
    void searchInfoId(QString id);
    void searchInfoType(QString type);
    void searchInfoCoord(QString coord);
    void searchInfoTime(QString time);

private:
    QSqlDatabase db;
//    QSqlQuery *query;
    QSqlTableModel *model;

    int row;
};



#endif
