#include "widget.h"
#include "ui_widget.h"

#include <QTimerEvent>
#include <QMouseEvent>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    id1 = startTimer(1000);
    id2 = startTimer(2000);
    id3 = startTimer(10000);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::timerEvent(QTimerEvent *e)
{
    if(e->timerId() == id1)
    {
        ui->label->setText(tr("%1").arg(qrand()%10));

    }
    else if(e->timerId() == id2)
    {
        ui->label_2->setText(tr("hello world!"));
    }
    else
    {
       // qApp->quit();
    }
}

void Widget::mousePressEvent(QMouseEvent *)
{
    ui->label->setText(tr("鼠标按下"));

}

void Widget::mouseReleaseEvent(QMouseEvent *)
{
    ui->label_2->setText(tr("鼠标释放"));
}
