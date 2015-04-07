#pragma once
#ifndef PPTX_LOGIC_BULLETSIZE_INCLUDE_H_
#define PPTX_LOGIC_BULLETSIZE_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "BuSzTx.h"
#include "BuSzPct.h"
#include "BuSzPts.h"

namespace PPTX
{
	namespace Logic
	{
		class BulletSize : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(BulletSize)

			BulletSize& operator=(const BulletSize& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				m_Size			= oSrc.m_Size;

				return *this;
			}

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				CString strName = node.GetName();

				if (strName == _T("a:buSzTx"))
					m_Size.reset(new Logic::BuSzTx(node));
				else if (strName == _T("a:buSzPct"))
					m_Size.reset(new Logic::BuSzPct(node));
				else if (strName == _T("a:buSzPts"))
					m_Size.reset(new Logic::BuSzPts(node));
				else m_Size.reset();
			}

			virtual void ReadBulletSizeFrom(XmlUtils::CXmlNode& element)
			{
				XmlUtils::CXmlNode oNode;
				if(element.GetNode(_T("a:buSzTx"), oNode))
					m_Size.reset(new Logic::BuSzTx(oNode));
				else if(element.GetNode(_T("a:buSzPct"), oNode))
					m_Size.reset(new Logic::BuSzPct(oNode));
				else if(element.GetNode(_T("a:buSzPts"), oNode))
					m_Size.reset(new Logic::BuSzPts(oNode));
				else m_Size.reset();
			}
			virtual bool is_init()const{return (m_Size.IsInit());};
			virtual bool has_spec_size()const{return ((is_init()) && (!is<BuSzTx>()));};
			
			template<class T> AVSINLINE const bool	is() const	{ return m_Size.is<T>(); }
			template<class T> AVSINLINE T&			as()		{ return m_Size.as<T>(); }
			template<class T> AVSINLINE const T&	as() const 	{ return m_Size.as<T>(); }

			virtual CString toXML()const
			{
				if (m_Size.IsInit())
					return m_Size->toXML();
				return _T("");
			}
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				if (m_Size.is_init())
					m_Size->toXmlWriter(pWriter);
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				if (m_Size.is_init())
					m_Size->toPPTY(pWriter);
			}

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
			{
				LONG _end_rec = pReader->GetPos() + pReader->GetLong() + 4;
				if (pReader->GetPos() == _end_rec)
					return;

				BYTE _type = pReader->GetUChar();

				if (_type == BULLET_TYPE_SIZE_TX)
				{
					m_Size.reset(new Logic::BuSzTx());
				}
				else if (_type == BULLET_TYPE_SIZE_PTS)
				{
					Logic::BuSzPts* p = new Logic::BuSzPts();
					pReader->Skip(6); // len + + startattr type(0)
					p->val = pReader->GetLong();
					m_Size.reset(p);
				}
				else
				{
					Logic::BuSzPct* p = new Logic::BuSzPct();
					pReader->Skip(6); // len + + startattr type(0)
					p->val = pReader->GetLong();
					m_Size.reset(p);
				}

				pReader->Seek(_end_rec);
			}

		public:
		//private:
			smart_ptr<WrapperWritingElement> m_Size;
		protected:
			virtual void FillParentPointersForChilds(){};
		public:
			virtual void SetParentPointer(const WrapperWritingElement* pParent)
			{
				if(is_init())
					m_Size->SetParentPointer(pParent);
			};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_BULLETSIZE_INCLUDE_H