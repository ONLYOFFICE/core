#pragma once
#ifndef PPTX_LOGIC_WAVAUDIOFILE_INCLUDE_H_
#define PPTX_LOGIC_WAVAUDIOFILE_INCLUDE_H_

#include "../../WrapperWritingElement.h"
#include "../../DocxFormat/RId.h"

namespace PPTX
{
	namespace Logic
	{
		class WavAudioFile : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(WavAudioFile)

			WavAudioFile& operator=(const WavAudioFile& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				name  = oSrc.name;
				embed = oSrc.embed;
				m_name = oSrc.m_name;
				return *this;
			}
		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				m_name	= XmlUtils::GetNameNoNS(node.GetName());

				embed	= node.GetAttribute(_T("r:embed"));
				node.ReadAttributeBase(L"name", name);
			}

			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("r:embed"), embed.ToString());
				oAttr.Write(_T("name"), name);

				return XmlUtils::CreateNode(_T("a:") + m_name, oAttr);
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(_T("a:") + m_name);

				pWriter->StartAttributes();
				pWriter->WriteAttribute(_T("r:embed"), embed.ToString());
				pWriter->WriteAttribute(_T("name"), name);
				pWriter->EndAttributes();
				
				pWriter->EndNode(_T("a:") + m_name);
			}

		public:
			nullable_string name;
			OOX::RId		embed;
		public:
			CString			m_name;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_WAVAUDIOFILE_INCLUDE_H_