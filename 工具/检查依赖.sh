#!/bin/bash
{
gcc --version | head -1
echo ---cocom---
command -v cocom || echo MISSING
echo ---pacman---
pacman -Q msys2-devel base-devel autotools cocom gcc gettext-devel libiconv-devel make mingw-w64-cross-crt mingw-w64-cross-gcc mingw-w64-cross-zlib perl zlib-devel xmlto docbook-xsl libzstd-devel 2>&1
} > /d/src/rysys/工具/依赖检查.log 2>&1