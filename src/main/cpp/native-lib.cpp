#include <jni.h>
#include <string>

#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, "liuweihao", __VA_ARGS__)
using namespace std;
string getJNIString(){
    string  str ="Hello from C++";
    return str;
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_shadowhook_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {

    string hello = getJNIString();
    return env->NewStringUTF(hello.c_str());
}