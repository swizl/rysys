/*
 * filter_as.c - wrapper around the Cygwin GNU assembler (native Windows build).
 *
 * clang emits the LLVM-only pseudo-ops ".seh_startepilogue" and
 * ".seh_endepilogue" when generating assembly for x86_64 SEH targets.
 * GNU as does not know them and aborts.  They only mark epilogue
 * boundaries for unwinding inside epilogues; the real unwind info comes
 * from .seh_pushreg/.seh_setframe/.seh_stackalloc, so dropping these
 * lines keeps the objects valid.
 *
 * GNU as also rejects the two-argument ".p2align N, 0x0" form, so the
 * fill byte is dropped (alignment/padding stay identical).
 *
 * Invoked by clang as the program named "as" (see -B <this dir>).
 *
 * 本版本全部使用宽字符 API：构建目录含中文（如 D:\src\rysys\构建）时，
 * ANSI 版的 fopen/_spawnv 会把路径按当前代码页拆坏，宽字符版不会。
 * 编译时需要 -municode 以使用 wmain 入口。
 */
#define _UNICODE
#define UNICODE
#define __USE_MINGW_ANSI_STDIO 1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <process.h>
#include <windows.h>

#define REAL_AS L"C:\\tools\\msys64\\usr\\bin\\as.exe"

static int
is_skip (const wchar_t *line)
{
  const wchar_t *p = line;
  while (*p == L' ' || *p == L'\t')
    p++;
  if (!wcsncmp (p, L".seh_startepilogue", 18))
    return 1;
  if (!wcsncmp (p, L".seh_endepilogue", 16))
    return 1;
  return 0;
}

static void
write_line (FILE *g, const wchar_t *line)
{
  const wchar_t *p = line;
  while (*p == L' ' || *p == L'\t')
    p++;
  if (!wcsncmp (p, L".section", 8))
    {
      /* LLVM COFF: .section .text$sym,"xr",discard,sym
	 GNU gas:   .section .text$sym,"xr"  +  .linkonce discard  */
      const wchar_t *d = wcsstr (p, L",discard");
      if (d)
	{
	  fwprintf (g, L"%.*ls\n", (int) (d - line), line);
	  fputws (L"\t.linkonce\tdiscard\n", g);
	  return;
	}
      /* ".ctors,"dw",unique,0": gas has no "unique" selector; plain
	 .ctors merging is done by the linker.  */
      const wchar_t *u = wcsstr (p, L",unique");
      if (u)
	{
	  fwprintf (g, L"%.*ls\n", (int) (u - line), line);
	  return;
	}
    }
  if (!wcsncmp (p, L".p2align", 8))
    {
      const wchar_t *comma = wcschr (p, L',');
      if (comma && wcsstr (comma, L"0x0"))
	{
	  fwprintf (g, L"%.*ls\n", (int) (comma - line), line);
	  return;
	}
    }
  fputws (line, g);
}

int
wmain (int argc, wchar_t **argv)
{
  const wchar_t *in = NULL;
  size_t inlen = 0;
  for (int i = 1; i < argc; i++)
    if (argv[i][0] != L'-'
	&& (inlen = wcslen (argv[i])) >= 2
	&& !wcscmp (argv[i] + inlen - 2, L".s"))
      in = argv[i];
  if (!in)
    {
      fputs ("filter_as: no input file\n", stderr);
      return 1;
    }

  wchar_t tmpdir[MAX_PATH];
  if (GetTempPathW (MAX_PATH, tmpdir) == 0)
    wcscpy (tmpdir, L"C:\\Windows\\Temp\\");
  wchar_t tmp[MAX_PATH];
  swprintf (tmp, MAX_PATH, L"%lsfilter_as_%lu.s", tmpdir,
	    (unsigned long) GetCurrentProcessId ());

  FILE *f = _wfopen (in, L"r");
  if (!f)
    {
      fwprintf (stderr, L"filter_as: cannot open input %ls\n", in);
      return 1;
    }
  FILE *g = _wfopen (tmp, L"w");
  if (!g)
    {
      fwprintf (stderr, L"filter_as: cannot create temp %ls\n", tmp);
      fclose (f);
      return 1;
    }

  wchar_t *line = (wchar_t *) malloc ((1 << 20) * sizeof (wchar_t));
  if (!line)
    return 1;
  while (fgetws (line, 1 << 20, f))
    if (!is_skip (line))
      write_line (g, line);
  free (line);
  fclose (f);
  fclose (g);

  wchar_t **nav = (wchar_t **) calloc (argc + 1, sizeof (wchar_t *));
  for (int i = 0; i < argc; i++)
    nav[i] = (argv[i] == in) ? tmp : argv[i];

  intptr_t rc = _wspawnv (_P_WAIT, REAL_AS, (const wchar_t *const *) nav);
  if (rc == -1)
    {
      fputs ("filter_as: spawn as failed\n", stderr);
      return 1;
    }
  if (rc == 0)
    DeleteFileW (tmp);
  else
    fwprintf (stderr, L"filter_as: kept filtered input at %ls\n", tmp);
  return (int) rc;
}
