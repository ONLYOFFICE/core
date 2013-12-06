#pragma once
#ifndef OOX_CORE_INCLUDE_H_
#define OOX_CORE_INCLUDE_H_

#include "File.h"

#include "../.././../../Common/DocxFormat/Source/Base/Nullable.h"
#include "../.././../../Common/DocxFormat/Source/Xml/XmlUtils.h"

namespace OOX
{
	class Core : public OOX::File
	{
	public:
		Core();
		Core(const OOX::CPath& filename);
		virtual ~Core();

	public:
		virtual void read(const OOX::CPath& oPath);
		virtual void write(const OOX::CPath& oPath, const OOX::CPath& directory, ContentTypes::File& content) const;

	public:
		virtual const FileType type() const;
		virtual const OOX::CPath DefaultDirectory() const;
		virtual const OOX::CPath DefaultFileName() const;

	private:

		NSCommon::nullable<std::wstring> m_sCategory;
		NSCommon::nullable<std::wstring> m_sContentStatus;
		NSCommon::nullable<std::wstring> m_sCreated;
		NSCommon::nullable<std::wstring> m_sCreator;
		NSCommon::nullable<std::wstring> m_sDescription;
		NSCommon::nullable<std::wstring> m_sIdentifier;
		NSCommon::nullable<std::wstring> m_sKeywords;
		NSCommon::nullable<std::wstring> m_sLanguage;
		NSCommon::nullable<std::wstring> m_sLastModifiedBy;
		NSCommon::nullable<std::wstring> m_sLastPrinted;
		NSCommon::nullable<std::wstring> m_sModified;
		NSCommon::nullable<std::wstring> m_sRevision;
		NSCommon::nullable<std::wstring> m_sSubject;
		NSCommon::nullable<std::wstring> m_sTitle;
		NSCommon::nullable<std::wstring> m_sVersion;

	};
} // namespace OOX

#endif // OOX_CORE_INCLUDE_H_