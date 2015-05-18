#pragma once

typedef void (*OdfOnProgressCallback)( void* lpParam, long nID, long nPercent );
typedef void (*OdfOnProgressExCallback)( void* lpParam, long nID, long nPercent, short* Cancel );

struct ProgressCallback
{
  OdfOnProgressCallback		OnProgress;
  OdfOnProgressExCallback	OnProgressEx;
  void*					caller;
};
