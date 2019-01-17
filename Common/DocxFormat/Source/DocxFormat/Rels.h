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
#pragma once
#ifndef OOX_RELS_INCLUDE_H_
#define OOX_RELS_INCLUDE_H_

#include "FileType.h"
#include "FileTypes.h"
#include "RId.h"
#include "WritingElement.h"

#include "External/External.h"

#include "../Base/SmartPtr.h"
#include "../SystemUtility/SystemUtility.h"

#include "../../../../DesktopEditor/common/File.h"

#include <boost/unordered_map.hpp>

namespace OOX
{
	namespace Rels
	{
		class CRelationShip : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CRelationShip)

            CRelationShip(const OOX::RId& rId, const std::wstring& sType, const OOX::CPath& oFilePath, bool bExternal = false) : m_rId(rId), m_oTarget(oFilePath), m_sType(sType)
			{
				XmlUtils::replace_all(m_oTarget.m_strFilename, L" ", L"_");

				if (bExternal)
				{
					m_sMode == L"External";
				}
			}

            CRelationShip(const OOX::RId& rId, const smart_ptr<External> pExternal) : m_rId(rId)
			{
                m_sMode = new std::wstring( _T("External") );

				if (pExternal.IsInit())
				{
					m_oTarget	= pExternal->Uri();
					m_sType		= pExternal->type().RelationType();
				}
			}

            virtual ~CRelationShip()
			{
			}
			
            virtual void  fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
            virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode,  _T("Id"),         m_rId );
				XmlMacroReadAttributeBase( oNode,  _T("Target"),     m_oTarget );
				XmlMacroReadAttributeBase( oNode,  _T("Type"),       m_sType );
				XmlMacroReadAttributeBase( oNode,  _T("TargetMode"), m_sMode );
			}
            virtual std::wstring toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write( _T("Id"),         m_rId.ToString() );
				oAttr.Write( _T("Type"),       m_sType );
                std::wstring sTarget = m_oTarget.m_strFilename;

                XmlUtils::replace_all(sTarget, _T("\\"), _T("/"));
				sTarget = XmlUtils::EncodeXmlString(sTarget);
				oAttr.Write( _T("Target"), sTarget);
				if(m_sMode.IsInit())
					oAttr.Write( _T("TargetMode"), m_sMode.get() );
				
				return XmlUtils::CreateNode( _T("Relationship"), oAttr );
			}

			virtual EElementType getType() const
			{
				return OOX::et_Relationship;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
                std::wstring sTempTarget;
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("Id"),         m_rId )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("Target"),     sTempTarget )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("Type"),       m_sType )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("TargetMode"), m_sMode )
				WritingElement_ReadAttributes_End( oReader )

				//External rels не нормализуем, иначе искажаются пути в гиперссылках.
				if(IsExternal())
					m_oTarget.SetName(sTempTarget, false);
				else
					m_oTarget.SetName(sTempTarget, true);
			}

		public:

            const std::wstring Type() const
			{
				return m_sType;
			}
			const CPath   Filename() const
			{
				return m_oTarget;
			}
			const CPath   Target() const
			{
				return m_oTarget;
			}
			const bool    IsExternal()const
			{
				if ( !m_sMode.IsInit() )
					return false;
                return ( *m_sMode == L"External");
			}

            const RId rId() const
			{
				return m_rId;
			}

		private:
			RId						m_rId;
			CPath					m_oTarget;
            std::wstring            m_sType;
            nullable<std::wstring>  m_sMode;
		};

	} // namespace Rels

	class CRels
	{
	public:

		CRels()
		{
		}
		CRels(const CPath& oFilePath)
		{
			Read( oFilePath );
		}
		~CRels()
		{
            for (size_t i = 0; i < m_arRelations.size(); ++i)
			{
				if ( m_arRelations[i] ) delete m_arRelations[i];
			}
			m_arRelations.clear();
			m_mapRelations.clear();
		}

		void Read (const CPath& oFilePath)
		{
            CPath oRelsPath = CreateFileName( oFilePath );

            XmlUtils::CXmlLiteReader oReader;

            if ( !oReader.FromFile( oRelsPath.GetPath() ) )
                return;

            if ( !oReader.ReadNextNode() )
                return;

            std::wstring sName = oReader.GetName();
            if ( _T("Relationships") == sName )
            {
                if ( !oReader.IsEmptyNode() )
                {
                    int nRelationshipsDepth = oReader.GetDepth();
                    while ( oReader.ReadNextSiblingNode( nRelationshipsDepth ) )
                    {
                        sName = oReader.GetName();
                        if ( _T("Relationship") == sName )
                        {
                            OOX::Rels::CRelationShip *pRel = new OOX::Rels::CRelationShip(oReader);
                            if (pRel) 
							{
								std::wstring rid = pRel->rId().get();
								
								m_arRelations.push_back(pRel);			
								m_mapRelations.insert(std::make_pair( rid, pRel) );
							}
                        }
                    }
                }
            }
		}
		void Write(const CPath& oFilePath) const
		{
			if ( m_mapRelations.empty() )return;
			CPath oFile = CreateFileName( oFilePath );
			CSystemUtility::CreateDirectories( oFile.GetDirectory() );

			XmlUtils::CXmlWriter oWriter;

			oWriter.WriteString(_T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>"));

            oWriter.WriteNodeBegin( _T("Relationships"), true );
			oWriter.WriteAttribute( _T("xmlns"), _T("http://schemas.openxmlformats.org/package/2006/relationships") );
            oWriter.WriteNodeEnd( _T("Relationships"), true, false );

			for (size_t i = 0; i < m_arRelations.size(); ++i)
			{
				if ( m_arRelations[i] )
					oWriter.WriteString( m_arRelations[i]->toXML() );
			}

			oWriter.WriteNodeEnd(_T("Relationships") );

			NSFile::CFileBinary::SaveToFile(oFile.GetPath(), oWriter.GetXmlString());
		}


		void Registration(const RId& rId, const FileType& oType, const CPath& oPath, bool bExternal = false)
		{
			if( FileTypes::Unknow == oType ) return;

			std::wstring strFileName = oPath.m_strFilename;
           
			std::wstring strDir = oPath.GetDirectory() + _T("");

			Rels::CRelationShip* pRel = NULL;

			if ( L"" == oPath.GetExtention() )
			{
				if ( oType.RelationType() == L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/oleObject" )
				{
					strFileName += (strFileName.empty() ? L"" : L".bin");
					pRel = new Rels::CRelationShip( rId, oType.RelationType(), strDir + strFileName, bExternal );
				}
				else if ( oType.RelationType() == L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/image" )
				{
					strFileName += L".wmf" ;					
					pRel = new Rels::CRelationShip( rId, oType.RelationType(), strDir + strFileName, bExternal );
				}
			}
			else
			{
				pRel = new Rels::CRelationShip( rId, oType.RelationType(), oPath.GetPath(), bExternal);
				
			}
			if (pRel)
			{
				m_arRelations.push_back(pRel);
				m_mapRelations.insert( std::make_pair( rId.get(), pRel));
			}
		}
		void Registration(const RId& rId, const smart_ptr<External> pExternal)
		{
			Rels::CRelationShip* pRel = new Rels::CRelationShip( rId, pExternal );
			
			m_arRelations.push_back(pRel);
			m_mapRelations.insert( std::make_pair( rId.get(), pRel) );
		}
		void GetRel(const RId& rId, Rels::CRelationShip** ppRelationShip)
		{
			(*ppRelationShip) = NULL;

            boost::unordered_map<std::wstring, Rels::CRelationShip*>::iterator pFind = m_mapRelations.find(rId.get());
			if (pFind != m_mapRelations.end())
			{
				(*ppRelationShip) = pFind->second;
			}
		}

	private:

		const CPath CreateFileName(const CPath& oFilePath) const
		{
            std::wstring strTemp = oFilePath.GetDirectory()  + FILE_SEPARATOR_STR + _T("_rels") + FILE_SEPARATOR_STR;

			if ( _T("") == oFilePath.GetFilename() )	strTemp += _T(".rels");
			else										strTemp += ( oFilePath.GetFilename() + _T(".rels") );

            CPath pathTemp = strTemp;
			return pathTemp.GetPath();
		}

	public:

		std::vector<Rels::CRelationShip*>				m_arRelations;
		boost::unordered_map<std::wstring, Rels::CRelationShip*>	m_mapRelations;
	};

} // namespace OOX

#endif // OOX_RELS_INCLUDE_H_
