#pragma once

typedef void (*OnProgressCallback)( LPVOID lpParam, long nID, long nPercent );
typedef void (*OnProgressExCallback)( LPVOID lpParam, long nID, long nPercent, short* Cancel );

struct ProgressCallback
{
  OnProgressCallback OnProgress;
  OnProgressExCallback OnProgressEx;
  LPVOID caller;
};
