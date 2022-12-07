#ifndef GENERATESIGNATURE_H
#define GENERATESIGNATURE_H

#include <string>
#include <vector>

#include "../../../common/File.h"

namespace MetaFile
{
    #define TO_RGB(r, g, b) ((unsigned int)( ( (unsigned char)(r) )| ( ( (unsigned char)(g) ) << 8 ) | ( ( (unsigned char)(b) ) << 16 ) ) )

	class CGenerateWmfSignature
	{
	public:
		CGenerateWmfSignature()
		    : m_ushWidth(0), m_ushHeight(0), m_nBKColor(-1), m_shObjectIndex(-1)
		{}

		CGenerateWmfSignature(short shWidth, unsigned short ushHeight)
		    : m_nBKColor(-1), m_shObjectIndex(-1)
		{
			SetSize(shWidth, ushHeight);
		}

		void SetSize(unsigned short ushWidth, unsigned short ushHeight)
		{
			m_ushWidth  = ushWidth  * 1440 / 25.4;
			m_ushHeight = ushHeight * 1440 / 25.4;
		}

		void SetFont(short shFontHeight, const std::wstring& wsFontName)
		{
			m_shFontHeight = shFontHeight * 15;
			m_wsFontName   = wsFontName;
		}

		void SetBKColor(char chR, char chG, char chB)
		{
			m_nBKColor = TO_RGB(chR, chG, chB);
		}

		bool GenerateWmfSignature(const std::wstring& wsFilePath, const std::vector<std::wstring>& arStrings, bool bIsUnsigned = false)
		{
			if (wsFilePath.empty() || 0 == m_ushWidth || 0 == m_ushHeight)
				return false;

			if (!m_oFile.CreateFileW(wsFilePath))
				return false;

			GenerateWmfHeader(arStrings, bIsUnsigned);

			GenerateWmfBKMode();

			GenerateWmfCheckmark();
			GenerateWmfMiddleLine();
			GenerateWmfStrings(arStrings);

			if (bIsUnsigned)
				GenerateCrossHair();

			GenerateWmfEof();

			m_oFile.CloseFile();

			return true;
		}
	private:

		void UpdateSizes(unsigned int& unMaxSize, unsigned int& unSize, const std::vector<std::wstring>& arStrings, bool bIsUnsigned)
		{
			unMaxSize = 0x0000001C;
			unSize    = 0x000000AC;

			unsigned int unStringSize;

			for (const std::wstring& wsString : arStrings)
			{
				unStringSize = 7 + (wsString.length() + ((wsString.length() % 2 != 0) ? 1 : 0)) / 2;
				unSize += unStringSize;

				if (unStringSize > unMaxSize)
					unMaxSize = unStringSize;
			}

			if (bIsUnsigned)
				unSize += 32;
		}

		void GenerateWmfHeader(const std::vector<std::wstring>& arStrings, bool bIsUnsigned)
		{
			struct
			{
				DWORD key;
				WORD  hwmf;
				WORD  bounding_box [4];
				WORD  inch;
				DWORD reserved;
				WORD  checksum;
			} oPlaceable;

			oPlaceable.key = 0x9AC6CDD7;
			oPlaceable.hwmf = 0;
			oPlaceable.inch = 1440;
			oPlaceable.bounding_box[0] = 0;
			oPlaceable.bounding_box[1] = 0;
			oPlaceable.bounding_box[2] = m_ushWidth;
			oPlaceable.bounding_box[3] = m_ushHeight;
			oPlaceable.checksum = 0;
			oPlaceable.reserved = 0;

			for (WORD *check = (WORD *)&oPlaceable; check != &oPlaceable.checksum; ++check)
				oPlaceable.checksum ^= *check;

		// META_PLACEABLE
			m_oFile.WriteFile((BYTE*)&oPlaceable.key,  4); // Key
			m_oFile.WriteFile((BYTE*)&oPlaceable.hwmf, 2); // HWmf

			// BoundingBox
			m_oFile.WriteFile((BYTE*)&oPlaceable.bounding_box[0], 2); // Left
			m_oFile.WriteFile((BYTE*)&oPlaceable.bounding_box[1], 2); // Top
			m_oFile.WriteFile((BYTE*)&oPlaceable.bounding_box[2], 2); // Right
			m_oFile.WriteFile((BYTE*)&oPlaceable.bounding_box[3], 2); // Bottom

			m_oFile.WriteFile((BYTE*)&oPlaceable.inch,     2); // Inch  (default = 1440)
			m_oFile.WriteFile((BYTE*)&oPlaceable.reserved, 4); // Reserved
			m_oFile.WriteFile((BYTE*)&oPlaceable.checksum, 2); // Checksum

		// -----------------------------------------------------------------
			unsigned short ushType            = 0x0001;
			unsigned short ushHeaderSize      = 0x0009;
			unsigned short ushVersion         = 0x0300;
			unsigned short ushNumberOfObjects = 0x0002 + bIsUnsigned;
			unsigned short ushZero            = 0x0000;

			unsigned int unSize, unMaxRecord;

			UpdateSizes(unMaxRecord, unSize, arStrings, bIsUnsigned);

		//META_HEADER
			m_oFile.WriteFile((BYTE*)&ushType,            2); // Type
			m_oFile.WriteFile((BYTE*)&ushHeaderSize,      2); // HeaderSize
			m_oFile.WriteFile((BYTE*)&ushVersion,         2); // Version
			m_oFile.WriteFile((BYTE*)&unSize,             4); // Size
			m_oFile.WriteFile((BYTE*)&ushNumberOfObjects, 2); // NumberOfObjects

			m_oFile.WriteFile((BYTE*)&unMaxRecord,   4); // MaxRecord
			m_oFile.WriteFile((BYTE*)&ushZero,       2); // NumberOfMembers
		}

		void GenerateWmfBKMode()
		{
			unsigned int unRecordSize        = 0x00000005;
			unsigned short ushRecordFunction = 0x0102;
			unsigned short ushBkMode         = 0x0001; // Фон прозрачный у текста
			unsigned short ushReserved       = 0x0000;
		// META_SETBKMODE
			m_oFile.WriteFile((BYTE*)&unRecordSize,      4); // RecordSize
			m_oFile.WriteFile((BYTE*)&ushRecordFunction, 2); // RecordFunction
			m_oFile.WriteFile((BYTE*)&ushBkMode,         2); // BkMode
			m_oFile.WriteFile((BYTE*)&ushReserved,       2); // Reserved
		}

		void GenerateWmfPen(unsigned int unColor, unsigned int unWidth)
		{
			unsigned int unRecordSize        = 0x00000008;
			unsigned short ushRecordFunction = 0x02FA;
			unsigned short ushZero           = 0x0000;

		// META_CREATEPENINDIRECT
			m_oFile.WriteFile((BYTE*)&unRecordSize,      4); // RecordSize
			m_oFile.WriteFile((BYTE*)&ushRecordFunction, 2); // RecordFunction
			//Pen Object
			m_oFile.WriteFile((BYTE*)&ushZero,  2); // PenStyle
			m_oFile.WriteFile((BYTE*)&unWidth,  2); // Width-x
			m_oFile.WriteFile((BYTE*)&ushZero,  2); // Width-y
			m_oFile.WriteFile((BYTE*)&unColor,  4); // ColorRef

			++m_shObjectIndex;
		}

		void GenerateWmfMiddleLine()
		{
			GenerateWmfPen(TO_RGB(0, 0, 0), m_ushWidth / 100);
			GenerateWmfSelectObject(m_shObjectIndex);
			GenerateWmfLine(0, m_ushHeight / 2, m_ushWidth, m_ushHeight / 2);
			GenerateWmfDeleteObject(m_shObjectIndex);
		}

		void GenerateWmfLine(short shX1, short shY1, short shX2, short shY2)
		{
			unsigned int unRecordSize        = 0x00000005;
			unsigned short ushRecordFunction = 0x0214;

		// META_MOVETO
			m_oFile.WriteFile((BYTE*)&unRecordSize,      4); // RecordSize
			m_oFile.WriteFile((BYTE*)&ushRecordFunction, 2); // RecordFunction
			m_oFile.WriteFile((BYTE*)&shY1,              2); // Y
			m_oFile.WriteFile((BYTE*)&shX1,              2); // X
		// -----------------------------------------------------------------
			unRecordSize      = 0x00000005;
			ushRecordFunction = 0x0213;

		// META_LINETO
			m_oFile.WriteFile((BYTE*)&unRecordSize,      4); // RecordSize
			m_oFile.WriteFile((BYTE*)&ushRecordFunction, 2); // RecordFunction
			m_oFile.WriteFile((BYTE*)&shY2,              2); // Y
			m_oFile.WriteFile((BYTE*)&shX2,              2); // X
		}

		void GeneratePolyLine(const std::vector<std::pair<short, short>>& arPoints)
		{
			unsigned int unRecordSize        = 4 + 2 * arPoints.size();
			unsigned short ushRecordFunction = 0x0325;
			unsigned short ushNumberOfPoints = arPoints.size();

		// META_POLYLINE
			m_oFile.WriteFile((BYTE*)&unRecordSize,      4); // RecordSize
			m_oFile.WriteFile((BYTE*)&ushRecordFunction, 2); // RecordFunction
			m_oFile.WriteFile((BYTE*)&ushNumberOfPoints, 2); // RecordFunction

			for (const std::pair<short, short>& oPoint : arPoints)
			{
				m_oFile.WriteFile((BYTE*)&oPoint.first,  2);
				m_oFile.WriteFile((BYTE*)&oPoint.second, 2);
			}
		}

		void GenerateWmfCheckmark()
		{
			GenerateWmfPen(0, m_ushWidth / 200);
			GenerateWmfSelectObject(m_shObjectIndex);

			short shX1 = m_ushWidth / 100;
			short shX2 = std::sqrt(2 * std::pow(m_ushWidth / 10, 2));

			short shY1 = m_ushHeight * 0.48;
			short shY2 = shY1 - shX2 / 2;
			short shY3 = shY2 - shX2 / 2;

			GeneratePolyLine({{shX1, shY1}, {shX2, shY2}, {shX2, shY2}, {shX1, shY3}});

			GenerateWmfDeleteObject(m_shObjectIndex);
		}

		void GenerateString(const std::wstring& wsString, unsigned int unLength = 0)
		{
			for (const wchar_t& wChar : wsString)
				m_oFile.WriteFile((BYTE*)&wChar, 1);

			BYTE chNull = 0x00;
			for (unsigned short ushIndex = wsString.length(); ushIndex < unLength; ++ushIndex)
				m_oFile.WriteFile((BYTE*)&chNull, 1);
		}

		void GenerateWmfFont(short shFontHeight, const std::wstring& wsFontName)
		{
			unsigned int unRecordSize        = 0x0000001C;
			unsigned short ushRecordFunction = 0x02FB;
			unsigned short ushZero           = 0x0000;
			BYTE chZero                      = 0x00;
			unsigned short ushWeight         = 0x0190;
			BYTE chCharSet                   = 0x01;

		// META_CREATEFONTINDIRECT
			m_oFile.WriteFile((BYTE*)&unRecordSize,      4); // RecordSize
			m_oFile.WriteFile((BYTE*)&ushRecordFunction, 2); // RecordFunction
			// Font Object
			m_oFile.WriteFile((BYTE*)&m_shFontHeight,  2); // Height
			m_oFile.WriteFile((BYTE*)&ushZero,         2); // Width
			m_oFile.WriteFile((BYTE*)&ushZero,         2); // Escapement
			m_oFile.WriteFile((BYTE*)&ushZero,         2); // Orientation
			m_oFile.WriteFile((BYTE*)&ushWeight,       2); // Weight (default = 400)
			m_oFile.WriteFile((BYTE*)&chZero,          1); // Italic
			m_oFile.WriteFile((BYTE*)&chZero,          1); // Underline
			m_oFile.WriteFile((BYTE*)&chZero,          1); // StrikeOut
			m_oFile.WriteFile((BYTE*)&chCharSet,       1); // CharSet
			m_oFile.WriteFile((BYTE*)&chZero,          1); // OutPrecision
			m_oFile.WriteFile((BYTE*)&chZero,          1); // ClipPrecision
			m_oFile.WriteFile((BYTE*)&chZero,          1); // Quality
			m_oFile.WriteFile((BYTE*)&chZero,          1); // PitchAndFamily

			GenerateString(m_wsFontName, 32);

			++m_shObjectIndex;
		}

		void GenerateWmfSelectObjects()
		{
			for (unsigned short ushIndex = 0; ushIndex < 2; ++ushIndex)
				GenerateWmfSelectObject(ushIndex);
		}

		void GenerateWmfSelectObject(unsigned short ushIndex)
		{
			unsigned int unRecordSize        = 0x00000004;
			unsigned short ushRecordFunction = 0x012D;

		// META_SELECTOBJECT
			m_oFile.WriteFile((BYTE*)&unRecordSize,      4); // RecordSize
			m_oFile.WriteFile((BYTE*)&ushRecordFunction, 2); // RecordFunction
			m_oFile.WriteFile((BYTE*)&ushIndex,          2); // ObjectIndex
		}

		void GenerateWmfDeleteObject(unsigned short ushIndex)
		{
			unsigned int unRecordSize        = 0x00000004;
			unsigned short ushRecordFunction = 0x01F0;

		// META_DELETEOBJECT
			m_oFile.WriteFile((BYTE*)&unRecordSize,      4); // RecordSize
			m_oFile.WriteFile((BYTE*)&ushRecordFunction, 2); // RecordFunction
			m_oFile.WriteFile((BYTE*)&ushIndex,          2); // ObjectIndex

			--m_shObjectIndex;
		}
		void GenerateWmfStrings(const std::vector<std::wstring>& arStrings)
		{
			GenerateWmfFont(m_shFontHeight, m_wsFontName);
			GenerateWmfSelectObject(m_shObjectIndex);

			unsigned short ushX = m_ushWidth / 100, ushY = m_ushHeight / 2 + m_ushWidth / 100;

			for (const std::wstring& wsString : arStrings)
			{
				GenerateWmfString(wsString, ushX, ushY);
				ushY += m_shFontHeight * 1.2;
			}

			GenerateWmfDeleteObject(m_shObjectIndex);
		}

		void GenerateWmfString(const std::wstring& wsString, unsigned short ushX, unsigned short ushY)
		{
			unsigned short ushLength         = wsString.length();
			unsigned int unRecordSize        = 7 + (ushLength + ((ushLength % 2 != 0) ? 1 : 0)) / 2;
			unsigned short ushRecordFunction = 0x0A32;
			unsigned short ushZero           = 0x0000;
			BYTE chZero                      = 0x00;

		// META_EXTTEXTOUT
			m_oFile.WriteFile((BYTE*)&unRecordSize,      4); // RecordSize
			m_oFile.WriteFile((BYTE*)&ushRecordFunction, 2); // RecordFunction
			m_oFile.WriteFile((BYTE*)&ushY,              2); // Y
			m_oFile.WriteFile((BYTE*)&ushX,              2); // X
			m_oFile.WriteFile((BYTE*)&ushLength,         2); // StringLength
			m_oFile.WriteFile((BYTE*)&ushZero,           2); // fwOpts
			//Rectangle (зависит от fwOpts и обрезает текст)
//			m_oFile.WriteFile((BYTE*)0x0000,     2);
//			m_oFile.WriteFile((BYTE*)0x0000,     2);
//			m_oFile.WriteFile((BYTE*)0x0000,     2);
//			m_oFile.WriteFile((BYTE*)0x0000,     2);
			GenerateString(wsString);

			if (ushLength % 2 != 0)
				m_oFile.WriteFile((BYTE*)&chZero,        1); // Выравнивание
		}

		void GenerateCrossHair()
		{
			GenerateWmfPen(TO_RGB(255, 0, 0), m_ushWidth / 200); // красное перо
			GenerateWmfSelectObject(m_shObjectIndex);
			GenerateWmfLine(0, 0, m_ushWidth, m_ushHeight);
			GenerateWmfLine(m_ushWidth, 0, 0, m_ushHeight);
			GenerateWmfDeleteObject(m_shObjectIndex);
		}

		void GenerateWmfEof()
		{
			unsigned int unRecordSize        = 0x00000003;
			unsigned short ushRecordFunction = 0x0000;

		// META_EOF
			m_oFile.WriteFile((BYTE*)&unRecordSize,      4); // RecordSize
			m_oFile.WriteFile((BYTE*)&ushRecordFunction, 2); // RecordFunction
		}

		NSFile::CFileBinary m_oFile;

		unsigned short m_ushWidth;
		unsigned short m_ushHeight;

		short          m_shFontHeight;
		std::wstring   m_wsFontName;

		int            m_nBKColor;

		short          m_shObjectIndex;
	};
}

#endif // GENERATESIGNATURE_H
