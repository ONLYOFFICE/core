#ifndef _PDF_WRITER_SRC_INFO_H
#define _PDF_WRITER_SRC_INFO_H

#include "Objects.h"

namespace PdfWriter
{
	enum EInfoType
	{
		// Данные относительно даты создания и изменения.
		InfoCreationDate = 0,
		InfoModaDate     = 1,

		// Текстовые данные
		InfoAuthor       = 2,
		InfoCreator      = 3,
		InfoProducer     = 4,
		InfoTitle        = 5,
		InfoSubject      = 6,
		InfoKeyWords     = 7,

		InfoMin          = 0,
		InfoMax          = 0
	};
	struct TDate
	{
		int  nYear;
		int  nMonth;
		int  nDay;
		int  nHour;
		int  nMinutes;
		int  nSeconds;
		char nInd;
		int  nOffHour;
		int  nOffMinutes;
	};

	class CXref;

	class CInfoDict : public CDictObject
	{
	public:
		CInfoDict(CXref* pXref);
		void        SetInfo(EInfoType eType, const char* sValue);
		const char* GetInfo(EInfoType eType);
		void        SetInfo(EInfoType eType, const TDate& oDate);
		void        SetCreationTime();
	};
}
#endif // _PDF_WRITER_SRC_INFO_H

