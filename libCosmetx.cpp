#include 
#include 
#include 
#include 

#define LOG_TAG "CosmetxLib"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

typedef void* (*il2cpp_domain_get_t)();
typedef void** (*il2cpp_domain_get_assemblies_t)(void*, size_t*);
typedef void* (*il2cpp_assembly_get_image_t)(void*);
typedef const char* (*il2cpp_image_get_name_t)(void*);
typedef void* (*il2cpp_class_from_name_t)(void*, const char*, const char*);
typedef void* (*il2cpp_class_get_field_from_name_t)(void*, const char*);
typedef void (*il2cpp_field_static_get_value_t)(void*, void*);
typedef void* (*il2cpp_class_get_method_from_name_t)(void*, const char*, int);
typedef void* (*il2cpp_runtime_invoke_t)(void*, void*, void**, void**);

namespace Cosmetx {
    void ExecuteMod() {
        void* handle = dlopen("libil2cpp.so", RTLD_NOLOAD);
        if (!handle) return;

        auto domain_get = (il2cpp_domain_get_t)dlsym(handle, "il2cpp_domain_get");
        auto get_assemblies = (il2cpp_domain_get_assemblies_t)dlsym(handle, "il2cpp_domain_get_assemblies");
        auto assembly_get_image = (il2cpp_assembly_get_image_t)dlsym(handle, "il2cpp_assembly_get_image");
        auto get_image_name = (il2cpp_image_get_name_t)dlsym(handle, "il2cpp_image_get_name");
        auto class_from_name = (il2cpp_class_from_name_t)dlsym(handle, "il2cpp_class_from_name");
        auto get_field = (il2cpp_class_get_field_from_name_t)dlsym(handle, "il2cpp_class_get_field_from_name");
        auto static_get_value = (il2cpp_field_static_get_value_t)dlsym(handle, "il2cpp_field_static_get_value");
        auto get_method = (il2cpp_class_get_method_from_name_t)dlsym(handle, "il2cpp_class_get_method_from_name");
        auto invoke = (il2cpp_runtime_invoke_t)dlsym(handle, "il2cpp_runtime_invoke");

        if (!domain_get || !get_assemblies) return;

        void* domain = domain_get();
        size_t size = 0;
        void** assemblies = get_assemblies(domain, &size);
        void* img = nullptr;

        for (size_t i = 0; i < size; i++) {
            void* current_image = assembly_get_image(assemblies[i]);
            const char* name = get_image_name(current_image);
            if (name && std::string(name) == "Assembly-CSharp.dll") {
                img = current_image;
                break;
            }
        }

        if (!img) return;

        void* klass = class_from_name(img, "", "CosmeticsController");
        if (!klass) return;

        void* instanceField = get_field(klass, "instance");
        if (!instanceField) return;

        void* instance = nullptr;
        static_get_value(instanceField, &instance);
        if (!instance) return;

        void* catalogMethod = get_method(klass, "GetCosmeticsPlayFabCatalogData", 0);
        if (catalogMethod) {
            invoke(catalogMethod, instance, nullptr, nullptr);
        }

        void* hideMethod = get_method(klass, "SetHideCosmeticsFromRemotePlayers", 1);
        if (hideMethod) {
            bool hideValue = true;
            void* args[1] = { &hideValue };
            invoke(hideMethod, instance, args, nullptr);
        }

        LOGI("Cosmetx native functions executed successfully");
    }
}

__attribute__((constructor)) void load() {
    Cosmetx::ExecuteMod();
}
