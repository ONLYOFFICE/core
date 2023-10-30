/*
 * (c) Copyright Ascensio System SIA 2010-2023
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */

#include "interactions.h"

#include "Writer/Format/anim_elements.h"

boost::shared_ptr<Oox2Odf::Converter> OOX2ODP_InteractionAnimationEnvironment::mConverter;
cpdoccore::odf_writer::odp_conversion_context* OOX2ODP_InteractionAnimationEnvironment::mContext;

OOX2ODP_InteractionAnimationEnvironment::OOX2ODP_InteractionAnimationEnvironment()
	: OOX2ODP_AnimationEnvironment(L"interaction.pptx", &mConverter, &mContext)
{
}

cpdoccore::odf_writer::odp_conversion_context* OOX2ODP_InteractionAnimationEnvironment::GetContext()
{
	return mContext;
}

void OOX2ODP_InteractionAnimationTest::SetUp()
{
	mContext = OOX2ODP_InteractionAnimationEnvironment::GetContext();
}

TEST_F(OOX2ODP_InteractionAnimationTest, timing_root)
{
	using namespace cpdoccore::odf_writer;
	using namespace cpdoccore::odf_types;

	const anim_par* root = GetTimingRoot();
	EXPECT_NE(root, nullptr);
}

TEST_F(OOX2ODP_InteractionAnimationTest, timing_root_duration)
{
	using namespace cpdoccore::odf_writer;
	using namespace cpdoccore::odf_types;

	const anim_par* root = GetTimingRoot();
	ASSERT_NE(root, nullptr);

	const int durExp = -1;

	EXPECT_EQ(root->attlist_.smil_dur_.value().get_value(), durExp);
}

TEST_F(OOX2ODP_InteractionAnimationTest, timing_root_node_type)
{
	using namespace cpdoccore::odf_writer;
	using namespace cpdoccore::odf_types;

	const anim_par* root = GetTimingRoot();
	ASSERT_NE(root, nullptr);

	const presentation_node_type nodeTypeExp = presentation_node_type::parse(L"timing-root");

	EXPECT_EQ(root->attlist_.presentation_node_type_.value().get_type(), nodeTypeExp.get_type());
}

TEST_F(OOX2ODP_InteractionAnimationTest, interactive_seq)
{
	using namespace cpdoccore::odf_writer;
	using namespace cpdoccore::odf_types;

	const anim_par* root = GetTimingRoot();
	ASSERT_NE(root, nullptr);
	const anim_seq* interactive = dynamic_cast<anim_seq*>(root->anim_seq_array_[0].get());

	EXPECT_NE(interactive, nullptr);
}

TEST_F(OOX2ODP_InteractionAnimationTest, interactive_seq_node_type)
{
	using namespace cpdoccore::odf_writer;
	using namespace cpdoccore::odf_types;

	const anim_par* root = GetTimingRoot();
	ASSERT_NE(root, nullptr);
	const anim_seq* interactive = dynamic_cast<anim_seq*>(root->anim_seq_array_[0].get());
	ASSERT_NE(interactive, nullptr);

	const presentation_node_type nodeTypeExp = presentation_node_type::parse(L"interactive-sequence");

	EXPECT_NE(interactive, nullptr);
}

TEST_F(OOX2ODP_InteractionAnimationTest, interactive_seq_restart)
{
	using namespace cpdoccore::odf_writer;
	using namespace cpdoccore::odf_types;

	const anim_par* root = GetTimingRoot();
	ASSERT_NE(root, nullptr);
	const anim_seq* interactive = dynamic_cast<anim_seq*>(root->anim_seq_array_[0].get());
	ASSERT_NE(interactive, nullptr);

	const std::wstring restartExp = L"";

	EXPECT_NE(interactive->attlist_.smil_restart_.value(), restartExp);
}

TEST_F(OOX2ODP_InteractionAnimationTest, interactive_seq_fill)
{
	using namespace cpdoccore::odf_writer;
	using namespace cpdoccore::odf_types;

	const anim_par* root = GetTimingRoot();
	ASSERT_NE(root, nullptr);
	const anim_seq* interactive = dynamic_cast<anim_seq*>(root->anim_seq_array_[0].get());
	ASSERT_NE(interactive, nullptr);

	const smil_fill fillExp = smil_fill::_remove;

	EXPECT_NE(interactive->attlist_.smil_fill_.value().get_type(), fillExp.get_type());
}

TEST_F(OOX2ODP_InteractionAnimationTest, interactive_seq_begin)
{
	using namespace cpdoccore::odf_writer;
	using namespace cpdoccore::odf_types;

	const anim_par* root = GetTimingRoot();
	ASSERT_NE(root, nullptr);
	const anim_seq* interactive = dynamic_cast<anim_seq*>(root->anim_seq_array_[0].get());
	ASSERT_NE(interactive, nullptr);

	const std::wstring restartExp = L"";

	EXPECT_NE(interactive->attlist_.smil_begin_.value(), restartExp);
}