#ifndef _CATALOG_H
#define _CATALOG_H

#include "GlobalParams.h"

class XRef;
class Object;
class Page;
class PageAttrs;
struct Ref;
class LinkDestination;

//------------------------------------------------------------------------
// Catalog
//------------------------------------------------------------------------

class Catalog 
{
public:

	Catalog(GlobalParams *pGlobalParams, XRef *pXref);

	~Catalog();

	BOOL CheckValidation() 
	{ 
		return m_bValid; 
	}

	int GetPagesCount() 
	{ 
		return m_nPagesCount; 
	}

	Page *GetPage(int nIndex) 
	{ 
		return m_ppPages[nIndex - 1]; 
	}

	Ref *GetPageRef(int nIndex) 
	{ 
		return &m_pPageRefs[nIndex - 1]; 
	}

	StringExt *GetBaseURI() 
	{ 
		return m_seBaseURI; 
	}

	StringExt *ReadMetadata();

	Object *GetStructTreeRoot() 
	{ 
		return &m_oStructTreeRoot; 
	}

	// Ищем номер страницы в списке, по ее объектным номерам.
	int FindPage(int nNum, int nGen);

	// Ищем Named destination.
	LinkDestination *FindDest(StringExt *seName);

	Object *GetDests() 
	{ 
		return &m_oDests; 
	}

	Object *GetNameTree() 
	{ 
		return &m_oNameTree; 
	}

	Object *GetOutline() 
	{ 
		return &m_oOutline; 
	}

	Object *GetAcroForm() 
	{ 
		return &m_oAcroForm; 
	}

private:

	int ReadPageTree(Dict *pPages, PageAttrs *pAttrs, int nStart, char *sAlreadyRead);
	Object *FindDestInTree(Object *pTree, StringExt *seName, Object *pObject);

private:

	XRef      *m_pXref;           // Таблица xref для данного PDF файла
	Page     **m_ppPages;         // Массив страниц
	Ref       *m_pPageRefs;       // Объектные номера всех страниц
	int        m_nPagesCount;     // Число страниц
	int        m_nPagesSize;      // Размер массива страниц(в байтах)
	Object     m_oDests;          // Destination dictionary
	Object     m_oNameTree;       // Name tree
	StringExt *m_seBaseURI;       // Для ссылок URI
	Object     m_oMetadata;       // Metadata
	Object     m_oStructTreeRoot; // Структура страниц
	Object     m_oOutline;        // Outline
	Object     m_oAcroForm;       // AcroForm

	BOOL       m_bValid;          // True, если Сatalog - корретный

	GlobalParams *m_pGlobalParams;
};

#endif /* _CATALOG_H */
