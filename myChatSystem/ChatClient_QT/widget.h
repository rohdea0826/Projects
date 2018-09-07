#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QMessageBox>
#include <QPushButton>
#include <QTextStream>
#include <QFileDialog>
#include <QTextEdit>
#include <QLineEdit>
#include <QTextBrowser>
#include <QUdpSocket>
#include <QHostAddress>
#include <QByteArray>
#include <QStringListModel>
#include <QString>
#include <iostream>
#include <string>
#include <stdlib.h>

//#include <QTimer>

#include "comm/data.h"


namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    void sendData(const std::string command);
    void delUser(std::vector<my_data> &flist, my_data d);
    void addUser(std::vector<my_data> &flist, my_data d);
    void flushlist();
private slots:
    void on_sendButton_clicked();

    void on_lineEdit_editingFinished();

    //void textChanged(my_data d);

    void receive();

    void on_ExitBtn_clicked();

private:
    Ui::Widget *ui;
    QUdpSocket *udpsocket;
    QPushButton *sendbtn;
    QLineEdit *edit;
    QTextBrowser *textb;

     std::vector<my_data> flist;
};

#endif // WIDGET_H
