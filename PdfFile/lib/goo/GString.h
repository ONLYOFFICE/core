//========================================================================
//
// GString.h
//
// Simple variable-length string type.
//
// Copyright 1996-2003 Glyph & Cog, LLC
//
//========================================================================

#ifndef GSTRING_H
#define GSTRING_H

#include <aconf.h>

#ifdef USE_GCC_PRAGMAS
#pragma interface
#endif

#include <limits.h> // for LLONG_MAX and ULLONG_MAX
#include <stdarg.h>
#include "gtypes.h"

class GString {
public:

  // Create an empty string.
  GString();

  // Create a string from a C string.
  GString(const char *sA);

  // Create a string from <lengthA> chars at <sA>.  This string
  // can contain null characters.
  GString(const char *sA, int lengthA, bool _bBinary = false);

  // Create a string from <lengthA> chars at <idx> in <str>.
  GString(GString *str, int idx, int lengthA);

  // Copy a string.
  GString(GString *str);
  GString *copy() { return new GString(this); }

  // Concatenate two strings.
  GString(GString *str1, GString *str2);

  // Convert an integer to a string.
  static GString *fromInt(int x);

  // Create a formatted string.  Similar to printf, but without the
  // string overflow issues.  Formatting elements consist of:
  //     {<arg>:[<width>][.<precision>]<type>}
  // where:
  // - <arg> is the argument number (arg 0 is the first argument
  //   following the format string) -- NB: args must be first used in
  //   order; they can be reused in any order
  // - <width> is the field width -- negative to reverse the alignment;
  //   starting with a leading zero to zero-fill (for integers)
  // - <precision> is the number of digits to the right of the decimal
  //   point (for floating point numbers)
  // - <type> is one of:
  //     d, x, o, b -- int in decimal, hex, octal, binary
  //     ud, ux, uo, ub -- unsigned int
  //     ld, lx, lo, lb, uld, ulx, ulo, ulb -- long, unsigned long
  //     lld, llx, llo, llb, ulld, ullx, ullo, ullb
  //         -- long long, unsigned long long
  //     f, g -- double
  //     c -- char
  //     s -- string (char *)
  //     t -- GString *
  //     w -- blank space; arg determines width
  // To get literal curly braces, use {{ or }}.
  static GString *format(const char *fmt, ...);
  static GString *formatv(const char *fmt, va_list argList);

  // Destructor.
  ~GString();

  // Get length.
  int getLength() { return length; }

  // Get C string.
  char *getCString() { return s; }

  // Get <i>th character.
  char getChar(int i) { return s[i]; }

  // Change <i>th character.
  void setChar(int i, char c) { s[i] = c; }

  // Clear string to zero length.
  GString *clear();

  // Append a character or string.
  GString *append(char c);
  GString *append(GString *str);
  GString *append(const char *str);
  GString *append(const char *str, int lengthA);

  // Append a formatted string.
  GString *appendf(const char *fmt, ...);
  GString *appendfv(const char *fmt, va_list argList);

  // Insert a character or string.
  GString *insert(int i, char c);
  GString *insert(int i, GString *str);
  GString *insert(int i, const char *str);
  GString *insert(int i, const char *str, int lengthA);

  // Delete a character or range of characters.
  GString *del(int i, int n = 1);

  // Convert string to all-upper/all-lower case.
  GString *upperCase();
  GString *lowerCase();

  // Compare two strings:  -1:<  0:=  +1:>
  int cmp(GString *str);
  int cmpN(GString *str, int n);
  int cmp(const char *sA);
  int cmpN(const char *sA, int n);

  bool isBinary() { return bBinary; }

private:

  bool bBinary;
  int length;
  char *s;

  void resize(int length1);
#ifdef LLONG_MAX
  static void formatInt(long long x, char *buf, int bufSize,
			GBool zeroFill, int width, int base,
			const char **p, int *len);
#else
  static void formatInt(long x, char *buf, int bufSize,
			GBool zeroFill, int width, int base,
			const char **p, int *len);
#endif
#ifdef ULLONG_MAX
  static void formatUInt(unsigned long long x, char *buf, int bufSize,
			 GBool zeroFill, int width, int base,
			 const char **p, int *len);
#else
  static void formatUInt(Gulong x, char *buf, int bufSize,
			 GBool zeroFill, int width, int base,
			 const char **p, int *len);
#endif
  static void formatDouble(double x, char *buf, int bufSize, int prec,
			   GBool trim, const char **p, int *len);
};

#endif
