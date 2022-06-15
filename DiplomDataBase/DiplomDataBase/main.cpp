#include <QCoreApplication>
#include <database.h>
#include <sys/time.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    database dbase;
    if(dbase.connectToDataBase()) {
        qDebug()<<"okay";
        info nform;
        nform.id = 1;
        nform.img = QImage("./imaga.jpg");
        gettimeofday(&nform.time, NULL);
        nform.type = "imaga";
        nform.coordinate = std::pair<double, double>(1.1, 6.5);
//        dbase.insertIntoTable(nform);
        dbase.searchInfoId("3");
        dbase.searchInfoType("imaga");
        dbase.searchInfoCoord("1.25 6.55");

    }
    else {
        qDebug()<<"not okay";
    }
    dbase.closeDataBase();
    return a.exec();
}
