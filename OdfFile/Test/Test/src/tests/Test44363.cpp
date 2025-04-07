#include "Test44363.h"

TestEnv44363* g_TestEnv44363 = (TestEnv44363*)testing::AddGlobalTestEnvironment(new TestEnv44363);

TestEnv44363::TestEnv44363()
	: ODT2DOCX_ConversionEnvironment(L"ExampleFiles/44363.odt")
{ }

void Test44363::SetUp()
{
	Docx = g_TestEnv44363->GetDocument();
}

TEST_F(Test44363, Docx)
{
	EXPECT_TRUE(Docx);
}

TEST_F(Test44363, document)
{
	EXPECT_TRUE(Docx->m_oMain.document);
}

TEST_F(Test44363, document_elements_size)
{
	uint32_t expected = 5;

	EXPECT_EQ(Docx->m_oMain.document->m_arrItems.size(), expected);
}

TEST_F(Test44363, paragraph)
{
	OOX::Logic::CParagraph* p1 = dynamic_cast<OOX::Logic::CParagraph*>(Docx->m_oMain.document->m_arrItems[0]);
	EXPECT_TRUE(p1);
}

TEST_F(Test44363, paragraph_props)
{
	OOX::Logic::CParagraph* p1 = dynamic_cast<OOX::Logic::CParagraph*>(Docx->m_oMain.document->m_arrItems[0]);

	EXPECT_TRUE(p1->m_oParagraphProperty);
}

TEST_F(Test44363, paragraph_props_run_props)
{
	OOX::Logic::CParagraph* p1 = dynamic_cast<OOX::Logic::CParagraph*>(Docx->m_oMain.document->m_arrItems[0]);

	EXPECT_TRUE(p1->m_oParagraphProperty->m_oRPr.IsInit());
}

TEST_F(Test44363, paragraph_props_run_props_bold)
{
	OOX::Logic::CParagraph* p1 = dynamic_cast<OOX::Logic::CParagraph*>(Docx->m_oMain.document->m_arrItems[0]);

	ASSERT_TRUE(p1->m_oParagraphProperty->m_oRPr.IsInit());
	EXPECT_TRUE(p1->m_oParagraphProperty->m_oRPr->m_oBold.IsInit());
}

TEST_F(Test44363, paragraph_props_run_props_italic)
{
	OOX::Logic::CParagraph* p1 = dynamic_cast<OOX::Logic::CParagraph*>(Docx->m_oMain.document->m_arrItems[0]);

	ASSERT_TRUE(p1->m_oParagraphProperty->m_oRPr.IsInit());
	EXPECT_TRUE(p1->m_oParagraphProperty->m_oRPr->m_oItalic.IsInit());
}
