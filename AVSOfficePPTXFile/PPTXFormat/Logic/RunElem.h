#pragma once
#ifndef PPTX_LOGIC_RUNELEM_INCLUDE_H_
#define PPTX_LOGIC_RUNELEM_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "Runs/RunBase.h"
#include "Runs/Run.h"
#include "Runs/Fld.h"
#include "Runs/Br.h"

namespace PPTX
{
	namespace Logic
	{
		class RunElem : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(RunElem)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				CString name = XmlUtils::GetNameNoNS(node.GetName());

				if(name == _T("r"))
					Elem.reset(new Logic::Run(node));
				else if(name == _T("fld"))
					Elem.reset(new Logic::Fld(node));
				else if(name == _T("br"))
					Elem.reset(new Logic::Br(node));
				else Elem.reset();
			}
			virtual CString toXML() const
			{
				if (is_init())
					return Elem->toXML();
				return _T("");
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				if (is_init())
					Elem->toXmlWriter(pWriter);
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				if (Elem.is_init())
					Elem->toPPTY(pWriter);
			}

			void InitRun(RunBase* pRun)
			{
				Elem.reset(pRun);
			}

			virtual bool is_init() const {return (Elem.IsInit());};
			virtual CString GetText() const{return Elem->GetText();}

			template<class T> AVSINLINE const bool	is() const	{ return Elem.is<T>(); }
			template<class T> AVSINLINE T&			as()		{ return Elem.as<T>(); }
			template<class T> AVSINLINE const T&	as() const 	{ return Elem.as<T>(); }

		//public:
		private:
			smart_ptr<RunBase> Elem;
		protected:
			virtual void FillParentPointersForChilds(){};
		public:
			virtual void SetParentPointer(const WrapperWritingElement* pParent){if(is_init()) Elem->SetParentPointer(pParent);}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_RUNELEM_INCLUDE_H