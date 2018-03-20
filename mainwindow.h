#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include "worker.h"
#include "simpletime.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_lineEdit_2_textEdited(const QString &arg1);

    void on_addButton(const QString &path);

    void on_Error(const QString &Error);

    void freeThread();

    void on_timerChange(int);

private:
    Ui::MainWindow *ui;
    QWidget* scrollAreaWidget;
    QList<QPushButton*>childButtons;
    void clearFields();
    QThread* thread;
    Worker* worker = nullptr;

    QThread* timer_thread;
    SimpleTime* timer = new SimpleTime();

};

#endif // MAINWINDOW_H
