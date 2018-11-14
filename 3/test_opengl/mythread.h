#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QObject>
#include <QThread>
#include <QtCore/qmath.h>
#include <QQuaternion>
#include <QVector3D>
#include <QTcpSocket>
#include <QMessageBox>
#include <QDataStream>
#include <QXmlStreamReader>
#include <iostream>
#include <vector>
#include "sensorsample.h"
#include <QVector>
using namespace std;

class MyThread : public QThread
{
    Q_OBJECT
public:
    MyThread(QObject *parent=0);
    ~MyThread();
    bool run_flag;
    QTcpSocket *socket;
    QVector<float> v;
signals:
    void setQuats(const float, const float, const float, const float);

private:
    void parse(char *);
    void run();
    float q0, q1, q2, q3;
};

#endif // MYTHREAD_H
