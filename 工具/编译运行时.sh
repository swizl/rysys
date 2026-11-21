#! /bin/bash
export MSYSTEM=MSYS
export PATH=/usr/bin:/bin
unset MAKEFLAGS
cd /d/src/rysys/src/msys2-runtime || exit 9
LOG=/d/src/rysys/工具/运行时编译.log
: > "$LOG"
echo "=== make -j4 ===" >> "$LOG"
make -j4 >> "$LOG" 2>&1
echo "MAKE_RC=$?" >> "$LOG"
