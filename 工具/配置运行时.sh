#!/bin/bash
export MSYSTEM=MSYS
export PATH=/usr/bin:/bin
unset MAKEFLAGS
cd /d/src/rysys/src/msys2-runtime || exit 9
LOG=/d/src/rysys/工具/运行时配置.log
: > "$LOG"
./configure --host=x86_64-pc-cygwin --target=x86_64-pc-cygwin --cache-file=/dev/null --disable-dependency-tracking --with-cross-bootstrap=yes >> "$LOG" 2>&1
echo "CONFIGURE_RC=$?" >> "$LOG"
grep '^S\["target_configdirs"\]' config.status >> "$LOG" 2>&1