# CMake toolchain: clang cross-ish build targeting x86_64-pc-cygwin,
# using the MSYS2 installation as sysroot.
#
# Usage:
#   cmake -G Ninja -S winsup/cygwin -B build/cmake-clang \
#         -DCMAKE_TOOLCHAIN_FILE=winsup/cygwin/cmake/toolchain-clang-cygwin.cmake

if(NOT DEFINED MSYS2_ROOT)
  set(MSYS2_ROOT "C:/tools/msys64" CACHE PATH "MSYS2 installation root")
endif()

set(CMAKE_SYSTEM_NAME CYGWIN)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

set(CMAKE_C_COMPILER   "${MSYS2_ROOT}/mingw64/bin/clang.exe")
set(CMAKE_CXX_COMPILER "${MSYS2_ROOT}/mingw64/bin/clang++.exe")
set(CMAKE_ASM_COMPILER "${MSYS2_ROOT}/mingw64/bin/clang.exe")

set(CMAKE_ASM_SOURCE_FILE_EXTENSIONS s;S;asm)

set(_cygwin_target "--target=x86_64-pc-cygwin")
set(_cygwin_sysroot "--sysroot=${MSYS2_ROOT}")

set(CMAKE_C_FLAGS_INIT   "${_cygwin_target} ${_cygwin_sysroot}")
set(CMAKE_CXX_FLAGS_INIT "${_cygwin_target} ${_cygwin_sysroot}")
set(CMAKE_ASM_FLAGS_INIT "${_cygwin_target} ${_cygwin_sysroot}")

# We only need the compiler to be able to compile probes, not link them.
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE NEVER)
