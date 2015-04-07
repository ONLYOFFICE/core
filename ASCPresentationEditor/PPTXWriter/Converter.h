#pragma once

#include "ImageManager.h"


class CStylesWriter;

namespace NSPresentationEditor
{
	class CDocument;
	class CShapeWriter;

	class CPPTXWriter
	{
	public:
		CString	m_strTempDirectory;
		CString m_strDstFileName;

		CDocument*		m_pDocument;
		CImageManager	m_oManager;

		CShapeWriter*	m_pShapeWriter;

	public:
		CPPTXWriter();
		~CPPTXWriter();

		void CreateFile(CDocument* pDocument);

		void CloseFile();

		void Write()
		{
		}

	protected:
		void WriteContentTypes();
		void WriteApp(CFile& oFile);
		void WritePresInfo();
		void WriteAll();
		void WriteBackground(CStringWriter& oWriter, CRelsGenerator& oRels, CBrush& oBackground);
		void WriteElement(CStringWriter& oWriter, CRelsGenerator& oRels, IElement* pElement, CLayout* pLayout = NULL);
		void WriteLayout(CLayout& oLayout, int nIndexLayout, int nStartLayout, int nIndexTheme);
		void WriteSlide(int nIndexSlide);

	};
}