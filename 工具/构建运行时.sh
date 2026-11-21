#!/bin/bash
# 构建 msys2-runtime（MSYS 子系统）
set -o pipefail
export MSYSTEM=MSYS
export PATH=/usr/bin:/bin
unset MAKEFLAGS
cd /d/src/rysys/src/msys2-runtime || exit 9
LOG=/d/src/rysys/工具/运行时构建.log
: > "$LOG"
echo "=== autogen ===" >> "$LOG"
( cd winsup && ./autogen.sh ) >> "$LOG" 2>&1 || { echo "AUTOGEN_FAIL" >> "$LOG"; exit 1; }
echo "=== configure ===" >> "$LOG"
./configure --disable-dependency-tracking >> "$LOG" 2>&1 || { echo "CONFIGURE_FAIL" >> "$LOG"; exit 2; }
echo "=== CONFIGURE_OK ===" >> "$LOG"