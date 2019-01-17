/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#ifndef _PDFREADER_GFONT_H
#define _PDFREADER_GFONT_H

#include "StringExt.h"
#include "Object.h"
#include "CharTypes.h"
#include "GlobalParams.h"

namespace PdfReader
{
	class Dict;
	class CMap;
	class CharCodeToUnicode;
	class CFontFileTrueType;
	struct GrFontCIDWidths;

	//------------------------------------------------------------------------
	// GrFontType
	//------------------------------------------------------------------------

	enum GrFontType
	{
		//----- Gr8BitFont
		fontUnknownType,
		fontType1,
		fontType1C,
		fontType1COT,
		fontType3,
		fontTrueType,
		fontTrueTypeOT,
		//----- GrCIDFont
		fontCIDType0,
		fontCIDType0C,
		fontCIDType0COT,
		fontCIDType2,
		fontCIDType2OT
	};

	//------------------------------------------------------------------------
	// GrFontCIDWidths
	//------------------------------------------------------------------------

	struct GrFontCIDWidthException
	{
		CID    nFirst;     // Данная запись применяется к 
		CID    nLast;      // CID: <nFirst>...<nLast>
		double dWidth;     // Новая ширина символов
	};

	struct GrFontCIDWidthExceptionV
	{
		CID    nFirst;    // Данная запись применяется к 
		CID    nLast;     // CID: <nFirst>...<nLast>
		double dHeight;   // Новая высота символов
		double dVx;       // Координаты вектора V: origin0->origin1
		double dVy;       // См. фигуру 40 в спецификации PDF 1.7
	};

	struct GrFontCIDWidths
	{
		double                   dDefaultWidth;     // Стандартное значение ширины символа
		double                   dDefaultHeight;    // Стандартное значение высоты символа
		double                   dDefaultV;         // Стандартное значение вектора V
		GrFontCIDWidthException *pExceptions;       // Исключения для горизонтальных метрик
		int                      nExceptionsCount;  // Число исключений
		GrFontCIDWidthExceptionV*pExceptionsV;      // Исключения для вертикальных метрик
		int                      nExceptionsVCount; // Число исключений
	};

	//------------------------------------------------------------------------
	// GrFont
	//------------------------------------------------------------------------

#define fontFixedWidth (1 << 0)
#define fontSerif      (1 << 1)
#define fontSymbolic   (1 << 2)
#define fontItalic     (1 << 6)
#define fontBold       (1 << 18)

	class GrFont
	{
	public:

		static GrFont *MakeFont(XRef *pXref, char *sTag, Ref oID, Dict *pFontDict, GlobalParams *pGlobalParams);

		GrFont(char *sTag, Ref oID, StringExt *seName, GlobalParams *pGlobalParams);

		virtual ~GrFont();

		bool CheckValidate()
		{
			return m_bValid;
		}

		StringExt *GetTag()
		{
			return m_seTag;
		}

		Ref *GetID()
		{
			return &m_oID;
		}

		bool CheckTag(char *sTag)
		{
			return !m_seTag->Compare(sTag);
		}

		StringExt *GetBaseName()
		{
			return m_seName;
		}

		StringExt *GetOriginalName()
		{
			return m_seOriginalName;
		}

		GrFontType GetType()
		{
			return m_eType;
		}
		virtual bool IsCIDFont()
		{
			return false;
		}

		// Внедренные фонты.
		bool       GetEmbeddedFontFileRef(Ref *pEmbRef)
		{
			*pEmbRef = m_oEmbFontFileRef;
			return m_oEmbFontFileRef.nNum >= 0;
		}

		StringExt *GetEmbeddedFontName()
		{
			return m_seEmbeddedFontName;
		}

		std::wstring GetExternalFontFilePath()
		{
			return m_wsExternalFontFilePath;
		}

		//
		int GetFlags()
		{
			return m_nFlags;
		}
		bool IsFixedWidth()
		{
			return (m_nFlags & fontFixedWidth ? true : false);
		}
		bool IsSerif()
		{
			return (m_nFlags & fontSerif ? true : false);
		}
		bool IsSymbolic()
		{
			return (m_nFlags & fontSymbolic ? true : false);
		}
		bool IsItalic()
		{
			return (m_nFlags & fontItalic ? true : false);
		}
		bool IsBold()
		{
			return (m_nFlags & fontBold ? true : false);
		}

		double *GetFontMatrix()
		{
			return m_arrFontMatrix;
		}

		double *GetFontBBox()
		{
			return m_arrFontBBox;
		}

		double  GetAscent()
		{
			return m_dAscent;
		}
		double  GetDescent()
		{
			return m_dDescent;
		}

		// 0 = пишем по горизонтали, 1 = по вертикали
		virtual int GetWMode()
		{
			return 0;
		}

		// Считываем внешний или включенный FontFile в буфер.
		char *ReadExternalFontFile(int *pnLength);
		char *ReadEmbeddedFontFile(XRef *pXref, int *pnLegth);

		// Считываем следующий символ из строки <sText> длины <nLen> байт, возвращем
		// код символа <unCode>, его юникодное значение <unUnicode>, вектор замещения 
		// (w0 или w1, см. спецификацию PDF 1.7 фигура 40) (<dDx>, <dDy>), и вектор V
		// (см. фигуру 40) (<Vx>, <Vy>). Возвращаем количество байт необходимых для punCode.
		virtual int GetNextChar(char *sText, int nLen, CharCode *punCode, Unicode *punUnicode, int uSize, int *uLen, double *pdDx, double *pdDy, double *pdVx, double *pdVy) = 0;

	protected:

		void ReadFontDescriptor(XRef *pXref, Dict *pFontDict);
		CharCodeToUnicode *ReadToUnicodeCMap(Dict *pFontDict, int nBitsCount, CharCodeToUnicode *pCharToUnicode);
		void FindExternalFontFile(bool bBuiltin = false);

	protected:

		StringExt*    m_seTag;                  // Тэг pdf-шрифта
		Ref           m_oID;                    // Ссылка на объект-шрифт (используем в качестве уникального идентефикатора)
		StringExt*    m_seName;                 // Название шрифта
		StringExt*    m_seOriginalName;         // Первоначальное название шрифта
		GrFontType    m_eType;                  // Тип шрифта
		int           m_nFlags;                 // Флаги в FontDescriptor
		StringExt*    m_seEmbeddedFontName;     // Название включенного в PDF шрифта(т.е. шрифт идет вместе с FontFile)
		Ref           m_oEmbFontFileRef;        // Ссылка(pdf-ссылка) на FontFile для включенного шрифта
		std::wstring  m_wsExternalFontFilePath; // Путь в FontFile(т.е. font file не в самом PDF)
					 
		double        m_arrFontMatrix[6];       // Матрица преобразования координат символа в координаты текста (только для Type 3)
		double        m_arrFontBBox[4];         // Наименьший прямоугольник задающий границы всех символов (только для Type 3)
		double        m_dMissingWidth;          // Стандартное значение ширины для пропущенных символов
		double        m_dAscent;                // Ascent
		double        m_dDescent;               // Descent
					 
		bool          m_bValid;

		GlobalParams* m_pGlobalParams;
	};

	//------------------------------------------------------------------------
	// Gr8BitFont
	//------------------------------------------------------------------------

	class Gr8BitFont : public GrFont
	{
	public:

		Gr8BitFont(XRef *pXref, char *sTag, Ref oID, StringExt *seName, GrFontType eType, Dict *pFontDict, GlobalParams *pGlobalParams);

		virtual ~Gr8BitFont();

		virtual int GetNextChar(char *sText, int nLen, CharCode *punCode, Unicode *punUnicode, int uSize, int *uLen, double *pdDx, double *pdDy, double *pdVx, double *pdVy);

		char **GetEncoding()
		{
			return m_ppEncoding;
		}

		CharCodeToUnicode *GetToUnicode();

		char *GetCharName(int nCode)
		{
			return m_ppEncoding[nCode];
		}

		bool GetHasEncoding()
		{
			return m_bHasEncoding;
		}

		bool GetUsesMacRomanEncoding()
		{
			return m_bUsesMacRomanEncoding;
		}

		double GetWidth(unsigned char unChar)
		{
			return m_arrWidths[unChar];
		}
		// Только для TrueType
		unsigned short *GetCodeToGIDMap(CFontFileTrueType *pTTF);

		// Следующие три функции используются только для Type 3
		Dict   *GetCharProcs();
		Object *GetCharProc(int nCode, Object *pProc);
		Dict   *GetResources();

	private:

		char              *m_ppEncoding[256];       // Кодировка( Сhar code --> Сhar name)
		bool               m_arrEncFree[256];       // Булевские значения для имени каждого символа: true, если под строку выделяли память
		CharCodeToUnicode *m_pCharToUnicode;        // Сhar code --> Unicode
		bool               m_bHasEncoding;          // Имеется ли кодировка вообще?
		bool               m_bUsesMacRomanEncoding; // Кодировка MacRomanEncoding?
		double             m_arrWidths[256];        // Ширины символов
		Object             m_oCharProcs;            // Специфично только для Type 3, словарь CharProcs
		Object             m_oResources;            // Специфично только для Type 3, словарь Resources 
	};

	//------------------------------------------------------------------------
	// GrCIDFont
	//------------------------------------------------------------------------

	class GrCIDFont : public GrFont
	{
	public:

		GrCIDFont(XRef *pXref, char *sTag, Ref oID, StringExt *seName, Dict *pFontDict, GlobalParams *pGlobalParams);

		virtual ~GrCIDFont();

		virtual bool IsCIDFont()
		{
			return true;
		}

		virtual int GetNextChar(char *sText, int nLen, CharCode *punCode, Unicode *punUnicode, int uSize, int *uLen, double *pdDx, double *pdDy, double *pdVx, double *pdVy);

		virtual int GetWMode();

		CharCodeToUnicode *GetToUnicode();

		// Сollection name (<registry>-<ordering>).
		StringExt *GetCollection();

		// CID-to-GID. Используется только если тип = fontCIDType2.
		unsigned short *GetCIDToGID()
		{
			return m_pCidToGID;
		}
		int GetCIDToGIDLen()
		{
			return m_nCidToGIDLen;
		}

		CMap *GetCMap()
		{
			return m_pCMap;
		}

	private:

		CMap              *m_pCMap;           // char code --> CID
		CharCodeToUnicode *m_pCharToUnicode;  // CID --> Unicode
		GrFontCIDWidths    m_oWidths;         // character widths
		unsigned short    *m_pCidToGID;       // CID --> GID mapping (for embedded TrueType fonts)
		int                m_nCidToGIDLen;
	};

	//------------------------------------------------------------------------
	// GrFontDict
	//------------------------------------------------------------------------

	class GrFontDict
	{
	public:

		GrFontDict(XRef *pXref, Ref *pFontDictRef, Dict *pFontDict, GlobalParams *pGlobalParams);

		~GrFontDict();

		GrFont *Search(char *sTag);

		int GetFontsCount()
		{
			return m_nFontsCount;
		}
		GrFont *GetFont(int nIndex)
		{
			return m_ppFonts[nIndex];
		}

	private:

		GrFont      **m_ppFonts;     // Список шрифтов
		int           m_nFontsCount; // Количество шрифтов
		GlobalParams *m_pGlobalParams;
	};
}

#endif // _PDFREADER_GFONT_H
