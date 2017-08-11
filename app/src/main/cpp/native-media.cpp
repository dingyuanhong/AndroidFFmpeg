#include "jni.h"
#include "string"
#include <assert.h>
#include <android/log.h>

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
    return JNI_VERSION_1_4;
}

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
//        YUVData(timestamp,frame);
        __android_log_print(ANDROID_LOG_INFO,"native media","SendPacket!");
    }

    void YUVData(int64_t timestamp,AVFrame *frame)
    {
        JNIEnv *env = env_;
        if(env == NULL) return;
        int width = 3040;
        int height = 1520;
        jbyteArray YUVData_ = env->NewByteArray(width*height*3/2);

        jclass clazz = env->GetObjectClass(thiz);
        jmethodID  jid = env->GetMethodID(clazz,"YUVData","(L[B)V");
        env->CallVoidMethod(thiz,jid,YUVData_);
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
    control->init(g_VM,env,thiz);
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
