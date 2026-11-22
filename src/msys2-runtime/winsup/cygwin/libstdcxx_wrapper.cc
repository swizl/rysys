/* libstdcxx_wrapper.cc

This file is part of Cygwin.

This software is a copyrighted work licensed under the terms of the
Cygwin license.  Please consult the file "CYGWIN_LICENSE" for
details.  */


/* We provide these stubs to call into a user's
   provided ONDEE replacement if there is one - otherwise
   it must fall back to the standard libstdc++ version.  */

#include "winsup.h"
#include "cygwin-cxx.h"
#include "perprocess.h"

/* We are declaring asm names for the functions we define here, as we want
   to define the wrappers in this file.  GCC links everything with wrappers
   around the standard C++ memory management operators; these are the wrappers,
   but we want the compiler to know they are the malloc operators and not have
   it think they're just any old function matching 'extern "C" _wrap_*'.  */
#define MANGLED_ZNWX			"__wrap__Znwm"
#define MANGLED_ZNAX			"__wrap__Znam"
#define MANGLED_ZNWX_NOTHROW_T		"__wrap__ZnwmRKSt9nothrow_t"
#define MANGLED_ZNAX_NOTHROW_T		"__wrap__ZnamRKSt9nothrow_t"

/* clang compatibility: asm labels are rejected after <new> declared
   the operators, so alias the wrapper names to the ABI definitions. */
asm (".globl __wrap__Znwm\n\t.set __wrap__Znwm, _Znwm\n"
     ".globl __wrap__Znam\n\t.set __wrap__Znam, _Znam\n"
     ".globl __wrap__ZdlPv\n\t.set __wrap__ZdlPv, _ZdlPv\n"
     ".globl __wrap__ZdaPv\n\t.set __wrap__ZdaPv, _ZdaPv\n"
     ".globl __wrap__ZnwmRKSt9nothrow_t\n\t.set __wrap__ZnwmRKSt9nothrow_t, _ZnwmRKSt9nothrow_t\n"
     ".globl __wrap__ZnamRKSt9nothrow_t\n\t.set __wrap__ZnamRKSt9nothrow_t, _ZnamRKSt9nothrow_t\n"
     ".globl __wrap__ZdlPvRKSt9nothrow_t\n\t.set __wrap__ZdlPvRKSt9nothrow_t, _ZdlPvRKSt9nothrow_t\n"
     ".globl __wrap__ZdaPvRKSt9nothrow_t\n\t.set __wrap__ZdaPvRKSt9nothrow_t, _ZdaPvRKSt9nothrow_t\n");
extern void *operator new(std::size_t sz) noexcept (false);
extern void *operator new[](std::size_t sz) noexcept (false);
extern void operator delete(void *p) noexcept (true);
extern void operator delete[](void *p) noexcept (true);
extern void *operator new(std::size_t sz, const std::nothrow_t &nt) noexcept (true);
extern void *operator new[](std::size_t sz, const std::nothrow_t &nt) noexcept (true);
extern void operator delete(void *p, const std::nothrow_t &nt) noexcept (true);
extern void operator delete[](void *p, const std::nothrow_t &nt) noexcept (true);

extern void *
operator new(std::size_t sz) noexcept (false)
{
  return (*user_data->cxx_malloc->oper_new) (sz);
}

extern void *
operator new[](std::size_t sz) noexcept (false)
{
  return (*user_data->cxx_malloc->oper_new__) (sz);
}

extern void
operator delete(void *p) noexcept (true)
{
  (*user_data->cxx_malloc->oper_delete) (p);
}

extern void
operator delete[](void *p) noexcept (true)
{
  (*user_data->cxx_malloc->oper_delete__) (p);
}

extern void *
operator new(std::size_t sz, const std::nothrow_t &nt) noexcept (true)
{
  return (*user_data->cxx_malloc->oper_new_nt) (sz, nt);
}

extern void *
operator new[](std::size_t sz, const std::nothrow_t &nt) noexcept (true)
{
  return (*user_data->cxx_malloc->oper_new___nt) (sz, nt);
}

extern void
operator delete(void *p, const std::nothrow_t &nt) noexcept (true)
{
  (*user_data->cxx_malloc->oper_delete_nt) (p, nt);
}

extern void
operator delete[](void *p, const std::nothrow_t &nt) noexcept (true)
{
  (*user_data->cxx_malloc->oper_delete___nt) (p, nt);
}

