#ifndef SIMPLETIME_H
#define SIMPLETIME_H

#include <QObject>

class SimpleTime : public QObject
{
    Q_OBJECT
public:
    explicit SimpleTime(QObject *parent = 0);
    bool is_terminated;

signals:
    void cicle(int);

public slots:
    void run();
};

#endif // SIMPLETIME_H
