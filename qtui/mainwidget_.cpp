#include "mainwidget_.h"
#include "ui_mainwidget_.h"
#include <QMessageBox>
#include "../Converter/Converter.h"

mainwidget_::mainwidget_(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::mainwidget_)
{
    ui->setupUi(this);
    init();
}

mainwidget_::~mainwidget_()
{
    delete cvt;
    delete ui;
}

//get music files and convert
void mainwidget_::on_pushButton_clicked()
{
    musics = QFileDialog::getOpenFileNames(this,tr("Open file"),"/home","ncm (*.ncm)");

    if(musics.empty())
        return;

    listMusic = musics.toList();

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
    this->setWindowTitle("网易云音乐转换");
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

    int totalCount = listMusic.size();
    int successCount = 0;
    int failedCount = 0;
    info = QString("Total %1 files to be conver.\n").arg(totalCount);
    ui->textEdit->append(info);

    for(QString music : listMusic)
    {
        info = QString("Converting %1.\n").arg(music);
        ui->textEdit->append(info);

        if(cvt->ncm2mp3(music.toStdString(),outputdir.absolutePath().toStdString()))
        {
            successCount++;
            info = QString("%1 converted successful.").arg(music);
            ui->textEdit->append(info);
        }
        else
        {
            failedCount++;
            info = QString("%1 converted failed.").arg(music);
            ui->textEdit->append(info);
        }
    }

    info = QString("Total %1 files , %2 successful, %3 failed.").arg(totalCount).arg(successCount).arg(failedCount);
    ui->textEdit->append(info);
}


void mainwidget_::on_lineEdit_textChanged(const QString &arg1)
{
    ;
}


void mainwidget_::on_lineEdit_2_textChanged(const QString &arg1)
{
    outputdir = arg1;
 }

