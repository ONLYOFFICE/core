#include "Test59708.h"

#include "../../../../../OdfFile/Reader/Format/office_text.h"
#include "../../../../../OdfFile/Reader/Format/text_elements.h"
#include "../../../../../OdfFile/Reader/Format/odfcontext.h"
#include "../../../../../OdfFile/Reader/Format/styles.h"
#include "../../../../../OdfFile/Reader/Format/style_text_properties.h"

TestEnv59708* g_TestEnv59708 = (TestEnv59708*)testing::AddGlobalTestEnvironment(new TestEnv59708);

TestEnv59708::TestEnv59708()
	: DOCX2ODT_ConvertsionEnvironment(L"ExampleFiles/59708.docx")
{ }

void Test59708::SetUp()
{
	Odf = g_TestEnv59708->GetDocument();
}

TEST_F(Test59708, Odf)
{
	EXPECT_TRUE(Odf);
}

TEST_F(Test59708, content)
{
	cpdoccore::odf_reader::office_document_content* content = g_TestEnv59708->GetContent();
	EXPECT_TRUE(content);
}

TEST_F(Test59708, body)
{
	cpdoccore::odf_reader::office_body* body = g_TestEnv59708->GetBody();
	EXPECT_TRUE(body);
}

TEST_F(Test59708, body_text)
{
	cpdoccore::odf_reader::office_body* body = g_TestEnv59708->GetBody();
	ASSERT_TRUE(body);

	cpdoccore::odf_reader::office_text* text = dynamic_cast<cpdoccore::odf_reader::office_text*>(body->content());
	EXPECT_TRUE(text);
}

TEST_F(Test59708, body_text_content_size)
{
	using namespace cpdoccore::odf_reader;

	office_body* body = g_TestEnv59708->GetBody();
	ASSERT_TRUE(body);

	office_text* text = dynamic_cast<cpdoccore::odf_reader::office_text*>(body->content());
	ASSERT_TRUE(text);

	office_element_ptr_array text_content = text->get_content();

	const size_t expected_content_size = 23;

	EXPECT_EQ(text_content.size(), expected_content_size);
}

TEST_F(Test59708, body_text_empty_paragraph_font_size_1)
{
	using namespace cpdoccore;
	using namespace cpdoccore::odf_reader;
	

	office_body* body = g_TestEnv59708->GetBody();
	ASSERT_TRUE(body);

	office_text* text = dynamic_cast<odf_reader::office_text*>(body->content());
	ASSERT_TRUE(text);

	office_element_ptr_array text_content = text->get_content();
	const size_t empty_paragraph_index = 3;

	text::p* par = dynamic_cast<text::p*>(text_content[empty_paragraph_index].get());
	ASSERT_TRUE(par);
	
	const std::wstring& text_style_name = par->paragraph_.attrs_.text_style_name_;

	style_instance* style = Odf->odf_context().styleContainer().style_by_name(text_style_name, odf_types::style_family::Paragraph, false);
	ASSERT_TRUE(style);

	const _CP_OPT(odf_types::font_size)& font_size = style->content()->get_style_text_properties()->content_.fo_font_size_;
	ASSERT_TRUE(font_size);

	const int expected_font_size_pt = 24;

	EXPECT_EQ(font_size->get_length().get_value_unit(odf_types::length::pt), expected_font_size_pt);
}

TEST_F(Test59708, body_text_empty_paragraph_font_size_2)
{
	using namespace cpdoccore;
	using namespace cpdoccore::odf_reader;


	office_body* body = g_TestEnv59708->GetBody();
	ASSERT_TRUE(body);

	office_text* text = dynamic_cast<odf_reader::office_text*>(body->content());
	ASSERT_TRUE(text);

	office_element_ptr_array text_content = text->get_content();
	const size_t empty_paragraph_index = 4;

	text::p* par = dynamic_cast<text::p*>(text_content[empty_paragraph_index].get());
	ASSERT_TRUE(par);

	const std::wstring& text_style_name = par->paragraph_.attrs_.text_style_name_;

	style_instance* style = Odf->odf_context().styleContainer().style_by_name(text_style_name, odf_types::style_family::Paragraph, false);
	ASSERT_TRUE(style);

	const _CP_OPT(odf_types::font_size)& font_size = style->content()->get_style_text_properties()->content_.fo_font_size_;
	ASSERT_TRUE(font_size);

	const int expected_font_size_pt = 34;

	EXPECT_EQ(font_size->get_length().get_value_unit(odf_types::length::pt), expected_font_size_pt);
}