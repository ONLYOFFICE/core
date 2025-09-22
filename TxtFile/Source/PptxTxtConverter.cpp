#include "PptxTxtConverter.h"

#include "../../../OOXML/PPTXFormat/FileTypes.h"
#include "../../../OOXML/PPTXFormat/Folder.h"
#include "../../../OOXML/PPTXFormat/Slide.h"
#include "../../../OOXML/PPTXFormat/Presentation.h"
#include "../../../OOXML/PPTXFormat/Logic/Shape.h"
#include "../../../OOXML/PPTXFormat/Logic/Csld.h"
#include "../../../OOXML/PPTXFormat/Logic/SpTree.h"
#include "../../../OOXML/PPTXFormat/Logic/SpTreeElem.h"

#include "../../../OOXML/DocxFormat/WritingElement.h"

#include "../../../Common/OfficeFileErrorDescription.h"
#include "../../../DesktopEditor/common/File.h"

class CPptxTxtConverter::CPptxTxtConverterImpl
{
public:
	CPptxTxtConverterImpl() = default;
	CPptxTxtConverterImpl(const CPptxTxtConverterImpl& other) = delete;
	CPptxTxtConverterImpl(CPptxTxtConverterImpl&& other) = delete;
	virtual ~CPptxTxtConverterImpl();

	void AddSlide(const PPTX::Slide& oSlide);

	// returns true if ok
	void Save(const std::wstring& wsDstTxtFile);

private:
	std::wstring GetTextFromShape(const PPTX::Logic::Shape& oShape) const;
	std::vector<std::wstring> m_arTxtData;

	const size_t knStringReserve = 1000;
};

CPptxTxtConverter::CPptxTxtConverter()
    : m_pImpl(std::unique_ptr<CPptxTxtConverterImpl>(new CPptxTxtConverterImpl()))
{
}
CPptxTxtConverter::~CPptxTxtConverter()
{
}
int CPptxTxtConverter::Convert(const std::wstring& wsSrcPptxDir, const std::wstring& wsDstTxtFile)
{
	std::wstring norm_src_pptx_dir = wsSrcPptxDir;
	if (norm_src_pptx_dir.find(L"./") == 0)
		norm_src_pptx_dir.erase(0, 2);

	norm_src_pptx_dir = CorrectPathW(norm_src_pptx_dir);
	if (norm_src_pptx_dir.find(L"\\\\?\\") == 0)
		norm_src_pptx_dir.erase(0, 4);

	if (norm_src_pptx_dir.back() != FILE_SEPARATOR_WCHAR)
		norm_src_pptx_dir += FILE_SEPARATOR_WCHAR;

	std::unique_ptr<PPTX::Document> pptx_document(new PPTX::Document());
	bool is_read_ok = pptx_document->read(norm_src_pptx_dir);
	if (!is_read_ok)
		return AVS_FILEUTILS_ERROR_CONVERT;

	const auto presentation_file_type = OOX::Presentation::FileTypes::Presentation;
	smart_ptr<PPTX::Presentation> presentation = pptx_document->Get(presentation_file_type).smart_dynamic_cast<PPTX::Presentation>();
	if (!presentation.is_init())
		return AVS_FILEUTILS_ERROR_CONVERT;

	for (size_t i = 0; i < presentation->sldIdLst.size(); ++i)
	{
		std::wstring rid = presentation->sldIdLst[i].rid.get();
		smart_ptr<PPTX::Slide> slide = ((*presentation)[rid]).smart_dynamic_cast<PPTX::Slide>();

		if (slide.IsInit() == false)
			continue;

		m_pImpl->AddSlide(*slide);
	}
	m_pImpl->Save(wsDstTxtFile);
	return S_OK;
}

CPptxTxtConverter::CPptxTxtConverterImpl::~CPptxTxtConverterImpl()
{
}

void CPptxTxtConverter::CPptxTxtConverterImpl::AddSlide(const PPTX::Slide& oSlide)
{
	std::wstring slide_text;
	slide_text.reserve(knStringReserve);

	auto& sp_tree_elems = oSlide.cSld->spTree.SpTreeElems;
	for (auto& elem : sp_tree_elems)
	{
		if (elem.getType() != OOX::et_a_Shape)
			continue;

		smart_ptr<PPTX::Logic::Shape> shape = elem.GetElem().smart_dynamic_cast<PPTX::Logic::Shape>();
		slide_text += GetTextFromShape(*shape);
	}
	m_arTxtData.push_back(std::move(slide_text));
}
void CPptxTxtConverter::CPptxTxtConverterImpl::Save(const std::wstring& wsDstTxtFile)
{
	NSStringUtils::CStringBuilder str_builder;
	str_builder.AddSize(1000);
	for (const auto& txt_str : m_arTxtData)
	{
		str_builder.WriteString(txt_str);
		str_builder.WriteString(L"\n");
	}
	NSFile::CFileBinary::SaveToFile(wsDstTxtFile, str_builder.GetData());
	m_arTxtData.clear();
}

std::wstring CPptxTxtConverter::CPptxTxtConverterImpl::GetTextFromShape(const PPTX::Logic::Shape& oShape) const
{
	std::wstring text;
	text.reserve(knStringReserve);

	if (oShape.txBody.IsInit())
		for (const auto& paragraph : oShape.txBody->Paragrs)
			text += paragraph.GetText(true);

	return text; // RVO
}
