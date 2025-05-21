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

#include "Blip.h"

#include "./../../Slide.h"
#include "./../../SlideMaster.h"
#include "./../../SlideLayout.h"
#include "./../../Theme.h"

#include "../../../../DesktopEditor/raster/ImageFileFormatChecker.h"

namespace PPTX
{
	namespace Logic
	{
		Blip::Blip(std::wstring ns)
		{
			m_namespace = ns;
			mediaExternal = false;
		}
		Blip& Blip::operator=(const Blip& oSrc)
		{
			parentFile		= oSrc.parentFile;
			parentElement	= oSrc.parentElement;

			Effects = oSrc.Effects;

			cstate	= oSrc.cstate;
			embed	= oSrc.embed;
			link	= oSrc.link;

			m_namespace		= oSrc.m_namespace;

			oleRid			= oSrc.oleRid;
			oleFilepathBin	= oSrc.oleFilepathBin;

			mediaRid		= oSrc.mediaRid;
			mediaFilepath	= oSrc.mediaFilepath;
			mediaExternal	= oSrc.mediaExternal;

			return *this;
		}
		OOX::EElementType Blip::getType() const
		{
			return OOX::et_a_blip;
		}
		void Blip::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, L"embed", embed)
				WritingElement_ReadAttributes_Read_else_if ( oReader, L"link", link )
				WritingElement_ReadAttributes_Read_else_if ( oReader, L"cstate", cstate )
			WritingElement_ReadAttributes_End_No_NS( oReader )
		}
		void Blip::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			m_namespace = XmlUtils::GetNamespace(oReader.GetName());
			
			ReadAttributes( oReader );

			Effects.clear();
			if ( oReader.IsEmptyNode() )
				return;
				
			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring strName = XmlUtils::GetNameNoNS(oReader.GetName());

				if (strName == L"extLst")
				{
					if (oReader.IsEmptyNode())
						continue;

					int nParentDepth1 = oReader.GetDepth();
					while (oReader.ReadNextSiblingNode(nParentDepth1))
					{
						Ext element;
						ExtLst.push_back(element);
						ExtLst.back().fromXML(oReader);
					}
				}
				else
				{
					UniEffect uni;
					Effects.push_back(uni);
					Effects.back().fromXML(oReader);
				}
			}
		
			FillParentPointersForChilds();
		}
		void Blip::fromXML(XmlUtils::CXmlNode& node)
		{
			m_namespace = XmlUtils::GetNamespace(node.GetName());

            XmlMacroReadAttributeBase(node, L"r:embed", embed);
            XmlMacroReadAttributeBase(node, L"r:link", link);
            XmlMacroReadAttributeBase(node, L"cstate", cstate);

			XmlUtils::CXmlNode list = node.ReadNodeNoNS(L"extLst");
			if (list.IsValid())
			{
				std::vector<XmlUtils::CXmlNode> oNodes;
				if (list.GetNodes(L"*", oNodes))
				{
					size_t nCount = oNodes.size();
					for (size_t i = 0; i < nCount; ++i)
					{
						XmlUtils::CXmlNode& oNode = oNodes[i];

						Ext element;
						element.fromXML(oNode);
					
						ExtLst.push_back(element);
					}
				}
			}
			Effects.clear();
			XmlMacroLoadArray(node, L"*", Effects, UniEffect);
			FillParentPointersForChilds();
		}
		std::wstring Blip::toXML() const
		{
			XmlUtils::CAttribute oAttr;

			if (embed.IsInit())
				oAttr.Write(L"r:embed", embed->ToString());
			if (link.IsInit())
				oAttr.Write(L"r:link", link->ToString());
			oAttr.WriteLimitNullable(L"cstate", cstate);

			XmlUtils::CNodeValue oValue;
			oValue.WriteArray(Effects);

			std::wstring strName = m_namespace.empty() ? L"blip" : m_namespace + L":blip";
			return XmlUtils::CreateNode(strName, oAttr, oValue);
		}
		void Blip::FillParentPointersForChilds()
		{
			for (size_t i = 0; i < Effects.size(); ++i)
			{
				Effects[i].SetParentPointer(this);
			}

			for (size_t i = 0; i < ExtLst.size(); ++i)
			{
				ExtLst[i].SetParentPointer(this);
			}

		}
		std::wstring Blip::GetFullPicName(OOX::IFileContainer* pRels)const
		{
			for (size_t i = 0; i < ExtLst.size(); ++i)
			{
				if (ExtLst[i].link_svg.IsInit())
				{
					if (pRels != NULL)
					{
						smart_ptr<OOX::Image> p = pRels->Get<OOX::Image>(*ExtLst[i].link_svg);
						if (p.is_init())
							return p->filename().m_strFilename;
					}

					if (parentFileIs<FileContainer>())
						return parentFileAs<FileContainer>().GetImagePathNameFromRId(*ExtLst[i].link_svg);
				}
			}
			if (embed.IsInit())
			{
				if (pRels != NULL)
				{
					smart_ptr<OOX::Image> p = pRels->Get<OOX::Image>(*embed);
					if (p.is_init())
						return p->filename().m_strFilename;
				}

				if (parentFileIs<FileContainer>())
					return parentFileAs<FileContainer>().GetImagePathNameFromRId(*embed);

				return L"";
			}
			else if (link.IsInit())
			{
				if (pRels != NULL)
				{
					smart_ptr<OOX::Image> p = pRels->Get<OOX::Image>(*link);
					if (p.is_init())
						return p->filename().m_strFilename;
				}

				if (parentFileIs<FileContainer>())
					return parentFileAs<FileContainer>().GetImagePathNameFromRId(*link);

				return L"";
			}
			return L"";
		}
		std::wstring Blip::GetFullOleName(const OOX::RId& oRId, OOX::IFileContainer* pRels)const
		{
			smart_ptr<OOX::OleObject> pOleObject;
			
			if (pRels != NULL)
				pOleObject = pRels->Get<OOX::OleObject>(oRId);
			else
			{
				OOX::IFileContainer* pContainer = dynamic_cast<OOX::IFileContainer*>(const_cast<PPTX::WrapperFile*>(parentFile));
				if (pContainer)
					pOleObject = pContainer->Get<OOX::OleObject>(oRId);
			}
			
			if (pOleObject.IsInit())
				return pOleObject->filename().m_strFilename;
			
			return L"";
		}
		void Blip::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			std::wstring strName;

			if (XMLWRITER_DOC_TYPE_WORDART == pWriter->m_lDocType)
				strName = L"w14:blip";
			else
				strName = m_namespace.empty() ? L"blip" : (m_namespace + L":blip");

			pWriter->StartNode(strName);

			pWriter->StartAttributes();
			if (embed.IsInit())
				pWriter->WriteAttribute(L"r:embed", embed->ToString());
			if (link.IsInit())
				pWriter->WriteAttribute(L"r:link", link->ToString());
			pWriter->WriteAttribute(L"cstate", cstate);
			pWriter->EndAttributes();

			for (size_t i = 0; i < Effects.size(); ++i)
			{
				Effects[i].toXmlWriter(pWriter);
			}
			pWriter->WriteArray(L"a:extLst", ExtLst);

			pWriter->EndNode(strName);
		}
		void Blip::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteLimit2(0, cstate);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

			if (embed.is_init())
				pWriter->WriteString1(10, embed->get());
			if (link.is_init())
				pWriter->WriteString1(11, link->get());

			pWriter->StartRecord(2);
			ULONG len = (ULONG)Effects.size();
			pWriter->WriteULONG(len);

			for (ULONG i = 0; i < len; ++i)
			{
				pWriter->WriteRecord1(0, Effects[i]);// id неважен
			}

			pWriter->EndRecord();

			double dX = pWriter->GetShapeX(); //mm
			double dY = pWriter->GetShapeY();

			double dW = pWriter->GetShapeWidth(); //mm
			double dH = pWriter->GetShapeHeight();

			OOX::IFileContainer* pRels = pWriter->GetRels().GetPointer();

			std::wstring	additionalPath;
			int				additionalType = 0;
			if (!oleFilepathBin.empty())
			{
				additionalPath = oleFilepathBin;
				additionalType = 1;
			}
			else if (!oleRid.empty())
			{
				additionalPath	= this->GetFullOleName(OOX::RId(oleRid), pRels);
				additionalType	= 1;
			}
			else if (!mediaRid.empty())
			{
				additionalPath	= this->GetFullOleName(OOX::RId(mediaRid), pRels);
				additionalType	= 2;
			}
			else if (!mediaFilepath.empty())
			{
				additionalPath	= mediaFilepath;
				additionalType	= 2;
			}
			NSShapeImageGen::CMediaInfo oId;
			if (!dataFilepathImageA.empty())
			{
				oId = pWriter->m_pCommon->m_pMediaManager->WriteImage(dataFilepathImageA, dX, dY, dW, dH, additionalPath, additionalType);
			}
			else if (!dataFilepathImage.empty())
			{
				oId = pWriter->m_pCommon->m_pMediaManager->WriteImage(dataFilepathImage, dX, dY, dW, dH, additionalPath, additionalType);
			}
			else if (!oleFilepathImage.empty())
			{
				std::wstring imagePath = oleFilepathImage;
				oId = pWriter->m_pCommon->m_pMediaManager->WriteImage(imagePath, dX, dY, dW, dH, additionalPath, additionalType);
			}
			else
			{
				std::wstring imagePath = this->GetFullPicName(pRels);
				oId = pWriter->m_pCommon->m_pMediaManager->WriteImage(imagePath, dX, dY, dW, dH, additionalPath, additionalType);
			}

			std::wstring s = oId.GetPath2();

			pWriter->StartRecord(3);

			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteString1(0, s);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);				

			pWriter->EndRecord();
		}
		void Blip::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _s2 = pReader->GetPos();
			LONG _e2 = _s2 + pReader->GetLong() + 4;

			pReader->Skip(1);

			while (true)
			{
				BYTE _at = pReader->GetUChar_TypeNode();
				if (NSBinPptxRW::g_nodeAttributeEnd == _at)
					break;

				if (_at == 0)
					pReader->Skip(1);
			}

			while (pReader->GetPos() < _e2)
			{
				BYTE _t = pReader->GetUChar();

				switch (_t)
				{
					case 0:
					case 1:
					{
						// id. embed / link
						pReader->Skip(4);						
					}break;
					case 10:
					case 11:
					{
						// id. embed / link
						pReader->GetString2();						
					}break;
					case 2:
					{
						pReader->Skip(4);
						ULONG count_effects = pReader->GetULong();
						for (ULONG _eff = 0; _eff < count_effects; ++_eff)
						{
							pReader->Skip(1); // type 

							Effects.push_back(UniEffect());
							Effects.back().fromPPTY(pReader);

							if (false == Effects.back().is_init())
							{
								Effects.pop_back();
							}
						}						
					}break;
					case 3:
					{
						pReader->Skip(6); // len + start attributes + type

						std::wstring strImagePath = pReader->GetString2(true);

						if (0 != strImagePath.find(L"http:") &&
							0 != strImagePath.find(L"https:") &&
							0 != strImagePath.find(L"ftp:") &&
							0 != strImagePath.find(L"file:"))
						{
							OOX::CPath pathNormalizer;

							if (0 == strImagePath.find(L"theme"))
							{
								pathNormalizer = pReader->m_strFolderExternalThemes;
							}
							else
							{
								pathNormalizer = pReader->m_strFolder + FILE_SEPARATOR_STR + L"media";
							}
							std::wstring strPath = pathNormalizer.GetPath();

							strImagePath = strPath  + FILE_SEPARATOR_STR + strImagePath;

							pathNormalizer = strImagePath;
							strImagePath = pathNormalizer.GetPath();

							if (std::wstring::npos != strImagePath.find(strPath))
							{
								CImageFileFormatChecker checker;
								if (false == checker.isImageFile(strImagePath))
								{
									strImagePath.clear();
								}				
							}
							else strImagePath.clear();
						}	
						if (false == strImagePath.empty())
						{
							std::vector<smart_ptr<OOX::File>> additionalFiles;
							NSBinPptxRW::_relsGeneratorInfo oRelsGeneratorInfo = pReader->m_pRels->WriteImage(strImagePath, additionalFiles, L"", L"");

							if (oRelsGeneratorInfo.nImageRId > 0)
							{
								embed = new OOX::RId(oRelsGeneratorInfo.nImageRId);
							}
						}
						pReader->Skip(1); // end attribute						
					}break;
					default:
					{
						pReader->SkipRecord();
						break;
					}
				}
			}

			pReader->Seek(_e2);
		}
	} // namespace Logic
} // namespace PPTX
