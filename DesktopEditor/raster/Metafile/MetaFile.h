#ifndef _METAFILE_H
#define _METAFILE_H

#include "../../fontengine/ApplicationFonts.h"

#include "Wmf/WmfFile.h"
#include "Wmf/RendererOutput.h"
#include "Emf/EmfFile.h"

namespace Metafile
{
	const long c_lMetaWmf = 0x01;
	const long c_lMetaEmf = 0x02;
	const long c_lMetaSvg = 0x04;

	class CMetaFile
	{
	public:
		CMetaFile(CApplicationFonts *pAppFonts);
		~CMetaFile();

		bool LoadFromFile(const wchar_t* wsFilePath);
		bool DrawOnRenderer(IRenderer* pRenderer, double dX, double dY, double dWidth, double dHeight);
		void Close();
		void GetBounds(double* pdX, double* pdY, double* pdW, double* pdH);
		void ConvertToRaster(const wchar_t* wsOutFilePath, unsigned int unFileType, int nWidth, int nHeight = -1);

	private:

		CApplicationFonts* m_pAppFonts;
		CFontManager*      m_pFontManager;
		CWmfFile           m_oWmfFile;
		TWmfRectF          m_oWmfRect;
		CEmfFile           m_oEmfFile;
		long               m_lType;
	};
}

#endif //_METAFILE_H