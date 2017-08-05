package com.example.kwu.codec;

import android.util.Log;

/**
 * Created by kwu on 2017/8/4 0004.
 */

public class MediaController
    extends JNIObject
{
    private String TAG = "MediaController";

    public MediaController()
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

    private void logMessage(String msg)
    {
        Log.d(TAG,"解码:" + msg);
    }

    private native void construct();
    private native void destruct();
    public native int open(String path);
    public native void close();
    public native int play();
    public native void pause();
    public native void seek(int millisecond);
}
