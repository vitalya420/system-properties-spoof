#define _GNU_SOURCE
#include <sys/system_properties.h>
#include <android/log.h>
#include <dlfcn.h>
#include <string.h>
#include <stdlib.h>

#include "props.h"

#define LOG_PREFIX "LIBFUCKINGHOOK"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_PREFIX, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_PREFIX, __VA_ARGS__)

int access(const char *pathname, int mode);
static int (* orig_access)(const char *pathname, int mode);

int __system_property_read_callback(const prop_info* pi, void (*callback)(void* cookie, const char* name, const char* value, uint32_t serial), void* cookie);
static int (*orig_system_property_read_callback)(const prop_info*, void (*)(void*, const char*, const char*, uint32_t), void*);

static void (*og_callback)(void* cookie, const char* name, const char* value, uint32_t serial) = NULL;

static void overridden_callback(void *cookie, const char *name, const char *value, uint32_t serial) {

    char *replace_value = getprop(name);

    char* updated = (char *)malloc(strlen(value) * sizeof(char) + 1);
    strcpy(updated, value);
    
    if (replace_value != NULL) {
        free(updated);
        updated = (char *)malloc(strlen(replace_value) * sizeof(char) + 1);
        LOGI("Replacing property %s with value: %s", name, replace_value);
        strcpy(updated, replace_value);
    }else{
        LOGI("Intercepted property: %s, Original: %s", name ? name : "NULL", value ? value : "NULL");
    }

    if (og_callback != NULL) {
        og_callback(cookie, name, updated, serial);
    }
    free(updated);
}


int access(const char *pathname, int mode){
    if(!orig_access){
        orig_access = dlsym(RTLD_NEXT, "access");
    }
    if (strcmp(pathname, "/system/bin/su") == 0){
        return -1;
    }
    int result = orig_access(pathname, mode);
    LOGI("Access to path: %s, result: %d", pathname, result);
    return result;
}

int __system_property_read_callback(const prop_info* pi, void (*callback)(void* cookie, const char* name, const char* value, uint32_t serial), void* cookie) {
    if (orig_system_property_read_callback) {
        og_callback = callback;
        return orig_system_property_read_callback(pi, overridden_callback, cookie);
    }

    return 0;
}

__attribute__((constructor))
void setup_hooks() {
    LOGI("Started.");
    void* handle = dlopen("libc.so", RTLD_NOW);
    if (!handle) {
        LOGI("Failed to open libc.so");
        return;
    }

    orig_system_property_read_callback = dlsym(handle, "__system_property_read_callback");


    if (!orig_system_property_read_callback) {
        LOGI("Failed to find __system_property_read_callback");
    } else {
        LOGI("Hooked __system_property_read_callback successfully!");
    }

    dlclose(handle);
}

__attribute__((destructor))
void libhook_fini(void) {
    cleanup_props();
    LOGI("Library unloaded! Cleaning up.");
}

//   setprop wrap.com.google.android.gms LD_PRELOAD=/data/data/com.google.android.gms/lib/libhook.so
//   setprop wrap.com.thend.integritychecker LD_PRELOAD=/data/data/com.thend.integritychecker/lib/libhook.so
