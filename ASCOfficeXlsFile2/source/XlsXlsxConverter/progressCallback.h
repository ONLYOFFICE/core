#pragma once

typedef void (*XlsOnProgressCallback)	( void* lpParam, long nID, long nPercent );
typedef void (*XlsOnProgressExCallback)	( void* lpParam, long nID, long nPercent, short* Cancel );

struct ProgressCallback
{
  XlsOnProgressCallback		OnProgress;
  XlsOnProgressExCallback	OnProgressEx;
  void*						caller;
};
