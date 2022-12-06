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
#pragma once

#include "../../../Common/3dParty/pole/pole.h"

#include "Types.h"
#include "LEStream.h"
#include "String.h"
#include "OutputDev.h"

using namespace POLE;


namespace MathEquation
{
	class CEquationReader
	{
	public:

		CEquationReader(const wchar_t* wsFilePath);
		~CEquationReader();

        void SetOutputDev(IOutputDev *pOutput);
		int Parse();

	private:

		enum MTOKENS 
		{
			END    = 0x00,
			LINE   = 0x01,
			CHAR   = 0x02,
			TMPL   = 0x03,
			PILE   = 0x04,
			MATRIX = 0x05,
			EMBEL  = 0x06,
			RULER  = 0x07,
			FONT   = 0x08,
			SIZE_CUSTOM      = 0x09,
			SIZE_REGULAR     = 0x0a, // обычный
			SIZE_BIGSCRIPT   = 0x0b, // крунпый индекс
			SIZE_SMALLSCRIT  = 0x0c, // мелкий индекс
			SIZE_BIGSYMBOL   = 0x0d, // большой символ
			SIZE_SMALLSYMBOL = 0x0e  // мелкий символ
		};

	private:

		void InitSizes();
		void InitFonts();

		int HandleRecords();

		inline int xfLMOVE (BYTE nTest) {return nTest & 0x80;}
		inline int xfAUTO  (BYTE nTest) {return nTest & 0x10;}
		inline int xfEMBELL(BYTE nTest) {return nTest & 0x20;}
		inline int xfNULL  (BYTE nTest) {return nTest & 0x10;}
		inline int xfLSPACE(BYTE nTest) {return nTest & 0x40;}
		inline int xfRULER (BYTE nTest) {return nTest & 0x20;}

		void HandleNudge();
        int  HandleChar(unsigned char nTag);
		int  HandleTemplate();
        void HandleStartTemplate(unsigned char nSelector, unsigned char nVariation);
        void HandleEndTemplate  (unsigned char nSelector, unsigned char nVariation);
		int  HandlePile();
		int  HandleMatrix();
		void HandleEmblishments();
		void HandleSetSize(MTOKENS eType);
		void HandleRuler();
		void HandleFont();
        bool IsSpecialChar(unsigned short nChar);

	private:

		// Header
		struct
		{
            _UINT32 nCBHdr;
            _UINT16 nVersion;
            _UINT16 nCf;
            _UINT32 nCBObject;
            _UINT32 nReserved1;
            _UINT32 nReserved2;
            _UINT32 nReserved3;
            _UINT32 nReserved4;
		} oHeader;

		// Ver 2 Header
        unsigned char nVersion;
        unsigned char nPlatform;
        unsigned char nProduct;
        unsigned char nProdVersion;
        unsigned char nProdSubVersion;

		Storage m_oStorage;

        Stream *pStm;
		CLEStream<Stream> *pS;

		IOutputDev* pOutputDev;

        _UINT16 aSizeTable[7];

        unsigned char nHAlign;
        unsigned char nVAlign;

		TFontMap mFontMap;
	};
}
