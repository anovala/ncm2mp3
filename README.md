# ncm2mp3
Translate netease cloud music .ncm encrypt file to .mp3 format


# binary format

Magic Header		  10 bytes	    跳过
KEY Length		    4 bytes		    用AES128加密RC4密钥后的长度(小端字节排序，无符号整形)
Key From		      KEY Length	  用AES128加密的RC4密钥(注意：1.按字节对0x64异或 2.AES解密，其中PKCS5Padding填充模
AES128 Decode 		128bytes	    式会去除末尾填充部分;)3.去除前面的neteasecloudmusic17个字节
Math length		    4bytes		    Mata的信息长度(小端字节排序，无符号整形)
Mata Data(Json)		Mata Length	  JSON格式的Mata信息(注意：1.按字节对0x63异或. 2.去除前面163key(Don't modify):22个
                                字节;3.Base64进行decode;4.AES解密.5.去除前面music:6个字节后获得json)
CRC校验码		      4bytes		    跳过
Gap			          5bytes		    跳过
Image Size		    Image Size	  图片数据
Music Data				RC4-KSA生成s盒，RC4-PRGA解密


# dependencies

1.Openssl
2.PkgConfig
3.Taglib
4.Boost
5.Icu
6.nlomann_json
