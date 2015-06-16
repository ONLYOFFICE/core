#ifndef _PDF_WRITER_SRC_OUTLINE_H
#define _PDF_WRITER_SRC_OUTLINE_H

#include "Objects.h"

namespace PdfWriter
{
	class CDestination;

	class COutline : public CDictObject
	{
	public:
		COutline(CXref* pXref);
		COutline(COutline* pParent, const char* sTitle, CXref* pXref);

		void SetDestination(CDestination* pDestination);
		void SetOpened(bool bOpened);
		void BeforeWrite();
		EDictType GetDictType() const
		{
			return dict_type_OUTLINE;
		}
		COutline* GetFirst();
		COutline* GetLast();
		COutline* GetPrev();
		COutline* GetNext();
		COutline* GetParent();
		bool      GetOpened();

	private:

		void         AddChild(COutline* pItem);
		unsigned int CountChild();
	};
}

#endif // _PDF_WRITER_SRC_OUTLINE_H

