# 把 msys2-runtime 构建输出中的英文提示翻译为中文
# 用法：make -j4 2>&1 | gawk -f 中文化输出.awk
# 只翻译可枚举的固定消息族；命令回显、源码路径、编译器诊断保持原样。
BEGIN {
  # automake 静默规则标签
  tag["CC"] = "编译"
  tag["CXX"] = "编译 C++"
  tag["CCAS"] = "汇编"
  tag["CPPAS"] = "预处理汇编"
  tag["CPP"] = "预处理"
  tag["GEN"] = "生成"
  tag["AR"] = "归档"
  tag["RANLIB"] = "建索引"
  tag["LD"] = "链接"
  tag["RM"] = "删除"
  tag["LN"] = "链接"
  tag["MKDIR"] = "建目录"
  tag["INSTALL"] = "安装"
  tag["MAKEINFO"] = "生成文档"
  tag["AS"] = "汇编"
  tag["FC"] = "编译 Fortran"
}

{
  line = $0

  # 本工程脚本自己写入的阶段标记
  if (line ~ /^=== make .* ===$/) {
    sub(/^=== make /, "=== 开始编译（make ", line)
    sub(/ ===$/, "）===", line)
    print line
    next
  }
  if (line == "=== autogen ===") {
    print "=== 生成构建脚本（autogen）==="
    next
  }
  if (line == "=== configure ===") {
    print "=== 配置（configure）==="
    next
  }
  if (line == "=== CONFIGURE_OK ===") {
    print "=== 配置完成 ==="
    next
  }
  if (line == "=== CONFIGURE_FAIL ===") {
    print "=== 配置失败 ==="
    next
  }
  if (line == "=== AUTOGEN_FAIL ===") {
    print "=== 构建脚本生成失败 ==="
    next
  }
  if (line ~ /^Checking multilib configuration for /) {
    sub(/^Checking multilib configuration for /, "正在检查 multilib 配置：", line)
    sub(/\.\.\.$/, "", line)
    print line
    next
  }

  # make 自身消息（未启用中文语言包时的英文版）
  if (line ~ /^make(\[[0-9]+\])?: (Entering|Leaving) directory /) {
    sub(/Entering directory /, "正在进入目录 ", line)
    sub(/Leaving directory /, "已离开目录 ", line)
    print line
    next
  }
  if (line ~ /^make(\[[0-9]+\])?: Nothing to be done for /) {
    sub(/Nothing to be done for /, "无需重新构建：", line)
    print line
    next
  }
  if (line ~ /^make(\[[0-9]+\])?: \*\*\* /) {
    sub(/No rule to make target /, "没有规则可生成目标 ", line)
    sub(/No targets specified and no makefile found/,
        "未指定目标且未找到 makefile", line)
    sub(/ Error ([0-9]+)$/, " 错误 \\1", line)
    sub(/Stop\.$/, "停止。", line)
    print line
    next
  }

  # configure / config.status 状态行
  if (line ~ /^configure: /) {
    sub(/^configure: /, "配置：", line)
    sub(/creating /, "正在创建 ", line)
    sub(/loading site script /, "加载站点脚本 ", line)
    sub(/WARNING: /, "警告：", line)
    sub(/error: /, "错误：", line)
    print line
    next
  }
  if (line ~ /^config\.status: /) {
    sub(/^config\.status: /, "配置状态：", line)
    sub(/creating /, "正在创建 ", line)
    sub(/executing /, "正在执行 ", line)
    sub(/ commands$/, " 命令", line)
    print line
    next
  }
  if (line ~ /^Configuring in /) {
    sub(/^Configuring in /, "正在配置 ", line)
    print line
    next
  }
  if (line ~ /^Adding multilib support to Makefile in /) {
    sub(/^Adding multilib support to Makefile in /,
        "正在为以下目录的 Makefile 添加 multilib 支持：", line)
    print line
    next
  }

  # autoconf 检查行
  if (line ~ /^checking /) {
    sub(/\.\.\. \(cached\) yes$/, "…… （已缓存）是", line)
    sub(/\.\.\. \(cached\) no$/, "…… （已缓存）否", line)
    sub(/\.\.\. \(cached\)/, "…… （已缓存）", line)
    sub(/\.\.\. yes$/, "…… 是", line)
    sub(/\.\.\. no, /, "…… 否，", line)
    sub(/\.\.\. no$/, "…… 否", line)
    sub(/\.\.\. done$/, "…… 完成", line)
    sub(/\.\.\. /, "…… ", line)
    sub(/^checking whether to enable /, "检查是否启用 ", line)
    sub(/^checking whether to /, "检查是否要 ", line)
    sub(/^checking to see if /, "检查是否 ", line)
    sub(/^checking whether /, "检查是否 ", line)
    sub(/^checking if /, "检查是否 ", line)
    sub(/^checking how to /, "检查如何 ", line)
    sub(/^checking for /, "检查 ", line)
    sub(/^checking /, "检查 ", line)
    print line
    next
  }

  # automake 静默规则标签行（只认已知标签，其余原样输出）
  if (match(line, /^  [A-Z][A-Z0-9_]* +/)) {
    head = substr(line, RSTART, RLENGTH)
    name = head
    gsub(/^ +| +$/, "", name)
    rest = substr(line, RLENGTH + 1)
    if (name in tag) {
      printf "  %s：%s\n", tag[name], rest
      next
    }
  }

  # 结果词兜底（也适用于已部分翻译过的旧日志）
  if (index(line, "…… ") > 0) {
    sub(/…… （已缓存） yes$/, "…… （已缓存）是", line)
    sub(/…… （已缓存） no$/, "…… （已缓存）否", line)
    sub(/…… \(cached\) yes$/, "…… （已缓存）是", line)
    sub(/…… \(cached\)/, "…… （已缓存）", line)
    sub(/…… no, /, "…… 否，", line)
    sub(/…… yes$/, "…… 是", line)
    sub(/…… no$/, "…… 否", line)
    sub(/…… done$/, "…… 完成", line)
  }
  sub(/^检查是否 to /, "检查是否要 ", line)
  sub(/^检查是否要 enable /, "检查是否启用 ", line)

  print line
}
