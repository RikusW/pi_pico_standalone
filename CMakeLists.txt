cmake_minimum_required(VERSION 3.12)

#---SDK---

if (DEFINED ENV{PICO_SDK_PATH})
    list(APPEND CMAKE_MODULE_PATH $ENV{PICO_SDK_PATH}/external/)
else ()
    message(FATAL_ERROR "Set PICO_SDK_PATH environment variable")
endif ()

include(pico_sdk_import RESULT_VARIABLE RV)
if (${RV} STREQUAL NOTFOUND)
    message(FATAL_ERROR "Failed to include pico_sdk_import.cmake")
else ()
    message("Found ${RV}")
endif ()

if (PICO_SDK_VERSION_STRING VERSION_LESS "1.5.1")
    message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.5.1 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

#---Extras---

if (DEFINED ENV{PICO_EXTRAS_PATH})
    list(APPEND CMAKE_MODULE_PATH $ENV{PICO_EXTRAS_PATH}/external)
else ()
    message(FATAL_ERROR "Set PICO_EXTRAS_PATH environment variable")
endif ()

message("test ${CMAKE_MODULE_PATH} test")

include(pico_extras_import RESULT_VARIABLE RV)
if (${RV} STREQUAL NOTFOUND)
    message(FATAL_ERROR "Failed to include pico_extras_import_optional.cmake")
else ()
    message("Found ${RV}")
endif ()

#TODO check version ?

#---Compiler---

#change project name here
set(project_name pi_pico_standalone)

project(${project_name} C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_compile_options(
    -Wall
    -Wno-format          # int != int32_t as far as the compiler is concerned because gcc has int32_t as long int
    -Wno-unused-function # we have some for the docs that aren't called
)

if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-Wno-maybe-uninitialized)
endif()

#---Project---

if (TARGET tinyusb_device)
    add_executable(${project_name}
        main.c
	#add more.c files here
    )

    # pull in common dependencies
    target_link_libraries(${project_name} pico_stdlib)

    # enable usb output, disable uart output
    pico_enable_stdio_usb(${project_name} 1)
    pico_enable_stdio_uart(${project_name} 0)

    # create map/bin/hex/uf2 file etc.
    pico_add_extra_outputs(${project_name})

elseif(PICO_ON_DEVICE)
    message(FATAL_ERROR "not building ${project_name} because TinyUSB submodule is not initialized in the SDK")
endif()

