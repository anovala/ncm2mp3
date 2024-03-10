#ifndef MAINWIDGET__H
#define MAINWIDGET__H

#include <QWidget>
#include <QFileDialog>
#include <QStringList>
#include <QThread>
#include "../Converter/Converter.h"
#include "../worker/worker.h"

namespace Ui {
class mainwidget_;
}

class mainwidget_ : public QWidget
{
    Q_OBJECT

public:
    explicit mainwidget_(QWidget *parent = nullptr);
    ~mainwidget_();

signals:
    void sigConvert(QList<QString> list,QDir outputdir);

private:
    void init();
    void resetCount();

    Converter *cvt;
    QDir outputdir;
    QString info;
    QStringList musics;
    QList<QString> listMusic;

    QThread *workerThread;

    int totalCount;
    int successCount;
    int failedCount;

private slots:
    void refreshTextEdit(QString);

    void on_pushButton_3_clicked();

    void on_lineEdit_textChanged(const QString &arg1);

    void on_lineEdit_2_textChanged(const QString &arg1);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void recvProcessingConvert(bool b, QString fileName);

    void recvConvertFin();

private:
    Ui::mainwidget_ *ui;
};

#endif // MAINWIDGET__H
