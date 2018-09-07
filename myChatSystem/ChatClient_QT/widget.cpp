#include "widget.h"
#include "ui_widget.h"
#include "comm/data.h"

const quint16 PORT = 8910;

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    sendbtn = ui->sendButton;
    textb = ui->textBrowser;
    edit = ui->lineEdit;

    udpsocket = new QUdpSocket;
    udpsocket->bind(PORT, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
    //QTimer *timer = new QTimer(this);
    //connect(timer,SIGNAL(timeout()),this, SLOT(receive()));
    //timer->start(1000);

    //UdpReceiver udprec;
    //udprec.start();

    //UdpReceiver udprec(this);
    QObject::connect(udpsocket, SIGNAL(readyRead()), this, SLOT(receive()));
    //QObject::connect(&udprec,SIGNAL(textChanged(my_data)),this,SLOT(textChanged(my_data)));
}

Widget::~Widget()
{
    delete ui;
    sendData("quit");
    delete udpsocket;
}

void Widget::sendData(const std::string command)
{
    //从文本框获取文本；
    QString str = ui->lineEdit->text();

    //数据需要序列化打包
    my_data d;
    d.nick_name = ui->name->text().toStdString(); //"rohdea";
    d.school = ui->school->text().toStdString();  //"xatu123";
    d.command = command;    //"None";
    d.message = str.toStdString();
    std::string sendString;
    d.serialize(sendString);

    //序列化之后将string转为QByteArray以便发送
    QString qstr = QString(QString::fromLocal8Bit(sendString.c_str()));
    QByteArray msg = qstr.toLocal8Bit();
    //发送
    udpsocket->writeDatagram(msg, QHostAddress("39.106.141.165"), PORT);
    if(command == "quit")
    {
        delUser(flist,d);
    }
    //清空输入框
    edit->clear();
}

//发送按钮
void Widget::on_sendButton_clicked()
{
    sendData("None");
}

//输入框回车发送
void Widget::on_lineEdit_editingFinished()
{
    on_sendButton_clicked();
}

//刷新用户列表
void Widget::flushlist()
{
    QStringList userList;
    for(size_t i = 0; i < flist.size(); i++)
    {
        QString usr = QString(QString::fromStdString(flist[i].nick_name));
        usr += "-";
        usr += QString(QString::fromStdString(flist[i].school));
        userList<<usr;
        QStringListModel *model = new QStringListModel(userList);
        ui->listView->setModel(model);
    }
}

//维护一个vector<my_data> 来管理连接的用户
void Widget::addUser(std::vector<my_data> &flist, my_data d)
{
    for(size_t i = 0; i < flist.size(); i++)
    {
        if(flist[i].nick_name == d.nick_name&& \
                flist[i].school == d.school)
            return;
    }
    //std::cout<<flist.size()<<std::endl;
    flist.push_back(d);
    flushlist();
}

void Widget::delUser(std::vector<my_data> &flist, my_data d)
{
    std::vector<my_data>::iterator it = flist.begin();
    for(;it != flist.end(); it++)
    {
        if(it->nick_name == d.nick_name && \
                it->school == d.school)
        {
            it = flist.erase(it);
            flushlist();
            return ;
        }
    }
    flushlist();
}

//接收到udp数据时的槽函数
void Widget::receive()
{
    while(udpsocket->hasPendingDatagrams())
    {
        QString out;
        char buff[1024] = {0};
        udpsocket->readDatagram(buff,udpsocket->pendingDatagramSize());
        out = QString(QString::fromLocal8Bit(buff));
        std::string str = out.toStdString();
        //std::cout<<str<<std::endl;

        my_data d;
        d.unserialize(str);

        std::string quit = "quit";
        if(d.command == quit)
        {
            std::cout<<"-"<<flist.size()<<std::endl;
            delUser(flist,d);
        }
        else
        {
            addUser(flist,d);
            //将接收到的消息追加显示到text browser上
            QString msg = QString(QString::fromStdString(d.nick_name));
            msg += "-";
            msg += QString(QString::fromStdString(d.school));
            msg += "#";
            msg += QString(QString::fromStdString(d.message));
            std::cout<<msg.toStdString()<<std::endl;
            textb->append(msg.toUtf8());
        }
    }
}

//退出按钮
void Widget::on_ExitBtn_clicked()
{
    sendData("quit");
    flushlist();
    exit(0);
}
