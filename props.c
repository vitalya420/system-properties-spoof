#include "props.h"
#include "hashmap.h"

const char *props[][2] = {
    {"ro.vendor.build.fingerprint", "samsung/r8qxx/r8q:11/RP1A.200720.012/G780GXXSDEXJ3:user/release-keys"},
    {"ro.build.fingerprint", "samsung/r8qxx/r8q:11/RP1A.200720.012/G780GXXSDEXJ3:user/release-keys"},
    {"ro.build.id", "TP1A.220624.014"},
    {"ro.build.display.id", "TP1A.220624.014.G780GXXSDEXJ3"},
    {"ro.product.device", "r8q"},
    {"ro.product.board", "kona"},
    {"ro.product.brand", "samsung"},
    {"ro.bootloader", "G780GXXSDEXJ3"},
    {"ro.boot.vbmeta.digest", "57813e92ce0885906c37976c74d935f79322100c54dc3568b528941e2a8e6402"},
    {"ro.boot.em.model", "SM-G780G"},
    {"ro.product.product.model", "SM-G780G"},
    {"ro.product.model", "SM-G780G"},
    {"ro.product.system.model", "SM-G780G"},
    {"ro.boot.verifiedbootstate", "green"},
    {"ro.oem.key1", "SEK"},
    {"ro.product.manufacturer", "samsung"},
    {"ro.kernel.qemu", "0"},
    {"ro.hardware", "qcom"},
    {"ro.product.name", "r8qxx"},
    {"gsm.version.baseband", "G780GXXSDEXJ3,G780GXXSDEXJ3"},
    {"ro.build.date.utc", "1728356604"},
    {"ro.build.user", "dpi"},
    {"ro.build.host", "SWDK6716"},
    {"ro.build.version.base_os", "samsung/r8qxx/r8q:13/TP1A.220624.014/G780GXXU6EWH5:user/release-keys"},
    {"ro.build.version.security_patch", "2024-10-01"},
    {"qemu.sf.lcd_density", ""},
    {"qemu.sf.lcd_density", ""},
    {"ro.zygote.disable_gl_preload", ""},
    {"debug.hwui.renderer", ""}
};

const size_t props_amount = 29;
static HashMap *prop_map = NULL;

HashMap *get_properties_map() {
    if (prop_map != NULL){
        return prop_map;
    }

    HashMap *map = create_hashmap(props_amount);
    for (size_t i = 0; i < props_amount; i++) {
        hashmap_insert(map, props[i][0], props[i][1]);
    }
    prop_map = map;
    return map;
}

char *getprop(const char *key){
    if (prop_map == NULL){
        prop_map = get_properties_map();
    }
    return hashmap_search(prop_map, key);
}

void cleanup_props(){
    free_hashmap(prop_map);
    prop_map = NULL;
}
