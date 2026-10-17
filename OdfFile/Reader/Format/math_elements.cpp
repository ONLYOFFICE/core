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

#include "math_elements.h"
#include "../Converter/StarMath2OOXML/shakey.h"
#include "../Converter/StarMath2OOXML/conversionmathformula.h"
#include "../../../OfficeUtils/src/OfficeUtils.h"
#include "../../../DesktopEditor/common/File.h"

namespace cpdoccore {

    using namespace odf_types;

    namespace odf_reader {

        //---------------------------------------------------------------
        const wchar_t* office_math_element::ns = L"math";
        const wchar_t* office_math_element::name = L"math-element";
        //---------------------------------------------------------------
        const wchar_t* office_math::ns = L"math";
        const wchar_t* office_math::name = L"math";

        //----------------------------------------------------------------------------------------------------

        void office_math::add_attributes(const xml::attributes_wc_ptr& Attributes)
        {

        }

        void office_math::add_child_element(xml::sax* Reader, const std::wstring& Ns, const std::wstring& Name)
        {
            if CP_CHECK_NAME1(L"semantics")
            {
                CP_CREATE_ELEMENT(semantics_);
            }
            else
            {//easy math ml 
                if (!semantics_)
                {
                    semantics_ = office_element_creator::get()->create(math_semantics::ns, math_semantics::name, getContext(), false);
                }
                math_semantics* math_semantics_ = dynamic_cast<math_semantics*>(semantics_.get());
                if (math_semantics_)
                {
                    CP_CREATE_ELEMENT(math_semantics_->content_);
                }
            }
        }


        void office_math::oox_convert(oox::math_context& Context, int iTypeConversion)
        {
            if (semantics_)
            {
                office_math_element* math_element = dynamic_cast<office_math_element*>(semantics_.get());
                math_element->oox_convert(Context, iTypeConversion);
            }
        }

        //----------------------------------------------------------------------------------------------------
        const wchar_t* math_semantics::ns = L"math";
        const wchar_t* math_semantics::name = L"semantics";
        //----------------------------------------------------------------------------------------------------

        void math_semantics::add_attributes(const xml::attributes_wc_ptr& Attributes)
        {
        }

        void math_semantics::add_child_element(xml::sax* Reader, const std::wstring& Ns, const std::wstring& Name)
        {
            if CP_CHECK_NAME1(L"annotation")
            {
                CP_CREATE_ELEMENT(annotation_);
            }
            else if (CP_CHECK_NAME1(L"signature"))
            {
                CP_CREATE_ELEMENT(signature_);
            }
            else
                CP_CREATE_ELEMENT(content_);

        }
        void math_semantics::oox_convert(oox::math_context& Context)
        {
            this->oox_convert(Context, 0);
        }
        void math_semantics::oox_convert(oox::math_context& Context, int iTypeConversion)
        {
            math_signature* pSignature = dynamic_cast<math_signature*>(signature_.get());
            math_annotation* annotation = dynamic_cast<math_annotation*>(annotation_.get());
            math_annotation_xml* annotation_xml = dynamic_cast<math_annotation_xml*>(annotation_.get());

            std::wstring annotation_text;
          
            if (annotation) annotation_text = annotation->text_;
            else if (annotation_xml) annotation_text = annotation_xml->text_;
           
            bool result = false;
           
            if (pSignature)
            {
                if (pSignature->binData_ && pSignature->alg_ == L"sha256" && HashSM::HashComparison(pSignature->shakey_, HashSM::HashingAnnotation(annotation_text, true)))
                {
                    int nLengthZip = 0;
                    BYTE* pDataZip = NULL;
                   
                    if (NSFile::CBase64Converter::Decode(pSignature->binData_->c_str(), pSignature->binData_->size(), pDataZip, nLengthZip))
                    {
                        if (nLengthZip > 4)
                        {
                            COfficeUtils OfficeUtils(NULL);

                            //uncompressed length
                            ULONG nLengthUtf8 = (ULONG)pDataZip[0] | ((ULONG)pDataZip[1] << 8) | ((ULONG)pDataZip[2] << 16) | ((ULONG)pDataZip[3] << 24);
                            BYTE* pDataUtf8 = new BYTE[nLengthUtf8 + 64];

                            if (OfficeUtils.Uncompress(pDataUtf8, &nLengthUtf8, pDataZip + 4, nLengthZip - 4) == S_OK)
                            {
                                std::wstring sOOXML = UTF8_TO_U(std::string((char*)pDataUtf8, nLengthUtf8));

                                Context.output_stream() << sOOXML;
                                result = true;
                            }
                            delete[]pDataUtf8; pDataUtf8 = NULL;
                        }
                        delete[]pDataZip; pDataZip = NULL;
                    }
                }
            }

            if (!annotation_text.empty() && !result)
            {
                result = true;

                StarMath::CStarMathConverter oConverterStarMath;

                oConverterStarMath.SetBaseFont(Context.base_font_name_);
                oConverterStarMath.SetBaseSize(Context.base_font_size_);
                oConverterStarMath.SetBaseAlignment(Context.base_alignment_);
                oConverterStarMath.SetBaseItalic(Context.base_font_italic_);
                oConverterStarMath.SetBaseBold(Context.base_font_bold_);

                std::wstring ws_conversion_result_sm_to_ooxml = oConverterStarMath.ConvertStarMathToOOXml(annotation_text, iTypeConversion);

                std::queue<StarMath::TFormulaSize> sizes = oConverterStarMath.GetFormulaSize();

                for (; !sizes.empty(); sizes.pop())
                {
                    if (sizes.front().m_iWidth > Context.width)
                        Context.width = sizes.front().m_iWidth;

                    Context.height += sizes.front().m_iHeight;
                }
                Context.output_stream() << ws_conversion_result_sm_to_ooxml;
            }

            if (!result)
            {
                Context.output_stream() << L"<m:oMathPara xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\">";
                Context.output_stream() << L"<m:oMath xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\">";
                for (size_t i = 0; i < content_.size(); i++)
                {
                    office_math_element* math_element = dynamic_cast<office_math_element*>(content_[i].get());
                    math_element->oox_convert(Context);
                }
                Context.output_stream() << L"</m:oMath>";
                Context.output_stream() << L"</m:oMathPara>";
            }
        }

        //----------------------------------------------------------------------------------------------------
        const wchar_t* math_annotation::ns = L"math";
        const wchar_t* math_annotation::name = L"annotation";
        //----------------------------------------------------------------------------------------------------

        void math_annotation::add_attributes(const xml::attributes_wc_ptr& Attributes)
        {
            // ver 2	
            CP_APPLY_ATTR(L"math:encoding", encoding_);

            // ver 3
            if (!encoding_)	CP_APPLY_ATTR(L"encoding", encoding_);

        }

        void math_annotation::add_child_element(xml::sax* Reader, const std::wstring& Ns, const std::wstring& Name)
        {
            CP_CREATE_ELEMENT(content_);

        }

        void math_annotation::add_text(const std::wstring& Text)
        {
            text_.append(Text);
        }

        //----------------------------------------------------------------------------------------------------
        const wchar_t* math_annotation_xml::ns = L"math";
        const wchar_t* math_annotation_xml::name = L"annotation-xml";
        //----------------------------------------------------------------------------------------------------

        void math_annotation_xml::add_attributes(const xml::attributes_wc_ptr& Attributes)
        {
            // ver 2	
            CP_APPLY_ATTR(L"math:encoding", encoding_);

            // ver 3
            if (!encoding_)	CP_APPLY_ATTR(L"encoding", encoding_);

        }
        void math_annotation_xml::add_text(const std::wstring& Text)
        {
            text_.append(Text);
        }
        void math_annotation_xml::add_child_element(xml::sax* Reader, const std::wstring& Ns, const std::wstring& Name)
        {
            CP_CREATE_ELEMENT(content_);

        }
//----------------------------------------------------------------------------------------------------
        const wchar_t* math_signature::ns = L"math";
        const wchar_t* math_signature::name = L"signature";
 //----------------------------------------------------------------------------------------------------

        void math_signature::add_attributes(const xml::attributes_wc_ptr& Attributes)
        {
            CP_APPLY_ATTR(L"encoding", encoding_);
            CP_APPLY_ATTR(L"alg", alg_, std::wstring(L""));
            CP_APPLY_ATTR(L"shakey", shakey_, std::wstring(L""));
            CP_APPLY_ATTR(L"binData", binData_);
        }
    }
}
