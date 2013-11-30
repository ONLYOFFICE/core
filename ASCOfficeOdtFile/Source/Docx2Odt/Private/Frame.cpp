
// auto inserted precompiled begin
#include "precompiled_docx2odt.h"
// auto inserted precompiled end

#include "Frame.h"
#include "NamedStyle.h"
#include <boost_filesystem_version.h>


namespace Docx2Odt
{
	namespace Private
	{

		Frame::Frame(Odt::Folder& odtFolder, const OOX::IFileContainer& docxFileContainer)
			: Base(odtFolder, docxFileContainer)
		{
		}


		const Odt::Content::Frame Frame::drawing2frame(const OOX::Logic::Drawing& ooxDrawing) const
		{
			Odt::Content::Frame odtFrame;

			NamedStyle styleConverter(m_odtFolder, m_docxFileContainer);
			Odt::Logic::NamedStyle odtStyle = styleConverter.drawing2style(ooxDrawing);

			styleConverter.addStyle2Content(odtStyle);

			odtFrame.Style = odtStyle.Name;
			odtFrame.Name = ooxDrawing.Inline->DocPr->Name;
			//odtFrame.Anchortype = "paragraph";
			if(*ooxDrawing.Inline->Name == "inline")
				odtFrame.Anchortype = "as-char";
			else
			{
				if(ooxDrawing.Inline->positionVRelativeFrom.get_value_or("paragraph") == "paragraph")
				{
					odtFrame.Anchortype = "paragraph";
					odtFrame.X = UniversalUnit(ooxDrawing.Inline->positionH.get_value_or(0), UniversalUnit::Emu);
					odtFrame.X->Type = UniversalUnit::Cm;
					odtFrame.Y = UniversalUnit(ooxDrawing.Inline->positionV.get_value_or(0), UniversalUnit::Emu);
					odtFrame.Y->Type = UniversalUnit::Cm;
				}
				else
				{
					odtFrame.Anchortype = "as-char";
				}
			}

			odtFrame.Size.init();
			odtFrame.Size->Width = *ooxDrawing.Inline->Extent->Size->Width;
			odtFrame.Size->Height = *ooxDrawing.Inline->Extent->Size->Height;

			odtFrame.ZIndex = 0; // ???
			odtFrame.Image = drawing2image(ooxDrawing);

			return odtFrame;
		}


		const Odt::Content::Image Frame::drawing2image(const OOX::Logic::Drawing& ooxDrawing) const
		{
			Odt::Content::Image odtImage;

			const OOX::RId rId = ooxDrawing.Inline->Graphic->Pic->rId;
			const OOX::Image& ooxImage = m_docxFileContainer.image(rId);
			const boost::filesystem::wpath path = m_odtFolder.Manifest->addPicture(ooxImage.filename());
			odtImage.Href = path.string();

			odtImage.Type = "simple";
			odtImage.Show = "embed";
			odtImage.Actuate = "onLoad";

			return odtImage;
		}
	} // namespace Private
} // namespace Docx2Odt