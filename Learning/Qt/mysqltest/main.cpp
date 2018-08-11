#include <QCoreApplication>
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlQuery>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    //打开MySQL
    QSqlDatabase db = QSqlDatabase::addDatabase(("QMYSQL"));
    db.setHostName("localhost");
    db.setDatabaseName("mydata");
    db.setUserName("root");
    db.setPassword("");
    if(!db.open())
        qDebug() << "Failed to conect to root mysql admin";
    else
        qDebug() << "opened";

    QSqlQuery query(db);

    query.exec("create table student(id int primary key,name varchar(20))");

    query.exec("insert into student values(1,'xiaogang')");
    query.exec("insert into student values(2,'xiaoming')");
    query.exec("insert into student values(3,'xiaohong')");

    query.exec("select id,name from student where id >= 2");
    while (query.next()) {
        int value0 = query.value(0).toInt();
        QString value1 = query.value(1).toString();
        qDebug() << value0 <<value1;
    }

    return a.exec();
}
