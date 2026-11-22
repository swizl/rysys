/* exception.h

This software is a copyrighted work licensed under the terms of the
Cygwin license.  Please consult the file "CYGWIN_LICENSE" for
details. */

#pragma once

#define exception_list void
typedef struct _DISPATCHER_CONTEXT *PDISPATCHER_CONTEXT;

class exception
{
  static EXCEPTION_DISPOSITION myfault (EXCEPTION_RECORD *, exception_list *,
					CONTEXT *, PDISPATCHER_CONTEXT);
  static EXCEPTION_DISPOSITION handle (EXCEPTION_RECORD *, exception_list *,
				       CONTEXT *, PDISPATCHER_CONTEXT);
public:
  exception () __attribute__ ((always_inline))
  {
    /* Install SEH handler. */
#ifdef __clang__
    /* clang's integrated assembler rejects the numeric-label ".rva 1b, 2f"
       relocations and the .seh_code directive, so the handler data is
       emitted in the destructor below instead.  */
    asm volatile ("\n\
    1:									\n");
#else
    asm volatile ("\n\
    1:									\n\
      .seh_handler							  \
	_ZN9exception6handleEP17_EXCEPTION_RECORDPvP8_CONTEXTP19_DISPATCHER_CONTEXT,	  \
	@except								\n\
      .seh_handlerdata							\n\
      .long 1								\n\
      .rva 1b, 2f, 2f, 2f						\n\
      .seh_code								\n");
#endif
  };
  ~exception () __attribute__ ((always_inline))
  {
#ifdef __clang__
    /* Both labels are backward references here, so alias them to named
       local symbols that ".rva" can relocate.  */
    asm volatile ("\n\
      nop								\n\
    2:									\n\
      .set .Lcygwin_exc_beg, 1b						\n\
      .set .Lcygwin_exc_end, 2b						\n\
      .seh_handler							  \
	_ZN9exception6handleEP17_EXCEPTION_RECORDPvP8_CONTEXTP19_DISPATCHER_CONTEXT,	  \
	@except								\n\
      .seh_handlerdata							\n\
      .long 1								\n\
      .rva .Lcygwin_exc_beg, .Lcygwin_exc_end, .Lcygwin_exc_end, .Lcygwin_exc_end	\n\
      .text								\n");
#else
    asm volatile ("\n\
      nop								\n\
    2:									\n\
      nop								\n");
#endif
  }
};

LONG CALLBACK myfault_altstack_handler (EXCEPTION_POINTERS *);

class cygwin_exception
{
  PUINT_PTR framep;
  PCONTEXT ctx;
  EXCEPTION_RECORD *e;
  HANDLE h;
  void dump_exception ();
  void open_stackdumpfile ();
public:
  cygwin_exception (PUINT_PTR in_framep, PCONTEXT in_ctx = NULL, EXCEPTION_RECORD *in_e = NULL):
    framep (in_framep), ctx (in_ctx), e (in_e), h (NULL) {}
  void dumpstack ();
  PCONTEXT context () const {return ctx;}
  EXCEPTION_RECORD *exception_record () const {return e;}
};
