#ifndef __CR4
#define __CR4
#include <string>
#include <vector>
#include <cstring>

class CR4 {

public:

    CR4(){}

    /// @brief CR4-KSA密钥调度算法
    /// 功能生成s-box
    /// @param key 密钥
    /// @param len 
    void KSA(char *key, int len){
        
        for(int i = 0; i < 256; i++) {
                box[i] = i;
        }

        for(int i = 0, j = 0 ; i < 256 ; i++) {
            j = ( j + box[i] + key[i % len]) & 0xff;
            int swap = box[i];
            box[i] = box[j];
            box[j] = swap;
        }
    }

    /// @brief CR4-PRGA伪随机数生成算法
    /*功能：加密或解密*/
    /// @param data  加密| 解密的数据
    /// @param length  数据长度
    void PRGA(char *data, int length) {
        for(int k = 0, i, j; k < length ; k++) {
            i = (k + 1) & 0xff;
            j = (box[i]+i) & 0xff;
            data[k] ^= box[(box[i]+ box[j]) & 0xff];
        }
    }
private:

    int box[256];
};

#endif
