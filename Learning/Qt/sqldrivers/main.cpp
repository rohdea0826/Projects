//无法实现数据库的插入操作，原因不明

#include <QCoreApplication>
#include <QSqlDatabase>
#include <QDebug>
//#include <QStringList>
#include <QSqlQuery>
#include <QString>


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

//    qDebug() << "Available drivers:";
//    QStringList drivers = QSqlDatabase::drivers();
//    foreach (QString driver, drivers) {
//        qDebug() <<driver;
      //drivers查看支持的数据库驱动
//    }

    qDebug()<< "hello SQLite";
    //创建SQLite数据库链接
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    //数据库连接命名
    db.setDatabaseName(":memory:");
    //打开数据库
    if(!db.open()) qDebug()<<"error";

    //执行sql语句
    QSqlQuery query;
    //新建student表，id设置为主键，还有一个name项
    query.exec("create table student(id int primary key,name varchar)");
    //向表中插入3条记录
    query.exec("insert into student values(1,'xiaogang')");
    query.exec("insert into student values(2,'xiaoming')");
    query.exec("insert into student values(3,'xiaohong')");
    //查找表中id >=2 的记录
    query.exec("select id from student where id == 2");
    //query.next指向查找到的第一条记录，然后每次后移一条记录
    qDebug() << ;
    while(query.next())
    {
        //query.value(0)是id的值，将其转换为int型
        int value0 = query.value(0).toInt();
        QString value1 = query.value(1).toString();
        qDebug() << value0 << value1 ;
        qDebug() << "hello test" ;
    }

    return a.exec();
}
