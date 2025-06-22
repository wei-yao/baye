#include "mythread.h"
#include <QDebug>

extern int g_startFlag;

void GamBaYeEng(void);

MyThread::MyThread(QObject * parent) :
    QThread(parent)
{
}
void MyThread::stop()
{
    exit();
}
void MyThread::run()
{

    while(g_startFlag!=5)
    {

        g_startFlag = 0;
        GamBaYeEng();
    }
    //exit();

  /*static int i =0;
   while(!stopped)
   {
   i++;
   qDebug()<<"i"<<i;
   sleep(1);
   }
   stopped=false;*/
}
