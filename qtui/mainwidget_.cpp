#include "mainwidget_.h"
#include "ui_mainwidget_.h"
#include <QMessageBox>
#include "../Converter/Converter.h"

mainwidget_::mainwidget_(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::mainwidget_)
{
    ui->setupUi(this);
}

mainwidget_::~mainwidget_()
{
    cvt->deleteLater();
    delete ui;
}

//get music files and convert
void mainwidget_::on_pushButton_clicked()
{
    musics = QFileDialog::getOpenFileNames(this,tr("Open file"),"/home","ncm (*.ncm)");

    if(musics.empty())
        return;

    listMusic = ncmList.toList();

    QString music1 = listMusic.at(0);
    QFileInfo fileinfo(music1);
    QString dir = fileinfo.filePath();

    ui->lineEdit->setText(dir);

}


//get output dir
void mainwidget_::on_pushButton_2_clicked()
{
    QString path = QFileDialog::getExistingDirectory(this,"Select a Dir","/home/");

    if(path.isEmpty())
        return;

    ui->lineEdit_2->setText(path);
    outputdir.setPath(path);
}

void mainwidget_::init()
{
    outputdir.setCurrent("~/Music/ncm");
    cvt = new Converter();


    connect(cvt,&Converter::processSig,this,&mainwidget_::refreshTextEdit);
}

void mainwidget_::refreshTextEdit(QString message)
{
    ui->textEdit->append(message);
}


void mainwidget_::on_pushButton_3_clicked()
{
    if(ui->lineEdit->text().isEmpty() || ui->lineEdit_2->text().isEmpty())
    {
        QMessageBox::information(this,"Tips","Plese select Input and Output info");
        return;
    }

    int count = listMusic.size();
    info = QString("Total %1 files to be conver.\n").arg(count);
    ui->textEdit->append(info);

    for(QString music : listMusic)
    {
        info = QString("Converting %1.\n").arg(music);
        ui->textEdit->append(info);
        cvt->ncm2mp3(music.toStdString(),outputdir.absolutePath().toStdString());
        info = QString("%1 converted.").arg(music);
        ui->textEdit->append(info);
    }

    info = QString("Total %1 files converted completed!").arg(count);
}


void mainwidget_::on_lineEdit_textChanged(const QString &arg1)
{
    ;
}


void mainwidget_::on_lineEdit_2_textChanged(const QString &arg1)
{
    outputdir = arg1;
}

