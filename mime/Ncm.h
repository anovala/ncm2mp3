#ifndef NCM_H
#define NCM_H
#include "Mata.h"
#include <cstring>

struct Image {
	Image(unsigned char *c, size_t sz):len(sz){
		data = new unsigned char[len];
		memcpy(data,c,len);
	}

	~Image() {

		if(data != nullptr)
	{	
		delete[] data;
		data = nullptr;
		len = 0;}
	}

	Image(Image &im){
		
		this->len = im.len;
		if(this->len > 0)
		{
			this->data = new unsigned char[this->len];
			memcpy(this->data,im.data,this->len);
		}
	}

	void setData(unsigned char *c, size_t sz){
		if(data == nullptr)
		{
			data = new unsigned char[sz];
			len = sz;
		}else{
			delete[] data;
			data = new unsigned char[sz];
			len = sz;
		}
	}

	unsigned char *data = nullptr;
	size_t len = 0;
};

//typedef Image img;

struct Ncm {

	Ncm(unsigned char *c, size_t sz):image(c,sz){}

	std::string ncmFile;

	std::string outFile;

	Mata mata;

	Image image;	 
};

#endif
