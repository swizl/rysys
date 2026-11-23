#ifndef M2L_MACROS_H
#define M2L_MACROS_H

#define UNUSED(x) ((void)(x))

#define STRINGIFY_A_(x) #x
#define STRINGIFY_A(x) STRINGIFY_A_(x)

#define STRINGIFY_W_(x) L ## #x
#define STRINGIFY_W(x) STRINGIFY_W_(x)

/* 启动器加载自身配置用的后缀；改名只改这里。 */
#define LAUNCHER_EXE_SUFFIX  L".exe"
#define LAUNCHER_CONF_SUFFIX L".ini"

#endif
