#pragma once
#ifndef PPTX_LOGIC_BULLETTYPEFACE_INCLUDE_H_
#define PPTX_LOGIC_BULLETTYPEFACE_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "./../TextFont.h"
#include "BuFontTx.h"

namespace PPTX
{
	namespace Logic
	{
		class BulletTypeface : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(BulletTypeface)

			BulletTypeface& operator=(const BulletTypeface& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				m_Typeface		= oSrc.m_Typeface;

				return *this;
			}

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				CString strName = node.GetName();

				if (strName == _T("a:buFontTx"))
					m_Typeface.reset(new Logic::BuFontTx(node));
				else if (strName == _T("a:buFont"))
					m_Typeface.reset(new Logic::TextFont(node));
				else m_Typeface.reset();
			}

			virtual void ReadBulletTypefaceFrom(XmlUtils::CXmlNode& element)
			{
				XmlUtils::CXmlNode oNode;
				if (element.GetNode(_T("a:buFontTx"), oNode))
					m_Typeface.reset(new Logic::BuFontTx(oNode));
				else if(element.GetNode(_T("a:buFont"), oNode))
					m_Typeface.reset(new Logic::TextFont(oNode));
				else m_Typeface.reset();
			}

			virtual bool is_init()const{return (m_Typeface.IsInit());};
			virtual bool has_spec_typeface()const{return is<TextFont>();};
			
			template<class T> AVSINLINE const bool	is() const	{ return m_Typeface.is<T>(); }
			template<class T> AVSINLINE T&			as()		{ return m_Typeface.as<T>(); }
			template<class T> AVSINLINE const T&	as() const 	{ return m_Typeface.as<T>(); }

			virtual CString toXML()const
			{
				if (m_Typeface.IsInit())
					return m_Typeface->toXML();
				return _T("");
			}
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				if (m_Typeface.is_init())
					m_Typeface->toXmlWriter(pWriter);
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				if (m_Typeface.is_init())
				{
					if (m_Typeface.is<Logic::TextFont>())
					{
						pWriter->StartRecord(BULLET_TYPE_TYPEFACE_BUFONT);
						m_Typeface->toPPTY(pWriter);
						pWriter->EndRecord();
					}
					else
					{
						m_Typeface->toPPTY(pWriter);
					}
				}
			}

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
			{
				LONG _end_rec = pReader->GetPos() + pReader->GetLong() + 4;
				if (pReader->GetPos() == _end_rec)
					return;

				BYTE _type = pReader->GetUChar();

				if (_type == BULLET_TYPE_TYPEFACE_BUFONT)
				{
					Logic::TextFont* p = new Logic::TextFont();
					p->m_name = _T("a:buFont");
					p->fromPPTY(pReader);
					m_Typeface.reset(p);
				}
				else
				{
					m_Typeface.reset(new Logic::BuFontTx());				
				}

				pReader->Seek(_end_rec);
			}

		//public:
		private:
			smart_ptr<WrapperWritingElement> m_Typeface;
		protected:
			virtual void FillParentPointersForChilds(){};
		public:
			virtual void SetParentPointer(const WrapperWritingElement* pParent)
			{
				if(is_init())
					m_Typeface->SetParentPointer(pParent);
			};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_BULLETTYPEFACE_INCLUDE_H