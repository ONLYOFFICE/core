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
		TEmfXForm* GetTransform();
		void SetTextColor(TEmfColor& oColor);

	private:

		TEmfXForm m_oTransform;
		TEmfColor m_oTextColor;
	};
}

#endif //_EMF_PLAYER_H