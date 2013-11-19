#pragma once
#ifndef PPTX_PRESENTATION_PHOTOALBUM_INCLUDE_H_
#define PPTX_PRESENTATION_PHOTOALBUM_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "./../Limit/FrameShape.h"
#include "./../Limit/AlbumLayout.h"

namespace PPTX
{
	namespace nsPresentation
	{
		class PhotoAlbum : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(PhotoAlbum)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				node.ReadAttributeBase(L"bw", bw);
				node.ReadAttributeBase(L"frame", frame);
				node.ReadAttributeBase(L"layout", layout);
				node.ReadAttributeBase(L"showCaptions", showCaptions);
			}
			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("bw"), bw);
				oAttr.WriteLimitNullable(_T("frame"), frame);
				oAttr.WriteLimitNullable(_T("layout"), layout);
				oAttr.Write(_T("showCaptions"), showCaptions);

				return XmlUtils::CreateNode(_T("p:photoAlbum"), oAttr);
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				// TODO:
			}

		public:
			nullable_bool							bw;
			nullable_limit<Limit::FrameShape>		frame;
			nullable_limit<Limit::AlbumLayout>		layout;
			nullable_bool							showCaptions;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace nsPresentation
} // namespace PPTX

#endif // PPTX_PRESENTATION_PHOTOALBUM_INCLUDE_H_