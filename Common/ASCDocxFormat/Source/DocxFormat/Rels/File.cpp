
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "File.h"
#include "./../FileTypes.h"

namespace OOX
{
	namespace Rels
	{
		File::File()
		{
		}

		File::File(const OOX::CPath& filename)
		{
			read(filename);
		}

		File::~File()
		{
		}

		void File::read(const OOX::CPath& filename)
		{
			const OOX::CPath file = createFileName(filename);

			if (OOX::CSystemUtility::IsFileExist(file))
			{
				XmlUtils::CXmlNode oXml;
				oXml.FromXmlFile(file.GetPath(), true );

				Relations = RelationTable(oXml);
			}
		}

		void File::write(const OOX::CPath& filename) const
		{
		}

		const bool File::isValid() const
		{
			return true;
		}

		void File::registration(const RId& rId, const FileType& type, const OOX::CPath& filename)
		{
		}

		void File::registration(const RId& rId, const NSCommon::smart_ptr<External> external)
		{
			Relations->registration(rId, external);
		}

		const OOX::CPath File::createFileName(const OOX::CPath& oFilePath) const
		{
			CString strTemp = oFilePath.GetDirectory() + _T("\\_rels\\");
			if (_T("") == oFilePath.GetFilename())
				strTemp += _T(".rels");
			else
				strTemp += ( oFilePath.GetFilename() + _T(".rels") );

			return strTemp;
		}

	} // namespace Rels
} // namespace OOX