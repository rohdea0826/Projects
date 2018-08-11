#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QPushButton>
#include <QFileDialog>
#include <QTextStream>
#include <QDialog>
#include <QLineEdit>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    isUntitled = true;
    curFile = tr("new.txt");
    setWindowTitle(curFile);

    findDlg = new QDialog(this);
    findDlg->setWindowTitle(tr("find"));
    findLineEdit = new QLineEdit(findDlg);
    QPushButton *btn = new QPushButton(tr("find next"),findDlg);
    QVBoxLayout *layout = new QVBoxLayout(findDlg);
    layout->addWidget(findLineEdit);
    layout->addWidget(btn);
    connect(btn,&QPushButton::clicked, this, &MainWindow::showFindText);

    ui->statusBar->showMessage(tr("Welcome to Rohdea's Editor!"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::newFile()
{
    if(maybeSave())
    {
        isUntitled = true;
        curFile = tr("new.txt");
        setWindowTitle(curFile);
        ui->textEdit->clear();
        ui->textEdit->setVisible(true);
    }
}

bool MainWindow::maybeSave()
{
    if(ui->textEdit->document()->isModified())
    {
        QMessageBox box;
        box.setWindowTitle(tr("warning"));
        box.setText(curFile + tr("Save?"));
        QPushButton *yesBtn = box.addButton(tr("Yes"),QMessageBox::YesRole);
        box.addButton(tr("No"),QMessageBox::NoRole);
        QPushButton *cancelBtn = box.addButton(tr("cancel"),QMessageBox::RejectRole);

        box.exec();
        if(box.clickedButton() == yesBtn)
            return save();
        else if(box.clickedButton() == cancelBtn)
            return false;
    }
    return true;
}

bool MainWindow::save()
{
    if(isUntitled)
    {
        return saveAs();
    }
    else
    {
        return saveFile(curFile);
    }
}

bool MainWindow::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this,tr("save as"),curFile);
    if(fileName.isEmpty()) return false;
    return saveFile(fileName);
}

bool MainWindow::saveFile(const QString &fileName)
{
    QFile file(fileName);
    if(!file.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(this,tr("muti edit"),tr("write error %1, /n %2").arg(fileName).arg(file.errorString()));
        return false;
    }
    QTextStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    out << ui->textEdit->toPlainText();

    QApplication::restoreOverrideCursor();
    isUntitled = false;

    curFile = QFileInfo(fileName).canonicalFilePath();
    setWindowTitle(curFile);
    return true;
}

bool MainWindow::loadFile(const QString &fileName)
{
    QFile file(fileName);
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this,tr("multiEditor"),tr("read error").arg(fileName).arg(file.errorString()));
        return false;
    }
    QTextStream in(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    ui->textEdit->setPlainText(in.readAll());
    QApplication::restoreOverrideCursor();
    curFile = QFileInfo(fileName).canonicalFilePath();
    setWindowTitle(curFile);
    return true;
}

void MainWindow::on_actionNew_N_triggered()
{
    newFile();
}

void MainWindow::on_actionSave_S_triggered()
{
    save();
}

void MainWindow::on_actionSave_as_A_triggered()
{
    saveAs();
}

void MainWindow::on_actionOpen_O_triggered()
{
    if(maybeSave())
    {
        QString fileName = QFileDialog::getOpenFileName(this);
        if(!fileName.isEmpty())
        {
            loadFile(fileName);
            ui->textEdit->setVisible(true);
        }
    }
}


void MainWindow::on_actionClose_triggered()
{
    if(maybeSave())
    {
        ui->textEdit->setVisible(false);
    }
}

void MainWindow::on_actionExit_X_triggered()
{
    on_actionClose_triggered();
    qApp->quit();
}

void MainWindow::on_actionUndo_triggered()
{
    ui->textEdit->undo();
}

void MainWindow::on_actionCut_X_triggered()
{
    ui->textEdit->cut();
}


void MainWindow::on_actionCopy_C_triggered()
{
    ui->textEdit->copy();
}

void MainWindow::on_actionPaste_V_triggered()
{
    ui->textEdit->paste();
}

void MainWindow::showFindText()
{
    QString str = findLineEdit->text();
    if(!ui->textEdit->find(str))
    {
        QMessageBox::warning(this,tr("Find"),tr("Can not find %1").arg(str));
    }
    ui->textEdit->setFocus();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(maybeSave())
    {
        event->accept();
    }
    else
    {
        event->ignore();
    }
}



void MainWindow::on_actionFind_F_triggered()
{
    findDlg->show();
}
