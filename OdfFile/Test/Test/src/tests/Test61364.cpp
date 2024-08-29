#include "Test61364.h" 

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
