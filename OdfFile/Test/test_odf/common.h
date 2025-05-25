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

#pragma once

#include "gtest/gtest.h"

//#include "Reader/Converter/pptx_conversion_context.h"
//#include "Reader/Format/odf_document.h"
//
//#include "Writer/Converter/Oox2OdfConverter.h"
//#include "Writer/Format/odp_conversion_context.h"
//#include "Writer/Format/anim_elements.h"
//#include "Writer/Format/draw_page.h"
//
//#include <boost/shared_ptr.hpp>
#include <string>

#define CH_DIR(x) FILE_SEPARATOR_STR + _T(x)

//boost::shared_ptr<cpdoccore::odf_reader::odf_document> ReadOdfDocument(const std::wstring& from, const std::wstring& temp, const std::wstring& tempUnpackedOdf);
//boost::shared_ptr<cpdoccore::oox::pptx_conversion_context> Convert(boost::shared_ptr<cpdoccore::odf_reader::odf_document> inputOdf);
//
//class ODP2OOX_AnimationEnvironment : public ::testing::Environment
//{
//public:
//	ODP2OOX_AnimationEnvironment(const std::wstring& exampleFilename, boost::shared_ptr<cpdoccore::odf_reader::odf_document>* doc, boost::shared_ptr<cpdoccore::oox::pptx_conversion_context>* context);
//	~ODP2OOX_AnimationEnvironment();
//
//	void SetUp() override;
//	void TearDown() override;
//
//private:
//	std::wstring sExampleFilename;
//	std::wstring sFrom;
//	std::wstring sTemp;
//	std::wstring sTempUnpackedOdf;
//
//	boost::shared_ptr<cpdoccore::odf_reader::odf_document>* mDocument;
//	boost::shared_ptr<cpdoccore::oox::pptx_conversion_context>* mContext;
//};
//
//class OOX2ODP_AnimationEnvironment : public testing::Environment
//{
//public:
//	OOX2ODP_AnimationEnvironment(const std::wstring& exampleFilename, 
//		boost::shared_ptr<Oox2Odf::Converter>* converter,
//		cpdoccore::odf_writer::odp_conversion_context** context);
//
//	void SetUp() override;
//	void TearDown() override;
//
//private:
//	std::wstring mExampleFilename;
//	std::wstring mFrom;
//	std::wstring mTemp;
//	std::wstring mTempUnpackedOox;
//
//	boost::shared_ptr<Oox2Odf::Converter>* mConverter;
//	cpdoccore::odf_writer::odp_conversion_context** mContext;
//};
//
//class OOX2ODP_AnimationTest : public testing::Test 
//{
//public:
//	const cpdoccore::odf_writer::anim_par* GetTimingRoot();
//	const cpdoccore::odf_writer::anim_seq* GetMainSequence();
//	const cpdoccore::odf_writer::anim_par* GetMainSequenceParByIndex(size_t index);
//	const cpdoccore::odf_writer::anim_par* GetInnerPar(const cpdoccore::odf_writer::anim_par* par);
//	const cpdoccore::odf_writer::anim_par* GetInnermostPar(const cpdoccore::odf_writer::anim_par* par);
//
//	const cpdoccore::odf_writer::odp_conversion_context* GetContext() const;
//
//	template<typename T>
//	const T* GetAnimationBehaviourByIndex(const cpdoccore::odf_writer::anim_par* par, size_t index)
//	{
//		using namespace cpdoccore::odf_writer;
//
//		if (!par)
//			return nullptr;
//
//		if (par->content_.size() <= index)
//			return nullptr;
//
//		T* behaviour = dynamic_cast<T*>(par->content_[index].get());
//		if (!behaviour)
//			return nullptr;
//
//		return behaviour;
//	}
//
//protected:
//	const Oox2Odf::Converter* mConverter;
//	cpdoccore::odf_writer::odp_conversion_context* mContext;
//};

class ODT2DOCX_ConversionEnvironment : public testing::Environment
{
public:
	ODT2DOCX_ConversionEnvironment(const std::wstring& filename);

	virtual void SetUp() override;
	virtual void TearDown() override;

private:
	std::wstring mFilename;

};
