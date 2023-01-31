/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#include "IFileContainer.h"
#include "Rels.h"
#include "FileFactory.h"
#include "ContentTypes.h"

#include "FileType.h"
#include "FileTypes.h"

#include "External/External.h"
#include "External/HyperLink.h"
#include "Media/Image.h"
#include "Media/OleObject.h"
#include "Media/ActiveX.h"

#include "../PPTXFormat/LegacyDiagramText.h"
#include "../XlsxFormat/FileFactory_Spreadsheet.h"

namespace OOX
{
    boost::unordered_map<std::wstring, size_t> IFileContainer::m_mapEnumeratedGlobal;

    UnknowTypeFile IFileContainer::m_oUnknown(NULL);

	IFileContainer::IFileContainer(OOX::Document* pMain) : m_pMainDocument(pMain)
	{
		m_bSpreadsheets = false;
		m_lMaxRid		= 0;
	}
	IFileContainer::~IFileContainer()
	{
	}
	void IFileContainer::Read (const OOX::CPath& oRootPath, const OOX::CPath& oPath)
	{
		// Находим связи(рельсы) с данным файлом
			m_pCurRels = new OOX::CRels(oPath);

			if (m_pMainDocument)
			{
				smart_ptr<OOX::File> pFile = dynamic_cast<OOX::File*>(this);
				pFile.AddRef();

				m_pMainDocument->m_mapContent.insert(std::make_pair(oPath.GetPath(), pFile));
			}
			// Читаем все файлы по рельсам
			Read( *m_pCurRels, oRootPath, oPath.GetDirectory() );
	}


	void IFileContainer::Read (const OOX::CRels& oRels, const OOX::CPath& oRootPath, const OOX::CPath& oPath)
	{
		for (size_t i = 0; i < oRels.m_arRelations.size(); ++i)
		{
			if (!oRels.m_arRelations[i]) continue;

			std::wstring pathFile = (oPath / oRels.m_arRelations[i]->Target()).GetPath();
			if (m_pMainDocument)
			{
				std::map<std::wstring, smart_ptr<OOX::File>>::iterator pFind = m_pMainDocument->m_mapContent.find(pathFile);

				if (pFind != m_pMainDocument->m_mapContent.end())
				{
					Add(oRels.m_arRelations[i]->rId(), pFind->second);
					continue;
				}
			}

			smart_ptr<OOX::File> pFile;

			if (m_bSpreadsheets)
				pFile = OOX::Spreadsheet::CreateFile(oRootPath, oPath, oRels.m_arRelations[i], m_pMainDocument);

			if (pFile.IsInit() == false || pFile->type() == FileTypes::Unknow)
				pFile = OOX::CreateFile(oRootPath, oPath, oRels.m_arRelations[i], m_pMainDocument);

			Add(oRels.m_arRelations[i]->rId(), pFile);
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
		const OOX::File* pFileOwner = dynamic_cast<const OOX::File*>(this);

		for (boost::unordered_map<std::wstring, smart_ptr<OOX::File>>::const_iterator pPair = m_mapContainer.begin(); pPair != m_mapContainer.end(); ++pPair)
		{
			smart_ptr<OOX::File>		pFile	= pPair->second;

			smart_ptr<OOX::External>	pExt	= pFile.smart_dynamic_cast<OOX::External>();
			smart_ptr<OOX::Media>		pMedia  = pFile.smart_dynamic_cast<OOX::Media>();

			bool bExternal = pExt.IsInit() || ((pMedia.IsInit()) && (pMedia->IsExternal()));
			if ( !bExternal )
			{
				OOX::CPath oDefDir = pFile->DefaultDirectory();
				OOX::CPath oName   = pFile->DefaultFileName();

				if (pFile->type() == OOX::Spreadsheet::FileTypes::PivotCacheDefinition &&
					pFileOwner->type() == OOX::Spreadsheet::FileTypes::PivotTable)
				{
					oDefDir = L"../" + oDefDir;
				}

                if (false == pFile->m_sOutputFilename.empty())
					oName.SetName(pFile->m_sOutputFilename, false);

				boost::unordered_map<std::wstring, std::wstring>::const_iterator itFind = m_mNoWriteContainer.find(pPair->first);
				if(m_mNoWriteContainer.end() == itFind)
				{
					OOX::CSystemUtility::CreateDirectories( oCurrent / oDefDir );
					pFile->write( oCurrent / oDefDir / oName, oDir / oDefDir, oContent );
				}
				else if(itFind->second.length() > 0)
				{
					oDefDir = itFind->second;
				}
				if(true != pFile->m_bDoNotAddRels)
                {
                    if (oDefDir.GetPath().length() > 0)//todooo перенести в CPath
                        oRels.Registration( pPair->first, pFile->type(), oDefDir / oName );
                    else
                        oRels.Registration( pPair->first, pFile->type(), oName );
                }
			}
			else
			{
				if (pExt.IsInit())
				{
					oRels.Registration( pPair->first, pExt );
				}
				else if ((pMedia.IsInit()) && (pMedia->IsExternal()))
				{
					oRels.Registration( pPair->first, pMedia->type(), pMedia->filename().GetPath(), true);
				}
			}
		}
	}


	void IFileContainer::Commit  (const OOX::CPath& oPath)
	{
		std::map<std::wstring, size_t> mNamepair;

		for (size_t i = 0; i < m_arContainer.size(); ++i)
		{
			smart_ptr<OOX::File>		&pFile	= m_arContainer[i];

			smart_ptr<OOX::External>	pExt	= pFile.smart_dynamic_cast<OOX::External>();
			smart_ptr<OOX::Media>		pMedia  = pFile.smart_dynamic_cast<OOX::Media>();

			bool bExternal = pExt.IsInit() || ((pMedia.IsInit()) && (pMedia->IsExternal()));
			if ( !bExternal )

			if (!pExt.IsInit())
			{
				OOX::CPath oDefDir = pFile->DefaultDirectory();
				OOX::CPath oName   = pFile->DefaultFileName();

				std::map<std::wstring, size_t>::const_iterator pNamePair = mNamepair.find( oName.m_strFilename );

				if (pNamePair == mNamepair.end())
					mNamepair [oName.m_strFilename] = 1;
				else
					oName = oName + pNamePair->first;

				OOX::CSystemUtility::CreateDirectories( oPath / oDefDir );
				
				smart_ptr<OOX::IFileBuilder> pFileBuilder = pFile.smart_dynamic_cast<OOX::IFileBuilder>();
				
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
		std::map<std::wstring, size_t> mNamepair;

		for (boost::unordered_map<std::wstring, smart_ptr<OOX::File>>::const_iterator pPair = m_mapContainer.begin(); pPair != m_mapContainer.end(); ++pPair)
		{
			smart_ptr<OOX::File>		pFile	= pPair->second;

			smart_ptr<OOX::External>	pExt	= pFile.smart_dynamic_cast<OOX::External>();
			smart_ptr<OOX::Media>		pMedia  = pFile.smart_dynamic_cast<OOX::Media>();

			bool bExternal = pExt.IsInit() || ((pMedia.IsInit()) && (pMedia->IsExternal()));
			if ( !bExternal )
			{
				OOX::CPath oDefDir = pFile->DefaultDirectory();
				OOX::CPath oName   = pFile->DefaultFileName();

				std::map<std::wstring, size_t>::iterator pNamePair = mNamepair.find( oName.m_strFilename );
				if ( pNamePair == mNamepair.end() )
					mNamepair [oName.m_strFilename] = 1;
				else
					oName = oName + pNamePair->first;

				boost::unordered_map<std::wstring, std::wstring>::const_iterator itFind = m_mNoWriteContainer.find(pPair->first);

                if(m_mNoWriteContainer.end() == itFind)
				{
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
				}
				else if(itFind->second.length() > 0)
				{
					oDefDir = itFind->second;
				}

				oRels.Registration( pPair->first, pFile->type(), oDefDir / oName );
			}
			else
			{
				oRels.Registration( pPair->first, pExt );
			}
		}
	}


	const bool IFileContainer::IsExist(const RId& rId) const
	{
        boost::unordered_map<std::wstring, smart_ptr<OOX::File>>::const_iterator pFind = m_mapContainer.find(rId.get());
		return (pFind != m_mapContainer.end());
	}

	template<typename T>
	const bool IFileContainer::IsExist() const
	{
		T oFile;
		return IsExist( oFile.type() );
	}	
	std::wstring IFileContainer::IsExistHyperlink(const std::wstring & href)
	{
         boost::unordered_map<std::wstring, std::wstring>::const_iterator pFind = m_mapHyperlinks.find(href);

		if (pFind != m_mapHyperlinks.end())
		{
			return pFind->second;
		}

        return std::wstring();
	}
	const bool IFileContainer::IsExternal(const OOX::RId& rId) const
	{
        boost::unordered_map<std::wstring, smart_ptr<OOX::File>>::const_iterator pFind = m_mapContainer.find(rId.get());

		if (pFind != m_mapContainer.end())
		{
			smart_ptr<OOX::Media> mediaFile = pFind->second.smart_dynamic_cast<OOX::Media>();

			if (mediaFile.IsInit())
			{
				return mediaFile->IsExternal();
			}
			else
			{
				return false;
			}
		}
		return true;
	}


	const RId IFileContainer::Add(smart_ptr<OOX::File>& pFile)
	{
		const RId rId = GetMaxRId().next();
		Add( rId, pFile );
		return rId;
	}

	void IFileContainer::AssignOutputFilename(smart_ptr<OOX::File>& pFile)
	{
		bool bEnumerated		= pFile->type().Enumerated();
		bool bEnumeratedGlobal	= pFile->type().EnumeratedGlobal();

		if((true == bEnumeratedGlobal || true == bEnumerated) && pFile->m_sOutputFilename.empty())
		{
			int nIndex = 0;

			if(true == bEnumeratedGlobal)
			{
				boost::unordered_map<std::wstring, size_t>::const_iterator pNamePair = m_mapEnumeratedGlobal.find (pFile->type().EnumerateType());
				if (pNamePair != m_mapEnumeratedGlobal.end())
					nIndex = pNamePair->second;
			}
			else
			{
				boost::unordered_map<std::wstring, size_t>::const_iterator pNamePair = m_mapAddNamePair.find (pFile->type().EnumerateType());
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

			std::wstring sPath = pFile->DefaultFileName().GetPath();
			size_t nDotIndex = sPath.rfind('.');
			if(std::wstring::npos != nDotIndex && nDotIndex > 0)
			{
				std::wstring sDigit = std::to_wstring( nIndex);
				sPath.insert(sPath.begin() + nDotIndex, sDigit.begin(), sDigit.end());
			}
			pFile->m_sOutputFilename = sPath;
			
			if(true == bEnumeratedGlobal)
				m_mapEnumeratedGlobal [pFile->type().EnumerateType()] = nIndex;
			else
				m_mapAddNamePair [pFile->type().EnumerateType()] = nIndex;
		}
	}

	void IFileContainer::Add (const OOX::RId& rId, smart_ptr<OOX::File>& pFile)
	{
		AssignOutputFilename(pFile);

		m_lMaxRid = (std::max)( m_lMaxRid, rId.getNumber() );
		
		m_arContainer.push_back(pFile);
		m_mapContainer [rId.get()] = pFile;

		smart_ptr<OOX::HyperLink> oHyperlinkFile = pFile.smart_dynamic_cast<OOX::HyperLink>();
		if (oHyperlinkFile.IsInit())
		{
			m_mapHyperlinks.insert(std::make_pair( oHyperlinkFile->Uri().GetPath(), rId.get() ));
		}
	}

	const RId IFileContainer::AddNoWrite(const smart_ptr<OOX::File>& pFile, const std::wstring& oDefDir)
	{
		const RId rId = GetMaxRId().next();
		AddNoWrite( rId, pFile, oDefDir );
		return rId;
	}

	void IFileContainer::AddNoWrite (const OOX::RId& rId, const smart_ptr<OOX::File>& pFile, const std::wstring& oDefDir)
	{
		m_lMaxRid = (std::max)( m_lMaxRid, rId.getNumber() );
		
		m_arContainer.push_back(pFile);
		m_mapContainer [rId.get()] = pFile;
		
		m_mNoWriteContainer[rId.get()] = oDefDir;
	}

	smart_ptr<OOX::File> IFileContainer::Find(const OOX::RId& rId) const
	{
		boost::unordered_map<std::wstring, smart_ptr<OOX::File>>::const_iterator pPair = m_mapContainer.find(rId.get());

		if ( pPair != m_mapContainer.end())
			return pPair->second;

		return smart_ptr<OOX::File>( (OOX::File*)new UnknowTypeFile(m_pMainDocument) );
	}

	smart_ptr<OOX::File> IFileContainer::Find(const FileType& oType) const
	{
		for (boost::unordered_map<std::wstring, smart_ptr<OOX::File>>::const_iterator pPair = m_mapContainer.begin(); pPair != m_mapContainer.end(); ++pPair)
		{
			if (pPair->second->type() == oType)
				return pPair->second;
		}
		return smart_ptr<OOX::File>( (OOX::File*)new UnknowTypeFile(m_pMainDocument) );
	}
	template<typename T>
    T& IFileContainer::Find()
	{
		T oFile;
        return dynamic_cast<T&>( Find( oFile.type() ) );
	}	
	smart_ptr<OOX::File> IFileContainer::operator [](const OOX::RId rId)
	{
        boost::unordered_map<std::wstring, smart_ptr<OOX::File>>::const_iterator pFind = m_mapContainer.find(rId.get());
		if ( pFind != m_mapContainer.end())
			return pFind->second;

		return smart_ptr<OOX::File>( (OOX::File*)new UnknowTypeFile(m_pMainDocument) );
	}
	smart_ptr<OOX::File> IFileContainer::operator [](const FileType& oType)
	{
		return Find(oType);
	}	
	const RId IFileContainer::GetMaxRId()
	{
		return RId( m_lMaxRid );
	}
    void IFileContainer::SetGlobalNumberByType(const std::wstring& sOverrideType, int val)
	{
		m_mapEnumeratedGlobal [sOverrideType] = val;
	}
    int IFileContainer::GetGlobalNumberByType(const std::wstring& sOverrideType)
	{
        boost::unordered_map<std::wstring, size_t>::const_iterator pNamePair = m_mapEnumeratedGlobal.find( sOverrideType );
		int nRes = 0;
		if(pNamePair != m_mapEnumeratedGlobal.end())
			nRes = pNamePair->second;
		return nRes;
	}
} // namespace OOX
