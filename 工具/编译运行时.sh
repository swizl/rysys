#! /bin/bash
export MSYSTEM=MSYS
export PATH=/usr/bin:/bin
export LANG=zh_CN.UTF-8
unset MAKEFLAGS
cd /d/src/rysys/src/msys2-runtime || exit 9
LOG=/d/src/rysys/工具/运行时编译.log
CN=/d/src/rysys/工具/中文化输出.awk
: > "$LOG"
echo "=== 开始编译（make -j4）===" >> "$LOG"
make -j4 2>&1 | gawk -f "$CN" >> "$LOG"
echo "MAKE_RC=${PIPESTATUS[0]}" >> "$LOG"
