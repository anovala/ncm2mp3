#include <openssl/aes.h>
#include <openssl/evp.h>
#include <stdexcept>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

class AES {

public:
    AES() {}

    static const std::vector<unsigned char> CORE_KEY;

    static const std::vector<unsigned char> MATA_KEY;

    static const std::string TRANSFORMATION;

    static const std::string ALGORITHM;

    static const unsigned int BUFSIZE = 1024;

    //padding bytes
    static const std::vector<uint8_t> v3;
    static const std::vector<uint8_t> v4;
    static const std::vector<uint8_t> v5;
    static const std::vector<uint8_t> v6;
    static const std::vector<uint8_t> v7;
    static const std::vector<uint8_t> v8;
    static const std::vector<uint8_t> v9;
    static const std::vector<uint8_t> va10;
    static const std::vector<uint8_t> vb11;
    static const std::vector<uint8_t> vc12;
    static const std::vector<uint8_t> vd13;
    static const std::vector<uint8_t> ve14;
    static const std::vector<uint8_t> vf15;


    /*
    static int decrypt(const unsigned char *cipher_data, int cipher_data_len, const unsigned char *key,unsigned char *plain_data)
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

    */

    static int CheckPaddingBytes(uint8_t *data ,int len)
    {
        uint8_t *foundv3 = std::search(data,data+len,AES::v3.begin(),AES::v3.end());
        uint8_t *foundv4 = std::search(data,data+len,AES::v4.begin(),AES::v4.end());
        uint8_t *foundv5 = std::search(data,data+len,AES::v5.begin(),AES::v5.end());
        uint8_t *foundv6 = std::search(data,data+len,AES::v6.begin(),AES::v6.end());
        uint8_t *foundv7 = std::search(data,data+len,AES::v7.begin(),AES::v7.end());
        uint8_t *foundv8 = std::search(data,data+len,AES::v8.begin(),AES::v8.end());
        uint8_t *foundv9 = std::search(data,data+len,AES::v9.begin(),AES::v9.end());
        uint8_t *founda = std::search(data,data+len,AES::va10.begin(),AES::va10.end());
        uint8_t *foundb = std::search(data,data+len,AES::vb11.begin(),AES::vb11.end());
        uint8_t *foundc = std::search(data,data+len,AES::vc12.begin(),AES::vc12.end());
        uint8_t *foundd = std::search(data,data+len,AES::vd13.begin(),AES::vd13.end());
        uint8_t *founde = std::search(data,data+len,AES::ve14.begin(),AES::ve14.end());
        uint8_t *foundf = std::search(data,data+len,AES::vf15.begin(),AES::vf15.end());

        uint8_t *pastOfEnd = data+len;
        int index = -1;

        if(foundv3 != pastOfEnd)
            index = foundv3 - data;
        else if(foundv4 != pastOfEnd)
            index = foundv4 - data;
        else if(foundv5 != pastOfEnd)
            index = foundv5 - data;
        else if(foundv6 != pastOfEnd)
            index = foundv6 - data;
        else if(foundv7 != pastOfEnd)
            index = foundv7 - data;
        else if(foundv8 != pastOfEnd)
            index = foundv8 - data;
        else if(foundv9 != pastOfEnd)
            index = foundv9 - data;
        else if(founda != pastOfEnd)
            index = founda - data;
        else if(foundb != pastOfEnd)
            index == foundb - data;
        else if(foundc != pastOfEnd)
            index = foundc - data;
        else if(foundd != pastOfEnd)
            index = foundd - data;
        else if(founde != pastOfEnd)
            index = founde - data;
        else if(foundf != pastOfEnd)
            index = foundf - data;


        return index;

    }

    static int decrypt(const uint8_t *cipher_data, int len, const uint8_t *key, uint8_t *plain_data)
    {
        AES_KEY decryptionKey;
        AES_set_decrypt_key(key, 128, &decryptionKey);

        int numBlocks = len / AES_BLOCK_SIZE; // Calculate the number of blocks
        int decryptedLen = numBlocks * AES_BLOCK_SIZE; // Calculate the total length of decrypted data

        /*
        if(len % AES_BLOCK_SIZE != 0)
        {
            numBlocks++;
        }
        */

        // Perform decryption
        for (int i = 0; i < numBlocks; ++i) {
            AES_decrypt(cipher_data + (i * AES_BLOCK_SIZE), plain_data + (i * AES_BLOCK_SIZE), &decryptionKey);
        }

        return decryptedLen;
    }


    static int pkcs5unpaddingCR4(uint8_t *data, int len)
    {
        unsigned char paddingsize = data[len - 1];

        unsigned int paddingindex = len - paddingsize;

        for(int i = 0;i < paddingsize; i++)
        {
            if(data[paddingindex+i] != (uint8_t)paddingsize)
            {
                return -1;
            }
        }
        return paddingindex;

    }

    static int findPaddingIndex(uint8_t *data,int len)
    {
        return AES::CheckPaddingBytes(data,len);
    }

    static int pkcs5unpaddingMata(uint8_t *data, int len)
    {
        std::cout << __LINE__ << " " << __func__ << " original decrypt: ";
        for (int i = 0; i < len; ++i) {
            std::cout << std::hex << static_cast<int>(data[i]) << " ";
        }
        std::cout << std::endl;


        unsigned int paddingindex = AES::findPaddingIndex(data,len);

        if(paddingindex == -1)
            return paddingindex;

        std::cout << "Padding index: " << paddingindex << std::endl;

        // Print padding bytes
        std::cout << "Padding bytes: ";
        for (int i = paddingindex; i < len; ++i) {
            std::cout << std::hex << static_cast<int>(data[i]) << " ";
        }
        std::cout << std::endl;

        /* Check padding correctness
        for (int i = paddingindex; i < len; ++i) {
            if (data[i] != paddingsize) {
                std::cerr << "Invalid padding detected." << std::endl;
                return -1; // Or handle the error in a different way
            }
        }
        */

        return paddingindex;
    }


};

const std::vector<unsigned char> AES::CORE_KEY = {0x68, 0x7A, 0x48, 0x52, 0x41, 0x6D, 0x73, 0x6F, 0x35, 0x6B, 0x49, 0x6E, 0x62, 0x61, 0x78, 0x57};
const std::vector<unsigned char> AES::MATA_KEY = {0x23, 0x31, 0x34, 0x6C, 0x6A, 0x6B, 0x5F, 0x21, 0x5C, 0x5D, 0x26, 0x30, 0x55, 0x3C, 0x27, 0x28};
const std::string AES::TRANSFORMATION = "AES/ECB/PKCS5Padding";
const std::string AES::ALGORITHM = "AES";
const std::vector<uint8_t> AES::v3   = {0x3,0x3,0x3};
const std::vector<uint8_t> AES::v4   = {0x4,0x4,0x4,0x4};
const std::vector<uint8_t> AES::v5   = {0x5,0x5,0x5,0x5,0x5};
const std::vector<uint8_t> AES::v6   = {0x6,0x6,0x6,0x6,0x6,0x6};
const std::vector<uint8_t> AES::v7   = {0x7,0x7,0x7,0x7,0x7,0x7,0x7};
const std::vector<uint8_t> AES::v8   = {0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8};
const std::vector<uint8_t> AES::v9   = {0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x9};
const std::vector<uint8_t> AES::va10 = {0xa,0xa,0xa,0xa,0xa,0xa,0xa,0xa,0xa,0xa};
const std::vector<uint8_t> AES::vb11 = {0xb,0xb,0xb,0xb,0xb,0xb,0xb,0xb,0xb,0xb,0xb};
const std::vector<uint8_t> AES::vc12 = {0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc,0xc};
const std::vector<uint8_t> AES::vd13 = {0xd,0xd,0xd,0xd,0xd,0xd,0xd,0xd,0xd,0xd,0xd,0xd,0xd};
const std::vector<uint8_t> AES::ve14 = {0xe,0xe,0xe,0xe,0xe,0xe,0xe,0xe,0xe,0xe,0xe,0xe,0xe,0xe};
const std::vector<uint8_t> AES::vf15 = {0xf,0xf,0xf,0xf,0xf,0xf,0xf,0xf,0xf,0xf,0xf,0xf,0xf,0xf,0xf};
