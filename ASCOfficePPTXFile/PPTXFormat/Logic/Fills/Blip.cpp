//#include "./stdafx.h"

#include "Blip.h"
#include "./../../Slide.h"
#include "./../../SlideMaster.h"
#include "./../../SlideLayout.h"
#include "./../../Theme.h"

namespace PPTX
{
	namespace Logic
	{
		void Blip::fromXML(XmlUtils::CXmlNode& node)
		{
			m_namespace = XmlUtils::GetNamespace(node.GetName());

			node.ReadAttributeBase(L"r:embed", embed);
			node.ReadAttributeBase(L"r:link", link);
			node.ReadAttributeBase(L"cstate", cstate);

			Effects.RemoveAll();
			node.LoadArray(_T("*"), Effects);

			FillParentPointersForChilds();
		}

		CString Blip::toXML() const
		{
			XmlUtils::CAttribute oAttr;

			if (embed.IsInit())
				oAttr.Write(_T("r:embed"), embed->ToString());
			if (link.IsInit())
				oAttr.Write(_T("r:link"), link->ToString());
			oAttr.WriteLimitNullable(_T("cstate"), cstate);

			XmlUtils::CNodeValue oValue;
			oValue.WriteArray(Effects);

			CString strName = (_T("") == m_namespace) ? _T("blip") : (m_namespace + _T(":blip"));
			return XmlUtils::CreateNode(strName, oAttr, oValue);
		}

		void Blip::FillParentPointersForChilds()
		{
			size_t count = Effects.GetCount();
			for(size_t i = 0; i < count; ++i)
				Effects[i].SetParentPointer(this);
		}

		CString Blip::GetFullPicName(FileContainer* pRels)const
		{
			if(embed.IsInit())
			{
				if (pRels != NULL)
				{
					smart_ptr<PPTX::Image> p = pRels->image(*embed);
					if (p.is_init())
						return p->filename().m_strFilename;
				}

				if(parentFileIs<Slide>())
					return parentFileAs<Slide>().GetMediaFullPathNameFromRId(*embed);
				else if(parentFileIs<SlideLayout>())
					return parentFileAs<SlideLayout>().GetMediaFullPathNameFromRId(*embed);
				else if(parentFileIs<SlideMaster>())
					return parentFileAs<SlideMaster>().GetMediaFullPathNameFromRId(*embed);
				else if(parentFileIs<Theme>())
					return parentFileAs<Theme>().GetMediaFullPathNameFromRId(*embed);
				return _T("");
			}
			else if(link.IsInit())
			{
				if (pRels != NULL)
				{
					smart_ptr<PPTX::Image> p = pRels->image(*link);
					if (p.is_init())
						return p->filename().m_strFilename;
				}

				if(parentFileIs<Slide>())
					return parentFileAs<Slide>().GetMediaFullPathNameFromRId(*link);
				else if(parentFileIs<SlideLayout>())
					return parentFileAs<SlideLayout>().GetMediaFullPathNameFromRId(*link);
				else if(parentFileIs<SlideMaster>())
					return parentFileAs<SlideMaster>().GetMediaFullPathNameFromRId(*link);
				else if(parentFileIs<Theme>())
					return parentFileAs<Theme>().GetMediaFullPathNameFromRId(*link);
				return _T("");
			}
			return _T("");
		}
	} // namespace Logic
} // namespace PPTX