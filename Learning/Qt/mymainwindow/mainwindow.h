#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>

class QLineEdit;
class QDialog;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void newFile();

    bool maybeSave();

    bool save();

    bool saveAs();

    bool saveFile(const QString &fileName);

    bool loadFile(const QString &fileName);


private slots:
    void on_actionNew_N_triggered();

    void on_actionSave_S_triggered();

    void on_actionSave_as_A_triggered();

    void on_actionOpen_O_triggered();

    void on_actionClose_triggered();

    void on_actionExit_X_triggered();

    void on_actionUndo_triggered();

    void on_actionCut_X_triggered();

    void on_actionCopy_C_triggered();

    void on_actionPaste_V_triggered();

    void showFindText();


    void on_actionFind_F_triggered();

private:
    Ui::MainWindow *ui;
    bool isUntitled; //真表示已保存过了
    QString curFile; //当前文件路径
    QLineEdit *findLineEdit;
    QDialog *findDlg;

protected:
    void closeEvent(QCloseEvent *event);
};

#endif // MAINWINDOW_H
