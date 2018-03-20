#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "worker.h"
#include <fstream>
#include <string>
#include <ctime>
#include <QString>
#include <QFileDialog>
#include <QThread>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->scrollAreaWidget = new QWidget(this);
    this->scrollAreaWidget->setLayout(new QVBoxLayout());
    ui->scrollArea->setWidget(this->scrollAreaWidget);
    this->scrollAreaWidget->layout()->setAlignment(Qt::AlignTop);

    std::fstream* fin = new std::fstream();
    fin->exceptions ( std::ifstream::failbit | std::ifstream::badbit );
    try{
        fin->open("Conf.ini", std::fstream::in);
        std::string path;
        std::getline(*fin, path);
        fin->close();
        ui->label_3->setText(QString::fromStdString(path.substr(path.find('"')+1,path.size()-path.find('"')-2)));
    }catch(std::ifstream::failure e){
        ui->label_3->setText("Unable to find Conf.ini");
    }
    delete fin;
}

MainWindow::~MainWindow()
{
    clearFields();
    delete timer;
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString dir  = QFileDialog::getExistingDirectory(this,tr("Open Directory"),"/home",QFileDialog::ShowDirsOnly);
    if(dir!=""){
        std::fstream* fout = new std::fstream();
        fout->exceptions ( std::ifstream::failbit | std::ifstream::badbit );
        try{
            fout->open("Conf.ini", std::fstream::out);
            (*fout)<<"Folder path = \""<<dir.toUtf8().constData()<<"\"";
            fout->close();
            ui->label_3->setText(dir);
        }catch(std::ifstream::failure e){
            ui->label_3->setText("Unable to find Conf.ini");
        }
        delete fout;
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    if(ui->lineEdit_2->text()!=""){
        clearFields();
        thread = new QThread();
        worker  = new Worker(ui->label_3->text(), ui->lineEdit->text(), ui->lineEdit_2->text());
        QObject::connect(thread, SIGNAL(started()), worker, SLOT(run()));
        QObject::connect(worker, SIGNAL(Finish()), this, SLOT(freeThread()));
        QObject::connect(worker, SIGNAL(Find(QString)), this, SLOT(on_addButton(const QString&)));
        QObject::connect(worker, SIGNAL(Error(QString)), this, SLOT(on_Error(const QString&)));

        timer = new SimpleTime();
        QObject::connect(timer,SIGNAL(cicle(int)), this, SLOT(on_timerChange(int)));
        timer_thread = new QThread();
        QObject::connect(timer_thread, SIGNAL(started()), timer, SLOT(run()));

        timer->moveToThread(timer_thread);

        worker->moveToThread(thread);
        thread->start();
        timer_thread->start();
        ui->pushButton_2->setEnabled(false);
    }else{
        ui->lineEdit_2->setStyleSheet("#lineEdit_2{background-color:rgba(255,0,0,0.25);}");
    }
}

void MainWindow::on_lineEdit_2_textEdited(const QString &arg1)
{
    ui->lineEdit_2->setStyleSheet("#lineEdit_2{background-color:white;}");
}


void MainWindow::freeThread(){
    timer->is_terminated = true;
    thread->terminate();
    timer_thread->terminate();
    thread->wait();
    timer_thread->wait();
    delete worker;
    worker = nullptr;
    delete thread;
    thread = nullptr;
    delete timer_thread;
    timer_thread = nullptr;
    delete timer;
    timer = nullptr;
    ui->label_4->setText("Сканирование завершено");
    ui->pushButton_2->setEnabled(true);
}

void MainWindow::clearFields(){

    while(!childButtons.empty()){
        QPushButton* temp = childButtons.first();
        childButtons.removeFirst();
        delete temp;
    }

}

void MainWindow::on_addButton(const QString &path){
    QPushButton* temp = new QPushButton(path);
    connect(temp,&QPushButton::clicked,[path](){
        QString command= "explorer.exe ";
        command+=path.toUtf8().constData();
        command.replace("/","\\");
        system(command.toUtf8().constData());
    });
    childButtons.append(temp);
    ui->scrollArea->widget()->layout()->addWidget(temp);

}

void MainWindow::on_Error(const QString &Error){
    std::fstream fout;
    fout.open("Errors.log",std::ios_base::app);
    if(fout.is_open()){
        std::time_t cur_time= time(0);
        fout<<ctime(&cur_time)<<" "<<Error.toUtf8().constData();
    }
}

void MainWindow::on_timerChange(int n){
    QString str = "Сканирование.";
    for(int i = 0;i<n;i++)
        str.append(".");
    ui->label_4->setText(str);
}
