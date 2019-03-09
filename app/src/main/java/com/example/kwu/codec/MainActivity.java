package com.example.kwu.codec;

import android.net.Uri;
import android.os.Bundle;
import android.os.Environment;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import com.google.android.gms.appindexing.Action;
import com.google.android.gms.appindexing.AppIndex;
import com.google.android.gms.appindexing.Thing;
import com.google.android.gms.common.api.GoogleApiClient;

import java.io.File;
import java.nio.Buffer;
import java.util.ArrayList;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }


    /**
     * ATTENTION: This was auto-generated to implement the App Indexing API.
     * See https://g.co/AppIndexing/AndroidStudio for more information.
     */
    private GoogleApiClient client;
    private MediaController mediaController = null;
    private Button bt;
    private TextView tv = null;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        mediaController = new MediaController();
        // Example of a call to a native method
        tv = (TextView) findViewById(R.id.sample_text);
        tv.setText(stringFromJNI());
        bt = (Button) findViewById(R.id.button_test);
        bt.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                String path = Environment.getExternalStorageDirectory().getAbsolutePath();
                path += "/UVCResource/video.h264";
//                path += "/UVCResource/Vid0616000023.mp4";
                mediaController.close();
                int ret = mediaController.open(path);
                if(ret != 0) tv.setText("打开文件错误:" + MediaController.errorString(ret));
                else {
                    ret = mediaController.play();
                    if (ret != 0) tv.setText("开始失败:" + MediaController.errorString(ret));
                    else {
                        tv.setText("开始成功");
                        //bt.setOnClickListener(null);
                    }
                }
            }
        });
        bt = (Button) findViewById(R.id.button_convert);
        bt.setOnClickListener(new View.OnClickListener() {
              public void onClick(View v) {
                  VideoInfo src = new VideoInfo();
                  src.Width = 3040;
                  src.Height = 1520;
                  src.Format = 25;

                  VideoInfo des = new VideoInfo();
                  des.Width = 1000;
                  des.Height = 1000;
                  des.Format = 25;
                  VideoConvert convert = new VideoConvert();
                  int ret = convert.Init(src,des);
                  if(ret != 1) return;
                  String name = "0_frame.txt";
                  File frame = new File(MediaController.defaultDir, "/" + name);
                  byte[] data = MediaController.readFrame(frame);
                  byte[] baRet = convert.Convert(data,data.length);
                  if(baRet != null)
                  {
                      name = "output.yuv";
                      frame = new File(MediaController.defaultDir, "/" + name);
                      MediaController.writeFrame(frame,baRet);
                      tv.setText("转换成功");
                  }else{
                      tv.setText("转换失败");
                  }
              }
        });
        // ATTENTION: This was auto-generated to implement the App Indexing API.
        // See https://g.co/AppIndexing/AndroidStudio for more information.
        client = new GoogleApiClient.Builder(this).addApi(AppIndex.API).build();
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();

    public native void brokenFromJNI();
    /**
     * ATTENTION: This was auto-generated to implement the App Indexing API.
     * See https://g.co/AppIndexing/AndroidStudio for more information.
     */
    public Action getIndexApiAction() {
        Thing object = new Thing.Builder()
                .setName("Main Page") // TODO: Define a title for the content shown.
                // TODO: Make sure this auto-generated URL is correct.
                .setUrl(Uri.parse("http://[ENTER-YOUR-URL-HERE]"))
                .build();
        return new Action.Builder(Action.TYPE_VIEW)
                .setObject(object)
                .setActionStatus(Action.STATUS_TYPE_COMPLETED)
                .build();
    }

    @Override
    public void onStart() {
        super.onStart();

        // ATTENTION: This was auto-generated to implement the App Indexing API.
        // See https://g.co/AppIndexing/AndroidStudio for more information.
        client.connect();
        AppIndex.AppIndexApi.start(client, getIndexApiAction());
    }

    @Override
    public void onStop() {
        super.onStop();

        // ATTENTION: This was auto-generated to implement the App Indexing API.
        // See https://g.co/AppIndexing/AndroidStudio for more information.
        AppIndex.AppIndexApi.end(client, getIndexApiAction());
        client.disconnect();
    }
}
