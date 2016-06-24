/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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

namespace OOX
{
	namespace Rels
	{
		class CRelationShip : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CRelationShip)
			CRelationShip(const OOX::RId& rId, const CString& sType, const OOX::CPath& oFilePath) : m_rId(rId), m_oTarget(oFilePath), m_sType(sType)
			{
				m_oTarget.m_strFilename.Replace(_T(" "), _T("_"));
			}
			CRelationShip(const OOX::RId& rId, const smart_ptr<External> pExternal): m_rId(rId), m_oTarget(pExternal->Uri()), m_sType(pExternal->type().RelationType())
			{
				m_sMode = new CString( _T("External") );
			}
			virtual ~CRelationShip()
			{
			}
			
		public:

			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				oNode.ReadAttributeBase( _T("Id"),         m_rId );
				oNode.ReadAttributeBase( _T("Target"),     m_oTarget );
				oNode.ReadAttributeBase( _T("Type"),       m_sType );
				oNode.ReadAttributeBase( _T("TargetMode"), m_sMode );
			}
			virtual CString      toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write( _T("Id"),         m_rId.ToString() );
				oAttr.Write( _T("Type"),       m_sType );
				CString sTarget = m_oTarget.m_strFilename;
				sTarget.Replace(_T("\\"), _T("/"));
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
				CString sTempTarget;
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

			//const bool operator <(const CRelationShip& rhs) const
			//{
			//	return m_rId < rhs.m_rId;
			//}

		public:

			const CString Type() const
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
                return ( *m_sMode == _T("External" ));
			}
			const RId     rId() const
			{
				return m_rId;
			}

		private:
			RId						m_rId;
			CPath					m_oTarget;
			CString					m_sType;
			nullable<CString>       m_sMode;
		};

	} // namespace Rels
} // namespace OOX
namespace OOX
{
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
            for ( unsigned int nIndex = 0; nIndex < m_arrRelations.size(); nIndex++ )
			{
				if ( m_arrRelations[nIndex] ) delete m_arrRelations[nIndex];
				m_arrRelations[nIndex] = NULL;
			}
			m_arrRelations.clear();

		}

	public:

		void Read (const CPath& oFilePath)
		{
			CPath oRelsPath = CreateFileName( oFilePath );

			if ( CSystemUtility::IsFileExist( oRelsPath ) )
			{
#ifdef USE_LITE_READER

				XmlUtils::CXmlLiteReader oReader;

				if ( !oReader.FromFile( oRelsPath.GetPath() ) )
					return;

				if ( !oReader.ReadNextNode() )
					return;

				CWCharWrapper sName = oReader.GetName();
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
								Rels::CRelationShip *oRel = new Rels::CRelationShip(oReader);
								if (oRel) m_arrRelations.push_back( oRel );
							}
						}
					}
				}

#else

				XmlUtils::CXmlNode oNode;

                if ( oNode.FromXmlFile2( oRelsPath.GetPath() ) && _T("Relationships") == oNode.GetName() )
				{
					XmlUtils::CXmlNodes oNodes;
					if ( oNode.GetNodes( _T("Relationship"), oNodes ) )
					{
						XmlUtils::CXmlNode oRelNode;
						for ( int nIndex = 0; nIndex < oNodes.GetCount(); nIndex++ )
						{
							if ( oNodes.GetAt( nIndex, oRelNode ) )
							{
                                //Rels::CRelationShip oRel = oRelNode;
                                Rels::CRelationShip *oRel = new Rels::CRelationShip (oRelNode);
                                m_arrRelations.push_back( oRel );
							}
						}
					}
				}
#endif
			}
		}
		void Write(const CPath& oFilePath) const
		{
			if ( 0 < m_arrRelations.size() )
			{
				CPath oFile = CreateFileName( oFilePath );
				CSystemUtility::CreateDirectories( oFile.GetDirectory() );

				XmlUtils::CXmlWriter oWriter;

				oWriter.WriteString(_T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>"));

                oWriter.WriteNodeBegin( _T("Relationships"), true );
				oWriter.WriteAttribute( _T("xmlns"), _T("http://schemas.openxmlformats.org/package/2006/relationships") );
                oWriter.WriteNodeEnd( _T("Relationships"), true, false );

				for ( unsigned int nIndex = 0; nIndex < m_arrRelations.size(); nIndex++ )
				{
					if ( m_arrRelations[nIndex])
						oWriter.WriteString( m_arrRelations[nIndex]->toXML() );
				}

				oWriter.WriteNodeEnd(_T("Relationships") );

				CDirectory::SaveToFile( oFile.GetPath(), oWriter.GetXmlString() );
			}
		}

	public:

		void Registration(const RId& rId, const FileType& oType, const CPath& oPath)
		{
			if( !( FileTypes::Unknow == oType ) )
			{
				CString strFileName	= oPath.m_strFilename;
				CString strDir		= oPath.GetDirectory() + _T("");

				if ( _T("") == oPath.GetExtention() )
				{
					if ( oType.RelationType() == _T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/oleObject") )
					{
						strFileName += L".bin";
						m_arrRelations.push_back(new Rels::CRelationShip( rId, oType.RelationType(), strDir + strFileName ) );
					}
					else if ( oType.RelationType() == _T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/image") )
					{
						strFileName += L".wmf" ;
						m_arrRelations.push_back(new Rels::CRelationShip( rId, oType.RelationType(), strDir + strFileName ) );
					}
				}
				else
				{
					m_arrRelations.push_back(new Rels::CRelationShip( rId, oType.RelationType(), oPath.GetPath()) );
					//m_arrRelations.push_back( Rels::CRelationShip( rId, oType.RelationType(),  replace_extension( oPath, L"svm", L"png") );
				}
			}
		}
		void Registration(const RId& rId, const smart_ptr<External> pExternal)
		{
			m_arrRelations.push_back( new Rels::CRelationShip( rId, pExternal ) );
		}
		void GetRel(const RId& rId, Rels::CRelationShip** ppRelationShip)
		{
			(*ppRelationShip) = NULL;
			for( unsigned int i = 0, length = m_arrRelations.size(); i < length; ++i)
			{
				if ((m_arrRelations[i]) && (m_arrRelations[i]->rId() == rId))
				{
					(*ppRelationShip) = new Rels::CRelationShip(*m_arrRelations[i]);
				}
			}
		}

	private:

		const CPath CreateFileName(const CPath& oFilePath) const
		{
            CString strTemp = oFilePath.GetDirectory()  + FILE_SEPARATOR_STR + _T("_rels") + FILE_SEPARATOR_STR;

			if ( _T("") == oFilePath.GetFilename() )	strTemp += _T(".rels");
			else										strTemp += ( oFilePath.GetFilename() + _T(".rels") );

            CPath pathTemp = strTemp;
			return pathTemp.GetPath();
		}



	public:

		std::vector<Rels::CRelationShip*> m_arrRelations;
	};

} // namespace OOX

#endif // OOX_RELS_INCLUDE_H_
