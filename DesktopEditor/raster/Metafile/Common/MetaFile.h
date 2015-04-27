#ifndef _METAFILE_COMMON_METAFILE_H
#define _METAFILE_COMMON_METAFILE_H

#include "MetaFileTypes.h"
#include "MetaFileObjects.h"
#include "MetaFileClip.h"
#include "../../fontengine/FontManager.h"

namespace MetaFile
{
	class CMetaFileBase
	{
	public:
		CMetaFileBase(){}
		virtual ~CMetaFileBase(){}

		virtual double       TranslateX(int nX) = 0;
		virtual double       TranslateY(int nY) = 0;
		virtual TRect*       GetDCBounds() = 0;
		virtual double       GetPixelHeight() = 0;
		virtual double       GetPixelWidth() = 0;
		virtual int          GetTextColor() = 0;
 		virtual CFont*       GetFont() = 0;
		virtual CBrush*      GetBrush() = 0;
		virtual CPen*        GetPen() = 0;
		virtual unsigned int GetTextAlign() = 0;
		virtual unsigned int GetTextBgMode() = 0;
		virtual int          GetTextBgColor() = 0;
		virtual unsigned int GetFillMode() = 0;
		virtual TPointL      GetCurPos() = 0;
		virtual TEmfXForm*   GetInverseTransform() = 0;
		virtual TEmfXForm*   GetTransform() = 0;
		virtual unsigned int GetMiterLimit() = 0;
		virtual unsigned int GetRop2Mode() = 0;
		virtual CClip*       GetClip() = 0;

		CFontManager* GetFontManager()
		{
			return m_pFontManager;
		}
		void SetFontManager(CFontManager* pFontManager)
		{
			m_pFontManager = pFontManager;
		}
		void SetError()
		{
			m_bError = true;
		}
		bool CheckError()
		{
			return m_bError;
		}

	private:

		CFontManager* m_pFontManager;
		bool          m_bError;
	};
}

#endif //_METAFILE_COMMON_METAFILE_H
