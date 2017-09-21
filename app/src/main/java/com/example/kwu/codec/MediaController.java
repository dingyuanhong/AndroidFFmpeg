package com.example.kwu.codec;

import android.os.Environment;
import android.util.Log;

import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;

/**
 * Created by kwu on 2017/8/4 0004.
 */

public class MediaController
    extends JNIObject
{
    private String TAG = "MediaController";
    private File defaultDir = new File(Environment.getExternalStorageDirectory().getAbsolutePath() + "/UVCResource/temp_frame_write");
    private int index  = 0;
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

    private void writeFrame(String name,byte[] bytes)
    {
        File frame = new File(defaultDir,"/" + name);
        writeFrame(frame,bytes);
    }

    private void writeFrame(File frame,byte[] bytes) {
        FileOutputStream outputStream = null;
        BufferedOutputStream boStream = null;
        try {
            outputStream = new FileOutputStream(frame);
            boStream = new BufferedOutputStream(outputStream);
            boStream.write(bytes);
            Log.d("MediaController","写文件完成.");
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            if (null != boStream) {
                try {
                    boStream.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
            if (null != outputStream) {
                try {
                    outputStream.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
    }

    public void YUVData(byte[] data)
    {
        String strName;
        strName = index + "_frame.txt";
        writeFrame(strName,data);
        Log.e("MediaController","存储索引:" + index);
        index++;
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
