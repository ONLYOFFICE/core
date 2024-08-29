#include "Test57197.h"

TestEnv57197* g_TestEnv44363 = (TestEnv57197*)testing::AddGlobalTestEnvironment(new TestEnv57197);

TestEnv57197::TestEnv57197()
	: ODT2DOCX_ConversionEnvironment(L"ExampleFiles/57197.odt")
{ }

void Test57197::SetUp()
{
	Docx = g_TestEnv44363->GetDocument();
}

TEST_F(Test57197, Docx)
{
	EXPECT_TRUE(Docx);
}

TEST_F(Test57197, document)
{
	EXPECT_TRUE(Docx->m_oMain.document);
}

TEST_F(Test57197, document_items_size)
{
	const size_t expected_size = 2;
	EXPECT_EQ(Docx->m_oMain.document->m_arrItems.size(), expected_size);
}

TEST_F(Test57197, document_first_paragraph)
{
	OOX::Logic::CParagraph* p = dynamic_cast<OOX::Logic::CParagraph*>(Docx->m_oMain.document->m_arrItems[0]);
	EXPECT_TRUE(p);
}

TEST_F(Test57197, document_first_paragraph_properties)
{
	OOX::Logic::CParagraph* p = dynamic_cast<OOX::Logic::CParagraph*>(Docx->m_oMain.document->m_arrItems[0]);
	EXPECT_TRUE(p->m_oParagraphProperty);
}

TEST_F(Test57197, document_first_paragraph_properties_section)
{
	OOX::Logic::CParagraph* p = dynamic_cast<OOX::Logic::CParagraph*>(Docx->m_oMain.document->m_arrItems[0]);
	ASSERT_TRUE(p);

	EXPECT_TRUE(p->m_oParagraphProperty->m_oSectPr.IsInit());
}

TEST_F(Test57197, document_first_paragraph_properties_section_page_size)
{
	OOX::Logic::CParagraph* p = dynamic_cast<OOX::Logic::CParagraph*>(Docx->m_oMain.document->m_arrItems[0]);
	ASSERT_TRUE(p);

	ASSERT_TRUE(p->m_oParagraphProperty->m_oSectPr.IsInit());
	const OOX::Logic::CSectionProperty& sectProps = p->m_oParagraphProperty->m_oSectPr.get();

	EXPECT_TRUE(p->m_oParagraphProperty->m_oSectPr->m_oPgSz.IsInit());
}

TEST_F(Test57197, document_first_paragraph_properties_section_page_size_width)
{
	OOX::Logic::CParagraph* p = dynamic_cast<OOX::Logic::CParagraph*>(Docx->m_oMain.document->m_arrItems[0]);
	ASSERT_TRUE(p);

	ASSERT_TRUE(p->m_oParagraphProperty->m_oSectPr.IsInit());
	const OOX::Logic::CSectionProperty& sectProps = p->m_oParagraphProperty->m_oSectPr.get();

	ASSERT_TRUE(sectProps.m_oPgSz.IsInit());
	const ComplexTypes::Word::CPageSz& pageSize = sectProps.m_oPgSz.get();

	const int expected_width_in_twips = 7200;
	EXPECT_EQ(pageSize.m_oW->ToTwips(), expected_width_in_twips);
}

TEST_F(Test57197, document_first_paragraph_properties_section_page_size_height)
{
	OOX::Logic::CParagraph* p = dynamic_cast<OOX::Logic::CParagraph*>(Docx->m_oMain.document->m_arrItems[0]);
	ASSERT_TRUE(p);

	ASSERT_TRUE(p->m_oParagraphProperty->m_oSectPr.IsInit());
	const OOX::Logic::CSectionProperty& sectProps = p->m_oParagraphProperty->m_oSectPr.get();

	ASSERT_TRUE(sectProps.m_oPgSz.IsInit());
	const ComplexTypes::Word::CPageSz& pageSize = sectProps.m_oPgSz.get();

	const int expected_height_in_twips = 4320;
	EXPECT_EQ(pageSize.m_oH->ToTwips(), expected_height_in_twips);
}

TEST_F(Test57197, document_section)
{
	EXPECT_TRUE(Docx->m_oMain.document->m_oSectPr.IsInit());
}

TEST_F(Test57197, document_section_page_size)
{
	const OOX::Logic::CSectionProperty& sectProps = Docx->m_oMain.document->m_oSectPr.get();
	
	EXPECT_TRUE(sectProps.m_oPgSz.IsInit());
}

TEST_F(Test57197, document_section_page_size_width)
{
	const OOX::Logic::CSectionProperty& sectProps = Docx->m_oMain.document->m_oSectPr.get();
	const ComplexTypes::Word::CPageSz& pageSize = sectProps.m_oPgSz.get();

	const int expected_width_in_twips = 12240;

	EXPECT_TRUE(pageSize.m_oW->ToTwips(), expected_width_in_twips);
}

TEST_F(Test57197, document_section_page_size_height)
{
	const OOX::Logic::CSectionProperty& sectProps = Docx->m_oMain.document->m_oSectPr.get();
	const ComplexTypes::Word::CPageSz& pageSize = sectProps.m_oPgSz.get();

	const int expected_height_in_twips = 15840;

	EXPECT_TRUE(pageSize.m_oH->ToTwips(), expected_height_in_twips);
}