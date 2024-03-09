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

    static int pkcs5unpadding(uint8_t *data, int len)
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

};

const std::vector<unsigned char> AES::CORE_KEY = {0x68, 0x7A, 0x48, 0x52, 0x41, 0x6D, 0x73, 0x6F, 0x35, 0x6B, 0x49, 0x6E, 0x62, 0x61, 0x78, 0x57};
const std::vector<unsigned char> AES::MATA_KEY = {0x23, 0x31, 0x34, 0x6C, 0x6A, 0x6B, 0x5F, 0x21, 0x5C, 0x5D, 0x26, 0x30, 0x55, 0x3C, 0x27, 0x28};
const std::string AES::TRANSFORMATION = "AES/ECB/PKCS5Padding";
const std::string AES::ALGORITHM = "AES";
