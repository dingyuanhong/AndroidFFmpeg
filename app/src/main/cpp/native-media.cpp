#include "native-jniload.h"
#include "string"
#include <assert.h>
#include "MediaControl.h"

class JNIMediaControl
        :public MediaControl
{
public:
    JNIMediaControl()
            :javaVM(NULL),env_(NULL),thiz(NULL)
    {

    }
    ~JNIMediaControl()
    {
        assert(thiz==NULL);
        assert(env_==NULL);
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

private:
    void AttachThread()
    {
        if(javaVM == NULL) return;
        int ret = javaVM->AttachCurrentThread(&env_,NULL);
        if(ret != 0)
        {
        }
    }

    void DetachThread()
    {
        if(javaVM == NULL) return;
        javaVM->DetachCurrentThread();
        env_ = NULL;
    }

private:
    void SendPacket(int64_t timestamp,AVFrame *frame)
    {
//        Callback(timestamp,frame);
//        YUVPacket(timestamp,frame);
        YUVData(timestamp,frame);
        __android_log_print(ANDROID_LOG_INFO,"native media","SendPacket!");
    }

    void YUVData(int64_t timestamp,AVFrame *frame)
    {
        JNIEnv *env = env_;
        if(env == NULL) return;
        int width = frame->width;
        int height = frame->height;
        __android_log_print(ANDROID_LOG_INFO,"native media","YUV:%d %d" , width , height);
        int length = width*height*3/2;
        jbyteArray YUVData_ = env->NewByteArray(length);
        jbyte * bytes = env->GetByteArrayElements(YUVData_,0);
        if(bytes == nullptr)
        {
            __android_log_print(ANDROID_LOG_ERROR,"native media","YUVData Error!");
            env->DeleteLocalRef(YUVData_);
            return;
        }
        memcpy(bytes,frame->data[0],length);
        jclass clazz = env->GetObjectClass(thiz);
        jmethodID  jid = env->GetMethodID(clazz,"YUVData","([B)V");
        env->CallVoidMethod(thiz,jid,YUVData_);
        env->ReleaseByteArrayElements(YUVData_,bytes,0);
        env->DeleteLocalRef(YUVData_);
    }

    void Callback(int64_t timestamp,AVFrame *frame)
    {
        JNIEnv *env = env_;
        if(env == NULL) return;
        char buffer[64] = {0};
        sprintf(buffer,"%lld",timestamp);
        jstring msg_ = env->NewStringUTF(buffer);
        jclass clazz = env->GetObjectClass(thiz);
        jmethodID  jid = env->GetMethodID(clazz,"logMessage","(Ljava/lang/String;)V");
        env->CallVoidMethod(thiz,jid,msg_);
        env->DeleteLocalRef(msg_);
        msg_ = NULL;
    }

    void YUVPacket(int64_t timestamp,AVFrame *frame)
    {
    }

private:
    JavaVM* javaVM;
    JNIEnv *env_;
    jobject thiz;
};

extern "C"
JNIEXPORT void JNICALL
Java_com_example_kwu_codec_MediaController_construct(JNIEnv* env, jobject thiz){
    CONSTRUCT(JNIMediaControl);
    OBJECT(JNIMediaControl,control);
    if(control == NULL) return ;
    control->init(GetVM(),env,thiz);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_kwu_codec_MediaController_destruct(JNIEnv *env,jobject thiz) {
    DESTRUCT(JNIMediaControl);
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_example_kwu_codec_MediaController_open(JNIEnv *env, jobject thiz,
                                                jstring file_) {
    const char *file = env->GetStringUTFChars(file_, 0);
    OBJECT(JNIMediaControl,control);
    if(control == NULL) return -1;
    int ret = control->Open(file);
    env->ReleaseStringUTFChars(file_, file);
    return ret;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_kwu_codec_MediaController_close(JNIEnv *env,jobject thiz) {
    OBJECT(JNIMediaControl,control);
    if(control == NULL) return ;
    control->Close();
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_example_kwu_codec_MediaController_play(JNIEnv *env,jobject thiz) {
    OBJECT(JNIMediaControl,control);
    if(control == NULL) return -1;
    return control->Play();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_kwu_codec_MediaController_pause(JNIEnv *env,jobject thiz) {
    OBJECT(JNIMediaControl,control);
    if(control == NULL) return ;
    control->Pause();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_kwu_codec_MediaController_seek(JNIEnv *env, jobject thiz,jint sec) {
    OBJECT(JNIMediaControl,control);
    if(control == NULL) return ;
    control->Seek(sec);
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_kwu_codec_MediaController_errorString(JNIEnv *env, jclass thiz,jint errorno) {
    char buf[255];
    int ret = av_strerror(errorno,buf,255);
    jstring sRet = env->NewStringUTF(buf);
    return sRet;
}
