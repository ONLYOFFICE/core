#ifndef OFFICE_UTILS_COMMON_H
#define OFFICE_UTILS_COMMON_H

#include <string>
#include "../../DesktopEditor/common/Types.h"
static const long UTILS_ONPROGRESSEVENT_ID = 0;

typedef void (*OnProgressCallback)( long nID, long nPercent, short* Cancel );
typedef void (*ExtractedFileCallback)( const std::wstring& file_name, BYTE* pData, const long& nSize, void* pParam );
typedef bool (*RequestFileCallback)( std::wstring& file_name, BYTE*& pData, long& nSize, void* pParam );


#endif