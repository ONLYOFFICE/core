/* -*- C -*-
// -------------------------------------------------------------------
// MiniLock - a quick mostly user space lock 
// Copyright (c) 2008  Leon Bottou. All rights reserved
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
// ------------------------------------------------------------------- */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "../../libdjvu/atomic.h"

int
atomicAcquire(int volatile *lock)
{
  int tmp;
  if ((tmp = !*lock))
    *lock = 1;
  return tmp;
}

void
atomicAcquireOrSpin(int volatile *lock)
{
  while (*lock)
    ;
  *lock = 1;
}

void
atomicRelease(int volatile *lock)
{
  *lock = 0;
}

int
atomicIncrement(int volatile *var)
{
  int res;
  res = ++(*var);
  return res;
}

int
atomicDecrement(int volatile *var)
{
  int res;
  res = --(*var);
  return res;
}

int
atomicCompareAndSwap(int volatile *var, int oldval, int newval)
{
  int ret;
  ret = *var;
  if (ret == oldval)
    *var = newval;
  return (ret == oldval);
}
