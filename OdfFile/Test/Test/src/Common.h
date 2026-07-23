/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
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