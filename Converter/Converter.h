#ifndef __CONVERTER
#define __CONVERTER
#include "../mime/Ncm.h"
#include <string>
#include <fstream>

class Converter {

public:
	Converter(){

	}

	/*Conver ncm to mp3
	 * @param ncmFilePath 
	 * @param outFilePath
	 * @return sucessful or fail */
	bool ncm2mp3(std::string ncmFilePath, std::string outFilePath);

	/*ncm format header read
	 * MagicHeader Reader
	 * @param inputStream ncm file stream input */
	void magicHeader(std::ifstream &inputStream);

	/*
	 * get the CR4 key
	 * decrypt the CRY key from AES128
	 * @param inputStream ncm file stream input
	 * @return CR4 key */
	std::string cr4Key(std::ifstream &inputStream);

	/*get the mata from header
	 * get the music mata from header, show in JSON format
	 *
	 * @param inputStream ncm stream input
	 * @return JSON format header*/

private:

	std::string mataData(std::ifstream &inputStream);

	/*
	 * the picture of album
	 * get the picture info
	 *
	 * @param inputStream ncm file input stream
	 * @return the info of album picture */
	
	std::string albumImage(std::ifstream &inputStream);

	/*
	 * music data
	 * get the music data
	 * @param inputStream ncm file input Stream
	 * @param ouputStream the ouput stream to save music info
	 * @param cryKey
	 *  */

	void musicData(std::ifstream &inputStream, std::ofstream &outputStream,\
			std::string cr4Key);

	/*intergrate all the info in the ncm together, convert to the correspond 
	 * music format*/

	/// @brief 
	/// @param ncm 
	void combineFile(Ncm ncm);

	/// @brief 
	/// @param ncm 
	// void combineFileFFmpeg(Ncm ncm);


};

#endif
