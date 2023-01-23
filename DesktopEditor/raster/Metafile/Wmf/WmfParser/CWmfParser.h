#ifndef CWMFPARSER_H
#define CWMFPARSER_H

#include "CWmfParserBase.h"

namespace MetaFile
{
	class CWmfParser : public CWmfParserBase
	{
	public:
		CWmfParser();
		virtual ~CWmfParser();

		bool            ReadFromBuffer(unsigned char* pBuffer, unsigned int unSize) override;
		bool            OpenFromFile(const wchar_t* wsFilePath)     override;

		void            PlayFile()                                  override;
		void            Scan()                                      override;

		WmfParserType   GetType()				                    override;
		void            SetStream(BYTE* pBuf, unsigned int unSize);

	private:

		void Read_META_HEADER();
		//-----------------------------------------------------------
		// 2.3.1 Bitmap records
		//-----------------------------------------------------------
		void Read_META_BITBLT();
		void Read_META_DIBBITBLT();
		void Read_META_DIBSTRETCHBLT();
		void Read_META_SETDIBTODEV();
		void Read_META_STRETCHBLT();
		void Read_META_STRETCHDIB();
		//-----------------------------------------------------------
		// 2.3.2 Control records
		//-----------------------------------------------------------
		void Read_META_EOF();
		//-----------------------------------------------------------
		// 2.3.3 Drawing records
		//-----------------------------------------------------------
		void Read_META_ARC();
		void Read_META_CHORD();
		void Read_META_ELLIPSE();
		void Read_META_EXTTEXTOUT();
		void Read_META_FILLREGION();
		void Read_META_FRAMEREGION();
		void Read_META_INVERTREGION();
		void Read_META_LINETO();
		void Read_META_PAINTREGION();
		void Read_META_PATBLT();
		void Read_META_PIE();
		void Read_META_POLYLINE();
		void Read_META_POLYGON();
		void Read_META_POLYPOLYGON();
		void Read_META_RECTANGLE();
		void Read_META_ROUNDRECT();
		void Read_META_SETPIXEL();
		void Read_META_TEXTOUT();
		//-----------------------------------------------------------
		// 2.3.4 Object records
		//-----------------------------------------------------------
		void Read_META_CREATEBRUSHINDIRECT();
		void Read_META_CREATEFONTINDIRECT();
		void Read_META_CREATEPALETTE();
		void Read_META_CREATEPATTERNBRUSH();
		void Read_META_CREATEPENINDIRECT();
		void Read_META_CREATEREGION();
		void Read_META_DELETEOBJECT();
		void Read_META_DIBCREATEPATTERNBRUSH();
		void Read_META_SELECTCLIPREGION();
		void Read_META_SELECTOBJECT();
		void Read_META_SELECTPALETTE();
		//-----------------------------------------------------------
		// 2.3.5 State records
		//-----------------------------------------------------------
		void Read_META_EXCLUDECLIPRECT();
		void Read_META_INTERSECTCLIPRECT();
		void Read_META_MOVETO();
		void Read_META_OFFSETCLIPRGN();
		void Read_META_OFFSETVIEWPORTORG();
		void Read_META_OFFSETWINDOWORG();
		void Read_META_RESTOREDC();
		void Read_META_SAVEDC();
		void Read_META_SCALEVIEWPORTEXT();
		void Read_META_SCALEWINDOWEXT();
		void Read_META_SETBKCOLOR();
		void Read_META_SETBKMODE();
		void Read_META_SETLAYOUT();
		void Read_META_SETMAPMODE();
		void Read_META_SETPOLYFILLMODE();
		void Read_META_SETROP2();
		void Read_META_SETSTRETCHBLTMODE();
		void Read_META_SETTEXTALIGN();
		void Read_META_SETTEXTCHAREXTRA();
		void Read_META_SETTEXTCOLOR();
		void Read_META_SETTEXTJUSTIFICATION();
		void Read_META_SETVIEWPORTEXT();
		void Read_META_SETVIEWPORTORG();
		void Read_META_SETWINDOWEXT();
		void Read_META_SETWINDOWORG();
		//-----------------------------------------------------------
		// 2.3.6 State records
		//-----------------------------------------------------------
		void Read_META_ESCAPE();
		//-----------------------------------------------------------
		// Неизвестные записи
		//-----------------------------------------------------------
		void Read_META_UNKNOWN();
	};
}

#endif // CWMFPARSER_H
