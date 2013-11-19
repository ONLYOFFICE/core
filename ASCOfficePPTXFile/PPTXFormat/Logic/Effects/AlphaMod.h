#pragma once
#ifndef PPTX_LOGIC_ALPHAMOD_INCLUDE_H_
#define PPTX_LOGIC_ALPHAMOD_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "./../EffectDag.h"

namespace PPTX
{
	namespace Logic
	{

		class AlphaMod : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(AlphaMod)

			AlphaMod& operator=(const AlphaMod& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				cont = oSrc.cont;
				return *this;
			}

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				cont = node.ReadNode(_T("a:cont"));
				FillParentPointersForChilds();
			}

			virtual CString toXML() const
			{
				return _T("<a:alphaMod>") + cont.toXML() + _T("</a:alphaMod>");
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->StartRecord(EFFECT_TYPE_ALPHAMOD);

				pWriter->WriteRecord1(0, cont);

				pWriter->EndRecord();
			}

		public:
			EffectDag cont;
		protected:
			virtual void AlphaMod::FillParentPointersForChilds()
			{
				cont.SetParentPointer(this);
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_ALPHAMOD_INCLUDE_H_