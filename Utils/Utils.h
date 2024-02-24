#ifndef __UTILS
#define __UTILS

#include <string>
#include "../mime/Ncm.h"

class Utils {

public:
    Utils() {

    }

    static int getLength(char *bytes);

    /**
     * 图片专辑MIME类型
     * 功能:获取图片的MIME类型
     *
     * @param albumImage 图片数据
     * @return 图片类型
     */
    static std::string albumImageMimeType(Image *albumImage);

};

#endif
