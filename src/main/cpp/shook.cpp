//
// Created by user on 2024/3/18.
//
#include <jni.h>
#include <string>
#include "settings.h"
#include "common.h"
using namespace std;

/*
 * 预定义修改速度参数
 * */
float g_game_speed_multiple = 1.0f; //游戏速度倍数
bool  g_is_need_modif_game_speed = true;   //是否需要修改游戏速度倍数
extern uintptr_t g_module_base_address; // 使用 uintptr_t 替换 int

// common.cpp
uintptr_t g_module_base_address = 0;            //模块基址
float g_game_self_speed = 10.0f;              //游戏自身速度
bool  g_is_need_get_game_self_speed = true;  //是否需要获得游戏自身速度
typedef void (*type_t) ();
typedef void (*pfn_update)(void* _this, float value);
pfn_update old_update = NULL;

void new_update(void* _this, float value)
{
    float new_value =value;
    if(g_game_speed_multiple > 0.0f && value > 0.0f)
    {
        new_value = value * g_game_speed_multiple;
    }
    old_update(_this,new_value);
}

void JNICALL changespped(JNIEnv* env, jclass xmyy_class, jfloat jspeed)
{
    LOGE("jspeed = %f", jspeed);
    if(jspeed == 0.0f)
    {
        g_game_speed_multiple = 0.5f;
    }
    else if(jspeed == 1.0f)
    {
        g_game_speed_multiple = 1.0f;
    }
    else
    {
        g_game_speed_multiple = jspeed;
    }
    g_is_need_modif_game_speed = true;


    /* 签名相关 */
    //new object
    //jmethodID mid_init = env->GetMethodID(xmyy_class, "<init>", "()V");
    //jobject  xmyy_object = env->NewObject(xmyy_class, mid_init);

}

jint hookcocosJS(JNIEnv* env){
    LOGE("hookcocosJS");
    if(env ==NULL){
        LOGE("env==nulllllllllllllll");
    }
    string str2="libguangyv.so";
    char* ptr_update = "_ZN7cocos2d9Scheduler6updateEf";
    starthook(str2.c_str(),
              ptr_update,
              (void*) new_update,
              (void**)&old_update);

    char* ptr_update2 ="_ZN7cocos2d11CCScheduler6updateEf";
    starthook(str2.c_str(),
              ptr_update2,
              (void*) new_update,
              (void**)&old_update);

    return 0;
}





//0x21D9C0C
//0xA6D564
//设置unity地址
#if defined(__aarch64__)
#define SET_TIMESCALE_ADDRESS static_cast<uintptr_t>(0x21D9C0C) // 用于arm64-v8a架构
#else
#define SET_TIMESCALE_ADDRESS static_cast<uintptr_t>(0x1E5AC60) // 默认为armeabi-v7a架构
#endif

typedef void (*pfn_set_timeScale)(float value);
pfn_set_timeScale old_set_timeScale = NULL;
void new_set_timeScale(float value)
{
    LOGE("new_set_timeScale: value = %f", value);
    //防止游戏运行过程中自己重置了速度导致用户设置的速度无效
    if(g_game_speed_multiple > 0.0f && value > 0.0f && g_game_speed_multiple > value)
    {
         value = g_game_speed_multiple*10;
    }
    old_set_timeScale(value);
}
typedef void* (*pfn_il2cpp_runtime_invoke)(void* param_method, void* param_obj, void** param_params, void** param_exc);
pfn_il2cpp_runtime_invoke old_il2cpp_runtime_invoke = NULL;
void* new_il2cpp_runtime_invoke(void* param_method, void* param_obj, void** param_params, void** param_exc)
{
//    LOGE("new_il2cpp_runtime_invoke");
    //获得模块基址
    if(g_module_base_address == 0)
    {
        g_module_base_address = get_module_base("libil2cpp.so");
        LOGE("g_module_base_address = %p", g_module_base_address);
        printCurrentProcessMaps();
    }
    //settimeScale函数
    if(g_module_base_address != 0 && old_set_timeScale == NULL)
    {
//        LOGE("bassddress:%p, timescaleAddr:%p", (void*)g_module_base_address, (void*)SET_TIMESCALE_ADDRESS);
        void *symbol_address = (void*)(g_module_base_address + SET_TIMESCALE_ADDRESS);
        LOGE("bassddress:%p, timescaleAddr:%p,symbol_address:%p", (void*)g_module_base_address, (void*)SET_TIMESCALE_ADDRESS,symbol_address);
//        startAddrHook(
//                symbol_address,
//                (void *)new_set_timeScale,
//                (void**)&old_set_timeScale);
        funshook=shadowhook_hook_func_addr(
                symbol_address,
                (void*)&new_set_timeScale,
                (void**)&old_set_timeScale);
        old_set_timeScale=(pfn_set_timeScale)old_set_timeScale;
        if(old_set_timeScale != NULL)
        {
            LOGE("old_set_timeScale = %p", old_set_timeScale);
        }
//        old_set_timeScale(10.0f);
    }

    return old_il2cpp_runtime_invoke(param_method, param_obj, param_params, param_exc);
}


bool judgeArm(){
#if defined(__aarch64__)
    LOGE("v8a架构=========================");
    return false;
#else
    LOGE("v7a架构=========================");
    return true;
#endif
}
jint hookUnity(JNIEnv* env){
    judgeArm();
//    starthook("libil2cpp.so",
//              "il2cpp_runtime_invoke",
//              (void*) new_il2cpp_runtime_invoke,
//              (void**)&old_il2cpp_runtime_invoke
//    );

    if(g_module_base_address == 0)
    {
        g_module_base_address = get_module_base("libil2cpp.so");
        LOGE("g_module_base_address = %p", g_module_base_address);
        printCurrentProcessMaps();
    }
    //settimeScale函数
    if(g_module_base_address != 0 && old_set_timeScale == NULL)
    {
//        LOGE("bassddress:%p, timescaleAddr:%p", (void*)g_module_base_address, (void*)SET_TIMESCALE_ADDRESS);
        void *symbol_address = (void*)(g_module_base_address + SET_TIMESCALE_ADDRESS);
        LOGE("bassddress:%p, timescaleAddr:%p,symbol_address:%p", (void*)g_module_base_address, (void*)SET_TIMESCALE_ADDRESS,symbol_address);
//        startAddrHook(
//                symbol_address,
//                (void *)new_set_timeScale,
//                (void**)&old_set_timeScale);
        funshook=shadowhook_hook_func_addr(
                symbol_address,
                (void*)&new_set_timeScale,
                (void**)&old_set_timeScale);
        old_set_timeScale=(pfn_set_timeScale)old_set_timeScale;
        if(old_set_timeScale != NULL)
        {
            LOGE("old_set_timeScale = %p", old_set_timeScale);
        }
//        old_set_timeScale(10.0f);
    }
    return 0;
}



JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved) {

    LOGE("JNI_OnLoadHOOK1.1");
    JNIEnv* env = NULL;
    // 获取JNIEnv指针
    if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK) {
        return -1; // JNI版本不支持
    }
    char* ptr_changespped = "changespped";
    char* ptr_changespped_signature = "(F)V";
    char* ptr_JNItest_class = "com/ssjj/shadow/JniChange";
    //做一个方法数组
    JNINativeMethod g_Methods[] = {
            ptr_changespped, ptr_changespped_signature, (void*)&changespped
    };
    if(env != NULL)
    {
        jclass clsMainActivity = env->FindClass(ptr_JNItest_class);
        env->RegisterNatives(clsMainActivity, g_Methods, sizeof(g_Methods) / sizeof(g_Methods[0]));
    }
    else
    {
        LOGE("env is null");
    }
//    hookcocosJS(env);
    hookUnity(env);

    return JNI_VERSION_1_6;
}