#pragma once

#if defined(_WIN32) || defined (_WIN64)
	#include <atlwin.h>
	#include <atltypes.h>
	#include <atlcoll.h>
#endif

#include "../../Common/DocxFormat/Source/SystemUtility/File.h"

#include <algorithm>

namespace NSWMFToImageConverter
{
	class CImageExt
	{
	public:
		LONG GetImageType(CString strFile)
		{		
			CFile oFile;
			if (S_OK != oFile.OpenFile(strFile))
			{
				oFile.CloseFile();
				return 0;
			}

			DWORD dwSize = (DWORD)oFile.GetFileSize();

			if (44 > dwSize)
			{
				return 0;
			}

			DWORD dwKey = 0;
			oFile.ReadFile((BYTE*)(&dwKey), 4);

			if (0x9AC6CDD7 == dwKey)
			{
				// placeable meta
				oFile.CloseFile();
				return 1;
			}

			if (0x00000001 == dwKey)
			{
				oFile.SetPosition(40);
				oFile.ReadFile((BYTE*)(&dwKey), 4);
				oFile.CloseFile();

				if (0x464D4520 == dwKey)
				{
					// EMF/EMF+
					return 2;
				}
				else
				{
					// 
					return 0;
				}
			}

			oFile.CloseFile();

			if (0x00090001 == dwKey)
			{
				return 1;
			}

			return 0;
		}
	};
}

