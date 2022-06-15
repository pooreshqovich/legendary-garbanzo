#include <database.h>
#include <QByteArray>
#include <QFile>

database::database()
{
//    query = new QSqlQuery(db);
//    query->exec("CREATE TABLE TestTable(Image TEXT, Type TEXT, Time TEXT, Coordinates TEXT);");

//    model = new QSqlTableModel (nullptr, db);
//    model->setTable("TestTable");
//    model->select();
}

database::~database()
{

}

/* Метод для открытия базы данных
 * */
bool database::openDataBase()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("./testDB.db");
    if(db.open()){
        return true;
    }
    else {
        return false;
    }
}

/* Перед подключением к базе данных производим проверку на её существование.
     * В зависимости от результата производим открытие базы данных или её восстановление
     * */
bool database::connectToDataBase()
{
    if(!QFile("./testDB.db").exists()){
        this->restoreDataBase();
    }
    else {
        this->openDataBase();
    }
    return true;
}

  //ПОИСК ПО ID
void database::searchInfoId(QString id) {
      QSqlQuery query;
    QString header = "SELECT * FROM ListTable WHERE track = ";
    QString request = header + "'" + id + "'";
      if(!query.exec(request)){
              qDebug() << "Unable to execute query - exiting";
          }
       QString BarCode;
       while(query.next()){
       // смещаем на 1 вперед от значений поиска из insertIntoTable
           BarCode = query.value(4).toString();
           qDebug() << "BarCode:" << BarCode;
       }
}
  //ПОИСК ПО ТИПУ
void database::searchInfoType(QString type)
{
      QSqlQuery query;
    QString header = "SELECT * FROM ListTable WHERE type = ";
    QString request = header + "'" + type + "'";
      if(!query.exec(request)){
              qDebug() << "Unable to execute query - exiting";
          }
       QString BarCode;
       while(query.next()){
       // смещаем на 1 вперед от значений поиска из insertIntoTable
           BarCode = query.value(4).toString();
           qDebug() << "BarCode:" << BarCode;
       }
}

//ПОИСК ПО ВРЕМЕНИ
void database::searchInfoTime(QString time)
{
    QSqlQuery query;
  QString header = "SELECT * FROM ListTable WHERE type = ";
  QString request = header + "'" + time + "'";
    if(!query.exec(request)){
            qDebug() << "Unable to execute query - exiting";
        }
     QString BarCode;
     while(query.next()){
     // смещаем на 1 вперед от значений поиска из insertIntoTable
         BarCode = query.value(4).toString();
         qDebug() << "BarCode:" << BarCode;
     }
}

//ПОИСК ПО КООРДИНАТАМ
void database::searchInfoCoord(QString coord)
{
  QStringList list = coord.split(" ");
  double baselat = list[0].toDouble();
  double baselon = list[1].toDouble();
  QSqlQuery query;
  QString header = "SELECT * FROM ListTable";
//  QString request = header + "'" + coord + "'";
    if(!query.exec(header)){
            qDebug() << "Unable to execute query - exiting";
        }
     QString BarCode;
     while(query.next()){
     // смещаем на 1 вперед от значений поиска из insertIntoTable
         BarCode = query.value(5).toString();
         QStringList list = BarCode.split(" ");
         double lat = list[0].toDouble();
         double lon = list[1].toDouble();
         if(abs(baselat-lat)<0.1 && abs(baselon - lon)<0.1){
            qDebug() << "LatLon: " << lat << lon;
         }
     }
}

/* Методы восстановления базы данных
 * */
bool database::restoreDataBase()
{
    // Если база данных открылась ...
    if(this->openDataBase()){
        // Производим восстановление базы данных
        return (this->createTable()) ? true : false;
    } else {
        qDebug() << "Не удалось восстановить базу данных";
        return false;
    }
    return false;
}

/* Методы закрытия базы данных
 * */
void database::closeDataBase()
{
    db.close();
}

/* Метод для вставки записи в базу данных
 * */
bool database::insertIntoTable(const QVariantList &data)
{
    /* Запрос SQL формируется из QVariantList,
     * в который передаются данные для вставки в таблицу.
     * */
    QSqlQuery query;
    /* В начале SQL запрос формируется с ключами,
     * которые потом связываются методом bindValue
     * для подстановки данных из QVariantList
     * */
    query.prepare(" INSERT INTO " TABLE " ( " TABLE_ID ", " TABLE_IMAGE ", " TABLE_TYPE ", " TABLE_TIME ", " TABLE_COORDINATES " ) "
                  "VALUES (:track, :img, :type, :time, :coord)");
    query.bindValue(":track",        data[0].toString());
    query.bindValue(":img",         data[1].toByteArray());
    query.bindValue(":type",        data[2].toString());
    query.bindValue(":time",        data[3].toString());
    query.bindValue(":coord",        data[4].toString());

    // После чего выполняется запросом методом exec()
    if(!query.exec()){
        qDebug() << "error insert into " << TABLE;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
    return false;
}

/* Второй метод для вставки записи в базу данных
 * */
bool database::insertIntoTable(const info & information)
{
    QVariantList data;
    data.append(information.id);
    QByteArray qba = QByteArray::fromRawData((const char*)information.img.bits(),information.img.byteCount());
    data.append(qba);
    data.append(QString::fromStdString(information.type));
    data.append(QDateTime::fromSecsSinceEpoch(information.time.tv_sec).toString());
    QString coordinate = QString::number(information.coordinate.first) + " " + QString::number(information.coordinate.second);
    data.append(coordinate);

    if(insertIntoTable(data))
        return true;
    else
        return false;
}

bool database::createTable()
{
    /* В данном случае используется формирование сырого SQL-запроса
        * с последующим его выполнением.
        * */
       QSqlQuery query;
       if(!query.exec( " CREATE TABLE " TABLE " ("
                               " id INTEGER PRIMARY KEY AUTOINCREMENT, "
                               TABLE_ID     " VARCHAR(255)    NOT NULL,"
                               TABLE_IMAGE      " BLOB            NOT NULL,"
                               TABLE_TYPE   " VARCHAR(255)   NOT NULL,"
                               TABLE_TIME   " VARCHAR(255)   NOT NULL,"
                               TABLE_COORDINATES   " VARCHAR(255)   NOT NULL"
                           " )"
                       )){
           qDebug() << "DataBase: error of create " << TABLE;
           qDebug() << query.lastError().text();
           db.close();
           return false;
       } else {
           return true;
       }
       return false;
}
