/*#include <iostream>
#include <regex>
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>
#include "Converter/Converter.h" */


#include "qtui/mainwidget_.h"
#include <QApplication>

/*
void processNpmFiles(const std::string &inputPath, const std::string &outputPath){

    boost::regex npmFilePattern(".*\\.ncm$");
    std::cout<<"cur inputPath is "<<inputPath <<std::endl;
    Converter *cvt = new Converter();   
    
    int n = 0;

    for(const boost::filesystem::directory_entry &entry : \
    boost::filesystem::directory_iterator(inputPath))
    {
        if (boost::filesystem::is_regular_file(entry.status())){
            std::string filePath = entry.path().string();
            std::cout << "File: "<<entry.path().string();

        if(boost::regex_match(filePath,npmFilePattern)) {
            cvt->ncm2mp3(entry.path().string(),outputPath);
            n++;
        } 
        }
      
    }
    printf("本次成功转换%d个文件，网易云哈哈\n",n);
    printf("转换后路径:%s\n",outputPath.c_str());

    delete cvt;
}


int main(void)
{
    printf("本程序用于解密网易云音乐的npm格式文件\n");
    printf("请输入源文件路径以及输出路径，间隔空格:\n");

    std::string in, out;

    in = "/home/anovala/mscTsc";
    out = "/home/anovala/mscTsc/decryptedMsc";
    //std::cin >>in>>out;

    processNpmFiles(in,out);

    printf("转换完成\n");

    return 0;
}
*/

int main(int argc, char *argv[])
{
    QApplication a(argc,argv);
    mainwidget_ w;
    w.show();
    return a.exec();
}
