#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QString>
#include <QDir>
#include "../Converter/Converter.h"

class Worker: public QObject
{
    Q_OBJECT

public:
    Worker() {
        cvt = new Converter();
    }

    ~Worker() {
        delete cvt;
        cvt = nullptr;
    }

public slots:
    void doConvert(const QList<QString> inputFilePath, const QDir outputDir)
    {
        bool bSuccess;
        QString outputFilePath = outputDir.absolutePath();
        for(QString inputFile:inputFilePath)
        {
            bSuccess = cvt->ncm2mp3(inputFile.toStdString(),outputFilePath.toStdString());
            emit processConvert(bSuccess,inputFile);
        }
        emit resultReady();
    }

signals:
    void processConvert(bool bSuccess, QString filename);
    void resultReady();
private:

    Converter *cvt;
};

#endif // WORKER_H
