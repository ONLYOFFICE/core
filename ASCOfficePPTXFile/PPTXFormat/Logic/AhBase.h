#pragma once
#ifndef PPTX_LOGIC_AHBASE_INCLUDE_H_
#define PPTX_LOGIC_AHBASE_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "Ah.h"
#include "AhXY.h"
#include "AhPolar.h"

namespace PPTX
{
	namespace Logic
	{
		class AhBase : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(AhBase)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				CString name = XmlUtils::GetNameNoNS(node.GetName());

				if (name == _T("ahXY"))
					ah.reset(new Logic::AhXY(node));
				else if (name == _T("ahPolar"))
					ah.reset(new Logic::AhPolar(node));
				else ah.reset();
			}

			virtual void GetAdjustHandleFrom(XmlUtils::CXmlNode& element)
			{
				XmlUtils::CXmlNode oNode;
				if (element.GetNode(_T("a:ahXY"), oNode))
					ah.reset(new Logic::AhXY(oNode));
				else if(element.GetNode(_T("a:ahPolar"), oNode))
					ah.reset(new Logic::AhPolar(oNode));
				else ah.reset();
			}

			virtual CString toXML() const
			{
				if (ah.is_init())
					return ah->toXML();

				return _T("");
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				if (ah.is_init())
					ah->toPPTY(pWriter);
			}
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				if (ah.is_init())
					ah->toXmlWriter(pWriter);
			}
			
			template<class T> const bool is() const { return (!ah.IsInit())?false:(typeid(*ah) == typeid(T));}
			template<class T> T& as() {return static_cast<T&>(*ah);}
			template<class T> const T& as() const {return static_cast<const T&>(*ah);}

			virtual bool is_init()const{return (ah.is_init());};

		public:
			smart_ptr<Ah> ah;
		protected:
			virtual void FillParentPointersForChilds(){};
		public:
			virtual void SetParentPointer(const WrapperWritingElement* pParent)
			{
				if(is_init())
					ah->SetParentPointer(pParent);
			}
			
			CString GetODString()const
			{
				if (!ah.IsInit())
					return _T("");
				return ah->GetODString();
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_AHBASE_INCLUDE_H_