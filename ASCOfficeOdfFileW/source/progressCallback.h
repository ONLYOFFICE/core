#pragma once

typedef void (*OdfOnProgressCallback)( LPVOID lpParam, long nID, long nPercent );
typedef void (*OdfOnProgressExCallback)( LPVOID lpParam, long nID, long nPercent, short* Cancel );

struct ProgressCallback
{
  OdfOnProgressCallback		OnProgress;
  OdfOnProgressExCallback	OnProgressEx;
  LPVOID					caller;
};
