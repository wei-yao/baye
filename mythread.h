#ifndef MYTHREAD_H
#define MYTHREAD_H
#include <QThread>
#include "gameview.h"

class MyThread :public QThread
{
public:
    explicit MyThread(QObject *parent = 0);
    void stop();
protected:
    void  run();
private:
    volatile bool stopped;
};
#endif // MYTHREAD_H
