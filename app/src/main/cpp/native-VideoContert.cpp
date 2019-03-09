//
// Created by kwu on 2017/9/22 0022.
//
#include "native-jniload.h"
#include <assert.h>
#include "EvoInterface/EvoVideoConvert.h"

class JNIVideoConvert{
public:
    JNIVideoConvert()
    :javaVM(NULL),
     thiz(NULL)
    {

        srcFrmae = av_frame_alloc();
        desFrame = av_frame_alloc();
    }
    ~JNIVideoConvert()
    {
        av_frame_free(&srcFrmae);
        av_frame_free(&desFrame);
        assert(thiz==NULL);
    }
    void init(JavaVM * vm,JNIEnv *env,jobject object)
    {
        javaVM =vm;
        thiz = env->NewGlobalRef(object);
    }

    void uninit(JNIEnv *env)
    {
        env->DeleteGlobalRef(thiz);
        thiz = NULL;
    }

    int Initialize(EvoVideoInfo srcInfo ,EvoVideoInfo desInfo)
    {
        return convert.Initialize(srcInfo,desInfo);
    }

    jbyteArray Convert(JNIEnv *env,u_char * data,int length)
    {
        int targetSize = convert.GetTargetSize();
        if(targetSize <= 0) return NULL;
        jbyteArray baRet = env->NewByteArray(targetSize);
        jbyte *bRet = env->GetByteArrayElements(baRet, NULL);
        if(bRet == NULL) return NULL;

        EvoVideoInfo srcInfo = convert.GetSourceInfo();
        av_image_fill_arrays(
                srcFrmae->data,
                srcFrmae->linesize,
                data,
                srcInfo.Format,
                srcInfo.Width,
                srcInfo.Height,
                1
        );
        EvoVideoInfo desInfo = convert.GetTargetInfo();
        av_image_fill_arrays(
                desFrame->data,
                desFrame->linesize,
                (uint8_t*)bRet,
                desInfo.Format,
                desInfo.Width,
                desInfo.Height,
                1
        );

        int ret = convert.Convert(srcFrmae,desFrame);

        env->ReleaseByteArrayElements(baRet, bRet, 0);
        memset(srcFrmae->data,0,AV_NUM_DATA_POINTERS*sizeof(void*));
        memset(srcFrmae->linesize,0,AV_NUM_DATA_POINTERS*sizeof(int));
        memset(desFrame->data,0,AV_NUM_DATA_POINTERS*sizeof(void*));
        memset(desFrame->linesize,0,AV_NUM_DATA_POINTERS*sizeof(int));
        av_frame_unref(srcFrmae);
        av_frame_unref(desFrame);

        if(ret <= 0){
            return NULL;
        }
        return baRet;
    }
private:
    JavaVM* javaVM;
    jobject thiz;

    EvoVideoConvert convert;
    AVFrame *srcFrmae;
    AVFrame *desFrame;
};

EvoVideoInfo GetVideoInfo(JNIEnv *env, jobject obj)
{
    jclass cls = env->GetObjectClass(obj);
    jfieldID widthID = env->GetFieldID(cls , "Width" , "I");
    int width = env->GetIntField(obj,widthID);
    jfieldID heightID = env->GetFieldID(cls,"Height","I");
    int height = env->GetIntField(obj,heightID);
    jfieldID formatID = env->GetFieldID(cls,"Format","I");
    int format = env->GetIntField(obj,formatID);

    EvoVideoInfo info;
    info.Width = width;
    info.Height = height;
    info.Format = (AVPixelFormat)format;
    return info;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_kwu_codec_VideoConvert_construct(JNIEnv *env, jobject thiz) {
    CONSTRUCT(JNIVideoConvert);
    OBJECT(JNIVideoConvert,control);
    if(control == NULL) return ;
    control->init(GetVM(),env,thiz);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_kwu_codec_VideoConvert_destruct(JNIEnv *env, jobject thiz) {
    DESTRUCT(JNIVideoConvert);
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_example_kwu_codec_VideoConvert_Init(JNIEnv *env, jobject thiz, jobject src,
                                             jobject des) {
    OBJECT(JNIVideoConvert,control);
    if(control == NULL) return NULL;
    EvoVideoInfo srcInfo = GetVideoInfo(env,src);
    EvoVideoInfo desInfo = GetVideoInfo(env,des);
    return control->Initialize(srcInfo,desInfo);
}

extern "C"
JNIEXPORT jbyteArray JNICALL
Java_com_example_kwu_codec_VideoConvert_Convert(JNIEnv *env, jobject thiz, jbyteArray data_,
                                                jint length) {
    OBJECT(JNIVideoConvert,control);
    jbyte *data = env->GetByteArrayElements(data_, NULL);
    jbyteArray ret = control->Convert(env,(u_char*)data,length);
    env->ReleaseByteArrayElements(data_, data, 0);
    return ret;
}