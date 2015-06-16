#ifndef _PDF_WRITER_SRC_CATALOG_H
#define _PDF_WRITER_SRC_CATALOG_H

#include "Objects.h"

namespace PdfWriter
{
	class CPageTree;
	class CDestination;

	class CCatalog : public CDictObject
	{
	public:
		CCatalog(CXref* pXref);
		CPageTree*   GetRoot() const;
		EPageLayout  GetPageLayout() const;
		void         SetPageLayout(EPageLayout eLayout);
		EPageMode    GetPageMode() const;
		void         SetPageMode(EPageMode eMode);
		void         SetOpenAction(CDestination* pOpenAction);
		void         AddPageLabel(unsigned int unIndex, CDictObject* pPageLabel);
		EDictType    GetDictType() const
		{
			return dict_type_CATALOG;
		}
	};
}

#endif // _PDF_WRITER_SRC_CATALOG_H
