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

    public void YUVData(byte[] data)
    {
//        try {
//            int width = 3040;
//            int height = 1520;
//            ByteBuffer y = ByteBuffer.wrap(data,0,width*height);
//            ByteBuffer u = ByteBuffer.wrap(data,width*height,width*height/4);
//            ByteBuffer v = ByteBuffer.wrap(data,width*height + width*height/4,width*height/4);
//        }
//        catch ( Exception e)
//        {
//        }
    }

    private void logMessage(String msg)
    {
        Log.d(TAG,"解码:");
    }

    private native void construct();
    private native void destruct();
    public native int open(String path);
    public native void close();
    public native int play();
    public native void pause();
    public native void seek(int millisecond);
    public native static String errorString(int errno);
}
