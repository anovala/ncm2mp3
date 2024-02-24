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
#include <nlohmann/json.hpp>
#include <boost/filesystem.hpp>
#include <unicode/unistr.h>
#include <unicode/ustream.h>
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/opt.h>

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

        icu::UnicodeString unicodeString = icu::UnicodeString::fromUTF8(icu::StringPiece(jsonMusicInfo.c_str()));

        std::string utf8String;
        unicodeString.toUTF8String(utf8String);

        nlohmann::json jsonData = nlohmann::json::parse(utf8String);
        
        Mata mata;
        mata.album = jsonData["album"];

        std::string _name = jsonData["artist"][0][0];
        unsigned int _num = jsonData["artist"][0][1];

        artistInfo info;
        info.name = _name;
        info.num = _num;
        mata.artist.push_back(info);

        mata.format = jsonData["format"];
        mata.musicName = jsonData["musicName"];

        std::string jsonStr = nlohmann::to_string(jsonData);

        std::cout << "jsonData:  "<<jsonStr << std::endl;

        ncm.mata = mata;

        std::string image = albumImage(inputStream);
        ncm.image.setData((unsigned char*)image.c_str(),image.length());
        
        boost::filesystem::path ncmFile = ncmFilePath;
        outFilePath += "/" + ncmFile.filename().string().substr(0,ncmFile.filename().string().length()\
        -3)+ncm.mata.format;
        ncm.outFile = outFilePath;

        std::cout << "out put path: "<<outFilePath << std::endl;

        std::ofstream outputStream(outFilePath);
        musicData(inputStream,outputStream,key);

        std::cout << "get music data success "<<std::endl;        
        combineFile(ncm);

        printf("文件转换成功: %s\n",outFilePath.c_str());
    }else {
        printf("文件转换失败\n");
    }
  

    return false;
}

void Converter::magicHeader(std::ifstream &inputStream)
{
    char buffer[10];

    inputStream.read(buffer,10);
}



std::string Converter::cr4Key(std::ifstream &inputStream)
{
    char keyLenBuffer[4];

    inputStream.read(keyLenBuffer,4);
    int len = Utils::getLength(keyLenBuffer);

    printf("len = %d\n",len);

    uint8_t encrypedKey[len];
    // uint8_t temp[len];
    uint8_t decryedKey[len*2];
    inputStream.read((char*)encrypedKey,len);

     std::cout << __func__ << __LINE__ << " read "<< inputStream.gcount() << " bytes from file stream."<<std::endl;


    //1.按字节对0x64异或
    for (int i = 0;i < len; i++) {
        encrypedKey[i] ^= 0x64;
    }

    //2.AES解密

    // std::vector <unsigned char> Key_v(encrypedKey,encrypedKey+len);

    // for(unsigned char c: Key_v)
    // {
    //     std::cout << c << " ";
    // }
    // std::cout << std::endl;

    // Key_v = AES::decrypt(Key_v,AES::CORE_KEY,AES::TRANSFORMATION,AES::ALGORITHM);

    std::cout << "  encrypted key: "<<encrypedKey<<std::endl;

    int plain_data_len = AES::decrypt(encrypedKey,len,AES::CORE_KEY.data(),decryedKey);

    memset(decryedKey+len,0,len);

    std::cout << __func__ << __LINE__ <<"  cr4 decrypt success!   "<<"  \nPlain data: "<<decryedKey<<std::endl;

    std::cout << __func__ << __LINE__ <<" plain data len : "<<plain_data_len<<std::endl;
    // memcpy(decryedKey,encrypedKey,len-17); 

    uint8_t finatmep[plain_data_len];

    memcpy(finatmep,decryedKey+17,plain_data_len-17);
    memset(finatmep+plain_data_len-17,0,17);
    std::cout<< __func__ << __LINE__ <<"  memcpy success!"<<std::endl;

    //3. skip 'neteasecloudmusic' 17bytes
    std::string ret((char*)&finatmep[0]);


    std::cout << __func__ << __LINE__ << "  cr4Key:  " << ret << std::endl;
    

    return ret;
}

std::string Converter::mataData(std::ifstream &inputStream)
{
    char bytes[4];

    inputStream.read(bytes,4);
    int len = Utils::getLength(bytes);

    std::cout<<__func__<<__LINE__<< "  len = "<<len<< std::endl;

    char byte[len];
    uint8_t encrypedKey[len+1];
    uint8_t decrypedKey[len*2];
    char *buffer; 
    int plain_data_len = 0;
    int encode_data_len = 0;
    inputStream.read(byte,len);

    std::cout << __func__ << __LINE__ << " read "<< inputStream.gcount() << " bytes from file stream."<<std::endl;

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
    encrypedKey[len-22]='\0';

    //3.Base64进行decode转码
    
    printf("mata before base64 decode: %s\n\n\n",encrypedKey);

    buffer = base64_decode((char*)encrypedKey,&encode_data_len);

    std::cout << "mata after base64 decode: "<<buffer<<std::endl;

    //4 AES解密

    // std::vector<unsigned char> temp_v(tempStr.begin(),tempStr.end());

    plain_data_len = AES::decrypt((unsigned char*)buffer,encode_data_len,AES::MATA_KEY.data(),decrypedKey);

    std::cout << "mata decrypt successful"<<std::endl;

    //5 去除前面 music： 6个字节后获得JSON

    uint8_t finatemp[plain_data_len];

    memcpy(finatemp,decrypedKey+6,plain_data_len-6);

    memset(finatemp+plain_data_len-6,0,6);

    std::string mata((char*)&finatemp[0]);

    std::cout<< __func__ << __LINE__ << "  mata: "<<finatemp<<std::endl;

    free(buffer);

    return mata;
}

std::string Converter::albumImage(std::ifstream &inputStream)
{
    char bytes[4];
    inputStream.read(bytes,4);
    int len = Utils::getLength(bytes);

    std::cout<<__func__<<__LINE__<<" image len: "<<len<<std::endl;
    
    char imageData[len];
    inputStream.read(imageData,len);

    if(!inputStream)
        throw std::runtime_error("read image data error!");
    else
        std::cout<<__func__<<__LINE__<<" read "<<inputStream.gcount()<<" from stream"<<std::endl;

    std::string img(imageData,len);

    return img; 
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
