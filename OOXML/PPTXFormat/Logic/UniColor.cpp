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


#include "UniColor.h"
#include "Colors/SrgbClr.h"
#include "Colors/PrstClr.h"
#include "Colors/SchemeClr.h"
#include "Colors/SysClr.h"

namespace PPTX
{
	namespace Logic
	{
		UniColor::UniColor()
		{
			Color.reset();
		}
		UniColor::~UniColor()
		{
			//Color.reset();
		}
		UniColor::UniColor(XmlUtils::CXmlNode& node)
		{
			fromXML(node);
		}
		const UniColor& UniColor::operator =(XmlUtils::CXmlNode& node)
		{
			fromXML(node);
			return *this;
		}
		void UniColor::fromXMLParent(XmlUtils::CXmlLiteReader& oReader)
		{
			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				fromXML(oReader);
				break;
			}
		}
		void UniColor::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			std::wstring name = XmlUtils::GetNameNoNS(oReader.GetName());
			
			if (name == _T("srgbClr"))
				Color.reset(new Logic::SrgbClr(oReader));
			else if (name == _T("scrgbClr"))
			{
				Logic::SrgbClr* pSrgbClr = new Logic::SrgbClr(oReader);
				pSrgbClr->fromXML(oReader);
				Color.reset(pSrgbClr);
			}
			else if (name == _T("prstClr"))
				Color.reset(new Logic::PrstClr(oReader));
			else if (name == _T("schemeClr"))
				Color.reset(new Logic::SchemeClr(oReader));
			else if (name == _T("sysClr"))
				Color.reset(new Logic::SysClr(oReader));
			else if (name == _T("styleClr"))
				Color.reset(new Logic::StyleClr(oReader));
			else Color.reset();
		}
		OOX::EElementType UniColor::getType () const
		{
			if (Color.IsInit())
				return Color->getType();
			return OOX::et_Unknown;
		}
		void UniColor::fromXML(XmlUtils::CXmlNode& node)
		{
			std::wstring name = XmlUtils::GetNameNoNS(node.GetName());

			if (name == _T("srgbClr"))
				Color.reset(new Logic::SrgbClr(node));
			else if (name == _T("scrgbClr"))
			{
				Logic::SrgbClr* pSrgbClr = new Logic::SrgbClr(node);
				pSrgbClr->fromXMLScRgb(node);
				Color.reset(pSrgbClr);
			}
			else if (name == _T("prstClr"))
				Color.reset(new Logic::PrstClr(node));
			else if (name == _T("schemeClr"))
				Color.reset(new Logic::SchemeClr(node));
			else if (name == _T("sysClr"))
				Color.reset(new Logic::SysClr(node));
			else if (name == _T("styleClr"))
				Color.reset(new Logic::StyleClr(node));
			else Color.reset();
		}
		void UniColor::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _len = pReader->GetLong();
			LONG read_end = pReader->GetPos() + _len;

			if (pReader->GetPos() < read_end)
			{
				BYTE _type = pReader->GetUChar();
				LONG _e = pReader->GetPos() + pReader->GetRecordSize() + 4;

				switch (_type)
				{
					case COLOR_TYPE_PRST:
					{
						pReader->Skip(2);
						Logic::PrstClr* pColor = new Logic::PrstClr();
						pColor->val.set(pReader->GetString2());
						pReader->Skip(1);

						Color = pColor;
						if (pReader->GetPos() < _e)
						{
							if (0 == pReader->GetUChar())
							{
								Color->ReadModsFromPPTY(pReader);
							}
						}						
					}break;
					case COLOR_TYPE_SCHEME:
					{
						pReader->Skip(2);
						Logic::SchemeClr* pColor = new Logic::SchemeClr();
						pColor->val.set(SchemeClr_GetStringCode(pReader->GetUChar()));
						
						Color = pColor;

						pReader->Skip(1);

						if (pReader->GetPos() < _e)
						{
							if (0 == pReader->GetUChar())
							{
								Color->ReadModsFromPPTY(pReader);
							}
						}						
					}break;
					case COLOR_TYPE_SRGB:
					{
						pReader->Skip(1);
						Color = new Logic::SrgbClr();
						pReader->Skip(1);
						Color->red = pReader->GetUChar();
						pReader->Skip(1);
						Color->green = pReader->GetUChar();
						pReader->Skip(1);
						Color->blue = pReader->GetUChar();
						pReader->Skip(1);

						if (pReader->GetPos() < _e)
						{
							if (0 == pReader->GetUChar())
							{
								Color->ReadModsFromPPTY(pReader);
							}
						}

						break;
					}
					case COLOR_TYPE_SYS:
					{
						pReader->Skip(1);
						Logic::SysClr* pColor = new Logic::SysClr();

						while (true)
						{
							BYTE _at = pReader->GetUChar_TypeNode();
							if (_at == NSBinPptxRW::g_nodeAttributeEnd)
								break;

							switch (_at)
							{
								case 0:
								{
									pColor->val = pReader->GetString2();									
								}break;
								case 1:
								{
									pColor->red = pReader->GetUChar();									
								}break;
								case 2:
								{
									pColor->green = pReader->GetUChar();									
								}break;
								case 3:
								{
									pColor->blue = pReader->GetUChar();									
								}break;
								default:
									break;
							}
						}
						Color = pColor;
						if (pReader->GetPos() < _e)
						{
							if (0 == pReader->GetUChar())
							{
								Color->ReadModsFromPPTY(pReader);
							}
						}						
					}break;
					case COLOR_TYPE_STYLE:
					{
						pReader->Skip(1);
						Logic::StyleClr* pColor = new Logic::StyleClr();
						while (true)
						{
							BYTE _at = pReader->GetUChar_TypeNode();
							if (_at == NSBinPptxRW::g_nodeAttributeEnd)
								break;

							switch (_at)
							{
							case 0:
							{
								pColor->val = pReader->GetULong();
							}break;
							case 1:
							{
								pColor->bAuto = pReader->GetBool();
							}break;
							default:
								break;
							}
						}
						Color = pColor;

						pReader->Skip(1);

						if (pReader->GetPos() < _e)
						{
							if (0 == pReader->GetUChar())
							{
								Color->ReadModsFromPPTY(pReader);
							}
						}
					}break;
				}
			}

			pReader->Seek(read_end);
		}
		void UniColor::GetColorFrom(XmlUtils::CXmlLiteReader& oReader)
		{
			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				fromXML(oReader);

				if (Color.is_init())
					break;
			}
		}
		void UniColor::GetColorFrom(XmlUtils::CXmlNode& element)
		{
			XmlUtils::CXmlNode oNode = element.ReadNodeNoNS(_T("srgbClr"));
			if (oNode.IsValid())
				Color.reset(new Logic::SrgbClr(oNode));
			else
			{
				oNode = element.ReadNodeNoNS(_T("prstClr"));
				if (oNode.IsValid())
					Color.reset(new Logic::PrstClr(oNode));
				else
				{
					oNode = element.ReadNodeNoNS(_T("schemeClr"));
					if (oNode.IsValid())
						Color.reset(new Logic::SchemeClr(oNode));
					else
					{
						oNode = element.ReadNodeNoNS(_T("sysClr"));
						if (oNode.IsValid())
							Color.reset(new Logic::SysClr(oNode));
						else
						{
							oNode = element.ReadNodeNoNS(_T("scrgbClr"));
							if (oNode.IsValid())
							{
								Logic::SrgbClr* pSrgbClr = new Logic::SrgbClr(oNode);
								pSrgbClr->fromXMLScRgb(oNode);
								Color.reset(pSrgbClr);
							}
							else
								Color.reset();
						}
					}
				}
			}
		}
		std::wstring UniColor::toXML() const
		{
			if (Color.IsInit())
				return Color->toXML();
			return _T("");
		}
		DWORD UniColor::GetRGBA(DWORD RGBA)const
		{
			if(is_init())
				return Color->GetRGBA(RGBA);
			return 0; //return 0; - заменить на просмотр настроек по умолчанию
		}
		DWORD UniColor::GetARGB(DWORD ARGB)const
		{
			if(is_init())
				return Color->GetARGB(ARGB);
			return 0; //return 0; - заменить на просмотр настроек по умолчанию
		}
		DWORD UniColor::GetBGRA(DWORD BGRA)const
		{
			if(is_init())
				return Color->GetBGRA(BGRA);
			return 0; //return 0; - заменить на просмотр настроек по умолчанию
		}
		DWORD UniColor::GetABGR(DWORD ABGR)const
		{
			if(is_init())
				return Color->GetABGR(ABGR);
			return 0; //return 0; - заменить на просмотр настроек по умолчанию
		}
		void UniColor::SetRGBColor(const BYTE& R, const BYTE& G, const BYTE& B)
		{
			Logic::SrgbClr* rgbColor = new Logic::SrgbClr();
			rgbColor->SetRGB(R, G, B);

			Color.reset(rgbColor);
		}
		DWORD UniColor::GetRGBColor(NSCommon::smart_ptr<PPTX::Theme>& _oTheme, NSCommon::smart_ptr<PPTX::Logic::ClrMap>& _oClrMap, DWORD ARGB)
		{
			if (Color.is_init())
				return Color->GetRGBColor(_oTheme,_oClrMap, ARGB);
			return 0;
		}
		void UniColor::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			if (Color.is_init())
				Color->toPPTY(pWriter);
		}
		void UniColor::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			if (Color.is_init())
				Color->toXmlWriter(pWriter);
		}
		void UniColor::FillParentPointersForChilds(){}
		void UniColor::SetParentPointer(const WrapperWritingElement* pParent)
		{
			if(is_init())
				Color->SetParentPointer(pParent);
		}
	} // namespace Logic
} // namespace PPTX
