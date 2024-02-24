#include "Utils.h"

//little endian
int Utils::getLength(char *bytes)
{
    int len = 0;
    len |= bytes[0] & 0xff;
    len |= (bytes[1] & 0xff) << 8;
    len |= (bytes[2] & 0xff) << 16;
    len |= (bytes[3] & 0xff) << 24;

    return len;
}

std::string Utils::albumImageMimeType(Image * albumImage)
{
    unsigned char mPNG[] = {0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A}; //PNG file header
    if(albumImage->len > 8) {
        for( int i = 0;i < 8 ; i++) {
            if(albumImage->data[i] != mPNG[i])
                return "image/jpg";
        }
    }

    return "image/png";
}
