//
// Created by kwu on 2017/9/22 0022.
//

#ifndef ANDROIDFFMPEG_NATIVE_JNILOAD_H
#define ANDROIDFFMPEG_NATIVE_JNILOAD_H

#include "jni.h"
#include <android/log.h>

#define  CONSTRUCT(T) { T *t = new T(); \
    jclass clazz = (jclass)(*env).GetObjectClass(thiz); \
    jfieldID fid = (jfieldID)(*env).GetFieldID(clazz, "mObj", "J"); \
    jlong jstr = (jlong) (*env).GetLongField(thiz, fid);  \
    (*env).SetLongField(thiz, fid, (jlong)t);}

#define OBJECT(T,name) jclass clazz = (jclass)env->GetObjectClass(thiz); \
     jfieldID fid = env->GetFieldID(clazz, "mObj","J");  \
     T *name = (T *)env->GetLongField(thiz, fid);

#define DESTRUCT(T)  jclass clazz = (jclass)env->GetObjectClass(thiz); \
     jfieldID fid = env->GetFieldID(clazz, "mObj","J");  \
     T *object = (T *)env->GetLongField(thiz, fid); \
     if(object != NULL) delete object; \
     (*env).SetLongField(thiz, fid, (jlong)0);


JavaVM *GetVM();

#endif //ANDROIDFFMPEG_NATIVE_JNILOAD_H
