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

find_library( log-lib log )

include_directories(${CMAKE_SOURCE_DIR}/libs/include)

target_link_libraries( native-lib
                       avcodec-56
                       avformat-56
                       avutil-54
                       swresample-1
                       swscale-3

                       # included in the NDK.
                       ${log-lib}
                       )