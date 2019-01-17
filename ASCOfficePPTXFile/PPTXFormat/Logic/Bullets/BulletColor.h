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
#pragma once
#ifndef PPTX_LOGIC_BULLETCOLOR_INCLUDE_H_
#define PPTX_LOGIC_BULLETCOLOR_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "BuClrTx.h"
#include "BuClr.h"

namespace PPTX
{
	namespace Logic
	{
		class BulletColor : public WrapperWritingElement
		{
		public:
			WritingElement_AdditionConstructors(BulletColor)
			PPTX_LOGIC_BASE2(BulletColor)

			BulletColor& operator=(const BulletColor& oColor)
			{
				parentFile		= oColor.parentFile;
				parentElement	= oColor.parentElement;

				m_Color			= oColor.m_Color;

				return *this;
			}
			virtual OOX::EElementType getType() const
			{
				if (m_Color.IsInit())
					return m_Color->getType();
				return OOX::et_Unknown;
			}			
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
                std::wstring strName = oReader.GetName();

				if (strName == _T("a:buClrTx"))
					m_Color.reset(new Logic::BuClrTx(oReader));
				else if (strName == _T("a:buClr"))
					m_Color.reset(new Logic::BuClr(oReader));
				else 
					m_Color.reset();
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				std::wstring strName = node.GetName();

				if (strName == _T("a:buClrTx"))
					m_Color.reset(new Logic::BuClrTx(node));
				else if (strName == _T("a:buClr"))
					m_Color.reset(new Logic::BuClr(node));
				else 
					m_Color.reset();
			}

			void ReadBulletColorFrom(XmlUtils::CXmlNode& element)
			{
				XmlUtils::CXmlNode oNode;
				if (element.GetNode(_T("a:buClrTx"), oNode))
					m_Color.reset(new Logic::BuClrTx(oNode));
				else if (element.GetNode(_T("a:buClr"), oNode))
					m_Color.reset(new Logic::BuClr(oNode));
				else 
					m_Color.reset();
			}
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				if (m_Color.is_init())
					m_Color->toPPTY(pWriter);
			}

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
			{
				LONG _end_rec = pReader->GetPos() + pReader->GetLong() + 4;
				if (pReader->GetPos() == _end_rec)
					return;

				BYTE _type = pReader->GetUChar();

				if (_type == BULLET_TYPE_COLOR_CLRTX)
				{
					m_Color.reset(new Logic::BuClrTx());
				}
				else
				{
					LONG len = pReader->GetLong();
					if(len > 0)
					{
						Logic::BuClr* pClr = new Logic::BuClr();
						pReader->Skip(1); // type(0)
						pClr->Color.fromPPTY(pReader);
						m_Color.reset(pClr);
					}
				}

				pReader->Seek(_end_rec);
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				if (m_Color.is_init())
					m_Color->toXmlWriter(pWriter);
			}

			virtual bool is_init()const{return (m_Color.IsInit());};
			virtual bool has_spec_color()const{return is<BuClr>();};
			
			template<class T> AVSINLINE const bool	is() const	{ return m_Color.is<T>(); }
			template<class T> AVSINLINE T&			as()		{ return m_Color.as<T>(); }
			template<class T> AVSINLINE const T&	as() const 	{ return m_Color.as<T>(); }

            virtual DWORD GetRGBA()const
			{
				if(has_spec_color())
					return as<BuClr>().GetRGBA();
				return 0;
			}

            virtual DWORD GetARGB()const
			{
				if(has_spec_color())
					return as<BuClr>().GetARGB();
				return 0;
			}

            virtual DWORD GetBGRA()const
			{
				if(has_spec_color())
					return as<BuClr>().GetBGRA();
				return 0;
			}

            virtual DWORD GetABGR()const
			{
				if(has_spec_color())
					return as<BuClr>().GetABGR();
				return 0;
			}

			virtual std::wstring toXML()const
			{
				if (m_Color.IsInit())
					return m_Color->toXML();
				return _T("");
			}

		public:
		//private:
			smart_ptr<WrapperWritingElement> m_Color;
		protected:
			virtual void FillParentPointersForChilds(){};
		public:
			virtual void SetParentPointer(const WrapperWritingElement* pParent)
			{
				if(is_init())
					m_Color->SetParentPointer(pParent);
			};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_BULLETCOLOR_INCLUDE_H
