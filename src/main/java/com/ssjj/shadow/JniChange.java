package com.ssjj.shadow;

import com.bytedance.shadowhook.ShadowHook;
import com.example.shadowhook.App;

import java.sql.Time;

public class JniChange {
    public static native void changespped(float speed);

    public static void initShadow(){

        ShadowHook.init(new ShadowHook.ConfigBuilder()
                .setMode(ShadowHook.Mode.UNIQUE)
                .build());

        System.loadLibrary("shook");
    }
}
