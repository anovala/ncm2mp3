cmake_minimum_required(VERSION 3.5)
project(Ncm2Mp3Cpp VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-D__STDC_CONSTANT_MACROS")
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

#Find and link Qt libraries
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)


#Add the executable target
set(PROJECT_SOURCES
   
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

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    add_executable(${PROJECT_NAME}
        ${PROJECT_SOURCES}
        qtui/mainwidget_.h
        qtui/mainwidget_.h
        qtui/mainwidget_.cpp qtui/mainwidget_.h qtui/mainwidget_.ui
        worker/worker.h


    )
else()
    if(ANDROID)
        add_library(${PROJECT_NAME} SHARED
                    ${PROJECT_SOURCES}
        )
    else()
        add_executable(${PROJECT_NAME}
            ${PROJECT_SOURCES}
        )
    endif()
endif()


target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.${PROJECT_NAME})
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${PROJECT_NAME})
endif()

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

#find_package(nlohmann_json 3.2.0 REQUIRED)
#target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)

find_package(Qt6 REQUIRED COMPONENTS Core5Compat)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core5Compat)


