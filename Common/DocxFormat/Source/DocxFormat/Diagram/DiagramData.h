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

#include "../IFileContainer.h"
#include "../FileTypes.h"
#include "../File.h"
#include "../../Base/Nullable.h"

#include "../WritingElement.h"
#include "../Drawing/DrawingExt.h"

namespace OOX
{
	namespace Diagram
	{
		class CPt : public WritingElement
		{
		public:
			CPt(XmlUtils::CXmlNode &oNode)
			{
				fromXML( oNode );
			}
			CPt(XmlUtils::CXmlLiteReader& oReader)
			{
				fromXML( oReader );
			}
			CPt() {}
			virtual ~CPt(){}
			virtual std::wstring toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					std::wstring sName = oReader.GetName();

					if ( _T("dgm:spPr") == sName )
						m_oSpPr = oReader;
					else if ( _T("dgm:t") == sName )
						m_oTxBody = oReader;
				}
			}

			virtual EElementType getType () const
			{
				return et_dgm_pt;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("modelId"), m_sModelId )
				WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<PPTX::Logic::SpPr>			m_oSpPr;
			nullable<PPTX::Logic::TxBody>		m_oTxBody;
			nullable<std::wstring>				m_sModelId;	//guid
		};


		class CPtLst : public WritingElementWithChilds<OOX::Diagram::CPt>
		{
		public:
			WritingElement_AdditionConstructors(CPtLst)
			CPtLst()
			{
			}
			virtual ~CPtLst()
			{
			}

		public:
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
					std::wstring sName = oReader.GetName();
					WritingElement *pItem = NULL;

					if ( _T("dgm:pt") == sName )
						m_arrItems.push_back( new Diagram::CPt( oReader ));
				}
			}
			virtual std::wstring toXML() const
			{
				std::wstring sResult = _T("");
				return sResult;
			}

			virtual EElementType getType() const
			{
				return et_dgm_ptLst;
			}
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start	( oReader )
				WritingElement_ReadAttributes_End	( oReader )
			}

		public:
			// Attributes
			// Childs
		};

	}
    class CDiagramData : public OOX::File, public OOX::IFileContainer
    {
    public:
        CDiagramData(OOX::Document* pMain) : OOX::File(pMain), OOX::IFileContainer(pMain)
        {
        }
		CDiagramData(OOX::Document* pMain, const CPath& uri) : OOX::File(pMain), OOX::IFileContainer(pMain)
		{
			read(uri.GetDirectory(), uri);
		}
		CDiagramData(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath) : OOX::File(pMain), OOX::IFileContainer(pMain)
        {
            read( oRootPath, oPath );
        }
        virtual ~CDiagramData()
        {
        }
        virtual void read(const CPath& oFilePath)
        {
            //don't use this. use read(const CPath& oRootPath, const CPath& oFilePath)
            CPath oRootPath;
            read(oRootPath, oFilePath);
        }
        virtual void read(const CPath& oRootPath, const CPath& oFilePath)
        {
            IFileContainer::Read( oRootPath, oFilePath );

			XmlUtils::CXmlLiteReader oReader;

            if ( !oReader.FromFile( oFilePath.GetPath() ) )
                return;

            if ( !oReader.ReadNextNode() )
                return;

            m_strFilename = oFilePath.GetPath();

            std::wstring sName = oReader.GetName();
            if ( L"dgm:dataModel" == sName && !oReader.IsEmptyNode() )
            {
                int nNumberingDepth = oReader.GetDepth();
                while ( oReader.ReadNextSiblingNode( nNumberingDepth ) )
                {
					sName = oReader.GetName();

                    if ( L"dgm:ptLst" == sName )
					{
						m_oDgmPtLst = oReader;
					}
					//else if ( L"dgm:cxnLst" == sName )
					//{
					//	m_oDgmCxnLst = new Diagram::CCxnLst(oReader);
					//}
					else if (L"dgm:extLst" == sName)
					{
						m_oExtLst = oReader; 
					}
                }
            }
        }

        virtual void write(const CPath& oFilePath, const CPath& oDirectory, CContentTypes& oContent) const
        {
        }

        virtual const OOX::FileType type() const
        {
            return FileTypes::Data;
        }
        virtual const CPath DefaultDirectory() const
        {
            return type().DefaultDirectory();
        }
        virtual const CPath DefaultFileName() const
        {
            return type().DefaultFileName();
        }

        std::wstring                                    m_strFilename;
        nullable<Diagram::CPtLst>                       m_oDgmPtLst;
        //nullable<Diagram::CCxnLst>                    m_oDgmCxnLst;
        nullable<OOX::Drawing::COfficeArtExtensionList>	m_oExtLst;

    };
} // namespace OOX
