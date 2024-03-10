#include "../Utils/_aes.h"
#include "Converter.h"
#include "../Utils/Utils.h"
#include <string.h>
#include "../Utils/Base64.h"
#include "../Utils/CR4.h"
#include <taglib/fileref.h>
#include <taglib/tag.h>
#include <taglib/mpegfile.h>
#include <taglib/id3v2tag.h>
#include <taglib/attachedpictureframe.h>
#include <taglib/tbytevector.h>
//#include <nlohmann/json.hpp>
#include <boost/filesystem.hpp>
#include <unicode/unistr.h>
#include <unicode/ustream.h>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTextCodec>
#include <QDebug>

// Function to perform AES decryption
// void decryptAES(const unsigned char *cipherText, size_t cipherTextLength, const unsigned char *key,  unsigned char *iv, unsigned char *plainText) {
//     AES_KEY decryptKey;
//     AES_set_decrypt_key(key, 128, &decryptKey);
    
//     AES_cbc_encrypt(cipherText, plainText, cipherTextLength, &decryptKey, iv, AES_DECRYPT);
// }


bool Converter::ncm2mp3(std::string ncmFilePath, std::string outFilePath)
{
    printf("input file path: %s\n",ncmFilePath.c_str());
    Ncm ncm(0,0);
    ncm.ncmFile = ncmFilePath;

    std::ifstream inputStream(ncm.ncmFile);
    if(inputStream.is_open()){

        magicHeader(inputStream);
        std::string key = cr4Key(inputStream);

        std::string jsonMusicInfo = mataData(inputStream);

        if(jsonMusicInfo.empty())
        {
            return false;

        }


        QByteArray byteArray(jsonMusicInfo.c_str(),jsonMusicInfo.length());
        QJsonParseError error;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(byteArray,&error);



        if(error.error != QJsonParseError::NoError)
        {
            std::cout<<__func__<<__LINE__ << "json: "<<jsonMusicInfo<<std::endl;
            std::cout<<"error on json parsing"<<std::endl;
            return false;
        }

        QJsonObject jsonData = jsonDoc.object();
        
        Mata mata;
        mata.album = jsonData["album"].toString().toStdString();

        std::string _name = jsonData["artist"][0][0].toString().toStdString();
        unsigned int _num = jsonData["artist"][0][1].toVariant().toUInt();

        artistInfo info;
        info.name = _name;
        info.num = _num;
        mata.artist.push_back(info);

        mata.format = jsonData["format"].toString().toStdString();
        mata.musicName = jsonData["musicName"].toString().toStdString();

        std::string jsonStr = jsonDoc.toJson(QJsonDocument::Indented).toStdString();

        //std::cout << "jsonData:  "<<jsonStr << std::endl;


        ncm.mata = mata;

        std::string image = albumImage(inputStream);
        ncm.image.setData((unsigned char*)image.c_str(),image.length());
        
        boost::filesystem::path ncmFile = ncmFilePath;
        outFilePath += "/" + ncmFile.filename().string().substr(0,ncmFile.filename().string().length()\
        -3)+ncm.mata.format;
        ncm.outFile = outFilePath;

        //std::cout << "out put path: "<<outFilePath << std::endl;

        std::ofstream outputStream(outFilePath);
        musicData(inputStream,outputStream,key);

        //std::cout << "get music data success "<<std::endl;
        combineFile(ncm);

        printf("文件转换成功: %s\n",outFilePath.c_str());
    }else {
        printf("文件转换失败\n");
    }
  

    return true;
}

void Converter::magicHeader(std::ifstream &inputStream)
{
    char buffer[10];

    inputStream.read(buffer,10);
}



std::string Converter::cr4Key(std::ifstream &inputStream)
{
    // 4 bytes key length , unsigned int
    uint8_t keyLenBuffer[4];

    inputStream.read(reinterpret_cast<char*>(keyLenBuffer),4);
    int len = Utils::getLength(keyLenBuffer);

    printf("len = %d\n",len);

    uint8_t encrypedKey[len];
    uint8_t decryedKey[len*2];
    inputStream.read((char*)encrypedKey,len);

    //std::cout << __func__ << __LINE__ << " read "<< inputStream.gcount() << " bytes from file stream."<<std::endl;


    //1.按字节对0x64异或
    for (int i = 0;i < len; i++) {
        encrypedKey[i] ^= 0x64;
    }

    //2.AES解密

    int decrylen = AES::decrypt(encrypedKey,len,AES::CORE_KEY.data(),decryedKey);
    int plain_data_len = AES::pkcs5unpaddingCR4(decryedKey,decrylen);

    //memset(decryedKey+len,0,len);
    //3. skip 'neteasecloudmusic' 17bytes

    uint8_t finatmep[plain_data_len-17];
    memcpy(finatmep,decryedKey+17,plain_data_len-17);
    //memset(finatmep+plain_data_len-17,0,17);

    int sz  = sizeof(finatmep)/sizeof(finatmep[0]);
    std::string ret(reinterpret_cast<const char*>(finatmep),sz);

    return ret;
}

std::string Converter::mataData(std::ifstream &inputStream)
{
    uint8_t bytes[4];

    inputStream.read(reinterpret_cast<char*>(bytes),4);
    int len = Utils::getLength(bytes);

    //std::cout<<__func__<<__LINE__<< "  len = "<<len<< std::endl;

    char byte[len];
    uint8_t encrypedKey[len];
    uint8_t decrypedKey[len*2];
    int plain_data_len = 0;
    int encode_data_len = 0;
    inputStream.read(byte,len);

    //std::cout << __func__ << __LINE__ << " read "<< inputStream.gcount() << " bytes from file stream."<<std::endl;

    //跳过：CRC(4字节)，unused Gap(5字节)
    char temp[9];
    inputStream.read(temp,9);

     
    if(!inputStream)
        throw std::runtime_error("read mata data exception!");


    //1.按字节对0x63异或
    for (int i = 0; i < len; i++)
    {
        byte[i] ^= 0x63;
    }

    // byte[len] = '\0';

    //2.去除前`163 key(Don't modity):`22个字符
    memcpy(encrypedKey,byte+22,len-22);
    //encrypedKey[len-22]='\0';

    //3.Base64进行decode转码
    
    //printf("mata before base64 decode: %s\n\n\n",encrypedKey);

    encode_data_len = len-12;

    int sz = sizeof(encrypedKey)/sizeof(encrypedKey[0]);
    uint8_t *buffer = nullptr;

    /*
    buffer = (uint8_t*)base64_decode((char*)encrypedKey,&encode_data_len);

    if(!buffer || (buffer[0] == '\0'))
    {
        return std::string("");
    }
    */


    QTextCodec *codec = QTextCodec::codecForName("GB18030");
    if(!codec)
    {
        std::cout<<"not codec"<<std::endl;
        return std::string("");
    }
    std::string encr1(reinterpret_cast<const char*>(encrypedKey),sz);
    std::string encr2 = codec->toUnicode(QByteArray::fromStdString(encr1)).toUtf8().toStdString();
    QByteArray byteArray = QByteArray::fromBase64(QByteArray::fromStdString(encr1));
    std::string decodeStr = byteArray.toStdString();

    //std::cout << "mata after base64 decode: "<<decodeStr<<std::endl;

    //4 AES解密

    // std::vector<unsigned char> temp_v(tempStr.begin(),tempStr.end());

    int encrylen = AES::decrypt(reinterpret_cast<const uint8_t*>(decodeStr.c_str()),encode_data_len,AES::MATA_KEY.data(),decrypedKey);
    plain_data_len = AES::pkcs5unpaddingMata(decrypedKey,encrylen);

    std::cout<<__LINE__ << __func__ << "json: "<< decrypedKey<<std::endl;

    if(plain_data_len < 100)
        return std::string("");
    //std::cout << "mata decrypt successful"<<std::endl;

    //5 去除前面 music： 6个字节后获得JSON

    uint8_t finatemp[plain_data_len-6];

    memcpy(finatemp,decrypedKey+6,plain_data_len-6);

    //memset(finatemp+plain_data_len-6,0,6);

    int sz2 = sizeof(finatemp)/sizeof(finatemp[0]);

    std::string mata(reinterpret_cast<const char*>(finatemp),sz2);


    delete buffer;
    return mata;
}

std::string Converter::albumImage(std::ifstream &inputStream)
{
    uint8_t bytes[4];
    inputStream.read(reinterpret_cast<char*>(bytes),4);
    int len = Utils::getLength(bytes);

    char imageData[len];
    inputStream.read(imageData,len);

    if(!inputStream)
        throw std::runtime_error("read image data error!");
    else
        //std::cout<<__func__<<__LINE__<<" read "<<inputStream.gcount()<<" from stream"<<std::endl;

    return std::string(imageData,len);

}

void Converter::musicData(std::ifstream &inputStream, std::ofstream &outputStream, std::string cr4Key__)
{
   char* cr4Key  = const_cast<char*>(cr4Key__.data()); 
   CR4 cr4;
   cr4.KSA(cr4Key,cr4Key__.length());
   char temp[0x8000];

    while(inputStream.read(temp,0x100))
    {
        size_t bytesRead = inputStream.gcount();
        cr4.PRGA(temp,bytesRead);
        outputStream.write(temp,bytesRead);
    }

   inputStream.close();
   outputStream.close();
}

void Converter::combineFile(Ncm ncm)
{
    TagLib::MPEG::File audioFile(ncm.outFile.c_str());

    
    if(audioFile.isValid() && audioFile.ID3v2Tag()) {
        TagLib::ID3v2::Tag *t = audioFile.ID3v2Tag();

        std::string s1(ncm.mata.artist[0].name);
        std::string s2 = std::to_string(ncm.mata.artist[0].num);

        t->setAlbum(ncm.mata.album);
        t->setArtist(s1+s2);
        t->setTitle(ncm.mata.musicName);

        std::vector<unsigned char> imageBytes(ncm.image.data,ncm.image.data+ncm.image.len);

        if(!imageBytes.empty()) {
            TagLib::ByteVector imageData((const char*)imageBytes.data(),imageBytes.size());
            //创建封面图像 
            TagLib::ID3v2::AttachedPictureFrame *converArt = new TagLib::ID3v2::AttachedPictureFrame();
            converArt->setMimeType(Utils::albumImageMimeType(&ncm.image));
            converArt->setType(TagLib::ID3v2::AttachedPictureFrame::FrontCover);
            converArt->setPicture(imageData);

            // 将封面图像添加到标签
            t->addFrame(converArt);
            audioFile.save();

        }


    }

    printf("Combine file sucess.\n");
}

/*
void Converter::combineFileFFmpeg(Ncm ncm)
{
    av_register_all();

    AVFormatContext *outputFormatContext = avformat_alloc_context();

    if(!outputFormatContext)
    {
        std::cerr<< "Error allocating output format context" << std::endl;
        return;
    }

    if(avformat_alloc_output_context2(&outputFormatContext, nullptr, nullptr, ncm.outFile.c_str()) < 0)
    {
        std::cerr << "Error allocating output format context " << std::endl;
        avformat_free_context(outputFormatContext);
        return;
    }

    if(avio_open(&outputFormatContext->pb, ncm.outFile.c_str(), AVIO_FLAG_WRITE) < 0)
    {
        std::cerr << "Error opening output file" << std::endl;
        avformat_free_context(outputFormatContext);
        return;
    }

    //set meta data
    AVDictionary *metadata = nullptr;
    av_dict_set(&metadata,"album",ncm.mata.album.c_str(),0);
    av_dict_set(&metadata,"artist",ncm.mata.artist[0].name.c_str(),0);
    av_dict_set(&metadata,"title",ncm.mata.musicName.c_str(),0);

    //add image 
    if(ncm.image.data && ncm.image.len > 0)
    {
        AVPacket pkt;
        av_init_packet(&pkt);
        pkt.data = ncm.image.data;
        pkt.size = ncm.image.len;
        av_write_frame(outputFormatContext,&pkt);
    }

    av_write_trailer(outputFormatContext);

    avformat_free_context(outputFormatContext);
}
*/
