
// auto inserted precompiled begin
#include "precompiled_docx2odt.h"
// auto inserted precompiled end

#include "Pict.h"


namespace Docx2Odt
{
	namespace Private
	{

		Pict::Pict(Odt::Folder& odtFolder, const OOX::IFileContainer& docxFileContainer)
			: Base(odtFolder, docxFileContainer)
		{
		}


		const Odt::Content::Frame Pict::pict2frame(const OOX::Logic::Pict& ooxPict) const
		{
			Odt::Content::Frame odtFrame;

			odtFrame.Name = "Pict";
			odtFrame.Anchortype = "paragraph";

			odtFrame.Size.init();
			odtFrame.Size = *ooxPict.shape->style->Size;

			odtFrame.ZIndex = 0; // ???
			if (ooxPict.hasPictures())
			{
				if (ooxPict.isOle())
				{
					odtFrame.Image = pict2oleView(ooxPict);
					odtFrame.ObjectOle = ole2ole(ooxPict);
				}
				else
				{
					odtFrame.Image = pict2image(ooxPict);
				}
			}
			
			return odtFrame;
		}


		const Odt::Content::Image Pict::pict2image(const OOX::Logic::Pict& ooxPict) const
		{
			Odt::Content::Image odtImage;

			const OOX::RId rId = ooxPict.shape->imageData->rId;
			if(m_docxFileContainer.isExternal(rId))
				return odtImage;
			const OOX::Image& ooxImage = m_docxFileContainer.image(rId);
			const boost::filesystem::wpath path = m_odtFolder.Manifest->addPicture(ooxImage.filename());
			odtImage.Href = path.string();

			odtImage.Type = "simple";
			odtImage.Show = "embed";
			odtImage.Actuate = "onLoad";

			return odtImage;
		}


		const Odt::Content::Image Pict::pict2oleView(const OOX::Logic::Pict& ooxPict) const
		{
			Odt::Content::Image odtImage;

			const OOX::RId rId = ooxPict.shape->imageData->rId;
			const OOX::Image& ooxImage = m_docxFileContainer.image(rId);
			const boost::filesystem::wpath path = m_odtFolder.Manifest->addOleView(ooxImage.filename());
			odtImage.Href = path.string();

			odtImage.Type = "simple";
			odtImage.Show = "embed";
			odtImage.Actuate = "onLoad";

			return odtImage;
		}


		const Odt::Content::ObjectOle Pict::ole2ole(const OOX::Logic::Pict& ooxPict) const
		{
			Odt::Content::ObjectOle odtOle;

			const OOX::RId rId = ooxPict.OleObject->rId;
			const OOX::OleObject& ooxOleObject = m_docxFileContainer.oleObject(rId);
			const boost::filesystem::wpath path = m_odtFolder.Manifest->addOleObject(ooxOleObject.filename());

			odtOle.Href = path.string();

			odtOle.Type = "simple";
			odtOle.Show = "embed";
			odtOle.Actuate = "onLoad";

			return odtOle;
		}

	} // namespace Private
} // namespace Docx2Odt