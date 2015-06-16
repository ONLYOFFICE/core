#ifndef _PDF_WRITER_SRC_ANNOTATION_DESTINATION_H
#define _PDF_WRITER_SRC_ANNOTATION_DESTINATION_H

#include "Objects.h"

namespace PdfWriter
{
	class CPage;

	class CDestination : public CArrayObject
	{
	public:
		CDestination(CPage* pPage, CXref* pXref);
		bool IsValid() const;
		void SetXYZ  (float fLeft, float fTop, float fZoom);
		void SetFit  ();
		void SetFitH (float fTop);
		void SetFitV (float fLeft);
		void SetFitR (float fLeft, float fBottom, float fRight, float fTop);
		void SetFitB ();
		void SetFitBH(float fTop);
		void SetFitBV(float fLeft);
	private:

		void PrepareArray();
	};
}
#endif // _PDF_WRITER_SRC_ANNOTATION_DESTINATION_H

