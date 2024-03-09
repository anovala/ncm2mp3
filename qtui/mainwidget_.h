#ifndef MAINWIDGET__H
#define MAINWIDGET__H

#include <QWidget>
#include <QFileDialog>
#include <QStringList>
#include "../Converter/Converter.h"

namespace Ui {
class mainwidget_;
}

class mainwidget_ : public QWidget
{
    Q_OBJECT

public:
    explicit mainwidget_(QWidget *parent = nullptr);
    ~mainwidget_();




private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();


private:
    void init();

    Converter *cvt;
    QDir outputdir;
    QString info;
    QStringList musics;
    QList<QString> listMusic;


private slots:
    void refreshTextEdit(QString);

    void on_pushButton_3_clicked();

    void on_lineEdit_textChanged(const QString &arg1);

    void on_lineEdit_2_textChanged(const QString &arg1);

private:
    Ui::mainwidget_ *ui;
};

#endif // MAINWIDGET__H
