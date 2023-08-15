#include "gtest/gtest.h"

#include <boost/shared_ptr.hpp>

#include "Reader/Converter/ConvertOO2OOX.h"
#include "Reader/Converter/pptx_conversion_context.h"
#include "Reader/Converter/pptx_package.h"
#include "Reader/Format/odf_document.h"
#include "../../DesktopEditor/common/File.h"
#include "../../DesktopEditor/common/Directory.h"
#include "../../OfficeUtils/src/OfficeUtils.h"

#define CH_DIR(x) FILE_SEPARATOR_STR + _T(x)

class TestEnvironment : public ::testing::Environment {
public:

	~TestEnvironment() override {}

	void SetUp() override 
	{
		Init();
		mInputOdf = ReadOdfDocument();
		mConverionContext = Convert(mInputOdf);
	}

	void TearDown() override 
	{ }

	static boost::shared_ptr<cpdoccore::oox::pptx_conversion_context> GetConversionContext()
	{
		return mConverionContext;
	}

	static const cpdoccore::oox::pptx_animation_context& GetAnimationContext()
	{
		return mConverionContext->get_slide_context().get_animation_context();
	}

private:
	
	void Init()
	{
		std::wstring exampleFilename = L"enter_exit";

		std::wstring rootDir = NSFile::GetProcessDirectory() + CH_DIR("..");
		std::wstring sExampleFilesDirectory = rootDir + CH_DIR("ExampleFiles");

		sFrom = sExampleFilesDirectory + FILE_SEPARATOR_STR + exampleFilename + L".odp";
		sTo = sExampleFilesDirectory + FILE_SEPARATOR_STR + exampleFilename + L".pptx";
		sTemp = rootDir + CH_DIR("tmp");
		sTempUnpackedOdf = sTemp + CH_DIR("odf_unpacked");
		sTempUnpackedOox = sTemp + CH_DIR("oox_unpacked");

		NSDirectory::CreateDirectory(sTemp);
		NSDirectory::CreateDirectory(sTempUnpackedOdf);
		NSDirectory::CreateDirectory(sTempUnpackedOox);

		mOutputPptx = boost::make_shared<cpdoccore::oox::package::pptx_document>();
	}

	boost::shared_ptr<cpdoccore::odf_reader::odf_document> ReadOdfDocument()
	{
		COfficeUtils oCOfficeUtils(NULL);
		if (S_OK == oCOfficeUtils.ExtractToDirectory(sFrom, sTempUnpackedOdf, NULL, 0))
			return boost::make_shared<cpdoccore::odf_reader::odf_document>(sTempUnpackedOdf, sTemp, L"");
		else
			return nullptr;
	}

	boost::shared_ptr<cpdoccore::oox::pptx_conversion_context> Convert(boost::shared_ptr<cpdoccore::odf_reader::odf_document> inputOdf)
	{
		boost::shared_ptr<cpdoccore::oox::pptx_conversion_context> conversionContext = boost::make_shared<cpdoccore::oox::pptx_conversion_context>(mInputOdf.get());

		conversionContext->set_output_document(mOutputPptx.get());
		conversionContext->set_font_directory(L"");

		mInputOdf->pptx_convert(*conversionContext);

		return conversionContext;
	}

private:
	std::wstring sFrom;
	std::wstring sTo;
	std::wstring sTemp;
	std::wstring sTempUnpackedOdf;
	std::wstring sTempUnpackedOox;

	static boost::shared_ptr<cpdoccore::oox::package::pptx_document> mOutputPptx;
	static boost::shared_ptr<cpdoccore::oox::pptx_conversion_context> mConverionContext;
	static boost::shared_ptr<cpdoccore::odf_reader::odf_document> mInputOdf;
};

boost::shared_ptr<cpdoccore::oox::package::pptx_document> TestEnvironment::mOutputPptx;
boost::shared_ptr<cpdoccore::oox::pptx_conversion_context> TestEnvironment::mConverionContext;
boost::shared_ptr<cpdoccore::odf_reader::odf_document> TestEnvironment::mInputOdf;

class ODP2OOX_EnterExitAnimationTest : public testing::Test 
{
public:
	void SetUp() override
	{
		mAnimationContext = &TestEnvironment::GetAnimationContext();
	}

	void TearDown() override
	{
		
	}
public:
	const cpdoccore::oox::pptx_animation_context* mAnimationContext;

};

TEST_F(ODP2OOX_EnterExitAnimationTest, timing_root_par)
{
	using namespace cpdoccore::oox;
	const pptx_animation_context::Impl::_par_animation_ptr& root = mAnimationContext->get_root_par_animation();

	const std::wstring expectedNodeType = L"tmRoot";

	EXPECT_EQ(root->Delay.value(), expectedNodeType);
}

TEST_F(ODP2OOX_EnterExitAnimationTest, asd)
{
	EXPECT_EQ(1, 2);
}

int main(int argc, char* argv[]) 
{
	::testing::InitGoogleTest(&argc, argv);
	::testing::AddGlobalTestEnvironment(new TestEnvironment);
	return RUN_ALL_TESTS();
}