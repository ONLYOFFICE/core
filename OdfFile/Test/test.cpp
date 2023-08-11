#include "gtest/gtest.h"

#include <boost/shared_ptr.hpp>

#include "Reader/Converter/ConvertOO2OOX.h"
#include "Reader/Converter/pptx_conversion_context.h"
#include "Reader/Converter/pptx_package.h"
#include "Reader/Format/odf_document.h"
#include "../../DesktopEditor/common/File.h"
#include "../../DesktopEditor/common/Directory.h"
#include "../../OfficeUtils/src/OfficeUtils.h"

class ODP2OOX_EnterExitAnimationTest : public testing::Test 
{
public:
	void SetUp() override
	{
		Init();

	}

	void TearDown() override
	{
		
	}

	void Init()
	{
		sTmpDirectory = NSFile::GetProcessDirectory() + FILE_SEPARATOR_STR + _T("../tmp");
		sExampleFilesDirectory = NSFile::GetProcessDirectory() + FILE_SEPARATOR_STR + _T("../ExampleFiles");
		sExampleFile = sExampleFilesDirectory + FILE_SEPARATOR_STR + _T("enter_exit.odp");

		NSDirectory::CreateDirectory(sTmpDirectory);
		
		mOdfDocument = ReadOdfDocument();
		mContext = Convert(mOdfDocument);
	}

	boost::shared_ptr<cpdoccore::odf_reader::odf_document> ReadOdfDocument()
	{
		std::wstring sTempUnpackedOdf = sTmpDirectory + FILE_SEPARATOR_STR + _T("odf_unpacked");
		std::wstring sTempUnpackedOox = sTmpDirectory + FILE_SEPARATOR_STR + _T("oox_unpacked");

		NSDirectory::CreateDirectory(sTempUnpackedOdf);
		NSDirectory::CreateDirectory(sTempUnpackedOox);

		COfficeUtils oCOfficeUtils(NULL);
		if (S_OK == oCOfficeUtils.ExtractToDirectory(sExampleFile, sTempUnpackedOdf, NULL, 0))
		{
			return boost::make_shared<cpdoccore::odf_reader::odf_document>(sTempUnpackedOdf, sTmpDirectory, L"");
		}
		else
		{
			return nullptr;
		}
	}

	boost::shared_ptr<cpdoccore::oox::pptx_conversion_context> Convert(boost::shared_ptr<cpdoccore::odf_reader::odf_document> inputOdf)
	{
		mOutputPptx = boost::make_shared<cpdoccore::oox::package::pptx_document>();
		boost::shared_ptr<cpdoccore::oox::pptx_conversion_context> context = boost::make_shared<cpdoccore::oox::pptx_conversion_context>(inputOdf.get());

		context->set_output_document(mOutputPptx.get());
		context->set_font_directory(L"");

		inputOdf->pptx_convert(*(context.get()));
 
		return context;
	}

private:
	std::wstring sTmpDirectory;
	std::wstring sExampleFilesDirectory;
	std::wstring sExampleFile;

	boost::shared_ptr<cpdoccore::oox::package::pptx_document> mOutputPptx;
	boost::shared_ptr<cpdoccore::oox::pptx_conversion_context> mContext;
	boost::shared_ptr<cpdoccore::odf_reader::odf_document> mOdfDocument;
};

TEST_F(ODP2OOX_EnterExitAnimationTest, qwe)
{

	EXPECT_EQ(1, 1);
}