#pragma once
#ifndef PPTX_LOGIC_UNIPATH2D_INCLUDE_H_
#define PPTX_LOGIC_UNIPATH2D_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "Path2D/PathBase.h"
#include "Path2D/MoveTo.h"
#include "Path2D/LineTo.h"
#include "Path2D/Close.h"
#include "Path2D/CubicBezTo.h"
#include "Path2D/ArcTo.h"
#include "Path2D/QuadBezTo.h"

namespace PPTX
{
	namespace Logic
	{
		class UniPath2D : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(UniPath2D)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				CString name = XmlUtils::GetNameNoNS(node.GetName());

				if (name == _T("moveTo"))
					Path2D.reset(new Logic::MoveTo(node));
				else if (name == _T("lnTo"))
					Path2D.reset(new Logic::LineTo(node));
				else if (name == _T("cubicBezTo"))
					Path2D.reset(new Logic::CubicBezTo(node));
				else if (name == _T("close"))
					Path2D.reset(new Logic::Close(node));
				else if (name == _T("arcTo"))
					Path2D.reset(new Logic::ArcTo(node));
				else if (name == _T("quadBezTo"))
					Path2D.reset(new Logic::QuadBezTo(node));
				else Path2D.reset();
			}

			virtual void GetPath2DFrom(XmlUtils::CXmlNode& element)
			{
				XmlUtils::CXmlNode oNode;
				
				if(element.GetNode(_T("a:moveTo"), oNode))
					Path2D.reset(new Logic::MoveTo(oNode));
				else if(element.GetNode(_T("a:lnTo"), oNode))
					Path2D.reset(new Logic::LineTo(oNode));
				else if(element.GetNode(_T("a:cubicBezTo"), oNode))
					Path2D.reset(new Logic::CubicBezTo(oNode));
				else if(element.GetNode(_T("a:close"), oNode))
					Path2D.reset(new Logic::Close(oNode));
				else if(element.GetNode(_T("a:arcTo"), oNode))
					Path2D.reset(new Logic::ArcTo(oNode));
				else if(element.GetNode(_T("a:quadBezTo"), oNode))
					Path2D.reset(new Logic::QuadBezTo(oNode));
				else Path2D.reset();
			}

			virtual CString toXML() const
			{
				if (Path2D.IsInit())
					return Path2D->toXML();
				return _T("");
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				if (Path2D.is_init())
					Path2D->toPPTY(pWriter);
			}
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				if (Path2D.is_init())
					Path2D->toXmlWriter(pWriter);
			}

			virtual bool is_init()const{return (Path2D.IsInit());};
			
			template<class T> const bool is() const { return (!Path2D.IsInit())?false:(typeid(*Path2D) == typeid(T));}
			template<class T> T& as() {return static_cast<T&>(*Path2D);}
			template<class T> const T& as() const {return static_cast<const T&>(*Path2D);}

		public:
			smart_ptr<PathBase> Path2D;
		protected:
			virtual void FillParentPointersForChilds(){};
		public:
			virtual void SetParentPointer(const WrapperWritingElement* pParent)
			{
				if(is_init())
					Path2D->SetParentPointer(pParent);
			};

			CString GetODString()const
			{
				return Path2D->GetODString();
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_UNIPATH2D_INCLUDE_H