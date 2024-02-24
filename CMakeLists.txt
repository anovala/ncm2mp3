cmake_minimum_required(VERSION 3.0)
project(Ncm2Mp3Cpp)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-D__STDC_CONSTANT_MACROS")

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

#[[
 find_package(PkgConfig REQUIRED)
 pkg_check_modules(LIBAV REQUIRED IMPORTED_TARGET
     libavdevice
     libavfilter
     libavformat
     libavcodec
     libswresample
     libswscale
     libavutil
 )
 set(CMAKE_FIND_LIBRARY_SUFFIXES .a ${CMAKE_FIND_LIBRARY_SUFFIXES})

 target_link_libraries(${PROJECT_NAME} PRIVATE PkgConfig::LIBAV)
]]

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


find_package(Boost REQUIRED COMPONENTS filesystem regex)
target_link_libraries(${PROJECT_NAME} PRIVATE Boost::filesystem Boost::regex)

find_package(ICU 70.1 COMPONENTS uc i18n REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE ICU::uc ICU::i18n)

set(CMAKE_BUILD_TYPE Debug)


