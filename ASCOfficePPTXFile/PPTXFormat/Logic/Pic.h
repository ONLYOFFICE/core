/*
 * (c) Copyright Ascensio System SIA 2010-2017
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
#ifndef PPTX_LOGIC_SLIDE_PIC_INCLUDE_H_
#define PPTX_LOGIC_SLIDE_PIC_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "NvPicPr.h"
#include "Fills/BlipFill.h"
#include "SpPr.h"
#include "ShapeStyle.h"

namespace PPTX
{
	namespace Limit
	{
		class OLEDrawAspectType : public BaseLimit
		{
		public:
			OLEDrawAspectType()
			{
				m_strValue = _T("Content");
			}

			_USE_STRING_OPERATOR

			virtual void set(const std::wstring& strValue)
			{
				if ((_T("Content") == strValue) ||
					(_T("Icon") == strValue))
				{
					m_strValue = strValue;
				}
			}

			virtual BYTE GetBYTECode() const
			{
				if (_T("Content") == m_strValue)
					return 0;
				if (_T("Icon") == m_strValue)
					return 1;
				return 0;
			}

			virtual void SetBYTECode(const BYTE& src)
			{
				switch (src)
				{
				case 0:
					m_strValue = _T("Content");
					break;
				case 1:
					m_strValue = _T("Icon");
					break;
				default:
					break;
				}
			}
		};
		class OLEType : public BaseLimit
		{
		public:
			OLEType()
			{
				m_strValue = _T("Embed");
			}

			_USE_STRING_OPERATOR

			virtual void set(const std::wstring& strValue)
			{
				if ((_T("Embed") == strValue) ||
					(_T("Link") == strValue))
				{
					m_strValue = strValue;
				}
			}

			virtual BYTE GetBYTECode() const
			{
				if (_T("Embed") == m_strValue)
					return 0;
				if (_T("Link") == m_strValue)
					return 1;
				return 0;
			}

			virtual void SetBYTECode(const BYTE& src)
			{
				switch (src)
				{
				case 0:
					m_strValue = _T("Embed");
					break;
				case 1:
					m_strValue = _T("Link");
					break;
				default:
					break;
				}
			}
		};
		class OLEUpdateMode : public BaseLimit
		{
		public:
			OLEUpdateMode()
			{
				m_strValue = _T("Always");
			}

			_USE_STRING_OPERATOR

			virtual void set(const std::wstring& strValue)
			{
				if ((_T("Always") == strValue) ||
					(_T("OnCall") == strValue))
				{
					m_strValue = strValue;
				}
			}

			virtual BYTE GetBYTECode() const
			{
				if (_T("Always") == m_strValue)
					return 0;
				if (_T("OnCall") == m_strValue)
					return 1;
				return 0;
			}

			virtual void SetBYTECode(const BYTE& src)
			{
				switch (src)
				{
				case 0:
					m_strValue = _T("Always");
					break;
				case 1:
					m_strValue = _T("OnCall");
					break;
				default:
					break;
				}
			}
		};
	} // namespace Limit
} // namespace PPTX

namespace PPTX
{
	namespace Logic
	{
		class COLEObject : public WrapperWritingElement
		{
		public:
			virtual OOX::EElementType getType () const
			{
				return OOX::et_pic;
			}
			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;
			
			virtual void toXmlWriter (NSBinPptxRW::CXmlWriter* pWriter) const;
			
			virtual void toPPTY (NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY (NSBinPptxRW::CBinaryFileReader* pReader);
			
			virtual void FillParentPointersForChilds();
			
			bool isValid();
			
			smart_ptr<OOX::OleObject>	GetOleObject(const OOX::RId& oRId, OOX::IFileContainer* pRels)const;
			std::wstring				GetOleData	(const std::wstring& sFilePath)const;

			nullable_limit<Limit::OLEDrawAspectType>m_oDrawAspect;
			nullable<OOX::RId>						m_oId;
			nullable_string							m_sObjectId;
			nullable_string							m_sProgId;
			nullable_string							m_sShapeId;
			nullable_limit<Limit::OLEType>			m_oType;
			nullable_limit<Limit::OLEUpdateMode>	m_oUpdateMode;

	//internal
			nullable_string						m_sData;
			
			smart_ptr<OOX::OleObject>			m_OleObjectFile;
		
			nullable_int						m_oDxaOrig;
			nullable_int						m_oDyaOrig;
		};

		class Pic : public WrapperWritingElement
		{
		public:
			Pic(std::wstring ns = L"p");
			virtual ~Pic();

			virtual OOX::EElementType getType () const
			{
				return OOX::et_pic;
			}

			explicit Pic(XmlUtils::CXmlNode& node);
			const Pic& operator =(XmlUtils::CXmlNode& node);
			
			explicit Pic(XmlUtils::CXmlLiteReader& oReader);
			const Pic& operator =(XmlUtils::CXmlLiteReader& oReader);

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			
			virtual std::wstring toXML() const;
			virtual void GetRect(Aggplus::RECT& pRect)const;
			virtual std::wstring GetFullPicName()const;
			virtual std::wstring GetVideoLink()const;
			virtual std::wstring GetAudioLink()const;
			DWORD GetFill(UniFill& fill)const;
			DWORD GetLine(Ln& line)const;
			double GetStTrim () const;
			double GetEndTrim () const;
			long GetRefId() const;

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				if(oleObject.IsInit())
				{
					pWriter->StartRecord(SPTREE_TYPE_OLE);
				}
				else
				{
					pWriter->StartRecord(SPTREE_TYPE_PIC);
				}

				pWriter->WriteRecord2(4, oleObject);
				pWriter->WriteRecord1(0, nvPicPr);
				pWriter->WriteRecord1(1, blipFill);
				pWriter->WriteRecord1(2, spPr);
				pWriter->WriteRecord2(3, style);

				pWriter->EndRecord();
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				std::wstring namespace_ = m_namespace;
				bool bOle = false;
				
				if		(pWriter->m_lDocType == XMLWRITER_DOC_TYPE_XLSX)	namespace_ = L"xdr";
				else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DOCX)	namespace_ = L"pic";

				if (pWriter->m_lDocType != XMLWRITER_DOC_TYPE_XLSX && 
					pWriter->m_lDocType != XMLWRITER_DOC_TYPE_DOCX)
				{
					if(oleObject.IsInit() && oleObject->isValid())
					{
						bOle = true;
						pWriter->WriteString(L"<p:graphicFrame><p:nvGraphicFramePr><p:cNvPr id=\"0\" name=\"\"/><p:cNvGraphicFramePr><a:graphicFrameLocks noChangeAspect=\"1\"/></p:cNvGraphicFramePr><p:nvPr><p:extLst><p:ext uri=\"{D42A27DB-BD31-4B8C-83A1-F6EECF244321}\"><p14:modId xmlns:p14=\"http://schemas.microsoft.com/office/powerpoint/2010/main\" val=\"2157879785\"/></p:ext></p:extLst></p:nvPr></p:nvGraphicFramePr>");
						if(spPr.xfrm.IsInit())
						{
							std::wstring oldNamespace = spPr.xfrm->m_ns;
							spPr.xfrm->m_ns = _T("p");
							spPr.xfrm->toXmlWriter(pWriter);
							spPr.xfrm->m_ns = oldNamespace;
						}
						pWriter->WriteString(L"<a:graphic><a:graphicData uri=\"http://schemas.openxmlformats.org/presentationml/2006/ole\">");

						pWriter->StartNode(_T("p:oleObj"));
						pWriter->WriteAttribute(L"name", (std::wstring)L"oleObj");
						if(oleObject->m_oId.IsInit())
						{
							pWriter->WriteAttribute2(L"r:id", oleObject->m_oId->get());
						}
						if(oleObject->m_oDxaOrig.IsInit())
						{
							int nDxaOrig = oleObject->m_oDxaOrig.get();
							pWriter->WriteAttribute(L"imgW", 635 * nDxaOrig); //twips to emu
						}
						if(oleObject->m_oDyaOrig.IsInit())
						{
							int nDyaOrig = oleObject->m_oDyaOrig.get();
							pWriter->WriteAttribute(L"imgH", 635 * nDyaOrig); //twips to emu
						}
						pWriter->WriteAttribute2(L"progId", oleObject->m_sProgId);
						pWriter->EndAttributes();

						pWriter->WriteString(L"<p:embed/>");
					}
				}
				pWriter->StartNode(namespace_ + L":pic");

				if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DOCX)
				{
					pWriter->StartAttributes();
					pWriter->WriteAttribute(_T("xmlns:pic"), (std::wstring)_T("http://schemas.openxmlformats.org/drawingml/2006/picture"));
				}
				pWriter->EndAttributes();

				nvPicPr.toXmlWriter(pWriter);

				blipFill.m_namespace = namespace_;
				blipFill.toXmlWriter(pWriter);

				pWriter->m_lFlag = 1;
				spPr.toXmlWriter(pWriter);
				pWriter->m_lFlag = 0;
				
				pWriter->Write(style);

				pWriter->EndNode(namespace_ + L":pic");
				
				if (pWriter->m_lDocType != XMLWRITER_DOC_TYPE_XLSX &&
					pWriter->m_lDocType != XMLWRITER_DOC_TYPE_DOCX)
				{
					if(bOle)
					{
						pWriter->WriteString(L"</p:oleObj></a:graphicData></a:graphic></p:graphicFrame>");
					}
				}
			}

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			void toXmlWriterVML(NSBinPptxRW::CXmlWriter* pWriter, smart_ptr<PPTX::Theme>& oTheme, smart_ptr<PPTX::Logic::ClrMap>& oClrMap, bool in_group = false);
			void fromXMLOle(XmlUtils::CXmlNode& node);
//----------------------------------------------------------------------
			NvPicPr					nvPicPr;
			BlipFill				blipFill;
			SpPr					spPr;
			nullable<ShapeStyle>	style;
		//internal
			std::wstring			m_namespace;
			nullable<COLEObject>	oleObject;
			nullable_string			m_sClientDataXml;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_SLIDE_PIC_INCLUDE_H_
