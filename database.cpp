#include "database.h"


//建立一个数据库连接
bool DataBase::createConnection()
{
    QSqlDatabase database;
    if (QSqlDatabase::contains("qt_sql_default_connection"))
    {
        database = QSqlDatabase::database("qt_sql_default_connection");
    }
    else
    {
        database = QSqlDatabase::addDatabase("QSQLITE");
        database.setDatabaseName("songlist.db");
        database.setUserName("admin");
        database.setPassword("123456");
    }
    if(!database.open())
    {
        qDebug() << "connect failed.";
        return false;
    }

    return true;
}

//创建数据库表
bool DataBase::createTable()
{
    QSqlDatabase db = QSqlDatabase::database("QSQLITE");
    QSqlQuery sql_query(db);
    QString create_sql = "create table songlist(id varchar primary key,songname varchar,"
                         "artistname varchar,songartist varchar,songdir varchar,imagedir varchar,lyrdir varchar,albumname varchar)";
    //sql_query.prepare();
    if(!sql_query.exec(create_sql))
    {
        qDebug() << "Error: Fail to create table." << sql_query.lastError();
        return false;
    }
    else
    {
        qDebug() << "Table created!";
        return true;
    }
}

//向数据库中插入记录
bool DataBase::insert(QString ID, QString Songname, QString Artistname, QString Songdir, QString Imagedir, QString Lyrdir, QString Albumname)
{
    QSqlDatabase db = QSqlDatabase::database("QSQLITE"); //建立数据库连接
    QSqlQuery query(db);
    query.prepare("INSERT INTO songlist (id,songname,artistname,songartist,songdir,imagedir,lyrdir,albumname)"
                      "VALUES (:id,:songname,:artistname,:songartist,:songdir,:imagedir,:lyrdir,:albumname)");
    query.bindValue(":id", ID);
    query.bindValue(":songname", Songname);
    query.bindValue(":artistname", Artistname);
    query.bindValue(":songartist", Songname + "-" + Artistname);
    query.bindValue(":songdir", Songdir);
    query.bindValue(":imagedir", Imagedir);
    query.bindValue(":lyrdir", Lyrdir);
    query.bindValue(":albumname", Albumname);
    if(!query.exec())
    {
        QSqlError lastError = query.lastError();
        qDebug() << lastError.driverText() << QString(QObject::tr("插入失败"));
        return false;
    }
    return true;
}
QString DataBase::querySong(QString name)
{
    QSqlDatabase db = QSqlDatabase::database("QSQLITE"); //建立数据库连接
    QSqlQuery query(db);
    query.prepare(QString("select songdir from songlist where songartist = '%1'").arg(name));
    if(!query.exec())
    {
        qDebug() << "song not found";
        return "";
    }
    if(query.first())
    {
        //qDebug() << query.value(0).toString();
        return query.value(0).toString();
    }
    return "";
}

Song DataBase::querySongInfo(QString url)
{
    Song song("","","","","","","","");
    QSqlDatabase db = QSqlDatabase::database("QSQLITE"); //建立数据库连接
    QSqlQuery query(db);
    query.prepare(QString("select * from songlist where songdir = '%1'").arg(url));
    if(!query.exec())
    {
        qDebug() << "song not found";
        //return song;
    }
    if(query.first())
    {
        Song song(query.value(0).toString(),
                  query.value(1).toString(),
                  query.value(2).toString(),
                  query.value(3).toString(),
                  query.value(4).toString(),
                  query.value(5).toString(),
                  query.value(6).toString(),
                  query.value(7).toString());
        return song;
    }
    return song;
}

QString DataBase::queryAlbum(QString name)
{
    QSqlDatabase db = QSqlDatabase::database("QSQLITE"); //建立数据库连接
    QSqlQuery query(db);
    query.prepare(QString("select albumdir from songlist where songartist = '%1'").arg(name));
    if(!query.exec())
    {
        qDebug() << "album not found";
        return "";
    }
    if(query.first())
    {
        //qDebug() << query.value(0).toString();
        return query.value(0).toString();
    }
    return "";
}

QString DataBase::queryImage(QString name)
{
    QSqlDatabase db = QSqlDatabase::database("QSQLITE"); //建立数据库连接
    QSqlQuery query(db);
    query.prepare(QString("select imagedir from songlist where songartist = '%1'").arg(name));
    if(!query.exec())
    {
        qDebug() << "song album image not found";
        return "";
    }
    if(query.first())
    {
        //qDebug() << query.value(0).toString();
        return query.value(0).toString();
    }
    return "";
}

QString DataBase::queryLyr(QString name)
{
    QSqlDatabase db = QSqlDatabase::database("QSQLITE"); //建立数据库连接
    QSqlQuery query(db);
    query.prepare(QString("select lyrdir from songlist where songartist = '%1'").arg(name));
    if(!query.exec())
    {
        qDebug() << "lyrics not found";
        return "";
    }
    if(query.first())
    {
        //qDebug() << query.value(0).toString();
        return query.value(0).toString();
    }
    return "";
}

//根据ID删除记录
/*
bool DataBase::deleteById(int id)
{
    QSqlDatabase db = QSqlDatabase::database("QSQLITE"); //建立数据库连接
    QSqlQuery query(db);
    query.prepare(QString("delete from songlist where id = %1").arg(id));
    if(!query.exec())
    {
        qDebug() << "delete failed.";
        return false;
    }
    return true;
}

//根据ID更新记录
bool DataBase::updateById(int id)
{
    QSqlDatabase db = QSqlDatabase::database("QSQLITE"); //建立数据库连接
    QSqlQuery query(db);
    query.prepare(QString("update songlist set attribute=?,type=?,"
                             "kind=?, nation=?,"
                             "carnumber=?, elevaltor=?,"
                             "distance=?, oil=?,"
                             "temperature=? where id=%1").arg(id));

     query.bindValue(0,"四轮");
     query.bindValue(1,"轿车");
     query.bindValue(2,"富康");
     query.bindValue(3,rand()%100);
     query.bindValue(4,rand()%10000);
     query.bindValue(5,rand()%300);
     query.bindValue(6,rand()%200000);
     query.bindValue(7,rand()%52);
     query.bindValue(8,rand()%100);

     bool success=query.exec();
     if(!success)
     {
          QSqlError lastError = query.lastError();
          qDebug() << lastError.driverText() << QString(QObject::tr("update database failed."));
     }
    return true;
}

//排序
bool DataBase::sortById()
{
    QSqlDatabase db = QSqlDatabase::database("QSQLITE"); //建立数据库连接
    QSqlQuery query(db);
    bool success=query.exec("select * from songlist order by id desc");
    if(success)
    {
        qDebug() << QObject::tr("database sorted.");
        return true;
    }
    else
    {
        qDebug() << QObject::tr("database sort failed.");
        return false;
    }
}
*/
