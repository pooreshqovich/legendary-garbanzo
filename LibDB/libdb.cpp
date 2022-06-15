#include "libdb.h"

/* Директивы имен таблицы, полей таблицы и базы данных */
#define DATABASE_HOSTNAME   "DataBase"
#define DATABASE_NAME       "testDB.db"

#define TABLE                   "ListTable"       // Название таблицы
#define TABLE_ID                "track"              // Вторая колонка
#define TABLE_IMAGE             "img"               // Третья колонка
#define TABLE_TYPE              "type"               // Четвертая колонка
#define TABLE_TIME              "time"               // Пятая колонка
#define TABLE_COORDINATES       "coord"               // Шестая колонка

LibDB::LibDB()
{
}

LibDB::~LibDB()
{

}

/* Метод для открытия базы данных
 * */
bool LibDB::openDataBase(QString databaseName)
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(databaseName);
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
bool LibDB::connectToDataBase(QString databaseName)
{
    if(!QFile(databaseName).exists()){
        this->restoreDataBase(databaseName);
    }
    else {
        this->openDataBase(databaseName);
    }
    return true;
}

//Метод для конвертации из Строковой в Пару
std::pair <double, double> StringToPair(QString coordinates){
    QStringList list = coordinates.split(" ");
    double lat = list[0].toDouble();
    double lon = list[1].toDouble();
    return std::pair <double, double>(lat, lon);
}

  //ПОИСК ПО ID
QList <outputInfo> LibDB::searchInfoId(QString id) {
    QList <outputInfo> outpInfo;
    QSqlQuery query;
    QString header = "SELECT * FROM ListTable WHERE track = ";
    QString request = header + "'" + id + "'";
      if(!query.exec(request)){
              std::cout << "Unable to execute query - exiting" << std::endl;
          }
       while(query.next()){
           outputInfo information;
       // смещаем на 1 вперед от значений поиска из insertIntoTable
           information.id = query.value(1).toInt();
//           information.img = QImage::fromData(query.value(2).toByteArray(),"JPG");
           information.type = query.value(3).toString().toStdString();
           information.time = query.value(4).toString().toStdString();
           information.coordinate = StringToPair(query.value(5).toString());
           outpInfo.push_back(information);
       }
       return outpInfo;
}

  //ПОИСК ПО ТИПУ
QList <outputInfo> LibDB::searchInfoType(QString type)
{
    QList <outputInfo> outpInfo;
      QSqlQuery query;
    QString header = "SELECT * FROM ListTable WHERE type = ";
    QString request = header + "'" + type + "'";
      if(!query.exec(request)){
              std::cout << "Unable to execute query - exiting" << std::endl;
          }
       while(query.next()){
           outputInfo information;
       // смещаем на 1 вперед от значений поиска из insertIntoTable
           information.id = query.value(1).toInt();
           information.img = query.value(2).toByteArray();
           information.type = query.value(3).toString().toStdString();
           information.time = query.value(4).toString().toStdString();
           information.coordinate = StringToPair(query.value(5).toString());
           outpInfo.push_back(information);
       }
       return outpInfo;
}

//ПОИСК ПО ВРЕМЕНИ
QList <outputInfo> LibDB::searchInfoTime(QString time)
{
    QList <outputInfo> outpInfo;
    QSqlQuery query;
  QString header = "SELECT * FROM ListTable WHERE type = ";
  QString request = header + "'" + time + "'";
    if(!query.exec(request)){
            std::cout << "Unable to execute query - exiting" << std::endl;
        }
     while(query.next()){
           outputInfo information;
         // смещаем на 1 вперед от значений поиска из insertIntoTable
             information.id = query.value(1).toInt();
//             information.img = QImage::fromData(query.value(2).toByteArray(),"JPG");
             information.type = query.value(3).toString().toStdString();
             information.time = query.value(4).toString().toStdString();
             information.coordinate = StringToPair(query.value(5).toString());
             outpInfo.push_back(information);
     }
     return outpInfo;
}

void LibDB::setAccuracy(double accuracy)
{
    this->accuracy = accuracy;
}

//ПОИСК ПО КООРДИНАТАМ
QList <outputInfo> LibDB::searchInfoCoord(QString coord)
{
    QList <outputInfo> outpInfo;
  QStringList list = coord.split(" ");
  double baselat = list[0].toDouble();
  double baselon = list[1].toDouble();
  QSqlQuery query;
  QString header = "SELECT * FROM ListTable";
//  QString request = header + "'" + coord + "'";
    if(!query.exec(header)){
            std::cout << "Unable to execute query - exiting" << std::endl;
        }
     QString BarCode;
     while(query.next()){
     // смещаем на 1 вперед от значений поиска из insertIntoTable
         BarCode = query.value(5).toString();
         QStringList list = BarCode.split(" ");
         double lat = list[0].toDouble();
         double lon = list[1].toDouble();
         if(abs(baselat-lat)<accuracy && abs(baselon - lon)<accuracy){
             outputInfo information;
         // смещаем на 1 вперед от значений поиска из insertIntoTable
             information.id = query.value(1).toInt();
//             information.img = QImage::fromData(query.value(2).toByteArray(),"JPG");
             information.type = query.value(3).toString().toStdString();
             information.time = query.value(4).toString().toStdString();
             information.coordinate = StringToPair(query.value(5).toString());
             outpInfo.push_back(information);
         }
     }
     return outpInfo;
}

/* Методы восстановления базы данных
 * */
bool LibDB::restoreDataBase(QString databaseName)
{
    // Если база данных открылась ...
    if(this->openDataBase(databaseName)){
        // Производим восстановление базы данных
        return (this->createTable()) ? true : false;
    } else {
        std::cout << "Не удалось восстановить базу данных" << std::endl;
        return false;
    }
    return false;
}

/* Методы закрытия базы данных
 * */
void LibDB::closeDataBase()
{
    db.close();
}

/* Метод для вставки записи в базу данных
 * */
bool LibDB::insertIntoTable(const QVariantList &data)
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
        std::cout << "error insert into " << TABLE << std::endl;
        std::cout << query.lastError().text().toStdString() << std::endl;
        return false;
    } else {
        return true;
    }
    return false;
}

/* Второй метод для вставки записи в базу данных
 * */
bool LibDB::insertIntoTable(const info & information)
{
    QVariantList data;
    data.append(information.id);
    QByteArray qba;
    QBuffer buff(&qba);
    QPixmap pmap = QPixmap::fromImage(information.img);
    pmap.save(&buff, "JPG");
//            QByteArray::fromRawData((const char*)information.img.bits(),information.img.byteCount());
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

bool LibDB::createTable()
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
           std::cout << "DataBase: error of create " << TABLE << std::endl;
           std::cout << query.lastError().text().toStdString() << std::endl;
           db.close();
           return false;
       } else {
           return true;
       }
       return false;
}
