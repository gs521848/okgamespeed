//
// Created by user on 2024/3/18.
//

#ifndef SHADOWHOOK_COMMON_H
#define SHADOWHOOK_COMMON_H

#include "shadowhook.h"
#include "settings.h"
#include <fstream> // Add this line
using namespace std;
void *shook = NULL;
// 原始的dlopen函数指针

void starthook(const char *param_target_so_path, const char *param_target_symbol, void *param_replace,
               void **param_original);


void *find_symbol(const char *param_library_name, const char *param_symbol_name);

uintptr_t get_module_base(const char* param_module_name);

uintptr_t get_module_base(const char* param_module_name)
{
    LOGE("get_module_base");
    uintptr_t ret_base = 0;
    FILE* fp = NULL;
    char map_file_path[256] = {0};
    char map_file_line[1024] = {0};

    if(param_module_name == NULL)
    {
        return ret_base;
    }

    sprintf(map_file_path, "/proc/%d/maps", getpid());
    fp = fopen(map_file_path, "r");

    if(fp != NULL)
    {
        while(fgets(map_file_line, sizeof(map_file_line), fp) != NULL)
        {
            if(strstr(map_file_line, param_module_name))
            {
                char* module_address_start = NULL;
                module_address_start = strtok(map_file_line, "-");
                if(module_address_start != NULL)
                {
                    ret_base = strtoull(module_address_start, NULL, 16); // 使用strtoull获取无符号长整型
                    if(ret_base == 0x8000)
                    {
                        ret_base = 0;
                    }
                    break;
                }
            }
        }
        fclose(fp);
    }
    return ret_base;
}
void printCurrentProcessMaps() {
    string line;
    ifstream mapsFile("/proc/self/maps");

    if (mapsFile.is_open()) {
        while (getline(mapsFile, line)) {
            LOGE("%s", line.c_str()); // Ensure we're passing a C string to LOGE
        }
        mapsFile.close();
    } else {
        LOGE("Unable to open /proc/self/maps");
    }
}

void *funshook = NULL;
void startAddrHook(void* func_addr,void* new_addr,void** orig_addr){
//    LOGE("startAddrHook:%d",func_addr);
    LOGE("startAddrHook called with sym_addr: %p, new_addr: %p, orig_addr: %p", func_addr, new_addr, *orig_addr);
    funshook=shadowhook_hook_func_addr(
            func_addr,
            new_addr,
            orig_addr);
    if(funshook == NULL)
    {
        int err_num = shadowhook_get_errno();
        const char *err_msg = shadowhook_to_errmsg(err_num);
        LOGE("hook fAddr error %d - %s", err_num, err_msg);
    }
}
void starthook(const char *param_target_so_path, const char *param_target_symbol, void *param_replace,
               void **param_original){
    LOGE("starthook:%s,func:%s",param_target_so_path,param_target_symbol);

    shook = shadowhook_hook_sym_name(
            param_target_so_path,
            param_target_symbol,
            param_replace,
            param_original);

    if(shook == NULL)
    {
        int err_num = shadowhook_get_errno();
        const char *err_msg = shadowhook_to_errmsg(err_num);
        LOGE("hook error %d - %s", err_num, err_msg);
    }



}
#endif //SHADOWHOOK_COMMON_H
