#include <jni.h>
#include <string>
#include <malloc.h>

extern "C"
jstring
Java_com_example_kwu_codec_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* thiz */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_kwu_codec_MainActivity_brokenFromJNI(JNIEnv *env, jobject thiz) {
    void* addr = (void*)0xFF001000;
    free(addr);
}
