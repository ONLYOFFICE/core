#pragma once
#ifndef PPTX_SLIDES_SLIDE_SHAPETREEELEM_INCLUDE_H_
#define PPTX_SLIDES_SLIDE_SHAPETREEELEM_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "SpPr.h"
#include "ShapeStyle.h"

namespace PPTX
{
	namespace Logic
	{
		void CalculateFill(PPTX::Logic::SpPr& oSpPr, nullable<ShapeStyle>& pShapeStyle, smart_ptr<PPTX::WrapperFile>& oTheme, smart_ptr<PPTX::WrapperWritingElement>& oClrMap, CString& strAttr, CString& strNode, bool bOle);
		void CalculateLine(PPTX::Logic::SpPr& oSpPr, nullable<ShapeStyle>& pShapeStyle, smart_ptr<PPTX::WrapperFile>& oTheme, smart_ptr<PPTX::WrapperWritingElement>& oClrMap, CString& strAttr, CString& strNode);

		class SpTreeElem : public WrapperWritingElement
		{
		public:
			SpTreeElem();
			virtual ~SpTreeElem();
			explicit SpTreeElem(XmlUtils::CXmlNode& node);
			const SpTreeElem& operator =(XmlUtils::CXmlNode& node);

			SpTreeElem& operator=(const SpTreeElem& oSrc)
			{
				m_elem = oSrc.m_elem;
				return *this;
			}
			
		public:
			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual CString toXML() const;
			virtual bool is_init() const {return (m_elem.IsInit());};

			template<class T> AVSINLINE const bool	is() const	{ return m_elem.is<T>(); }
			template<class T> AVSINLINE T&			as()		{ return m_elem.as<T>(); }
			template<class T> AVSINLINE const T&	as() const 	{ return m_elem.as<T>(); }

			void InitElem(WrapperWritingElement* pElem)
			{
				m_elem.reset(pElem);
			}
			
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				if (m_elem.is_init())
					m_elem->toPPTY(pWriter);
			}
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				if (m_elem.is_init())
					m_elem->toXmlWriter(pWriter);
			}

			smart_ptr<WrapperWritingElement> GetElem()
			{
				return m_elem;
			}

		//public:
		private:
			smart_ptr<WrapperWritingElement> m_elem;
		protected:
			virtual void FillParentPointersForChilds(){};
		public:
			virtual void SetParentPointer(const WrapperWritingElement* pParent) {if(is_init()) m_elem->SetParentPointer(pParent);};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_SLIDES_SLIDE_SHAPETREEELEM_INCLUDE_H_
