#ifndef CSVGSTORAGE_H
#define CSVGSTORAGE_H

#include "SvgObjects/CObjectBase.h"
#include "SvgObjects/CStyle.h"

namespace SVG
{
	class CSvgStorage
	{
	public:
		CSvgStorage();
		~CSvgStorage();

		void Clear();

		bool Empty() const;

		void AddObject(CObjectBase* pObject);
		CObjectBase* GetFirstObject();

		void AddStyle(const std::wstring& wsStyle);
		CGeneralStyle* GetStyle();

		bool Draw(IRenderer *pRenderer) const;
	private:
		std::vector<CObjectBase*> m_arObjects;
		CGeneralStyle             m_oStyle;
	};
}

#endif // CSVGSTORAGE_H
