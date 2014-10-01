#ifndef _MATH_EQUATION_READER_H
#define _MATH_EQUATION_READER_H

#include "../CompoundDocument/pole.h"
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

		CEquationReader(const wchar_t* wsFilePath) : m_oStorage(wsFilePath), pS(NULL), nHAlign(0), nVAlign(0)
		{
			InitSizes();
		}

		~CEquationReader()
		{
			if (NULL != pS)
				delete pS;
		}

		void SetOutputDev(IOutputDev *pOutput)
		{
			pOutputDev = pOutput;
			InitFonts();
		}

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
		int  HandleChar(uint8_t nTag);
		int  HandleTemplate();
		void HandleStartTemplate(uint8_t nSelector, uint8_t nVariation);
		void HandleEndTemplate  (uint8_t nSelector, uint8_t nVariation);
		int  HandlePile();
		int  HandleMatrix();
		void HandleEmblishments();
		void HandleSetSize(MTOKENS eType);
		void HandleRuler();
		void HandleFont();
		bool IsSpecialChar(Unicode_t nChar);

	private:

		// Header
		struct
		{
			uint32_t nCBHdr;
			uint16_t nVersion;
			uint16_t nCf;
			uint32_t nCBObject;
			uint32_t nReserved1;
			uint32_t nReserved2;
			uint32_t nReserved3;
			uint32_t nReserved4;
		} oHeader;

		// Ver 2 Header
		uint8_t nVersion;
		uint8_t nPlatform;
		uint8_t nProduct;
		uint8_t nProdVersion;
		uint8_t nProdSubVersion;

		Storage m_oStorage;

		CLEStream<Stream> *pS;

		IOutputDev* pOutputDev;

		uint16_t aSizeTable[7];

		uint8_t nHAlign;
		uint8_t nVAlign;

		TFontMap mFontMap;
	};
}

#endif
