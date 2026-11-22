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
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <process.h>
#include <windows.h>

#define REAL_AS "C:\\tools\\msys64\\usr\\bin\\as.exe"

static int
is_skip (const char *line)
{
  const char *p = line;
  while (*p == ' ' || *p == '\t')
    p++;
  if (!strncmp (p, ".seh_startepilogue", 18))
    return 1;
  if (!strncmp (p, ".seh_endepilogue", 16))
    return 1;
  return 0;
}

static void
write_line (FILE *g, const char *line)
{
  const char *p = line;
  while (*p == ' ' || *p == '\t')
    p++;
  if (!strncmp (p, ".section", 8))
    {
      /* LLVM COFF: .section .text$sym,"xr",discard,sym
	 GNU gas:   .section .text$sym,"xr"  +  .linkonce discard  */
      const char *d = strstr (p, ",discard");
      if (d)
	{
	  fwrite (line, 1, d - line, g);
	  fputc ('\n', g);
	  fputs ("\t.linkonce\tdiscard\n", g);
	  return;
	}
      /* ".ctors,"dw",unique,0": gas has no "unique" selector; plain
	 .ctors merging is done by the linker.  */
      const char *u = strstr (p, ",unique");
      if (u)
	{
	  fwrite (line, 1, u - line, g);
	  fputc ('\n', g);
	  return;
	}
    }
  if (!strncmp (p, ".p2align", 8))
    {
      const char *comma = strchr (p, ',');
      if (comma && strstr (comma, "0x0"))
	{
	  fwrite (line, 1, comma - line, g);
	  fputc ('\n', g);
	  return;
	}
    }
  fputs (line, g);
}

int
main (int argc, char **argv)
{
  const char *in = NULL;
  size_t inlen = 0;
  for (int i = 1; i < argc; i++)
    if (argv[i][0] != '-'
	&& (inlen = strlen (argv[i])) >= 2
	&& !strcmp (argv[i] + inlen - 2, ".s"))
      in = argv[i];
  if (!in)
    {
      fprintf (stderr, "filter_as: no input file\n");
      return 1;
    }

  const char *tmpdir = getenv ("TEMP");
  if (!tmpdir)
    tmpdir = "C:\\Windows\\Temp";
  char tmp[MAX_PATH];
  snprintf (tmp, sizeof (tmp), "%s\\filter_as_%lu.s", tmpdir,
	    (unsigned long) GetCurrentProcessId ());

  FILE *f = fopen (in, "r");
  if (!f)
    {
      perror ("filter_as: open input");
      return 1;
    }
  FILE *g = fopen (tmp, "w");
  if (!g)
    {
      perror ("filter_as: create temp");
      fclose (f);
      return 1;
    }

  char *line = malloc (1 << 20);
  if (!line)
    return 1;
  while (fgets (line, 1 << 20, f))
    if (!is_skip (line))
      write_line (g, line);
  free (line);
  fclose (f);
  fclose (g);

  char **nav = calloc (argc + 1, sizeof (char *));
  for (int i = 0; i < argc; i++)
    nav[i] = (argv[i] == in) ? tmp : argv[i];

  intptr_t rc = _spawnv (_P_WAIT, REAL_AS, (const char *const *) nav);
  if (rc == -1)
    {
      perror ("filter_as: spawn as");
      return 1;
    }
  if (rc == 0)
    DeleteFileA (tmp);
  else
    fprintf (stderr, "filter_as: kept filtered input at %s\n", tmp);
  return (int) rc;
}
