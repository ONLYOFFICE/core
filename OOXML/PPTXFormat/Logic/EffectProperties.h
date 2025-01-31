﻿/*
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
#ifndef PPTX_LOGIC_EFFECTPROPERTIES_INCLUDE_H_
#define PPTX_LOGIC_EFFECTPROPERTIES_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "EffectLst.h"
#include "EffectDag.h"

namespace PPTX
{
	namespace Logic
	{
		class EffectProperties : public WrapperWritingElement
		{
		public:
			WritingElement_AdditionMethods(EffectProperties)
			PPTX_LOGIC_BASE2(EffectProperties)

			EffectProperties& operator=(const EffectProperties& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				return *this;
			}

			virtual OOX::EElementType getType () const
			{
				if (List.IsInit())
					return List->getType();
				return OOX::et_Unknown;
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				std::wstring strName = XmlUtils::GetNameNoNS(oReader.GetName());
					
				if (strName == _T("effectLst"))
				{
					Logic::EffectLst* pEffectLst = new Logic::EffectLst();
					*pEffectLst = oReader;
					List.reset(pEffectLst);
				}
				else if(strName == _T("effectDag"))
				{
					Logic::EffectDag* pEffectDag = new Logic::EffectDag();
					*pEffectDag = oReader;
					List.reset(pEffectDag);
				}
				else 
					List.reset();
			}
			
			virtual bool is_init() const {return (List.IsInit());};

			template<class T> const bool is() const { return (!List.IsInit())?false:(typeid(*List) == typeid(T));}
			template<class T> T& as() {return static_cast<T&>(*List);}
			template<class T> const T& as() const {return static_cast<const T&>(*List);}

			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				std::wstring strName = XmlUtils::GetNameNoNS(node.GetName());

				if (strName == _T("effectLst"))
				{
					Logic::EffectLst* pEffectLst = new Logic::EffectLst();
					*pEffectLst = node;
					List.reset(pEffectLst);
				}
				else if(strName == _T("effectDag"))
				{
					Logic::EffectDag* pEffectDag = new Logic::EffectDag();
					*pEffectDag = node;
					List.reset(pEffectDag);
				}
				else List.reset();
			}

			virtual void GetEffectListFrom(XmlUtils::CXmlNode& element)
			{
				XmlUtils::CXmlNode oNode = element.ReadNodeNoNS(_T("effectLst"));
				if (oNode.IsValid())
				{
					Logic::EffectLst* pEffectLst = new Logic::EffectLst();
					*pEffectLst = oNode;
					List.reset(pEffectLst);
					return;
				}
				oNode = element.ReadNodeNoNS(_T("effectDag"));
				if (oNode.IsValid())
				{
					Logic::EffectDag* pEffectDag = new Logic::EffectDag();
					*pEffectDag = oNode;
					List.reset(pEffectDag);
				}
				else List.reset();
			}

			virtual std::wstring toXML() const
			{
				if (!List.IsInit())
					return _T("");
				return List->toXML();
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				if (List.is_init())
					List->toPPTY(pWriter);
			}
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				if (List.is_init())
					List->toXmlWriter(pWriter);
			}

			void Merge(EffectProperties& effectProperties) const
			{
				if (List.IsInit() && List.is<EffectLst>())
				{
					effectProperties.List.reset(new EffectLst());
					List.as<EffectLst>().Merge(effectProperties.List.as<EffectLst>());
				}
			}

			nullable<WrapperWritingElement> List;
		protected:
			virtual void FillParentPointersForChilds(){};
		public:
			virtual void SetParentPointer(const WrapperWritingElement* pParent)
			{
				if(is_init())
					List->SetParentPointer(pParent);
			};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_EFFECTPROPERTIES_INCLUDE_H
