#include <openssl/aes.h>
#include <openssl/evp.h>
#include <stdexcept>
#include <vector>
#include <string>
#include <iostream>

class AES {

public:
    AES() {}

    static const std::vector<unsigned char> CORE_KEY;

    static const std::vector<unsigned char> MATA_KEY;

    static const std::string TRANSFORMATION;

    static const std::string ALGORITHM;

    static const unsigned int BUFSIZE = 1024;

    // static std::vector<unsigned char> decrypt(const std::vector<unsigned char> &str,\
    // const std::vector<unsigned char> &key, const std::string &transformation, const std::string\
    // algorithm) {
    //     EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    //     if(!ctx){
    //         throw std::runtime_error("EVP_CIPHER_CTX_new_failed");
    //     }

    //     int len = 0;
    //     int plain_data_len = 0;
        
    //     //mode = ECB
    //     const EVP_CIPHER *cipher_type = EVP_aes_128_ecb();

    //     std::cout<< __func__<<"    "<< __LINE__<< "    algorithnm name: "<<algorithm<<"  key's len: "<<key.size()<<std::endl;

    //     std::cout<< __func__<<"    "<< __LINE__<< "    transformation:  "<<transformation<<std::endl;

    //     std::cout<< __func__<<"    "<< __LINE__<< "    origin cr4: "<<str.data()<<std::endl;

    //     std::cout<< __func__<<"    "<< __LINE__<< "    encrypted key: " << key.data() <<std::endl;        

    //     if(EVP_DecryptInit_ex(ctx,cipher_type,NULL,&key[0],NULL)!=1){
    //         EVP_CIPHER_CTX_free(ctx);
    //         throw std::runtime_error("EVP_DecryptInit_ex failed");

    //     }

    //     std::vector<unsigned char> decryptedText(4096);
        

    //     // std::cout<< __func__<<"    "<< __LINE__<< "  out size : " << outLen << std::endl;

    //     std::cout<< __func__<<"    "<< __LINE__<< "  decryedText size:  " << decryptedText.size() <<std::endl;
        
    //     // std::cout<< __func__<<"    "<< __LINE__<< " cipher_block_size:  " <<cipher_block_size << std::endl;

    //     std::cout<< __func__<<"    "<< __LINE__<< " origin text size: "<< str.size() << std::endl;

    //     if(EVP_DecryptUpdate(ctx,&decryptedText[0],&len,&str[0],plain_data_len)!=1){
    //         EVP_CIPHER_CTX_free(ctx);
    //         throw std::runtime_error("EVP_DecryptUpdate failed");
    //     }
    //     plain_data_len = len;

    //     if(EVP_DecryptFinal_ex(ctx,&decryptedText[len],&len)!=1){
    //         EVP_CIPHER_CTX_free(ctx);
    //         throw std::runtime_error("EVP_DecryptFinal_ex_failed");
    //     }
    //     plain_data_len += len;

    //     EVP_CIPHER_CTX_free(ctx);

    //     return decryptedText;
    // }

    static int decrypt(unsigned char *cipher_data, int cipher_data_len, const unsigned char *key, unsigned char *plain_data)
    {
        EVP_CIPHER_CTX *ctx;

        int len;

        int plain_data_len;

        if(!(ctx = EVP_CIPHER_CTX_new()))
        {
            EVP_CIPHER_CTX_free(ctx);
            throw std::runtime_error("EVP_CIPHER_CTX_new failed");
        }

        if(1 != EVP_DecryptInit_ex(ctx,EVP_aes_128_ecb(),NULL,key,NULL))
        {
            EVP_CIPHER_CTX_free(ctx);
            throw std::runtime_error("EVP_DecryptInit_ex failed");
        }

        if(1 != EVP_DecryptUpdate(ctx,plain_data,&len,cipher_data,cipher_data_len))
        {
            EVP_CIPHER_CTX_free(ctx);
            throw std::runtime_error("EVP_DecryptUpdate failed");
        }

        plain_data_len = len;

        if(1 != EVP_DecryptFinal_ex(ctx,plain_data+len, &len))
        {
            EVP_CIPHER_CTX_free(ctx);
            throw std::runtime_error("EVP_DecryptFinal_ex failed");
        } 

        plain_data_len += len;

        return plain_data_len;
    
    }
};

const std::vector<unsigned char> AES::CORE_KEY = {0x68, 0x7A, 0x48, 0x52, 0x41, 0x6D, 0x73, 0x6F, 0x35, 0x6B, 0x49, 0x6E, 0x62, 0x61, 0x78, 0x57};
const std::vector<unsigned char> AES::MATA_KEY = {0x23, 0x31, 0x34, 0x6C, 0x6A, 0x6B, 0x5F, 0x21, 0x5C, 0x5D, 0x26, 0x30, 0x55, 0x3C, 0x27, 0x28};
const std::string AES::TRANSFORMATION = "AES/ECB/PKCS5Padding";
const std::string AES::ALGORITHM = "AES";
