#pragma once
#ifndef OOX_CONTENT_TYPES_FILE_INCLUDE_H_
#define OOX_CONTENT_TYPES_FILE_INCLUDE_H_

#include "OverrideTable.h"
#include "DefaultTable.h"
#include "./../FileType.h"


namespace OOX
{
	namespace ContentTypes
	{
		static const CPath s_filename = L"[Content_Types].xml";

		class File
		{
		public:
			File()
			{
			}
			File(const CPath& path)
			{
				read(path);
			}
			virtual ~File()
			{
			}

		public:
			virtual void read(const CPath& path)
			{
				OOX::CPath oPath = path / s_filename;
				XmlUtils::CXmlNode oNode;
				if (oNode.FromXmlFile(oPath.m_strFilename))
				{
					Default		= oNode;
					Override	= oNode;
				}
			}
			virtual void write(const CPath& path) const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("xmlns"), _T("http://schemas.openxmlformats.org/package/2006/content-types"));

				XmlUtils::CNodeValue oValue;
				oValue.Write(Default);
				oValue.Write(Override);

				OOX::CPath savepath = path / s_filename;

				XmlUtils::SaveToFile(savepath.m_strFilename, XmlUtils::CreateNode(_T("Types"), oAttr, oValue));
			}
			virtual const bool isValid() const
			{
				return true;
			}

		public:
			void registration(const CString& type, const CPath& directory, const CPath& filename)
			{
				Override.add(type, directory / filename.m_strFilename);
				Default.add(directory / filename.m_strFilename);
			}

		public:
			OverrideTable	Override;
			DefaultTable	Default;
		};
	} // namespace ContentTypes
} // namespace OOX

#endif // DOCX_CONTENT_TYPES_FILE_INCLUDE_H_