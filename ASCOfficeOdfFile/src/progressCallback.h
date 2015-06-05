#pragma once

typedef void (*OnProgressCallback)( void* lpParam, long nID, long nPercent );
typedef void (*OnProgressExCallback)( void* lpParam, long nID, long nPercent, short* Cancel );

struct ProgressCallback
{
  OnProgressCallback    OnProgress;
  OnProgressExCallback  OnProgressEx;
  void* caller;
};
