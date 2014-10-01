//#include "stdafx.h"
#include "CommonInclude.h"

#include "IFileContainer_Spreadsheet.h"
#include "FileFactory_Spreadsheet.h"

#include "../DocxFormat/Rels.h"
#include "../DocxFormat/ContentTypes.h"

#include "../DocxFormat/FileType.h"
#include "../DocxFormat/External/External.h"

#include "../DocxFormat/External/HyperLink.h"

namespace OOX
{
	namespace Spreadsheet
	{
		std::map<CString, size_t> IFileContainer::m_mapEnumeratedGlobal;
		UnknowTypeFile IFileContainer::Unknown;

		void IFileContainer::Read (const OOX::CPath& oPath)
		{
			// Находим связи(рельсы) с данным файлом
			RELEASEOBJECT(m_pCurRels);
			m_pCurRels = new OOX::CRels(oPath);

			// Читаем все файлы по рельсам
			Read( *m_pCurRels, oPath.GetDirectory() );
		}


		void IFileContainer::Read (const OOX::CRels& oRels, const OOX::CPath& oPath)
		{
			unsigned int nCount = oRels.m_arrRelations.size();

			for (unsigned int nIndex = 0; nIndex < nCount; ++nIndex )
			{
                smart_ptr<OOX::File> pFile = OOX::Spreadsheet::CreateFile( oPath, oRels.m_arrRelations[nIndex] );
				Add( oRels.m_arrRelations[nIndex]->rId(), pFile );
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
			for (std::map<CString, smart_ptr<OOX::File>>::const_iterator pPair = m_mContainer.begin(); pPair != m_mContainer.end(); ++pPair)
			{
				smart_ptr<OOX::File>     pFile = pPair->second;
				smart_ptr<OOX::External> pExt  = pFile.smart_dynamic_cast<OOX::External>();

				if ( !pExt.IsInit() )
				{
					OOX::CPath oDefDir = pFile->DefaultDirectory();
					OOX::CPath oName   = pFile->DefaultFileName();
					if(false == pFile->m_sFilename.IsEmpty())
						oName.SetName(pFile->m_sFilename, false);

					OOX::CSystemUtility::CreateDirectories( oCurrent / oDefDir );
					pFile->write( oCurrent / oDefDir / oName, oDir / oDefDir, oContent );
					if(true != pFile->m_bDoNotAddRels)
						oRels.Registration( pPair->first, pFile->type(), oDefDir / oName );
				}
				else
				{
					oRels.Registration( pPair->first, pExt );
				}
			}
		}


		void IFileContainer::Commit  (const OOX::CPath& oPath)
		{
			std::map<CString, size_t> mNamepair;

			for (std::map<CString, smart_ptr<OOX::File>>::const_iterator pPair = m_mContainer.begin(); pPair != m_mContainer.end(); ++pPair)
			{
				smart_ptr<OOX::File>     pFile = pPair->second;
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

					smart_ptr<OOX::IFileBuilder> pFileBuilder = pPair->second.smart_dynamic_cast<OOX::IFileBuilder>();
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

			for (std::map<CString, smart_ptr<OOX::File>>::const_iterator pPair = m_mContainer.begin(); pPair != m_mContainer.end(); ++pPair)
			{
				smart_ptr<OOX::File>     pFile = pPair->second;
				smart_ptr<OOX::External> pExt  = pFile.smart_dynamic_cast<OOX::External>();

				if ( !pExt.IsInit() )
				{
					OOX::CPath oDefDir = pFile->DefaultDirectory();
					OOX::CPath oName   = pFile->DefaultFileName();

					std::map<CString, size_t>::const_iterator pNamePair = mNamepair.find( oName.m_strFilename );
					if (pNamePair == mNamepair.end())
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

					oRels.Registration( pPair->first, pFile->type(), oDefDir / oName );
				}
				else
				{
					oRels.Registration( pPair->first, pExt );
				}
			}
		}

		const bool IFileContainer::IsExist(const FileType& oType) const
		{
			for (std::map<CString, smart_ptr<OOX::File>>::const_iterator pPair = m_mContainer.begin(); pPair != m_mContainer.end(); ++pPair)
			{
				if ( oType == pPair->second->type() )
					return true;
			}
			return false;
		}
		const bool IFileContainer::IsExist(const RId& rId) const
		{
			std::map<CString, smart_ptr<OOX::File>>::const_iterator pPair = m_mContainer.find( rId.get() );
			return ( pPair != m_mContainer.end() );
		}


		template<typename T>
		const bool IFileContainer::IsExist() const
		{
			T oFile;
			return IsExist( oFile.type() );
		}
		CString IFileContainer::IsExistHyperlink(smart_ptr<OOX::HyperLink>& pHyperLink)
		{
			for (std::map<CString, smart_ptr<OOX::File>>::const_iterator pPair = m_mContainer.begin(); pPair != m_mContainer.end(); ++pPair)
			{
				if(OOX::FileTypes::HyperLink == pPair->second->type())
				{
					smart_ptr<OOX::HyperLink> pCurHyperlink = pPair->second.smart_dynamic_cast<OOX::HyperLink>();
					if(pCurHyperlink->Uri().GetPath() == pHyperLink->Uri().GetPath())
						return pPair->first;
				}
			}
			return CString();
		}
		const bool IFileContainer::IsExternal(const OOX::RId& rId) const
		{
			std::map<CString, smart_ptr<OOX::File>>::const_iterator pPair = m_mContainer.find( rId.get() );

			if ( pPair != m_mContainer.end() )
			{
				CString sType = pPair->second->type().RelationType();
				CString sName = pPair->second->type().DefaultFileName().m_strFilename;

				return (( ( sType == OOX::FileTypes::ExternalAudio.RelationType() ) || ( sType == OOX::FileTypes::ExternalImage.RelationType() ) || ( sType == OOX::FileTypes::ExternalVideo.RelationType() ) ) && ( sName == _T("") ) );
			}
			return true;
		}


		smart_ptr<OOX::File> IFileContainer::Get(const FileType& oType)
		{
			for (std::map<CString, smart_ptr<OOX::File>>::const_iterator pPair = m_mContainer.begin(); pPair != m_mContainer.end(); ++pPair)
			{
				if ( oType == pPair->second->type() )
					return pPair->second;
			}
			return smart_ptr<OOX::File>(new UnknowTypeFile( Unknown ));
		}


		const RId IFileContainer::Add(smart_ptr<OOX::File>& pFile)
		{
			const RId rId = GetMaxRId().next();
			Add( rId, pFile );
			return rId;
		}


        void      IFileContainer::Add(const OOX::RId& rId, smart_ptr<OOX::File>& pFile)
		{
			bool bEnumerated = pFile->type().Enumerated();
			bool bEnumeratedGlobal = pFile->type().EnumeratedGlobal();

			if(true == bEnumeratedGlobal || true == bEnumerated)
			{
				int nIndex = 0;

				if(true == bEnumeratedGlobal)
				{
					std::map<CString, size_t>::const_iterator pNamePair = m_mapEnumeratedGlobal.find (pFile->type().OverrideType());
					if (pNamePair != m_mapEnumeratedGlobal.end())
						nIndex = pNamePair->second;
				}
				else
				{
					std::map<CString, size_t>::const_iterator pNamePair = m_mapAddNamePair.find (pFile->type().OverrideType());
					if (pNamePair != m_mapAddNamePair.end())
						nIndex = pNamePair->second;	
				}
				
				nIndex++;

				if(true == bEnumeratedGlobal)
				{
					if(pFile.is<OOX::FileGlobalEnumerated>())
					{
						OOX::FileGlobalEnumerated& oFileGlobalEnumerated = pFile.as<OOX::FileGlobalEnumerated>();
						oFileGlobalEnumerated.SetGlobalNumber(nIndex);
					}
				}

				CString sPath = pFile->DefaultFileName().GetPath();
				int nDotIndex = sPath.ReverseFind('.');
				if(-1 != nDotIndex && nDotIndex > 0)
				{
					CString sDigit;sDigit.Format(_T("%d"), nIndex);
					sPath.Insert(nDotIndex, sDigit);
				}
				pFile->m_sFilename = sPath;
				if(true == bEnumeratedGlobal)
					m_mapEnumeratedGlobal [pFile->type().OverrideType()] = nIndex;
				else
					m_mapAddNamePair [pFile->type().OverrideType()] = nIndex;
			}

			m_lMaxRid = max( m_lMaxRid, rId.getNumber() );
			m_mContainer [rId.get()] = pFile;
		}
		smart_ptr<OOX::File> IFileContainer::Find(const FileType& oType) const
		{
			for (std::map<CString, smart_ptr<OOX::File>>::const_iterator pPair = m_mContainer.begin(); pPair != m_mContainer.end(); ++pPair)
			{
				if ( oType == pPair->second->type() )
					return pPair->second;
			}
			return smart_ptr<OOX::File>( (OOX::File*)new UnknowTypeFile() );
		}
		void IFileContainer::FindAllByType(const FileType& oType, std::map<CString, smart_ptr<OOX::File>>& aOutput) const
		{
			for (std::map<CString, smart_ptr<OOX::File>>::const_iterator pPair = m_mContainer.begin(); pPair != m_mContainer.end(); ++pPair)
			{
				if ( oType == pPair->second->type() )
				{
					aOutput [pPair->first] = pPair->second;
				}
			}
		}
		smart_ptr<OOX::File> IFileContainer::Find(const OOX::RId& rId) const
		{
			std::map<CString, smart_ptr<OOX::File>>::const_iterator pPair = m_mContainer.find(rId.get());

			if ( pPair != m_mContainer.end())
				return pPair->second;

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
			std::map<CString, smart_ptr<OOX::File>>::const_iterator pPair = m_mContainer.find(rId.get());
			if ( pPair != m_mContainer.end())
				return pPair->second;

			return smart_ptr<OOX::File>( (OOX::File*)new UnknowTypeFile() );
		}


		smart_ptr<OOX::File> IFileContainer::operator [](const FileType& oType)
		{
			return Find( oType );
		}

		const RId IFileContainer::GetMaxRId()
		{
			return RId( m_lMaxRid );
		}
		void IFileContainer::SetGlobalNumberByType(const CString& sOverrideType, int val)
		{
			m_mapEnumeratedGlobal [sOverrideType] = val;
		}
		int IFileContainer::GetGlobalNumberByType(const CString& sOverrideType)
		{
			std::map<CString, size_t>::const_iterator pNamePair = m_mapEnumeratedGlobal.find( sOverrideType );
			int nRes = 0;
			if(pNamePair != m_mapEnumeratedGlobal.end())
				nRes = pNamePair->second;
			return nRes;
		}
	}
} // namespace OOX
