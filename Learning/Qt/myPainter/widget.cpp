#include "widget.h"
#include "ui_widget.h"

#include <QPaintEvent>
#include <QPainter>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

}

Widget::~Widget()
{
    delete ui;
}

void Widget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    //painter.drawLine(QPointF(0,0),QPointF(100,100));
    QPen pen;
    pen.setColor(QColor(255,0,0));
    QBrush brush(QColor(0,255,0,125));
    painter.setPen(pen);
    painter.setBrush(brush);
    painter.drawRect(0,0,400,300);
}
