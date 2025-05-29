#include "Test69238.h"

#include "../../../../../OdfFile/Reader/Format/draw_shapes.h"
#include "../../../../../OdfFile/Reader/Format/odfcontext.h"
#include "../../../../../OdfFile/Reader/Format/style_graphic_properties.h"

TestEnv69238* g_TestEnv69238 = (TestEnv69238*)testing::AddGlobalTestEnvironment(new TestEnv69238);

TestEnv69238::TestEnv69238() 
	: PPTX2ODP_ConversionEnvironment(L"ExampleFiles/69238.pptx")
{ }

void Test69238::SetUp()
{
	Odf = g_TestEnv69238->GetDocument();
}

TEST_F(Test69238, odf)
{
	EXPECT_TRUE(Odf);
}

TEST_F(Test69238, page_7)
{
	using namespace cpdoccore::odf_reader;

	draw_page* page = g_TestEnv69238->GetPage(7);
	EXPECT_TRUE(page);
}

TEST_F(Test69238, page_7_content_size)
{
	using namespace cpdoccore::odf_reader;

	draw_page* page = g_TestEnv69238->GetPage(7);
	ASSERT_TRUE(page);

	const size_t expected_page7_content_size = 19;

	EXPECT_EQ(page->content_.size(), expected_page7_content_size);
}

TEST_F(Test69238, page_7_arrow_1)
{
	using namespace cpdoccore::odf_reader;

	draw_page* page = g_TestEnv69238->GetPage(7);
	ASSERT_TRUE(page);

	const size_t arrow_shape_index = 1;
	draw_custom_shape* arrow = dynamic_cast<draw_custom_shape*>(page->content_[arrow_shape_index].get());

	EXPECT_TRUE(arrow);
}

TEST_F(Test69238, page_7_arrow_1_geometry)
{
	using namespace cpdoccore::odf_reader;

	draw_page* page = g_TestEnv69238->GetPage(7);
	ASSERT_TRUE(page);

	const size_t arrow_shape_index = 1;
	draw_custom_shape* arrow = dynamic_cast<draw_custom_shape*>(page->content_[arrow_shape_index].get());

	ASSERT_TRUE(arrow);

	draw_enhanced_geometry* geom = dynamic_cast<draw_enhanced_geometry*>(arrow->enhanced_geometry_.get());

	EXPECT_TRUE(geom);
}

TEST_F(Test69238, page_7_arrow_1_geometry_path_init)
{
	using namespace cpdoccore::odf_reader;

	draw_page* page = g_TestEnv69238->GetPage(7);
	ASSERT_TRUE(page);

	const size_t arrow_shape_index = 1;
	draw_custom_shape* arrow = dynamic_cast<draw_custom_shape*>(page->content_[arrow_shape_index].get());

	ASSERT_TRUE(arrow);

	draw_enhanced_geometry* geom = dynamic_cast<draw_enhanced_geometry*>(arrow->enhanced_geometry_.get());

	EXPECT_TRUE(geom->attlist_.draw_enhanced_path_.is_initialized());
}

TEST_F(Test69238, page_7_arrow_1_geometry_path)
{
	using namespace cpdoccore::odf_reader;

	draw_page* page = g_TestEnv69238->GetPage(7);
	ASSERT_TRUE(page);

	const size_t arrow_shape_index = 1;
	draw_custom_shape* arrow = dynamic_cast<draw_custom_shape*>(page->content_[arrow_shape_index].get());

	ASSERT_TRUE(arrow);

	draw_enhanced_geometry* geom = dynamic_cast<draw_enhanced_geometry*>(arrow->enhanced_geometry_.get());

	const std::wstring expected_path = L"M ?f103 ?f108 G ?f145 ?f146 ?f519 ?f520 L ?f180 ?f185 G ?f222 ?f223 ?f521 ?f522 L ?f226 ?f227 ?f228 ?f229 ?f230 ?f231 ?f263 ?f268 G ?f303 ?f304 ?f523 ?f524 L ?f338 ?f343 G ?f378 ?f379 ?f525 ?f526 Z N M ?f413 ?f418 G ?f455 ?f456 ?f527 ?f528 F N ";

	EXPECT_EQ(geom->attlist_.draw_enhanced_path_.get(), expected_path);
}

TEST_F(Test69238, page_7_arrow_style_name)
{
	using namespace cpdoccore::odf_reader;

	draw_page* page = g_TestEnv69238->GetPage(7);
	ASSERT_TRUE(page);

	const size_t arrow_shape_index = 1;
	draw_custom_shape* arrow = dynamic_cast<draw_custom_shape*>(page->content_[arrow_shape_index].get());

	ASSERT_TRUE(arrow);
	EXPECT_TRUE(arrow->common_draw_attlists_.shape_with_text_and_styles_.common_shape_draw_attlist_.draw_style_name_);
}

TEST_F(Test69238, page_7_arrow_style)
{
	using namespace cpdoccore::odf_reader;
	using namespace cpdoccore::odf_types;

	draw_page* page = g_TestEnv69238->GetPage(7);
	ASSERT_TRUE(page);

	const size_t arrow_shape_index = 1;
	draw_custom_shape* arrow = dynamic_cast<draw_custom_shape*>(page->content_[arrow_shape_index].get());

	ASSERT_TRUE(arrow);

	const std::wstring style_name = arrow->common_draw_attlists_.shape_with_text_and_styles_.common_shape_draw_attlist_.draw_style_name_.get();
	style_instance* style_inst = Odf->get_impl()->odf_context().styleContainer().style_by_name(style_name, style_family::Graphic, false);

	EXPECT_TRUE(style_inst);
}

TEST_F(Test69238, page_7_arrow_style_graphic_properties)
{
	using namespace cpdoccore::odf_reader;
	using namespace cpdoccore::odf_types;

	draw_page* page = g_TestEnv69238->GetPage(7);
	ASSERT_TRUE(page);

	const size_t arrow_shape_index = 1;
	draw_custom_shape* arrow = dynamic_cast<draw_custom_shape*>(page->content_[arrow_shape_index].get());

	ASSERT_TRUE(arrow);

	const std::wstring style_name = arrow->common_draw_attlists_.shape_with_text_and_styles_.common_shape_draw_attlist_.draw_style_name_.get();
	style_instance* style_inst = Odf->get_impl()->odf_context().styleContainer().style_by_name(style_name, style_family::Graphic, false);
	ASSERT_TRUE(style_inst);

	graphic_format_properties* graphic_props = style_inst->content()->get_graphic_properties();

	EXPECT_TRUE(graphic_props);
}

TEST_F(Test69238, page_7_arrow_fill)
{
	using namespace cpdoccore::odf_reader;
	using namespace cpdoccore::odf_types;

	draw_page* page = g_TestEnv69238->GetPage(7);
	ASSERT_TRUE(page);

	const size_t arrow_shape_index = 1;
	draw_custom_shape* arrow = dynamic_cast<draw_custom_shape*>(page->content_[arrow_shape_index].get());

	ASSERT_TRUE(arrow);

	const std::wstring style_name = arrow->common_draw_attlists_.shape_with_text_and_styles_.common_shape_draw_attlist_.draw_style_name_.get();
	style_instance* style_inst = Odf->get_impl()->odf_context().styleContainer().style_by_name(style_name, style_family::Graphic, false);
	ASSERT_TRUE(style_inst);

	graphic_format_properties* graphic_props = style_inst->content()->get_graphic_properties();

	EXPECT_TRUE(graphic_props->common_draw_fill_attlist_.draw_fill_color_);
}

TEST_F(Test69238, page_7_arrow_fill_color)
{
	using namespace cpdoccore::odf_reader;
	using namespace cpdoccore::odf_types;

	draw_page* page = g_TestEnv69238->GetPage(7);
	ASSERT_TRUE(page);

	const size_t arrow_shape_index = 1;
	draw_custom_shape* arrow = dynamic_cast<draw_custom_shape*>(page->content_[arrow_shape_index].get());

	ASSERT_TRUE(arrow);

	const std::wstring style_name = arrow->common_draw_attlists_.shape_with_text_and_styles_.common_shape_draw_attlist_.draw_style_name_.get();
	style_instance* style_inst = Odf->get_impl()->odf_context().styleContainer().style_by_name(style_name, style_family::Graphic, false);
	ASSERT_TRUE(style_inst);

	graphic_format_properties* graphic_props = style_inst->content()->get_graphic_properties();

	const color expected_color = color::parse(L"#BBE0E3");

	EXPECT_EQ(graphic_props->common_draw_fill_attlist_.draw_fill_color_.get(), expected_color);
}

TEST_F(Test69238, page_15)
{
	using namespace cpdoccore::odf_reader;

	draw_page* page = g_TestEnv69238->GetPage(15);
	EXPECT_TRUE(page);
}

TEST_F(Test69238, page_15_shape)
{
	using namespace cpdoccore::odf_reader;

	draw_page* page = g_TestEnv69238->GetPage(15);
	ASSERT_TRUE(page);

	const size_t shape_index = 2;
	draw_custom_shape* shape = dynamic_cast<draw_custom_shape*>(page->content_[shape_index].get());

	EXPECT_TRUE(shape);
}

TEST_F(Test69238, page_15_custom_shape_with_multiple_path)
{
	using namespace cpdoccore::odf_reader;

	draw_page* page = g_TestEnv69238->GetPage(15);
	ASSERT_TRUE(page);

	const size_t arrow_shape_index = 2;
	draw_custom_shape* arrow = dynamic_cast<draw_custom_shape*>(page->content_[arrow_shape_index].get());

	ASSERT_TRUE(arrow);

	draw_enhanced_geometry* geom = dynamic_cast<draw_enhanced_geometry*>(arrow->enhanced_geometry_.get());

	const std::wstring expected_path = L"M ?f7 ?f8 L ?f9 ?f10 ?f11 ?f12 F N M ?f13 ?f14 L ?f15 ?f16 ?f17 ?f18 ?f19 ?f20 Z N ";

	EXPECT_EQ(geom->attlist_.draw_enhanced_path_.get(), expected_path);
}

TEST_F(Test69238, page_19)
{
	using namespace cpdoccore::odf_reader;

	draw_page* page = g_TestEnv69238->GetPage(19);
	EXPECT_TRUE(page);
}

TEST_F(Test69238, page_19_cloud_shape_shadow_visibility)
{
	using namespace cpdoccore::odf_reader;
	using namespace cpdoccore::odf_types;

	draw_page* page = g_TestEnv69238->GetPage(19);
	ASSERT_TRUE(page);

	const size_t shape_index = 6;
	draw_custom_shape* shape = dynamic_cast<draw_custom_shape*>(page->content_[shape_index].get());
	ASSERT_TRUE(shape);

	const std::wstring style_name = shape->common_draw_attlists_.shape_with_text_and_styles_.common_shape_draw_attlist_.draw_style_name_.get();
	style_instance* style_inst = Odf->get_impl()->odf_context().styleContainer().style_by_name(style_name, style_family::Graphic, false);
	ASSERT_TRUE(style_inst);

	graphic_format_properties* graphic_props = style_inst->content()->get_graphic_properties();

	const shadow_type1 expected_shadow = shadow_type1::parse(L"visible");

	EXPECT_EQ(graphic_props->common_shadow_attlist_.draw_shadow_.get().get_type(), expected_shadow.get_type());
}

TEST_F(Test69238, page_19_cloud_shape_shadow_color)
{
	using namespace cpdoccore::odf_reader;
	using namespace cpdoccore::odf_types;

	draw_page* page = g_TestEnv69238->GetPage(19);
	ASSERT_TRUE(page);

	const size_t shape_index = 6;
	draw_custom_shape* shape = dynamic_cast<draw_custom_shape*>(page->content_[shape_index].get());
	ASSERT_TRUE(shape);

	const std::wstring style_name = shape->common_draw_attlists_.shape_with_text_and_styles_.common_shape_draw_attlist_.draw_style_name_.get();
	style_instance* style_inst = Odf->get_impl()->odf_context().styleContainer().style_by_name(style_name, style_family::Graphic, false);
	ASSERT_TRUE(style_inst);

	graphic_format_properties* graphic_props = style_inst->content()->get_graphic_properties();

	const color expected_shadow_color = color::parse(L"#808080");

	EXPECT_EQ(graphic_props->common_shadow_attlist_.draw_shadow_color_.get(), expected_shadow_color);
}

TEST_F(Test69238, page_19_cloud_shape_shadow_offset_x)
{
	using namespace cpdoccore::odf_reader;
	using namespace cpdoccore::odf_types;

	draw_page* page = g_TestEnv69238->GetPage(19);
	ASSERT_TRUE(page);

	const size_t shape_index = 6;
	draw_custom_shape* shape = dynamic_cast<draw_custom_shape*>(page->content_[shape_index].get());
	ASSERT_TRUE(shape);

	const std::wstring style_name = shape->common_draw_attlists_.shape_with_text_and_styles_.common_shape_draw_attlist_.draw_style_name_.get();
	style_instance* style_inst = Odf->get_impl()->odf_context().styleContainer().style_by_name(style_name, style_family::Graphic, false);
	ASSERT_TRUE(style_inst);

	graphic_format_properties* graphic_props = style_inst->content()->get_graphic_properties();

	const length expected_shadow_offset_x = length::parse(L"0.2117cm");

	EXPECT_EQ(graphic_props->common_shadow_attlist_.draw_shadow_offset_x_.get(), expected_shadow_offset_x);
}

TEST_F(Test69238, page_19_cloud_shape_shadow_offset_y)
{
	using namespace cpdoccore::odf_reader;
	using namespace cpdoccore::odf_types;

	draw_page* page = g_TestEnv69238->GetPage(19);
	ASSERT_TRUE(page);

	const size_t shape_index = 6;
	draw_custom_shape* shape = dynamic_cast<draw_custom_shape*>(page->content_[shape_index].get());
	ASSERT_TRUE(shape);

	const std::wstring style_name = shape->common_draw_attlists_.shape_with_text_and_styles_.common_shape_draw_attlist_.draw_style_name_.get();
	style_instance* style_inst = Odf->get_impl()->odf_context().styleContainer().style_by_name(style_name, style_family::Graphic, false);
	ASSERT_TRUE(style_inst);

	graphic_format_properties* graphic_props = style_inst->content()->get_graphic_properties();

	const length expected_shadow_offset_y = length::parse(L"0.2117cm");

	EXPECT_EQ(graphic_props->common_shadow_attlist_.draw_shadow_offset_y_.get(), expected_shadow_offset_y);
}

TEST_F(Test69238, page_25)
{
	using namespace cpdoccore::odf_reader;
	using namespace cpdoccore::odf_types;

	draw_page* page = g_TestEnv69238->GetPage(25);
	EXPECT_TRUE(page);
}

TEST_F(Test69238, page_25_rotation)
{
	using namespace cpdoccore::odf_reader;
	using namespace cpdoccore::odf_types;

	draw_page* page = g_TestEnv69238->GetPage(25);
	ASSERT_TRUE(page);

	const size_t shape_index = 3;
	draw_custom_shape* shape = dynamic_cast<draw_custom_shape*>(page->content_[shape_index].get());
	ASSERT_TRUE(shape);

	const std::wstring expected_rotation = L"rotate(0.65839055372982092)";
	const std::wstring value = shape->common_draw_attlists_.shape_with_text_and_styles_.common_shape_draw_attlist_.draw_transform_.get();

	EXPECT_TRUE(value.find(expected_rotation) != std::wstring::npos);
}