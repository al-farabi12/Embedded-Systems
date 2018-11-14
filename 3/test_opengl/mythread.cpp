#include "mythread.h"
#include <QDebug>
#include <qmath.h>
#include <QQuaternion>
#include <QTcpSocket>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <QHostAddress>
#include <QXmlStreamReader>
#include <QVector>
using namespace std;



MyThread::MyThread(QObject *parent)
    :QThread(parent)
{

}

MyThread::~MyThread()
{

}


void MyThread::run()
{

     // ADD CODE HERE: add here connection setup for TCP/IP
    QScopedPointer<QTcpSocket> socket(new QTcpSocket());
    socket->connectToHost("192.168.7.2", 5555);
    qDebug()<<"ccc";

    if(socket->waitForConnected(5000)){
        qDebug()<<"Connected";
    }
    qDebug()<<"sad";
    while(run_flag)
    {
        qDebug()<<"run_flag";

        // ADD CODE HERE: this is infinite loop, you need parse here the TCP/IP read buffer
        if(!socket->waitForReadyRead(5000)){
           this->run_flag = false;
        }

        while(socket->bytesAvailable()>0)    // are bytes available?
               {
                   int x = socket->bytesAvailable();  // how many?
                   qDebug() << "There are " << x << " bytes available"; //debug
                   char data[10000];                // capacity for up to 2000 bytes
                   x = (qint64) socket->read(&data[0], (qint64) x);
                   data[x] = '\0';                 // add null in case of print output
                   this->parse(&data[0]);          // parse the XML string
                   // SAMPLE CODE FOR RANDOM QUATERNIONS: please, delete these codes, it is only for demonstration
                   if(!v.isEmpty()){
                    q0 = v[0];
                    q1 = v[1];
                    q2 = v[2];
                    q3 = v[3];
                   }
                   v.clear();
                   qDebug()<<q0;
                   QQuaternion temp_q = QQuaternion(q0,q1,q2,q3);
                   temp_q.normalize();

                   emit setQuats(temp_q.scalar(), temp_q.x(),temp_q.y(), temp_q.z());
                   msleep(20);
        }



    }
    socket->close();
    qDebug()<< "Disconnect";
    // ADD CODE HERE: safely disconnect from TCP/IP
}

void MyThread::parse(char *data){
    float qq0 = 0.0f, qq1 = 0.0f, qq2 = 0.0f, qq3 = 0.0f;
   // QVector<float> vec;
    QXmlStreamReader xml(data);
      while(!xml.atEnd() && !xml.hasError()){
        if((xml.tokenType()==QXmlStreamReader::StartElement) &&    //found <sample>
           (xml.name()=="sample")){
          //This is a data sample <sample> - need to loop until </sample>

          xml.readNext();                                    // read next token
          while(!((xml.tokenType()==QXmlStreamReader::EndElement)&&    // </sample>
                  (xml.name()=="sample")))
          {
            qDebug() << "Found a sample";
            if((xml.tokenType()==QXmlStreamReader::StartElement)       // <quaternions>
                      &&(xml.name()=="quaternions")){
              xml.readNext();
              qDebug() << "-- it has an quaternion elements";
              while(!((xml.tokenType()==QXmlStreamReader::EndElement)  // </quaternions>
                        &&(xml.name()=="quaternions")))
              {
                if(xml.tokenType() == QXmlStreamReader::StartElement){ // <pitch>

                  if(xml.name() == "q0") {

                    QString temp = xml.readElementText();    // read the value
                    qq0 = (temp).toFloat();                // convert to float
                    v.append(qq0);
                    //v.push_back(qq0);

                  }
                  if(xml.name() == "q1") {                           // <roll>
                    QString temp = xml.readElementText();
                    qq1 = (temp).toFloat();
                    v.append(qq1);
                    //v.push_back(qq1);

                  }
                  if(xml.name() == "q2") {                           // <roll>
                    QString temp = xml.readElementText();
                    qq2 = (temp).toFloat();
                    v.append(qq2);
                    //v.push_back(qq2);
                                      }
                  if(xml.name() == "q3") {                           // <roll>
                    QString temp = xml.readElementText();
                    qq3 = (temp).toFloat();
                    v.append(qq3);
                    //v.push_back(qq3);
                    qDebug()<<"asdasdsads"<<v[0];
                  }
                }


                xml.readNext();
              }
            }
            xml.readNext();
          }
         // SensorSample sample(q0, q1, q2, q3);      // create a sample object and
         // emit sampleReceived(sample);           // emit it as a signal -- caught

        }                                      // by a slot in mainWindow, which
        xml.readNext();                          // updates the display widgets
        }

    }



