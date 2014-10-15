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
			PPTX_LOGIC_BASE(BulletColor)

			BulletColor& operator=(const BulletColor& oColor)
			{
				parentFile		= oColor.parentFile;
				parentElement	= oColor.parentElement;

				m_Color			= oColor.m_Color;

				return *this;
			}

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				CString strName = node.GetName();

				if (strName == _T("a:buClrTx"))
					m_Color.reset(new Logic::BuClrTx(node));
				else if (strName == _T("a:buClr"))
					m_Color.reset(new Logic::BuClr(node));
				else m_Color.reset();
			}

			void ReadBulletColorFrom(XmlUtils::CXmlNode& element)
			{
				XmlUtils::CXmlNode oNode;
				if (element.GetNode(_T("a:buClrTx"), oNode))
					m_Color.reset(new Logic::BuClrTx(oNode));
				else if (element.GetNode(_T("a:buClr"), oNode))
					m_Color.reset(new Logic::BuClr(oNode));
				else m_Color.reset();
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
					Logic::BuClr* pClr = new Logic::BuClr();
					pReader->Skip(5); // len + type(0)
					pClr->Color.fromPPTY(pReader);
					m_Color.reset(pClr);					
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

			virtual CString toXML()const
			{
				if (m_Color.IsInit())
					return m_Color->toXML();
				return _T("");
			}

		//public:
		private:
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
