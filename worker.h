#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QFile>

class Worker : public QObject
{
    Q_OBJECT
public:
    explicit Worker(QString, QString, QString, QObject *parent = 0);

signals:
    void Finish();
    void Find(const QString);
    void Error(const QString);

public slots:
    void run();

private:
    QString path, name, obj;
    void watchFolder(const QString&);
    bool isGood(QFile&);
};

#endif // WORKER_H
