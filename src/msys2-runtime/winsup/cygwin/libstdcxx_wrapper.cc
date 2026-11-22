/* libstdcxx_wrapper.cc

This file is part of Cygwin.

This software is a copyrighted work licensed under the terms of the
Cygwin license.  Please consult the file "CYGWIN_LICENSE" for
details.  */


/* We provide these stubs to call into a user's
   provided ONDEE replacement if there is one - otherwise
   it must fall back to the standard libstdc++ version.  */

/* Clang rejects asm labels on a function that was already declared, so
   pre-declare the operators before any header pulls in <new>.  */
namespace std { struct nothrow_t; }
typedef unsigned long __cygwin_size_t;
extern void *operator new (__cygwin_size_t) noexcept (false)
			__asm__ ("__wrap__Znwm");
extern void *operator new[] (__cygwin_size_t) noexcept (false)
			__asm__ ("__wrap__Znam");
extern void operator delete (void *) noexcept (true)
			__asm__ ("__wrap__ZdlPv");
extern void operator delete[] (void *) noexcept (true)
			__asm__ ("__wrap__ZdaPv");
extern void *operator new (__cygwin_size_t, const std::nothrow_t &) noexcept (true)
			__asm__ ("__wrap__ZnwmRKSt9nothrow_t");
extern void *operator new[] (__cygwin_size_t, const std::nothrow_t &) noexcept (true)
			__asm__ ("__wrap__ZnamRKSt9nothrow_t");
extern void operator delete (void *, const std::nothrow_t &) noexcept (true)
			__asm__ ("__wrap__ZdlPvRKSt9nothrow_t");
extern void operator delete[] (void *, const std::nothrow_t &) noexcept (true)
			__asm__ ("__wrap__ZdaPvRKSt9nothrow_t");

#include "winsup.h"
#include "cygwin-cxx.h"
#include "perprocess.h"

extern void *
operator new (std::size_t sz) noexcept (false)
{
  return (*user_data->cxx_malloc->oper_new) (sz);
}

extern void *
operator new[] (std::size_t sz) noexcept (false)
{
  return (*user_data->cxx_malloc->oper_new__) (sz);
}

extern void
operator delete (void *p) noexcept (true)
{
  (*user_data->cxx_malloc->oper_delete) (p);
}

extern void
operator delete[] (void *p) noexcept (true)
{
  (*user_data->cxx_malloc->oper_delete__) (p);
}

extern void *
operator new (std::size_t sz, const std::nothrow_t &nt) noexcept (true)
{
  return (*user_data->cxx_malloc->oper_new_nt) (sz, nt);
}

extern void *
operator new[] (std::size_t sz, const std::nothrow_t &nt) noexcept (true)
{
  return (*user_data->cxx_malloc->oper_new___nt) (sz, nt);
}

extern void
operator delete (void *p, const std::nothrow_t &nt) noexcept (true)
{
  (*user_data->cxx_malloc->oper_delete_nt) (p, nt);
}

extern void
operator delete[] (void *p, const std::nothrow_t &nt) noexcept (true)
{
  (*user_data->cxx_malloc->oper_delete___nt) (p, nt);
}
