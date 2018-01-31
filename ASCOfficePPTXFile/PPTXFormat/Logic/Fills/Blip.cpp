﻿/*
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


#include "Blip.h"
#include "./../../Slide.h"
#include "./../../SlideMaster.h"
#include "./../../SlideLayout.h"
#include "./../../Theme.h"

namespace PPTX
{
	namespace Logic
	{
		void Blip::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			Effects.clear();
			if ( oReader.IsEmptyNode() )
				return;
				
			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = oReader.GetName();

				UniEffect uni;
				Effects.push_back(uni);
				Effects.back().fromXML(oReader);
			}
		
			FillParentPointersForChilds();
		}
		void Blip::fromXML(XmlUtils::CXmlNode& node)
		{
			m_namespace = XmlUtils::GetNamespace(node.GetName());

			node.ReadAttributeBase(L"r:embed", embed);
			node.ReadAttributeBase(L"r:link", link);
			node.ReadAttributeBase(L"cstate", cstate);

			Effects.clear();
			node.LoadArray(_T("*"), Effects);

			FillParentPointersForChilds();
		}

		std::wstring Blip::toXML() const
		{
			XmlUtils::CAttribute oAttr;

			if (embed.IsInit())
				oAttr.Write(_T("r:embed"), embed->ToString());
			if (link.IsInit())
				oAttr.Write(_T("r:link"), link->ToString());
			oAttr.WriteLimitNullable(_T("cstate"), cstate);

			XmlUtils::CNodeValue oValue;
			oValue.WriteArray(Effects);

			std::wstring strName = (_T("") == m_namespace) ? _T("blip") : (m_namespace + _T(":blip"));
			return XmlUtils::CreateNode(strName, oAttr, oValue);
		}

		void Blip::FillParentPointersForChilds()
		{
			size_t count = Effects.size();
			for(size_t i = 0; i < count; ++i)
				Effects[i].SetParentPointer(this);
		}

		std::wstring Blip::GetFullPicName(OOX::IFileContainer* pRels)const
		{
			if(embed.IsInit())
			{
				if (pRels != NULL)
				{
					smart_ptr<OOX::Image> p = pRels->Get<OOX::Image>(*embed);
					if (p.is_init())
						return p->filename().m_strFilename;
				}

				if(parentFileIs<FileContainer>())
					return parentFileAs<FileContainer>().GetImagePathNameFromRId(*embed);

				return _T("");
			}
			else if(link.IsInit())
			{
				if (pRels != NULL)
				{
					smart_ptr<OOX::Image> p = pRels->Get<OOX::Image>(*link);
					if (p.is_init())
						return p->filename().m_strFilename;
				}

				if(parentFileIs<FileContainer>())
					return parentFileAs<FileContainer>().GetImagePathNameFromRId(*link);

				return _T("");
			}
			return _T("");
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
			std::wstring strName = (_T("") == m_namespace) ? _T("blip") : (m_namespace + _T(":blip"));
			pWriter->StartNode(strName);

			pWriter->StartAttributes();
			if (embed.IsInit())
				pWriter->WriteAttribute(_T("r:embed"), embed->ToString());
			if (link.IsInit())
				pWriter->WriteAttribute(_T("r:link"), link->ToString());
			pWriter->WriteAttribute(_T("cstate"), cstate);
			pWriter->EndAttributes();

			// TODO:
			size_t nCount = Effects.size();
			for (size_t i = 0; i < nCount; ++i)
			{
				Effects[i].toXmlWriter(pWriter);
			}

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
				pWriter->WriteRecord1(3, Effects[i]);
			}

			pWriter->EndRecord();

			double dX = pWriter->GetShapeX(); //mm
			double dY = pWriter->GetShapeY();

			double dW = pWriter->GetShapeWidth(); //mm
			double dH = pWriter->GetShapeHeight();

			OOX::IFileContainer* pRels = NULL;
			
			if (pWriter->m_pCurrentContainer->is_init())
				pRels = pWriter->m_pCurrentContainer->operator ->();

			std::wstring	additionalPath;
			int				additionalType = 0;
			if(!oleFilepathBin.empty())
			{
				additionalPath = oleFilepathBin;
				additionalType = 1;
			}
			else if(!oleRid.empty())
			{
				additionalPath	= this->GetFullOleName(OOX::RId(oleRid), pRels);
				additionalType	= 1;
			}
			else if(!mediaRid.empty())
			{
				additionalPath	= this->GetFullOleName(OOX::RId(mediaRid), pRels);
				additionalType	= 2;
			}
			else if(!mediaFilepath.empty())
			{
				additionalPath	= mediaFilepath;
				additionalType	= 2;
			}

			std::wstring imagePath;
			if(!oleFilepathImage.empty())
			{
				imagePath = oleFilepathImage;
			}
			else
			{
				imagePath = this->GetFullPicName(pRels);
			}

			NSShapeImageGen::CMediaInfo oId = pWriter->m_pCommon->m_pMediaManager->WriteImage(imagePath, dX, dY, dW, dH, additionalPath, additionalType);
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
						break;
					}
					case 10:
					case 11:
					{
						// id. embed / link
						pReader->GetString2();
						break;
					}
					case 2:
					{
						pReader->Skip(4);
						ULONG count_effects = pReader->GetULong();
						for (ULONG _eff = 0; _eff < count_effects; ++_eff)
						{
							pReader->Skip(1); // type 
							ULONG rec_len = pReader->GetULong();
							if (0 == rec_len)
								continue;
							
							BYTE rec = pReader->GetUChar();
							
							if (rec == EFFECT_TYPE_ALPHAMODFIX)
							{
								// alpha!!!
								LONG _e22 = pReader->GetPos() + pReader->GetLong() + 4;

								pReader->Skip(1); // startattr

								PPTX::Logic::AlphaModFix* pEffect = new PPTX::Logic::AlphaModFix();
								while (true)
								{
									BYTE _at = pReader->GetUChar_TypeNode();
									if (NSBinPptxRW::g_nodeAttributeEnd == _at)
										break;

									if (_at == 0)
										pEffect->amt = pReader->GetLong();
								}

								Effects.push_back(UniEffect());
								Effects[0].InitPointer(pEffect);

								pReader->Seek(_e22);
							}
							else
							{
								pReader->SkipRecord();
							}
						}
						break;
					}
					case 3:
					{
						pReader->Skip(6); // len + start attributes + type

						std::wstring strImagePath = pReader->GetString2();

						if (0 != strImagePath.find(_T("http:")) &&
							0 != strImagePath.find(_T("https:")) &&
							0 != strImagePath.find(_T("ftp:")) &&
							0 != strImagePath.find(_T("file:")))
						{
							if (0 == strImagePath.find(_T("theme")))
							{
                                strImagePath = pReader->m_strFolderExternalThemes + FILE_SEPARATOR_STR  + strImagePath;
							}
							else
							{
                                strImagePath = pReader->m_strFolder + FILE_SEPARATOR_STR + _T("media")  + FILE_SEPARATOR_STR + strImagePath;
							}

							OOX::CPath pathUrl = strImagePath;
							strImagePath = pathUrl.GetPath();
						}
	
						smart_ptr<OOX::File> additionalFile;
						NSBinPptxRW::_relsGeneratorInfo oRelsGeneratorInfo = pReader->m_pRels->WriteImage(strImagePath, additionalFile, L"", L"");

						if (oRelsGeneratorInfo.nImageRId > 0)
						{
							embed = new OOX::RId((size_t)oRelsGeneratorInfo.nImageRId);
						}
						pReader->Skip(1); // end attribute
						break;
					}
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