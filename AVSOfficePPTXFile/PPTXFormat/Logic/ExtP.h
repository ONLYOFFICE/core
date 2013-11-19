#ifndef PPTX_LOGIC_EXTP_PROPERTIES_INCLUDE_H_
#define PPTX_LOGIC_EXTP_PROPERTIES_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "../DocxFormat/RId.h"

/*

<p:ext uri="{DAA4B4D4-6D71-4841-9C94-3DE7FCFB9230}">
<p14:media xmlns:p14="http://schemas.microsoft.com/office/powerpoint/2010/main" r:embed="rId2">
<p14:trim st="93333.0816" end="96583.4816"/>
</p14:media>
</p:ext>

*/

namespace PPTX
{
	namespace Logic
	{
		class Ext : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(Ext)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				XmlUtils::CXmlNode media;
				if (node.GetNode(_T("p14:media"), media))
				{
					link	=	media.GetAttribute(_T("r:embed"));

					CString xmkl = media.GetXml();

					XmlUtils::CXmlNode trim;
					if (media.GetNode(_T("p14:trim"), trim))
					{
						trim.ReadAttributeBase(_T("st"), st);
						trim.ReadAttributeBase(_T("end"), end);
					}
				}
			}

			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				XmlUtils::CNodeValue oValue;

				return XmlUtils::CreateNode(_T("p:ext"), oValue);
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				// TODO:
			}
			
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				link.toPPTY(0, pWriter);
				pWriter->WriteDouble2(1, st);
				pWriter->WriteDouble2(2, end);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
			}

		public:

			OOX::RId				link;

			// trim
			nullable_double			st;
			nullable_double			end;

		protected:
			virtual void FillParentPointersForChilds()
			{

			}
		};
	} 
} 

#endif 