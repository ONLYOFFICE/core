#pragma once
#ifndef PPTX_VMLDRAWING_INCLUDE_H_
#define PPTX_VMLDRAWING_INCLUDE_H_

#include "WrapperFile.h"
#include "FileContainer.h"

#include "DocxFormat/RId.h"
#include "DocxFormat/Media/Image.h"

namespace PPTX
{
	class VmlDrawing : public WrapperFile, public PPTX::FileContainer
	{
	public:
		VmlDrawing()
		{
		}
		VmlDrawing(const OOX::CPath& filename, FileMap& map)
		{
			read(filename, map);
		}
		virtual ~VmlDrawing()
		{
		}

	public:
		virtual void read(const OOX::CPath& filename, FileMap& map)
		{
			//FileContainer::read(filename, map);
			XmlUtils::CXmlNode oNode;
			oNode.FromXmlFile2(filename.m_strFilename);

			m_strDocument = oNode.GetXml();
		}
		virtual void write(const OOX::CPath& filename, const OOX::CPath& directory, PPTX::ContentTypes::File& content)const
		{
			XmlUtils::SaveToFile(filename.m_strFilename, m_strDocument);

			content.registration(type().OverrideType(), directory, filename);
			m_written = true;
			m_WrittenFileName = filename.GetFilename();
			FileContainer::write(filename, directory, content);
		}

	public:
		virtual const PPTX::FileType type() const
		{
			return PPTX::FileTypes::VmlDrawing;
		}
		virtual const OOX::CPath DefaultDirectory() const
		{
			return type().DefaultDirectory();
		}
		virtual const OOX::CPath DefaultFileName() const
		{
			return type().DefaultFileName();
		}

		
		CAtlMap<CString, OOX::CPath> SpIds;
	private:
		CString m_strDocument;

	public:
		void FillRIds()
		{
			XmlUtils::CXmlNode oNode;
			oNode.FromXmlString(m_strDocument);
			GetRIds(oNode);
		}
		void GetRIds(XmlUtils::CXmlNode& element)
		{
			XmlUtils::CXmlNodes oNodes;
			if (element.GetNodes(_T("*"), oNodes))
			{
				int nCount = oNodes.GetCount();
				for (int i = 0; i < nCount; ++i)
				{
					XmlUtils::CXmlNode oNode;
					oNodes.GetAt(i, oNode);

					CString strName = XmlUtils::GetNameNoNS(oNode.GetName());
					
					if (_T("shape") != strName)
					{
						GetRIds(oNode);
						continue;
					}

					nullable_string		id;
					nullable<PPTX::RId>	rid;

					oNode.ReadAttributeBase(L"id", id);
					
					CString bsMem = oNode.ReadAttributeBase(L"spid");
					if (!bsMem.IsEmpty())
						id = bsMem;

					if (id.IsInit())
					{
						XmlUtils::CXmlNode oNodeR = oNode.ReadNodeNoNS(_T("imagedata"));
						if (oNodeR.IsValid())
						{
							oNodeR.ReadAttributeBase(L"o:relid", rid);

							if (rid.IsInit())
							{
								OOX::CPath path = image(*rid)->filename();
						 		SpIds.SetAt(*id, path);
							}
						}
					}

					oNode.ReadAttributeBase(L"spid", id);
				}
			}
		}
	};
} // namespace PPTX

#endif // PPTX_VmlDrawingS_VmlDrawing_INCLUDE_H_