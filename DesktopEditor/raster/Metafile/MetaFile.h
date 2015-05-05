#ifndef _METAFILE_H
#define _METAFILE_H

#include "../../fontengine/ApplicationFonts.h"
#include "../../graphics/IRenderer.h"

#include "Wmf/WmfFile.h"
#include "Emf/EmfFile.h"

namespace MetaFile
{
	const int c_lMetaWmf = 0x01;
	const int c_lMetaEmf = 0x02;
	const int c_lMetaSvg = 0x04; 
	// третий поросенок еще бегает )

	class CMetaFile
	{
	public:
		CMetaFile(CApplicationFonts *pAppFonts);
		~CMetaFile();

		bool LoadFromFile(const wchar_t* wsFilePath);
		bool DrawOnRenderer(IRenderer* pRenderer, double dX, double dY, double dWidth, double dHeight);
		void Close();
		void GetBounds(double* pdX, double* pdY, double* pdW, double* pdH);
		int GetType();
		void ConvertToRaster(const wchar_t* wsOutFilePath, unsigned int unFileType, int nWidth, int nHeight = -1);
		CFontManager* get_FontManager();

	private:

		CApplicationFonts* m_pAppFonts;
		CFontManager*      m_pFontManager;
		CWmfFile           m_oWmfFile;
		CEmfFile           m_oEmfFile;
		int                m_lType;
	};
}

#endif //_METAFILE_H
