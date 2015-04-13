#ifndef _EMF_PLAYER_H
#define _EMF_PLAYER_H

#include <vector>
#include <map>
#include "EmfTypes.h"
#include "EmfObjects.h"

namespace Metafile
{
	class CEmfFile;
	class CEmfDC;

	class CEmfPlayer
	{
	public:

		CEmfPlayer(CEmfFile* pFile);
		~CEmfPlayer();
		void Clear();
		CEmfDC* SaveDC();
		CEmfDC* RestoreDC();
		CEmfDC* GetDC();
		void RegisterObject(unsigned long ulIndex, CEmfObjectBase* pObject);
		void SelectObject(unsigned long ulIndex);

	private:

		typedef std::map < unsigned long, CEmfObjectBase* > CEmfObjectMap;

		CEmfDC*              m_pDC;
		std::vector<CEmfDC*> m_vDCStack;
		CEmfFile*            m_pEmfFile;		
		CEmfObjectMap        m_mObjects;
	};

	class CEmfDC
	{
	public:

		CEmfDC();
		~CEmfDC();
		CEmfDC* Copy();
		TEmfXForm*      GetTransform();
		void            SetTextColor(TEmfColor& oColor);
		TEmfColor&      GetTextColor();
		void            SetBrush(CEmfLogBrushEx* pBrush);
		CEmfLogBrushEx* GetBrush();
		void            SetFont(CEmfLogFont* pFont);
		CEmfLogFont*    GetFont();
		void            SetTextAlign(unsigned long ulAlign);
		unsigned long   GetTextAlign();
		void            SetBgMode(unsigned long ulBgMode);
		unsigned long   GetBgMode();
		void            SetBgColor(TEmfColor& oColor);
		TEmfColor&      GetBgColor();
		void            SetMiterLimit(unsigned long ulMiter);
		unsigned long   GetMiterLimit();
		void            SetFillMode(unsigned long ulFillMode);
		unsigned long   GetFillMode();
		void            SetPen(CEmfLogPen* pPen);
		CEmfLogPen*     GetPen();

	private:

		CEmfLogBrushEx* m_pBrush;
		CEmfLogPen*     m_pPen;
		CEmfLogFont*    m_pFont;
		TEmfXForm       m_oTransform;
		TEmfColor       m_oTextColor;
		TEmfColor       m_oBgColor;
		unsigned long   m_ulTextAlign;
		unsigned long   m_ulBgMode;
		unsigned long   m_ulMiterLimit;
		unsigned long   m_ulFillMode;
	};
}

#endif //_EMF_PLAYER_H