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
             src/main/cpp/native-jniload.cpp
             src/main/cpp/native-media.cpp
             src/main/cpp/native-lib.cpp
             src/main/cpp/native-VideoContert.cpp
              )

add_library( libevoffmpeg SHARED IMPORTED )
set_target_properties( libevoffmpeg
             PROPERTIES
             IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/libs/${ANDROID_ABI}/libevoffmpeg.so
             )

find_library( log-lib log )

include_directories(${CMAKE_SOURCE_DIR}/libs/include)

target_link_libraries( native-lib
                       libevoffmpeg

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