//
// Created by jephy on 7/13/17.
//

#ifndef MEDIACONTROLLERDEMO_MEDIACONTROL_H
#define MEDIACONTROLLERDEMO_MEDIACONTROL_H
#include "EvoInterface/EvoMediaSource.h"
#include "EvoInterface/VideoDecoder.h"
#include "EvoInterface/EvoVideoConvert.h"
#include <pthread.h>

class MediaControl {
public:
    MediaControl();
    ~MediaControl();
    int Open(const char * file);
    int Close();
    int Play();
    int Pause();
    int Seek(int second);
    void Run();
protected:
    virtual void AttachThread() = 0;
    virtual void DetachThread() = 0;
    virtual void SendPacket(int64_t timestamp,AVFrame *frame);
private:
    EvoMediaSource source;
    EvoVideoConvert convert;
    VideoDecoder *decoder;
    pthread_mutex_t lock;
    int64_t timestamp_now;
    int64_t timestamp_last;
    int64_t time_last;
    AVFrame *frame_last;
    bool bStop;
    bool bPause;
    pthread_t thread;
};


#endif //MEDIACONTROLLERDEMO_MEDIACONTROL_H
