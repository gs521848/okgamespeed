package com.example.shadowhook;

import android.app.Application;
import android.content.Context;
import android.util.Log;

import com.bytedance.shadowhook.ShadowHook;
import com.ssjj.shadow.JniChange;

public class App extends Application {
    public static String packageName;
    @Override
    protected void attachBaseContext(Context base) {
        super.attachBaseContext(base);
    }

    @Override
    public void onCreate() {
        super.onCreate();
        Log.e("liuweihao","shadowhook init");
        JniChange.initShadow();
        JniChange.changespped(15);
    }
}
