if(NOT TARGET shadowhook::shadowhook)
add_library(shadowhook::shadowhook SHARED IMPORTED)
set_target_properties(shadowhook::shadowhook PROPERTIES
    IMPORTED_LOCATION "C:/Users/user/.gradle/caches/transforms-4/ea394b3e00e25a81d30ac6210202619f/transformed/shadowhook-1.0.9/prefab/modules/shadowhook/libs/android.armeabi-v7a/libshadowhook.so"
    INTERFACE_INCLUDE_DIRECTORIES "C:/Users/user/.gradle/caches/transforms-4/ea394b3e00e25a81d30ac6210202619f/transformed/shadowhook-1.0.9/prefab/modules/shadowhook/include"
    INTERFACE_LINK_LIBRARIES ""
)
endif()

