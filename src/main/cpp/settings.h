//
// Created by user on 2024/3/18.
//

#ifndef SHADOWHOOK_SETINGS_H
#define SHADOWHOOK_SETINGS_H
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, "liuweihao", __VA_ARGS__)
#include <unistd.h>
#include <thread>
#include <string>
#include <android/log.h>
#include "settings.h"
#include <dlfcn.h>
#include "common.h"
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <elf.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <cctype>
#include "time.h"
#include <sstream>
#include <fstream>
#include <cstdint>


//MonoImage 结构体定义
struct _MonoImage
{
    int   ref_count;
    void* raw_data_handle;
    char* raw_data;
    int   raw_data_len;
}typedef MonoImage;

//MonoMethod 结构体定义
struct _MonoMethod {
    short  flags;
    short  iflags;
    int  token;
    void *klass;
    void *signature;
    const char *name;
    int inline_info;
}typedef MonoMethod;

//MonoMethod 结构体定义
struct MethodInfo  {
    short  flags;
    short  iflags;
    int  token;
    void *klass;
    void *signature;
    const char *name;
    int inline_info;
}typedef MethodInfo;

#endif //SHADOWHOOK_SETINGS_H
