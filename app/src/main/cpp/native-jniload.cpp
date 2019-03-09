//
// Created by kwu on 2017/9/22 0022.
//
#include "native-jniload.h"
#include <assert.h>

static JavaVM* g_VM = NULL;

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved)
{
    JNIEnv* env = NULL;
    jint result = -1;

    if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
        return -1;
    }
    assert(env != NULL);
    g_VM = vm;
#ifdef USE_NEW_API
    av_jni_set_java_vm(g_VM,NULL);
#endif
    return JNI_VERSION_1_4;
}

JavaVM *GetVM()
{
    return g_VM;
}