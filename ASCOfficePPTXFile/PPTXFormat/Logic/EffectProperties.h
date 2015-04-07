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
			PPTX_LOGIC_BASE(EffectProperties)

			EffectProperties& operator=(const EffectProperties& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				return *this;
			}

		public:
			
			virtual bool is_init() const {return (List.IsInit());};

			template<class T> const bool is() const { return (!List.IsInit())?false:(typeid(*List) == typeid(T));}
			template<class T> T& as() {return static_cast<T&>(*List);}
			template<class T> const T& as() const {return static_cast<const T&>(*List);}

			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				CString strName = XmlUtils::GetNameNoNS(node.GetName());

				if (strName == _T("effectLst"))
					List.reset(new Logic::EffectLst(node));
				else if(strName == _T("effectDag"))
					List.reset(new Logic::EffectDag(node));
				else List.reset();
			}

			virtual void GetEffectListFrom(XmlUtils::CXmlNode& element)
			{
				XmlUtils::CXmlNode oNode = element.ReadNodeNoNS(_T("effectLst"));
				if (oNode.IsValid())
				{
					List.reset(new Logic::EffectLst(oNode));
					return;
				}
				oNode = element.ReadNodeNoNS(_T("effectDag"));
				if (oNode.IsValid())
					List.reset(new Logic::EffectDag(oNode));
				else List.reset();
			}

			virtual CString toXML() const
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
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				if (List.is_init())
					List->toXmlWriter(pWriter);
			}

		//public:
		private:
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