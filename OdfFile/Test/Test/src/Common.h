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

#include <gtest/gtest.h>
#include <boost/smart_ptr.hpp>

#include "../../../../OOXML/DocxFormat/Docx.h"
#include "../../../../OOXML/DocxFormat/DocxFlat.h"
#include "../../../../OOXML/DocxFormat/Document.h"
#include "../../../../OOXML/DocxFormat/Logic/Run.h"
#include "../../../../OOXML/DocxFormat/Logic/Paragraph.h"
#include "../../../../OOXML/DocxFormat/Logic/ParagraphProperty.h"
#include "../../../../OOXML/DocxFormat/Logic/Run.h"
#include "../../../../OOXML/DocxFormat/Logic/RunProperty.h"

#include "../../../../OdfFile/Reader/Format/odf_document.h"
#include "../../../../OdfFile/Reader/Format/odf_document_impl.h"
#include "../../../../OdfFile/Reader/Format/office_document.h"
#include "../../../../OdfFile/Reader/Format/office_body.h"
#include "../../../../OdfFile/Reader/Format/office_presentation.h"
#include "../../../../OdfFile/Reader/Format/draw_page.h"

class ODT2DOCX_ConversionEnvironment : public testing::Environment
{
public:
	ODT2DOCX_ConversionEnvironment(const std::wstring& filename);

	OOX::CDocx* GetDocument();

	virtual void SetUp() override;
	virtual void TearDown() override;

private:
	std::wstring mFilename;
	OOX::CDocx* mDocx;
};

class DOCX2ODT_ConvertsionEnvironment : public testing::Environment
{
public:
	DOCX2ODT_ConvertsionEnvironment(const std::wstring& filename);

	cpdoccore::odf_reader::odf_document*			GetDocument();
	cpdoccore::odf_reader::office_document_content* GetContent();
	cpdoccore::odf_reader::office_body*				GetBody();

	virtual void SetUp() override;
	virtual void TearDown() override;

private:
	std::wstring mFilename;
	boost::shared_ptr<cpdoccore::odf_reader::odf_document> mOdf;
};

class PPTX2ODP_ConversionEnvironment : public testing::Environment
{
public:
	PPTX2ODP_ConversionEnvironment(const std::wstring& filename);

	cpdoccore::odf_reader::odf_document*			GetDocument();
	cpdoccore::odf_reader::office_document_content* GetContent();
	cpdoccore::odf_reader::office_body*				GetBody();
	cpdoccore::odf_reader::draw_page*				GetPage(size_t page_index);

	virtual void SetUp() override;
	virtual void TearDown() override;

private:
	std::wstring mFilename;
	boost::shared_ptr<cpdoccore::odf_reader::odf_document> mOdf;
};