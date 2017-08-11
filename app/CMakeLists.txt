# Sets the minimum version of CMake required to build the native
# library. You should either keep the default value or only pass a
# value of 3.4.0 or lower.

cmake_minimum_required(VERSION 3.4.1)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds it for you.
# Gradle automatically packages shared libraries with your APK.

add_library( native-lib SHARED
             src/main/cpp/EvoInterface/EvoMediaSource.cpp
             src/main/cpp/EvoInterface/sei_packet.cpp
             src/main/cpp/EvoInterface/VideoDecoder.cpp
             src/main/cpp/EvoInterface/EvoVideoConvert.cpp
             src/main/cpp/EvoInterface/AudioDecoder.cpp
             src/main/cpp/MediaControl.cpp
             src/main/cpp/native-media.cpp
             src/main/cpp/native-lib.cpp
              )

add_library( ijkffmpeg SHARED IMPORTED )
set_target_properties( ijkffmpeg
             PROPERTIES
             IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/libs/${ANDROID_ABI}/libijkffmpeg.so
             )

add_library( avcodec-56 SHARED IMPORTED )
set_target_properties( avcodec-56
             PROPERTIES
             IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/libs/${ANDROID_ABI}/libavcodec-56.so
             )

add_library( avformat-56 SHARED IMPORTED )
set_target_properties( avformat-56
             PROPERTIES
             IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/libs/${ANDROID_ABI}/libavformat-56.so
             )

add_library( avutil-54 SHARED IMPORTED )
set_target_properties( avutil-54
             PROPERTIES
             IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/libs/${ANDROID_ABI}/libavutil-54.so
             )

add_library( swresample-1 SHARED IMPORTED )
set_target_properties( swresample-1
             PROPERTIES
             IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/libs/${ANDROID_ABI}/libswresample-1.so
             )

add_library( swscale-3 SHARED IMPORTED )
set_target_properties( swscale-3
             PROPERTIES
             IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/libs/${ANDROID_ABI}/libswscale-3.so
             )

add_library( libijkffmpeg SHARED IMPORTED )
set_target_properties( libijkffmpeg
             PROPERTIES
             IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/libs/${ANDROID_ABI}/libijkffmpeg.so
             )

add_library( libavcodec SHARED IMPORTED )
set_target_properties( libavcodec
             PROPERTIES
             IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/libs/${ANDROID_ABI}/libavcodec.so
             )

add_library( libavformat SHARED IMPORTED )
set_target_properties( libavformat
             PROPERTIES
             IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/libs/${ANDROID_ABI}/libavformat.so
             )

add_library( libavutil SHARED IMPORTED )
set_target_properties( libavutil
             PROPERTIES
             IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/libs/${ANDROID_ABI}/libavutil.so
             )

add_library( libavresample SHARED IMPORTED )
set_target_properties( libavresample
             PROPERTIES
             IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/libs/${ANDROID_ABI}/libavresample.so
             )

add_library( libswresample SHARED IMPORTED )
set_target_properties( libswresample
             PROPERTIES
             IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/libs/${ANDROID_ABI}/libswresample.so
             )

add_library( libswscale SHARED IMPORTED )
set_target_properties( libswscale
             PROPERTIES
             IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/libs/${ANDROID_ABI}/libswscale.so
             )

add_library( libavfilter SHARED IMPORTED )
set_target_properties( libavfilter
             PROPERTIES
             IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/libs/${ANDROID_ABI}/libavfilter.so
             )

find_library( log-lib log )

#include_directories(${CMAKE_SOURCE_DIR}/libs/include)
include_directories(${CMAKE_SOURCE_DIR}/libs/includeSelf)

target_link_libraries( native-lib
                       libavformat
                       libavcodec
                       libavutil
                       libswresample
                       libswscale
                       libavfilter

                       # included in the NDK.
                       ${log-lib}
                       )


#libavformat
#libavcodec
#libavutil
#libswresample
#libswscale

#avformat-56
#avcodec-56
#avutil-54
#swresample-1
#swscale-3