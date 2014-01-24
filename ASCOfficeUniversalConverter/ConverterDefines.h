#pragma once
#include "../Common/OfficeFileErrorDescription.h"


#define DOCUMENTCONVERTER_STATE_IDLE						0x0000
#define DOCUMENTCONVERTER_STATE_RUN							0x0001
#define DOCUMENTCONVERTER_STATE_SUSPENDED					0x0002

const int g_cnOfficeCacheMaxFilesCount = 100;
const int g_cnOfficeCacheMaxFilesStarted = 5;