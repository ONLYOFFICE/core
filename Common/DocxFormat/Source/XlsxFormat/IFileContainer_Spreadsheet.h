#pragma once
#ifndef OOX_IFILE_CONTAINER_INCLUDE_H_
#define OOX_IFILE_CONTAINER_INCLUDE_H_

#include "../DocxFormat/RId.h"
#include "../DocxFormat/UnknowTypeFile.h"
#include "../DocxFormat/IFileBuilder.h"
#include <map>

namespace OOX 
{
	class File;
	class FileType;
	class CRels;
	class CContentTypes;

	class HyperLink;
} // OOX

namespace OOX
{
	namespace Spreadsheet
	{
		class IFileContainer
		{
		public:

			IFileContainer()
			{
				m_lMaxRid = 0;
				m_pCurRels = NULL;
			}
			~IFileContainer()
			{
				RELEASEOBJECT(m_pCurRels);
			}
		protected:

			std::map<CString, smart_ptr<OOX::File>> m_mContainer;
			size_t                                m_lMaxRid;
			static std::map<CString, size_t> m_mapEnumeratedGlobal;

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
			OOX::CRels* GetCurRls()
			{
				return m_pCurRels;
			}
		public:

			template<typename T>
			const bool IsExist() const;
			const bool IsExist(const FileType& oType) const;
			const bool IsExist(const OOX::RId& rId) const;
			const bool IsExternal(const OOX::RId& rId) const;
			CString IsExistHyperlink(smart_ptr<OOX::HyperLink>& pHyperLink);

			smart_ptr<OOX::File> Get(const FileType& oType);
			const RId Add(smart_ptr<OOX::File>& pFile);
            void      Add(const OOX::RId& rId, const smart_ptr<OOX::File>& pFile);

			template<typename T> 
			T&                   Find();
			smart_ptr<OOX::File> Find(const FileType& type) const;
			void FindAllByType(const FileType& oType, std::map<CString, smart_ptr<OOX::File>>& aOutput) const;
			smart_ptr<OOX::File> Find(const OOX::RId& type) const;

			smart_ptr<OOX::File> operator [](const OOX::RId rId);
			smart_ptr<OOX::File> operator [](const FileType& oType);
			void SetGlobalNumberByType(const CString& sOverrideType, int val);
			int GetGlobalNumberByType(const CString& sOverrideType);
		protected:

			static UnknowTypeFile Unknown;

		private:
			std::map<CString, size_t> m_mapAddNamePair;
			OOX::CRels* m_pCurRels;
			const RId GetMaxRId();
		};
	}
} // namespace OOX

#endif // OOX_IFILE_CONTAINER_INCLUDE_H_
