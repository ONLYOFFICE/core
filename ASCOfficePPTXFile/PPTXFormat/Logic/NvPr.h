/*
 * (c) Copyright Ascensio System SIA 2010-2019
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
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
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
#ifndef PPTX_LOGIC_NV_PROPERTIES_INCLUDE_H_
#define PPTX_LOGIC_NV_PROPERTIES_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "Ph.h"
#include "UniMedia.h"
#include "ExtP.h"

namespace PPTX
{
	namespace Logic
	{
		class NvPr : public WrapperWritingElement
		{
		public:
			WritingElement_AdditionConstructors(NvPr)
			
			NvPr(std::wstring ns = L"p")
			{
				m_namespace = ns;
			}
			NvPr& operator=(const NvPr& oSrc)
			{
				isPhoto		=	oSrc.isPhoto;
				userDrawn	=	oSrc.userDrawn;
				media		=	oSrc.media;
				ph			=	oSrc.ph;

				for (size_t i=0; i < oSrc.extLst.size(); i++)
					extLst.push_back(oSrc.extLst[i]);
				
				return *this;
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				m_namespace = XmlUtils::GetNamespace(oReader.GetName());
				
				ReadAttributes(oReader);

				if ( oReader.IsEmptyNode() )
					return;
						
				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
					std::wstring strName = XmlUtils::GetNameNoNS(oReader.GetName());

					if (strName == L"ph")
						ph = oReader;
					else if (strName == L"extLst")
					{
						if ( oReader.IsEmptyNode() )
							continue;

						int nParentDepth1 = oReader.GetDepth();
						while( oReader.ReadNextSiblingNode( nParentDepth1 ) )
						{
							Ext element;
							element.fromXML(oReader);
							extLst.push_back (element);
						}
					}
					else
					{
						media.fromXML(oReader);
					}
				}
			}
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_Read_if		( oReader, _T("isPhoto"),	isPhoto)
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("userDrawn"),	userDrawn)
				WritingElement_ReadAttributes_End( oReader )
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				m_namespace = XmlUtils::GetNamespace(node.GetName());

				XmlMacroReadAttributeBase(node, L"isPhoto", isPhoto);
				XmlMacroReadAttributeBase(node, L"userDrawn", userDrawn);

				ph = node.ReadNodeNoNS(_T("ph"));
				media.GetMediaFrom(node);

				XmlUtils::CXmlNode list = node.ReadNodeNoNS(_T("extLst"));
				if (list.IsValid())
				{		
					XmlUtils::CXmlNodes oNodes;
					if (list.GetNodes(_T("*"), oNodes))
					{
						int nCount = oNodes.GetCount();
						for (int i = 0; i < nCount; ++i)
						{
							XmlUtils::CXmlNode oNode;
							oNodes.GetAt(i, oNode);	

							Ext element;
							element.fromXML(oNode);
							extLst.push_back (element);
						}
					}
				}
			}
			virtual std::wstring toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("isPhoto"), isPhoto);
				oAttr.Write(_T("userDrawn"), userDrawn);

				XmlUtils::CNodeValue oValue;
				oValue.WriteNullable(ph);
				oValue.Write(media);

                return XmlUtils::CreateNode(_T("p:nvPr"), oAttr, oValue);

			}
            virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
            {
                std::wstring namespace_ = m_namespace;
                if		(pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DOCX)	namespace_= _T("pic");
                else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_XLSX)	namespace_= _T("xdr");

                toXmlWriter2(namespace_, pWriter);
            }
            void toXmlWriter2(const std::wstring& strNS, NSBinPptxRW::CXmlWriter* pWriter) const
            {
                pWriter->StartNode(strNS + _T(":nvPr"));

				pWriter->StartAttributes();
				pWriter->WriteAttribute(_T("isPhoto"), isPhoto);
				pWriter->WriteAttribute(_T("userDrawn"), userDrawn);
				pWriter->EndAttributes();

				pWriter->Write(ph);
				media.toXmlWriter(pWriter);
				pWriter->WriteArray(strNS + _T(":extLst"), extLst);				
				
                pWriter->EndNode(strNS + _T(":nvPr"));
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteBool2(0, isPhoto);
				pWriter->WriteBool2(1, userDrawn);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

				pWriter->WriteRecord2(0, ph);
				pWriter->WriteRecord1(1, media);
				pWriter->WriteRecordArray(2, 0, extLst);
			}

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
			{
				LONG _end_rec = pReader->GetPos() + pReader->GetLong() + 4;

				pReader->Skip(1); // start attributes

				while (true)
				{
					BYTE _at = pReader->GetUChar_TypeNode();
					if (_at == NSBinPptxRW::g_nodeAttributeEnd)
						break;

					switch (_at)
					{
						case 0:
						{
							isPhoto = pReader->GetBool();
							break;
						}
						case 1:
						{
							userDrawn = pReader->GetBool();
							break;
						}
						default:
							break;
					}
				}

				while (pReader->GetPos() < _end_rec)
				{
					BYTE _at = pReader->GetUChar();
					switch (_at)
					{
						case 0:
						{
							ph = new Ph();
							ph->fromPPTY(pReader);
							break;
						}
						default:
						{
							pReader->SkipRecord();
							break;
						}
					}
				}

				pReader->Seek(_end_rec);
			}
			std::wstring		m_namespace;
	// Attributes
			nullable_bool		isPhoto;
			nullable_bool		userDrawn;

	//Childs
			nullable<Ph>		ph;
			UniMedia			media;
	//custDataLst
			std::vector<Ext>	extLst;
		protected:
			virtual void FillParentPointersForChilds()
			{
				if(ph.IsInit())
					ph->SetParentPointer(this);
				if(media.is_init())
					media.SetParentPointer(this);
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_NV_PROPERTIES_INCLUDE_H
