#pragma once

typedef void (*OnProgressCallbackDocFile)( LPVOID lpParam, long nID, long nPercent );
typedef void (*OnProgressExCallbackDocFile)( LPVOID lpParam, long nID, long nPercent, short* Cancel );

struct ProgressCallback
{
  OnProgressCallbackDocFile OnProgress;
  OnProgressExCallbackDocFile OnProgressEx;
  LPVOID caller;
};

static const long DOC_ONPROGRESSEVENT_ID = 0;
