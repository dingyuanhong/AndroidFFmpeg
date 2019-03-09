package com.example.kwu.codec;

import android.util.Log;

/**
 * Created by kwu on 2017/9/22 0022.
 */
public class VideoConvert
    extends JNIObject
{
    private String TAG = "VideoConvert";
    public VideoConvert()
    {
        construct();
    }
    @Override
    protected void finalize() throws Throwable
    {
        Log.e(TAG,"gc 处理");
        destruct();
        super.finalize();
    }

    private native void construct();
    private native void destruct();
    public native int Init(VideoInfo src,VideoInfo des);
    public native byte[] Convert(byte[] data,int length);
}
