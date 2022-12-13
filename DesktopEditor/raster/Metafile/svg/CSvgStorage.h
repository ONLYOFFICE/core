#ifndef CSVGSTORAGE_H
#define CSVGSTORAGE_H

#include "SvgObjects/CObjectBase.h"

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

		bool Draw(IRenderer *pRenderer) const;
	private:
		std::vector<CObjectBase*> m_arObjects;
	};
}

#endif // CSVGSTORAGE_H
