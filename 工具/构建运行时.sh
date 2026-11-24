#!/bin/bash
# 构建 msys2-runtime（MSYS 子系统）
set -o pipefail
export MSYSTEM=MSYS
export PATH=/usr/bin:/bin
export LANG=zh_CN.UTF-8
unset MAKEFLAGS
cd /d/src/rysys/src/msys2-runtime || exit 9
LOG=/d/src/rysys/工具/运行时构建.log
CN=/d/src/rysys/工具/中文化输出.awk
: > "$LOG"
echo "=== 生成构建脚本（autogen）===" >> "$LOG"
( cd winsup && ./autogen.sh ) 2>&1 | gawk -f "$CN" >> "$LOG" || { echo "AUTOGEN_FAIL" >> "$LOG"; exit 1; }
echo "=== 配置（configure）===" >> "$LOG"
./configure --disable-dependency-tracking 2>&1 | gawk -f "$CN" >> "$LOG" || { echo "CONFIGURE_FAIL" >> "$LOG"; exit 2; }
echo "=== CONFIGURE_OK ===" >> "$LOG"
