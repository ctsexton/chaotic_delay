# Brian Heim
# 2018-08-26
# Functions for configuring SuperCollider server plugins

include(SuperColliderCompilerConfig)

function(sc_check_sc_path path)
    if(NOT path)
        set(sc_path_default "../supercollider")
        message(WARNING "No SC_PATH specified, defaulting to '${sc_path_default}'.")
        set(path "${sc_path_default}")
    endif()

    get_filename_component(full_path "${path}" ABSOLUTE BASE_DIR "${CMAKE_SOURCE_DIR}")

    # check main paths
    if(NOT EXISTS "${full_path}/include/plugin_interface/SC_PlugIn.h")
        set(msg_end "\nPlease set SC_PATH to the root folder of the SuperCollider project relative to the folder containing this CMakeLists.txt file")
        message(FATAL_ERROR "Could not find SuperCollider3 headers at '${full_path}'.${msg_end}")
    endif()

    set(SC_PATH ${full_path} PARENT_SCOPE)
endfunction()

set(all_sc_server_plugins)
