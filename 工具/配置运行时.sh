#!/bin/bash
export MSYSTEM=MSYS
export PATH=/usr/bin:/bin
export LANG=zh_CN.UTF-8
unset MAKEFLAGS
cd /d/src/rysys/src/msys2-runtime || exit 9
LOG=/d/src/rysys/工具/运行时配置.log
CN=/d/src/rysys/工具/中文化输出.awk
: > "$LOG"
./configure --host=x86_64-pc-cygwin --target=x86_64-pc-cygwin --cache-file=/dev/null --disable-dependency-tracking --with-cross-bootstrap=yes 2>&1 | gawk -f "$CN" >> "$LOG"
echo "CONFIGURE_RC=${PIPESTATUS[0]}" >> "$LOG"
grep '^S\["target_configdirs"\]' config.status >> "$LOG" 2>&1
