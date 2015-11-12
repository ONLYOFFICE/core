#pragma once

#include "ImageManager.h"


class CStylesWriter;
class CPPTUserInfo;

namespace NSPresentationEditor
{
	class CDocument;
	class CShapeWriter;

	class CPPTXWriter
	{
	public:
		CString	m_strTempDirectory;
		CString m_strDstFileName;

		CDocument	*	m_pDocument;
		CPPTUserInfo*	m_pUserInfo;
		CImageManager	m_oManager;

		CShapeWriter*	m_pShapeWriter;


	public:
		CPPTXWriter();
		~CPPTXWriter();

		void CreateFile(CDocument	* pDocument);
		void CreateFile(CPPTUserInfo* pUserInfo);

		void CloseFile();

		void Write()
		{
		}

	protected:
		void WriteApp			(CFile& oFile);
		void WriteContentTypes	();
		void WritePresInfo		();
		void WriteAll			();
		void WriteThemes		();
		void WriteSlides		();
		void WriteLayout		(CLayout& oLayout, int nIndexLayout, int nStartLayout, int nIndexTheme);
		void WriteSlide			(int nIndexSlide);
		
		void WriteColorScheme	(CStringWriter& oWriter, const std::wstring & name, const std::vector<CColor> & colors, bool extra = false);
		void WriteBackground	(CStringWriter& oWriter, CRelsGenerator& oRels, CBrush& oBackground);
		void WriteElement		(CStringWriter& oWriter, CRelsGenerator& oRels, IElement* pElement, CLayout* pLayout = NULL);
		

	};
}