/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */
#include "MediaFileFormatChecker.h"

#include "../DesktopEditor/common/File.h"

#include <string.h>

#define MIN_SIZE_BUFFER 4096
#define MAX_SIZE_BUFFER 102400

// ASF (WMA/WMV) header object GUID: 30 26 B2 75 8E 66 CF 11 A6 D9 00 AA 00 62 CE 6C
static const BYTE g_arAsfHeaderGuid[16] =
{
	0x30, 0x26, 0xB2, 0x75, 0x8E, 0x66, 0xCF, 0x11, 0xA6, 0xD9, 0x00, 0xAA, 0x00, 0x62, 0xCE, 0x6C
};
// ASF audio media type GUID: F8699E40 5B4D 11CF A8FD 00805F5C442B
static const BYTE g_arAsfAudioMediaGuid[16] =
{
	0x40, 0x9E, 0x69, 0xF8, 0x4D, 0x5B, 0xCF, 0x11, 0xA8, 0xFD, 0x00, 0x80, 0x5F, 0x5C, 0x44, 0x2B
};
// ASF video media type GUID: BC19EFC0 5B4D 11CF A8FD 00805F5C442B
static const BYTE g_arAsfVideoMediaGuid[16] =
{
	0xC0, 0xEF, 0x19, 0xBC, 0x4D, 0x5B, 0xCF, 0x11, 0xA8, 0xFD, 0x00, 0x80, 0x5F, 0x5C, 0x44, 0x2B
};

// search a 16-byte GUID inside the buffer
static bool FindGuid(BYTE* pBuffer, DWORD dwBytes, const BYTE* pGuid)
{
	if (dwBytes < 16)
		return false;

	for (DWORD i = 0; i <= dwBytes - 16; ++i)
	{
		if (0 == memcmp(pBuffer + i, pGuid, 16))
			return true;
	}
	return false;
}

CMediaFileFormatChecker::CMediaFileFormatChecker()
{
	eFileType = _MEDIA_FORMAT_UNKNOWN;
}
CMediaFileFormatChecker::CMediaFileFormatChecker(const std::wstring& sFileName)
{
	eFileType = _MEDIA_FORMAT_UNKNOWN;
	if (false == isAudioFile(sFileName))
	{
		isVideoFile(sFileName);
	}
}
CMediaFileFormatChecker::CMediaFileFormatChecker(BYTE* pBuffer, DWORD dwBytes)
{
	eFileType = _MEDIA_FORMAT_UNKNOWN;
	if (false == isAudioFile(pBuffer, dwBytes))
	{
		isVideoFile(pBuffer, dwBytes);
	}
}

//wav (RIFF ....  WAVE) - Hex: 52 49 46 46 xx xx xx xx 57 41 56 45
bool CMediaFileFormatChecker::isWavFile(BYTE* pBuffer, DWORD dwBytes)
{
	if (eFileType)
		return false;

	if ( (12 <= dwBytes) && (0x52 == pBuffer[0]) && (0x49 == pBuffer[1]) && (0x46 == pBuffer[2]) && (0x46 == pBuffer[3])
		 && (0x57 == pBuffer[8]) && (0x41 == pBuffer[9]) && (0x56 == pBuffer[10]) && (0x45 == pBuffer[11]) )
		return true;

	return false;
}
//avi (RIFF .... AVI ) - Hex: 52 49 46 46 xx xx xx xx 41 56 49 20
bool CMediaFileFormatChecker::isAviFile(BYTE* pBuffer, DWORD dwBytes)
{
	if (eFileType)
		return false;

	if ( (12 <= dwBytes) && (0x52 == pBuffer[0]) && (0x49 == pBuffer[1]) && (0x46 == pBuffer[2]) && (0x46 == pBuffer[3])
		 && (0x41 == pBuffer[8]) && (0x56 == pBuffer[9]) && (0x49 == pBuffer[10]) && (0x20 == pBuffer[11]) )
		return true;

	return false;
}
//mp4 (ISO base media) - offset 4: 'ftyp', brand at offset 8 (isom, iso2, mp41, mp42, avc1, dash, mmp4, M4V ...)
bool CMediaFileFormatChecker::isMp4File(BYTE* pBuffer, DWORD dwBytes)
{
	if (eFileType)
		return false;

	if ( (12 <= dwBytes) && (0x66 == pBuffer[4]) && (0x74 == pBuffer[5]) && (0x79 == pBuffer[6]) && (0x70 == pBuffer[7]) )
	{
		// mov is a separate ISO base media format ('qt  ' brand) - see isMovFile
		if (0 == memcmp(pBuffer + 8, "qt  ", 4))
			return false;

		return true;
	}

	return false;
}
//mov (QuickTime) - offset 4: 'ftyp' with 'qt  ' brand, or a top level atom (moov, mdat, free, wide, pnot, skip)
bool CMediaFileFormatChecker::isMovFile(BYTE* pBuffer, DWORD dwBytes)
{
	if (eFileType)
		return false;

	if ( (12 <= dwBytes) && (0x66 == pBuffer[4]) && (0x74 == pBuffer[5]) && (0x79 == pBuffer[6]) && (0x70 == pBuffer[7])
		 && 0 == memcmp(pBuffer + 8, "qt  ", 4) )
		return true;

	if ( (8 <= dwBytes) && ( 0 == memcmp(pBuffer + 4, "moov", 4) || 0 == memcmp(pBuffer + 4, "mdat", 4) ||
							 0 == memcmp(pBuffer + 4, "free", 4) || 0 == memcmp(pBuffer + 4, "wide", 4) ||
							 0 == memcmp(pBuffer + 4, "pnot", 4) || 0 == memcmp(pBuffer + 4, "skip", 4) ) )
		return true;

	return false;
}
//wma (ASF container with an audio stream)
bool CMediaFileFormatChecker::isWmaFile(BYTE* pBuffer, DWORD dwBytes)
{
	if (eFileType)
		return false;

	if ( (16 <= dwBytes) && 0 == memcmp(pBuffer, g_arAsfHeaderGuid, 16) )
	{
		// an audio only ASF file has an audio stream and no video stream
		if (FindGuid(pBuffer, dwBytes, g_arAsfAudioMediaGuid) &&
			!FindGuid(pBuffer, dwBytes, g_arAsfVideoMediaGuid))
			return true;
	}

	return false;
}
//wmv (ASF container with a video stream)
bool CMediaFileFormatChecker::isWmvFile(BYTE* pBuffer, DWORD dwBytes)
{
	if (eFileType)
		return false;

	if ( (16 <= dwBytes) && 0 == memcmp(pBuffer, g_arAsfHeaderGuid, 16) )
	{
		if (FindGuid(pBuffer, dwBytes, g_arAsfVideoMediaGuid))
			return true;
	}

	return false;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CMediaFileFormatChecker::isAudioFile(BYTE* pBuffer, DWORD dwBytes)
{
	eFileType = _MEDIA_FORMAT_UNKNOWN;

	if (!pBuffer || dwBytes <= 0)
		return false;

	if (isWavFile(pBuffer, dwBytes))
	{
		eFileType = _MEDIA_FORMAT_WAV;
	}
	else if (isWmaFile(pBuffer, dwBytes))
	{
		eFileType = _MEDIA_FORMAT_WMA;
	}

	if (eFileType) return true;
	return false;
}
bool CMediaFileFormatChecker::isVideoFile(BYTE* pBuffer, DWORD dwBytes)
{
	eFileType = _MEDIA_FORMAT_UNKNOWN;

	if (!pBuffer || dwBytes <= 0)
		return false;

	if (isAviFile(pBuffer, dwBytes))
	{
		eFileType = _MEDIA_FORMAT_AVI;
	}
	else if (isMovFile(pBuffer, dwBytes))
	{
		eFileType = _MEDIA_FORMAT_MOV;
	}
	else if (isMp4File(pBuffer, dwBytes))
	{
		eFileType = _MEDIA_FORMAT_MP4;
	}
	else if (isWmvFile(pBuffer, dwBytes))
	{
		eFileType = _MEDIA_FORMAT_WMV;
	}

	if (eFileType) return true;
	return false;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static bool ReadFileHeader(const std::wstring& fileName, BYTE* buffer, DWORD bufferSize, DWORD& sizeRead)
{
	sizeRead = 0;

	NSFile::CFileBinary file;
	if (!file.OpenFile(fileName))
		return false;

	bool bResult = file.ReadFile(buffer, bufferSize, sizeRead);
	file.CloseFile();

	return bResult;
}

bool CMediaFileFormatChecker::isAudioFile(const std::wstring& fileName)
{
	eFileType = _MEDIA_FORMAT_UNKNOWN;

	BYTE* buffer = new BYTE[MIN_SIZE_BUFFER];
	if (!buffer)
		return false;

	DWORD sizeRead = 0;
	if (!ReadFileHeader(fileName, buffer, MIN_SIZE_BUFFER, sizeRead))
	{
		delete [] buffer;
		return false;
	}

	bool bResult = isAudioFile(buffer, sizeRead);

	delete [] buffer;
	return bResult;
}
bool CMediaFileFormatChecker::isVideoFile(const std::wstring& fileName)
{
	eFileType = _MEDIA_FORMAT_UNKNOWN;

	BYTE* buffer = new BYTE[MIN_SIZE_BUFFER];
	if (!buffer)
		return false;

	DWORD sizeRead = 0;
	if (!ReadFileHeader(fileName, buffer, MIN_SIZE_BUFFER, sizeRead))
	{
		delete [] buffer;
		return false;
	}

	bool bResult = isVideoFile(buffer, sizeRead);

	delete [] buffer;
	return bResult;
}

bool CMediaFileFormatChecker::isWavFile(const std::wstring& fileName)
{
	eFileType = _MEDIA_FORMAT_UNKNOWN;

	BYTE* buffer = new BYTE[MIN_SIZE_BUFFER];
	if (!buffer)
		return false;

	DWORD sizeRead = 0;
	if (!ReadFileHeader(fileName, buffer, MIN_SIZE_BUFFER, sizeRead))
	{
		delete [] buffer;
		return false;
	}

	if (isWavFile(buffer, sizeRead))
		eFileType = _MEDIA_FORMAT_WAV;

	delete [] buffer;

	if (eFileType) return true;
	return false;
}
bool CMediaFileFormatChecker::isAviFile(const std::wstring& fileName)
{
	eFileType = _MEDIA_FORMAT_UNKNOWN;

	BYTE* buffer = new BYTE[MIN_SIZE_BUFFER];
	if (!buffer)
		return false;

	DWORD sizeRead = 0;
	if (!ReadFileHeader(fileName, buffer, MIN_SIZE_BUFFER, sizeRead))
	{
		delete [] buffer;
		return false;
	}

	if (isAviFile(buffer, sizeRead))
		eFileType = _MEDIA_FORMAT_AVI;

	delete [] buffer;

	if (eFileType) return true;
	return false;
}
bool CMediaFileFormatChecker::isMp4File(const std::wstring& fileName)
{
	eFileType = _MEDIA_FORMAT_UNKNOWN;

	BYTE* buffer = new BYTE[MIN_SIZE_BUFFER];
	if (!buffer)
		return false;

	DWORD sizeRead = 0;
	if (!ReadFileHeader(fileName, buffer, MIN_SIZE_BUFFER, sizeRead))
	{
		delete [] buffer;
		return false;
	}

	if (isMp4File(buffer, sizeRead))
		eFileType = _MEDIA_FORMAT_MP4;

	delete [] buffer;

	if (eFileType) return true;
	return false;
}
bool CMediaFileFormatChecker::isMovFile(const std::wstring& fileName)
{
	eFileType = _MEDIA_FORMAT_UNKNOWN;

	BYTE* buffer = new BYTE[MIN_SIZE_BUFFER];
	if (!buffer)
		return false;

	DWORD sizeRead = 0;
	if (!ReadFileHeader(fileName, buffer, MIN_SIZE_BUFFER, sizeRead))
	{
		delete [] buffer;
		return false;
	}

	if (isMovFile(buffer, sizeRead))
		eFileType = _MEDIA_FORMAT_MOV;

	delete [] buffer;

	if (eFileType) return true;
	return false;
}
bool CMediaFileFormatChecker::isWmaFile(const std::wstring& fileName)
{
	eFileType = _MEDIA_FORMAT_UNKNOWN;

	BYTE* buffer = new BYTE[MIN_SIZE_BUFFER];
	if (!buffer)
		return false;

	DWORD sizeRead = 0;
	if (!ReadFileHeader(fileName, buffer, MIN_SIZE_BUFFER, sizeRead))
	{
		delete [] buffer;
		return false;
	}

	if (isWmaFile(buffer, sizeRead))
		eFileType = _MEDIA_FORMAT_WMA;

	delete [] buffer;

	if (eFileType) return true;
	return false;
}
bool CMediaFileFormatChecker::isWmvFile(const std::wstring& fileName)
{
	eFileType = _MEDIA_FORMAT_UNKNOWN;

	BYTE* buffer = new BYTE[MIN_SIZE_BUFFER];
	if (!buffer)
		return false;

	DWORD sizeRead = 0;
	if (!ReadFileHeader(fileName, buffer, MIN_SIZE_BUFFER, sizeRead))
	{
		delete [] buffer;
		return false;
	}

	if (isWmvFile(buffer, sizeRead))
		eFileType = _MEDIA_FORMAT_WMV;

	delete [] buffer;

	if (eFileType) return true;
	return false;
}

std::wstring CMediaFileFormatChecker::DetectFormatByData(BYTE *Data, int DataSize)
{
	if (isWavFile(Data, DataSize)) return L"wav";
	else if (isAviFile(Data, DataSize)) return L"avi";
	else if (isMovFile(Data, DataSize)) return L"mov";
	else if (isMp4File(Data, DataSize)) return L"mp4";
	else if (isWmaFile(Data, DataSize)) return L"wma";
	else if (isWmvFile(Data, DataSize)) return L"wmv";

	return L"";
}
