cmake_minimum_required(VERSION 3.0)
project(Ncm2Mp3Cpp)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-D__STDC_CONSTANT_MACROS")

find_path(AVCODEC_INCLUDE_DIR libavcodec/avcodec.h)
find_library(AVCODEC_LIBRARY avcodec)

find_path(AVFORMAT_INCLUDE_DIR libavformat/avformat.h)
find_library(AVFORMAT_LIBRARY avformat)

find_path(AVUTIL_INCLUDE_DIR libavutil/avutil.h)
find_library(AVUTIL_LIBRARY avutil)

find_path(AVDEVICE_INCLUDE_DIR libavdevice/avdevice.h)
find_library(AVDEVICE_LIBRARY avdevice)

#Add the executable target
add_executable(${PROJECT_NAME}
   
    Utils/CR4.h
    Utils/Utils.cpp
    Utils/Utils.h
    Utils/_aes.h
    mime/Mata.h
    mime/Ncm.h 
    Utils/Base64.h 
    Converter/Converter.h 
    Converter/Converter.cpp
    main.cpp
)


add_executable(decode_encode main.cpp)
target_include_directories(decode_encode PRIVATE ${AVCODEC_INCLUDE_DIR} ${AVFORMAT_INCLUDE_DIR} ${AVUTIL_INCLUDE_DIR} ${AVDEVICE_INCLUDE_DIR})
target_link_libraries(decode_encode PRIVATE ${AVCODEC_LIBRARY} ${AVFORMAT_LIBRARY} ${AVUTIL_LIBRARY} ${AVDEVICE_LIBRARY})


#Set C++ standart to C++11
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_11)

#Find and link OpenSSL Library
find_package(OpenSSL REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE OpenSSL::SSL OpenSSL::Crypto)


# Find and link TagLib library using package config
find_package(PkgConfig REQUIRED)
pkg_check_modules(TagLib REQUIRED taglib)

# Add include directories and link libraries
target_include_directories(${PROJECT_NAME} PRIVATE ${TagLib_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${TagLib_LIBRARIES})


find_package(Boost REQUIRED COMPONENTS filesystem)
target_link_libraries(${PROJECT_NAME} PRIVATE Boost::filesystem)

find_package(ICU 70.1 COMPONENTS uc i18n REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE ICU::uc ICU::i18n)

set(CMAKE_BUILD_TYPE Debug)


