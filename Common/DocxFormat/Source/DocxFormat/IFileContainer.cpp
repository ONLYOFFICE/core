#include "IFileContainer.h"
#include "Rels.h"
#include "FileFactory.h"
#include "ContentTypes.h"

#include "FileType.h"
#include "External/External.h"
#include "External/HyperLink.h"
#include "Media/Image.h"
#include "Media/OleObject.h"
#include "FileTypes.h"

#include "../XlsxFormat/FileFactory_Spreadsheet.h"
//BOOL XmlUtils::CStringWriter::m_bInitTable = FALSE;
//BYTE XmlUtils::CStringWriter::m_arTableUnicodes[65536];

namespace OOX
{
	UnknowTypeFile IFileContainer::Unknown;

	void IFileContainer::Read (const OOX::CPath& oRootPath, const OOX::CPath& oPath)
	{
		// Находим связи(рельсы) с данным файлом
		OOX::CRels oRels( oPath );

		// Читаем все файлы по рельсам
		Read( oRels, oRootPath, oPath.GetDirectory() );
	}


	void IFileContainer::Read (const OOX::CRels& oRels, const OOX::CPath& oRootPath, const OOX::CPath& oPath)
	{
		unsigned int nCount = oRels.m_arrRelations.size();

		for ( unsigned int nIndex = 0; nIndex < nCount; ++nIndex )
		{
			Rels::CRelationShip *oCurRels = oRels.m_arrRelations[nIndex];

			if (oCurRels == NULL) continue;
			
			smart_ptr<OOX::File> oFile = OOX::CreateFile( oRootPath, oPath, oCurRels );
			if(oFile.IsInit() && FileTypes::Unknow == oFile->type())
				oFile = OOX::Spreadsheet::CreateFile( oRootPath, oPath, oCurRels );
			Add( oCurRels->rId(), oFile );
		}
    }


	void IFileContainer::Write(const OOX::CPath& oFileName, const OOX::CPath& oDirectory, OOX::CContentTypes& oContent) const
	{
		OOX::CPath oCurrent = oFileName.GetDirectory();
		OOX::CRels oRels;

		Write( oRels, oCurrent, oDirectory, oContent );
		oRels.Write( oFileName );
	}
	void IFileContainer::Write(OOX::CRels& oRels, const OOX::CPath& oCurrent, const OOX::CPath& oDir, OOX::CContentTypes& oContent) const
	{
		std::map<CString, size_t> mNamePair;
		for (std::map<CString, smart_ptr<OOX::File>>::const_iterator it = m_mContainer.begin(); it != m_mContainer.end(); ++it)
		{
			smart_ptr<OOX::File>     pFile = it->second;
			smart_ptr<OOX::External> pExt  = pFile.smart_dynamic_cast<OOX::External>();
			if ( !pExt.IsInit() )
			{
				OOX::CPath oDefDir = pFile->DefaultDirectory();
				OOX::CPath oName   = pFile->DefaultFileName();
		
				std::map<CString, size_t>::const_iterator pNamePair = mNamePair.find( oName.m_strFilename );
				if ( pNamePair == mNamePair.end())
					mNamePair [oName.m_strFilename] = 1;
				else
					oName = oName + pNamePair->first;

				OOX::CSystemUtility::CreateDirectories( oCurrent / oDefDir );
				pFile->write( oCurrent / oDefDir / oName, oDir / oDefDir, oContent );
				oRels.Registration( it->first, pFile->type(), oDefDir / oName );
			}
			else
			{
				oRels.Registration( it->first, pExt );
			}
		}
	}


	void IFileContainer::Commit  (const OOX::CPath& oPath)
	{

		std::map<CString, size_t> mNamepair;

		for (std::map<CString, smart_ptr<OOX::File>>::const_iterator it = m_mContainer.begin(); it != m_mContainer.end(); ++it)
		{
			smart_ptr<OOX::File>     pFile = it->second;
			smart_ptr<OOX::External> pExt  = pFile.smart_dynamic_cast<OOX::External>();

			if (!pExt.IsInit())
			{
				OOX::CPath oDefDir = pFile->DefaultDirectory();
				OOX::CPath oName   = pFile->DefaultFileName();

				std::map<CString, size_t>::const_iterator pNamePair = mNamepair.find( oName.m_strFilename );

				if (pNamePair == mNamepair.end())
					mNamepair [oName.m_strFilename] = 1;
				else
					oName = oName + pNamePair->first;

				OOX::CSystemUtility::CreateDirectories( oPath / oDefDir );
				
				smart_ptr<OOX::IFileBuilder> pFileBuilder = it->second.smart_dynamic_cast<OOX::IFileBuilder>();
				if ( pFileBuilder.is_init() )
					pFileBuilder->Commit( oPath / oDefDir / oName );
			}
		}
	}


	void IFileContainer::Finalize(const OOX::CPath& oFileName, const OOX::CPath& oDirectory, OOX::CContentTypes& oContent)
	{
		OOX::CPath oCurrent = oFileName.GetDirectory();
		OOX::CRels oRels;

		Finalize( oRels, oCurrent, oDirectory, oContent );
		oRels.Write( oFileName );
	}	
	void IFileContainer::Finalize(OOX::CRels& oRels, const OOX::CPath& oCurrent, const OOX::CPath& oDir, OOX::CContentTypes& oContent)
	{
		std::map<CString, size_t> mNamepair;

		for (std::map<CString, smart_ptr<OOX::File>>::const_iterator it = m_mContainer.begin(); it != m_mContainer.end(); ++it)
		{			
			smart_ptr<OOX::File>     pFile = it->second;
			smart_ptr<OOX::External> pExt  = pFile.smart_dynamic_cast<OOX::External>();

			if ( !pExt.IsInit() )
			{
				OOX::CPath oDefDir = pFile->DefaultDirectory();
				OOX::CPath oName   = pFile->DefaultFileName();

				std::map<CString, size_t>::iterator pNamePair = mNamepair.find( oName.m_strFilename );
				if ( pNamePair == mNamepair.end() )
					mNamepair [oName.m_strFilename] = 1;
				else
					oName = oName + pNamePair->first;

				OOX::CSystemUtility::CreateDirectories( oCurrent / oDefDir );				
				smart_ptr<OOX::IFileBuilder> pFileBuilder = pFile.smart_dynamic_cast<OOX::IFileBuilder>(); 
				if ( pFileBuilder.is_init() )
				{
					pFileBuilder->Finalize( oCurrent / oDefDir / oName, oDir / oDefDir, oContent );
				}
				else
				{
					pFile->write( oCurrent / oDefDir / oName, oDir / oDefDir, oContent );
				}

				oRels.Registration( it->first, pFile->type(), oDefDir / oName );
			}
			else
			{
				oRels.Registration( it->first, pExt );
			}
		}
	}


	void IFileContainer::ExtractPictures(const OOX::CPath& oPath) const
	{
		for (std::map<CString, smart_ptr<OOX::File>>::const_iterator it = m_mContainer.begin(); it != m_mContainer.end(); ++it)
		{
			smart_ptr<OOX::File> pFile  = it->second;

			smart_ptr<Image>     pImage = pFile.smart_dynamic_cast<Image>();
			if ( pImage.is_init() )
			{
				pImage->copy_to( oPath );
				continue;
			}

			smart_ptr<IFileContainer> pExt = pFile.smart_dynamic_cast<IFileContainer>();
			if ( pExt.is_init() )
			{
				pExt->ExtractPictures( oPath );
				continue;
			}
		}
	}


	smart_ptr<Image>     IFileContainer::GetImage    (const RId& rId) const
	{
		std::map<CString, smart_ptr<OOX::File>>::const_iterator pPair = m_mContainer.find(rId.get());
		if (pPair == m_mContainer.end ())
			return smart_ptr<Image>();
		return pPair->second.smart_dynamic_cast<Image>();
	}

	smart_ptr<HyperLink> IFileContainer::GetHyperlink(const RId& rId) const
	{
		std::map<CString, smart_ptr<OOX::File>>::const_iterator pPair = m_mContainer.find(rId.get());
		if (pPair == m_mContainer.end ())
			return smart_ptr<HyperLink>();
		return pPair->second.smart_dynamic_cast<HyperLink>();
	}

	smart_ptr<OleObject> IFileContainer::GetOleObject(const RId& rId) const
	{
		std::map<CString, smart_ptr<OOX::File>>::const_iterator pPair = m_mContainer.find(rId.get());
		if (pPair == m_mContainer.end ())
			return smart_ptr<OleObject>();
		return pPair->second.smart_dynamic_cast<OleObject>();
	}

	const bool IFileContainer::IsExist(const FileType& oType) const
	{
		for (std::map<CString, smart_ptr<OOX::File>>::const_iterator it = m_mContainer.begin(); it != m_mContainer.end(); ++it)
		{
			if (oType == it->second->type())
				return true;
		}

		return false;
	}
	const bool IFileContainer::IsExist(const RId& rId) const
	{
		std::map<CString, smart_ptr<OOX::File>>::const_iterator it = m_mContainer.find(rId.get());
		return (it != m_mContainer.end());
	}


	template<typename T>
	const bool IFileContainer::IsExist() const
	{
		T oFile;
		return IsExist( oFile.type() );
	}	
	const bool IFileContainer::IsExternal(const OOX::RId& rId) const
	{
		std::map<CString, smart_ptr<OOX::File>>::const_iterator it = m_mContainer.find(rId.get());

		if (it != m_mContainer.end())
		{
			CString sType = it->second->type().RelationType();
			CString sName = it->second->type().DefaultFileName().m_strFilename;
			
			return (( ( sType == OOX::FileTypes::ExternalAudio.RelationType() ) || ( sType == OOX::FileTypes::ExternalImage.RelationType() ) || ( sType == OOX::FileTypes::ExternalVideo.RelationType() ) ) && ( sName == _T("") ) );
		}
		return true;
	}


	smart_ptr<OOX::File> IFileContainer::Get(const FileType& oType)
	{
		for (std::map<CString, smart_ptr<OOX::File>>::const_iterator it = m_mContainer.begin(); it != m_mContainer.end(); ++it)
		{
			if (oType == it->second->type())
				return it->second;
		}

		return smart_ptr<OOX::File>(new UnknowTypeFile( Unknown ));
	}


	const RId IFileContainer::Add(smart_ptr<OOX::File>& pFile)
	{
		const RId rId = GetMaxRId().next();
		Add( rId, pFile );
		return rId;
	}


	void      IFileContainer::Add(const OOX::RId& rId, const smart_ptr<OOX::File>& pFile)
	{
        m_lMaxRid = (std::max)( m_lMaxRid, rId.getNumber() );
		m_mContainer [rId.get()] = pFile;
	}
	smart_ptr<OOX::File> IFileContainer::Find(const FileType& oType) const
	{
		for (std::map<CString, smart_ptr<OOX::File>>::const_iterator it = m_mContainer.begin(); it != m_mContainer.end(); ++it)
		{
			if (oType == it->second->type())
				return it->second;

		}

		return smart_ptr<OOX::File>( (OOX::File*)new UnknowTypeFile() );
	}

	smart_ptr<OOX::File> IFileContainer::Find(const OOX::RId& rId) const
	{
		std::map<CString, smart_ptr<OOX::File>>::const_iterator it = m_mContainer.find(rId.get());

		if (it != m_mContainer.end())
			return it->second;

		return smart_ptr<OOX::File>( (OOX::File*)new UnknowTypeFile() );
	}


	template<typename T>
	T&                   IFileContainer::Find()
	{
		T oFile;
		return dynamic_cast<T&>( Find( oFile.type() ) );
	}	
	smart_ptr<OOX::File> IFileContainer::operator [](const OOX::RId rId)
	{
		std::map<CString, smart_ptr<OOX::File>>::const_iterator it = m_mContainer.find(rId.get());

		if (it != m_mContainer.end())
			return it->second;

		return smart_ptr<OOX::File>( (OOX::File*)new UnknowTypeFile() );
	}


	smart_ptr<OOX::File> IFileContainer::operator [](const FileType& oType)
	{
		return Find( oType );
	}

	const RId IFileContainer::GetMaxRId()
	{
		++m_lMaxRid;
		return RId( m_lMaxRid );
	}





} // namespace OOX
