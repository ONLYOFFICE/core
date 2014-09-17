#pragma once
#ifndef OOX_XLSXIFILE_CONTAINER_INCLUDE_H_
#define OOX_XLSXIFILE_CONTAINER_INCLUDE_H_

#include "RId.h"
#include "UnknowTypeFile.h"
#include "IFileBuilder.h"

namespace OOX 
{
	class File;
	class FileType;
	class CRels;
	class CContentTypes;
	
	class Image;
	class HyperLink;
	class OleObject;
} // OOX

namespace OOX
{
	class IFileContainer
	{
	public:

		IFileContainer()
		{
			m_lMaxRid = 0;
		}
	protected:

        //CAtlMap<CString, smart_ptr<OOX::File>> m_mContainer;
        std::map<CString, smart_ptr<OOX::File>> m_mContainer;
		size_t                                m_lMaxRid;

	protected:

		void Read (const OOX::CPath& oPath);
		void Read (const OOX::CRels& oRels,     const CPath& oPath);
		void Write(const OOX::CPath& oFileName, const CPath& oDir, OOX::CContentTypes& oContent) const;
		void Write(OOX::CRels& oRels, const CPath& oCurrent, const CPath& oDir, OOX::CContentTypes& oContent) const;

	protected:
		void Commit  (const CPath& oPath);
		void Finalize(const CPath& oFilefilename, const CPath& oDir, OOX::CContentTypes& oContent);
		void Finalize(OOX::CRels& oRels, const CPath& oCurrent, const CPath& oDir, OOX::CContentTypes& oContent);

	public:

		void ExtractPictures(const OOX::CPath& oPath) const;

	public:

		virtual smart_ptr<Image>     GetImage    (const RId& rId) const;
		virtual smart_ptr<HyperLink> GetHyperlink(const RId& rId) const;
		virtual smart_ptr<OleObject> GetOleObject(const RId& rId) const;

	public:

		template<typename T>
		const bool IsExist() const;
		const bool IsExist(const FileType& oType) const;
		const bool IsExist(const OOX::RId& rId) const;
		const bool IsExternal(const OOX::RId& rId) const;

		smart_ptr<OOX::File> Get(const FileType& oType);
		const RId Add(smart_ptr<OOX::File>& pFile);
		void      Add(const OOX::RId& rId, const smart_ptr<OOX::File>& pFile);

		template<typename T> 
		T&                   Find();
		smart_ptr<OOX::File> Find(const FileType& type) const;
		smart_ptr<OOX::File> Find(const OOX::RId& type) const;

		smart_ptr<OOX::File> operator [](const OOX::RId rId);
		smart_ptr<OOX::File> operator [](const FileType& oType);


	protected:

		static UnknowTypeFile Unknown;

	private:

		const RId GetMaxRId();
	};

} // namespace OOX

#endif // OOX_XLSXIFILE_CONTAINER_INCLUDE_H_
