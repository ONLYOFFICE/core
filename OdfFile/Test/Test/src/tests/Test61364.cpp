#include "Test61364.h" 

#include "../../../../../OOXML/DocxFormat/Settings/Settings.h"

TestEnv61364* g_TestEnv61364 = (TestEnv61364*)testing::AddGlobalTestEnvironment(new TestEnv61364);

TestEnv61364::TestEnv61364()
	: ODT2DOCX_ConversionEnvironment(L"ExampleFiles/61364.odt")
{ }

void Test61364::SetUp()
{
	Docx = g_TestEnv61364->GetDocument();
}

TEST_F(Test61364, Docx)
{
	EXPECT_TRUE(Docx);
}

TEST_F(Test61364, document)
{
	EXPECT_TRUE(Docx->m_oMain.document);
}

TEST_F(Test61364, settings)
{
	EXPECT_TRUE(Docx->m_oMain.settings);
}

TEST_F(Test61364, settings_default_tab_stop)
{
	ASSERT_TRUE(Docx->m_oMain.settings->m_oDefaultTabStop.IsInit());
	const ComplexTypes::Word::CTwipsMeasure& tabStop = Docx->m_oMain.settings->m_oDefaultTabStop.get();

	ASSERT_TRUE(tabStop.m_oVal.IsInit());
	const SimpleTypes::CTwipsMeasure& tabStopValue = tabStop.m_oVal.get();

	const int expected_tab_stop_in_twips = 282;

	EXPECT_EQ(tabStopValue.ToTwips(), expected_tab_stop_in_twips);
}