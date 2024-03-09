#ifndef __BASE64
#define __BASE64
#include <iostream>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/buffer.h>
#include <string.h>
#include <math.h>

/*  Calculates the  length of a decoded base64 string*/
int calcDecodeLength(const char *b64input) {

    int len = strlen(b64input);
    int padding = 0;

    if (b64input[len - 1] == '=' && b64input[len - 2] == '=') //last two chars are =
        padding = 2;
    else if (b64input[len-1] == '=') // last char is =
        padding = 1;
    else{
        //printf("base64 encode not end of =\n");
    }

    return (int)len*0.75 - padding;
}

/* Decodes a base64 encode string*/
int Base64Decode(char *encodeData, char **plainData, int *encode_data_len){
    
    BIO *bio, *b64;
    int padding = 0;

    int decodeLen = calcDecodeLength(encodeData), len = 0;
    *encode_data_len = decodeLen;
    *plainData = (char*)malloc(decodeLen+1);
    FILE *stream = fmemopen(encodeData,strlen(encodeData),"r");

    b64 = BIO_new(BIO_f_base64());
    bio = BIO_new_fp(stream,BIO_NOCLOSE);
    bio = BIO_push(b64, bio);
    BIO_set_flags(bio,BIO_FLAGS_BASE64_NO_NL); //Do not use newLines to flush buffer
    len = BIO_read(bio,*plainData,strlen(encodeData));

    /*Can test here if len == decodeLen - if not, then return an error*/
    (*plainData)[len] = '\0';

    BIO_free_all(bio);
    fclose(stream);

    return 0; // success
}

/* Encodes a string to base64*/
int Base64Encode(const char *plainData, char **encodeData) {
    
    BIO *bio, *b64;
    FILE *stream;
    int encodeSize = 4*ceil((double)strlen(plainData)/3);
    *encodeData = (char *)malloc(encodeSize+1);

    stream = fmemopen(*encodeData,encodeSize+1,"w");
    b64 = BIO_new(BIO_f_base64());
    bio = BIO_new_fp(stream,BIO_NOCLOSE);
    bio = BIO_push(b64,bio);
    BIO_set_flags(bio,BIO_FLAGS_BASE64_NO_NL); //Ignore newlines - write everting in one line
    BIO_write(bio,plainData,strlen(plainData));
    BIO_flush(bio);
    BIO_free_all(bio);
    fclose(stream);

    return 0; // success
}

std::string base64_encode(const unsigned char *data, size_t length){
    // BIO *bio , *b64;
    // BUF_MEM *bptr;

    // b64 = BIO_new(BIO_f_base64());
    // bio = BIO_new(BIO_s_mem());
    // bio = BIO_push(b64,bio);

    // BIO_write(bio,data,static_cast<int>(length));
    // BIO_flush(bio);
    // BIO_get_mem_ptr(bio,&bptr);

    // std::string result(bptr->data, bptr->length);

    // BIO_free_all(bio);

    char *buffer = NULL;
    Base64Encode(reinterpret_cast<const char*>(data),&buffer);

    return std::string(buffer);
}


//problem
char* base64_decode(char * encodeData, int *encode_data_len){
    // BIO *bio, *b64;
    // char *buffer = new char[encoded.size()];

    // b64 = BIO_new(BIO_f_base64());
    // bio = BIO_new_mem_buf(encoded.c_str(), static_cast<int>(encoded.size()));
    // bio = BIO_push(b64, bio);

    // BIO_read(bio, buffer, static_cast<int>(encoded.size()));

    // BIO_free_all(bio);

    // std::string result(buffer);
    // delete[] buffer;

    char *buffer = NULL;
    Base64Decode(encodeData,&buffer,encode_data_len);

    return buffer;

}


#endif
