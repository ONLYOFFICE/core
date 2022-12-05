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
#include "Comments.h"

#include "Math/oMathPara.h"
#include "Math/OMath.h"

#include "Logic/Annotations.h"
#include "Logic/Hyperlink.h"
#include "Logic/Paragraph.h"
#include "Logic/Sdt.h"
#include "Logic/Table.h"

#include "Logic/Run.h"
#include "Logic/SmartTag.h"
#include "Logic/Dir.h"
#include "Logic/Bdo.h"

namespace OOX
{
	CComment::CComment(OOX::Document *pMain) : WritingElementWithChilds<>(pMain)
	{
	}
	CComment::~CComment()
	{
	}
	void CComment::fromXML(XmlUtils::CXmlNode& oNode)
	{
	}
	std::wstring CComment::toXML() const
	{
		return L"";
	}
	EElementType CComment::getType() const
	{
		return et_w_comment;
	}
	void CComment::fromXML2(XmlUtils::CXmlLiteReader& oReader)
	{
		if ( oReader.IsEmptyNode() )
			return;

		int nParentDepth = oReader.GetDepth();
		while( oReader.ReadNextSiblingNode( nParentDepth ) )
		{
			std::wstring sName = oReader.GetName();
			WritingElement *pItem = NULL;

			if ( L"w:altChunk" ==sName )
				pItem = new Logic::CAltChunk( oReader );
			else if ( L"w:bookmarkEnd" ==sName )
				pItem = new Logic::CBookmarkEnd( oReader );
			else if ( L"w:bookmarkStart" ==sName )
				pItem = new Logic::CBookmarkStart( oReader );
			else if ( L"w:commentRangeEnd" ==sName )
				pItem = new Logic::CCommentRangeEnd( oReader );
			else if ( L"w:commentRangeStart" ==sName )
				pItem = new Logic::CCommentRangeStart( oReader );
			//else if ( L"w:customXml" ==sName )
			//	pItem = new Logic::CCustomXml( oReader );
			else if ( L"w:customXmlDelRangeEnd" ==sName )
				pItem = new Logic::CCustomXmlDelRangeEnd( oReader );
			else if ( L"w:customXmlDelRangeStart" ==sName )
				pItem = new Logic::CCustomXmlDelRangeStart( oReader );
			else if ( L"w:customXmlInsRangeEnd" ==sName )
				pItem = new Logic::CCustomXmlInsRangeEnd( oReader );
			else if ( L"w:customXmlInsRangeStart" ==sName )
				pItem = new Logic::CCustomXmlInsRangeStart( oReader );
			else if ( L"w:customXmlMoveFromRangeEnd" ==sName )
				pItem = new Logic::CCustomXmlMoveFromRangeEnd( oReader );
			else if ( L"w:customXmlMoveFromRangeStart" ==sName )
				pItem = new Logic::CCustomXmlMoveFromRangeStart( oReader );
			else if ( L"w:customXmlMoveToRangeEnd" ==sName )
				pItem = new Logic::CCustomXmlMoveToRangeEnd( oReader );
			else if ( L"w:customXmlMoveToRangeStart" ==sName )
				pItem = new Logic::CCustomXmlMoveToRangeStart( oReader );
			else if ( L"w:del" ==sName )
				pItem = new Logic::CDel( oReader );
			else if ( L"w:ins" ==sName )
				pItem = new Logic::CIns( oReader );
			else if ( L"w:moveFrom" ==sName )
				pItem = new Logic::CMoveFrom( oReader );
			else if ( L"w:moveFromRangeEnd" ==sName )
				pItem = new Logic::CMoveFromRangeEnd( oReader );
			else if ( L"w:moveFromRangeStart" ==sName )
				pItem = new Logic::CMoveFromRangeStart( oReader );
			else if ( L"w:moveTo" ==sName )
				pItem = new Logic::CMoveTo( oReader );
			else if ( L"w:moveToRangeEnd" ==sName )
				pItem = new Logic::CMoveToRangeEnd( oReader );
			else if ( L"w:moveToRangeStart" ==sName )
				pItem = new Logic::CMoveToRangeStart( oReader );
			else if ( L"m:oMath" ==sName )
				pItem = new Logic::COMath( oReader );
			else if ( L"m:oMathPara" ==sName )
				pItem = new Logic::COMathPara( oReader );
			else if ( L"w:p" ==sName )
				pItem = new Logic::CParagraph( oReader );
			else if ( L"w:permEnd" ==sName )
				pItem = new Logic::CPermEnd( oReader );
			else if ( L"w:permStart" ==sName )
				pItem = new Logic::CPermStart( oReader );
			else if ( L"w:proofErr" ==sName )
				pItem = new Logic::CProofErr( oReader );
			else if ( L"w:sdt" ==sName )
				pItem = new Logic::CSdt( oReader );
			else if ( L"w:tbl" ==sName )
				pItem = new Logic::CTbl( oReader );
			else if ( L"w:tbl" ==sName )
				pItem = new Logic::CTbl( oReader );
			else if ( L"aml:content" ==sName )
			{
				fromXML2(oReader);
				break;
			}

			if ( pItem )
				m_arrItems.push_back( pItem );
		}
	}
	void CComment::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes( oReader );

		if ( oReader.IsEmptyNode() )
			return;

		fromXML2(oReader);
	}
	std::wstring CComment::getText() const
	{
		bool bFirstPar = true;
		std::wstring sRes = getTextArr(m_arrItems, bFirstPar);
		return sRes;
	}
	std::wstring CComment::getTextArr(const std::vector<WritingElement* > & arrItems, bool& bFirstPar) const
	{
		std::wstring sRes;
		for ( size_t i = 0; i < arrItems.size(); ++i)
		{
			WritingElement* item = arrItems[i];
			if (item == NULL) continue;

			switch(item->getType())
			{
			case OOX::et_w_sdt:
				{
					OOX::Logic::CSdt* pStd = static_cast<OOX::Logic::CSdt*>(item);
					if(pStd->m_oSdtContent.IsInit())
						sRes += getTextArr(pStd->m_oSdtContent->m_arrItems, bFirstPar);
				}
				break;
			case OOX::et_w_smartTag:
				{
					OOX::Logic::CSmartTag* pSmartTag = static_cast<OOX::Logic::CSmartTag*>(item);
					sRes += getTextArr(pSmartTag->m_arrItems, bFirstPar);
				}
				break;
			case OOX::et_w_dir:
				{
					OOX::Logic::CDir* pDir = static_cast<OOX::Logic::CDir*>(item);
					sRes += getTextArr(pDir->m_arrItems, bFirstPar);
				}
				break;
			case OOX::et_w_bdo:
				{
					OOX::Logic::CBdo* pBdo = static_cast<OOX::Logic::CBdo*>(item);
					sRes += getTextArr(pBdo->m_arrItems, bFirstPar);
				}
				break;
			case OOX::et_w_tbl:
				{
					OOX::Logic::CTbl* pTbl = static_cast<OOX::Logic::CTbl*>(item);
					sRes += getTextArr(pTbl->m_arrItems, bFirstPar);
				}
				break;
			case OOX::et_w_tr:
				{
					OOX::Logic::CTr* pTr = static_cast<OOX::Logic::CTr*>(item);
					sRes += getTextArr(pTr->m_arrItems, bFirstPar);
				}
				break;
			case OOX::et_w_tc:
				{
					OOX::Logic::CTc* pTc = static_cast<OOX::Logic::CTc*>(item);
					sRes += getTextArr(pTc->m_arrItems, bFirstPar);
				}
				break;
			case OOX::et_w_hyperlink:
				{
					OOX::Logic::CHyperlink* pHyperlink = static_cast<OOX::Logic::CHyperlink*>(item);
					sRes += getTextArr(pHyperlink->m_arrItems, bFirstPar);
				}
				break;
			case OOX::et_w_p:
				{
					if(bFirstPar)
						bFirstPar = false;
					else
						sRes += L"\n";
					OOX::Logic::CParagraph* pParagraph = static_cast<OOX::Logic::CParagraph*>(item);
					sRes += getTextArr(pParagraph->m_arrItems, bFirstPar);
				}
				break;
			case OOX::et_w_r:
				{
					OOX::Logic::CRun* pRun = static_cast<OOX::Logic::CRun*>(item);
					sRes += getTextArr(pRun->m_arrItems, bFirstPar);
				}
				break;
			case OOX::et_w_cr:
			case OOX::et_w_br:
				sRes += L"\n";
				break;
			case OOX::et_w_nonBreakHyphen:
				{
					wchar_t oNewChar = 0x2013;
					sRes += oNewChar;
					break;
				}
			case OOX::et_w_tab:
				sRes += L" ";
				break;
			case OOX::et_w_sym:
				{
					OOX::Logic::CSym* oSym = static_cast<OOX::Logic::CSym*>(item);
					wchar_t oNewChar = 0x0FFF & oSym->m_oChar->GetValue();
					sRes += oNewChar;
					//sRes.AppendChar(0x0FFF & oSym->m_oChar->GetValue());
				}
				break;
			case OOX::et_w_t:
				{
					std::wstring& sText = static_cast<OOX::Logic::CText*>(item)->m_sText;
					if(!sText.empty())
					{
						sRes += sText;
					}
				}
				break;
			default:
				break;
			}
		}
		return sRes;
	}
	void CComment::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if     ( oReader, L"w:author",		m_oAuthor )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"w:date",			m_oDate )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"oodata",			m_oOOData )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"w:id",			m_oId )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"w:initials",      m_oInitials )

			WritingElement_ReadAttributes_Read_if     ( oReader, L"aml:author",		m_oAuthor )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"aml:createdate",	m_oDate )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"aml:id",			m_oId )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"aml:initials",	m_oInitials )
		WritingElement_ReadAttributes_End( oReader )
	}

	CComments::CComments(OOX::Document *pMain) : OOX::File(pMain), OOX::IFileContainer(pMain)
	{
		CDocx* docx = dynamic_cast<CDocx*>(File::m_pMainDocument);

		if (docx)
		{
			if (docx->m_bGlossaryRead)	docx->m_oGlossary.comments = this;
			else						docx->m_oMain.comments = this;
		}
	}
	CComments::CComments(OOX::Document *pMain, const CPath& oPath) : OOX::File(pMain), OOX::IFileContainer(pMain)
	{
		CDocx* docx = dynamic_cast<CDocx*>(File::m_pMainDocument);

		if (docx)
		{
			if (docx->m_bGlossaryRead)	docx->m_oGlossary.comments = this;
			else						docx->m_oMain.comments = this;
		}
		read( oPath );
	}
	CComments::~CComments()
	{
		for(size_t i = 0, length = m_arrComments.size(); i < length; ++i)
		{
			if (m_arrComments[i]) delete m_arrComments[i];
			m_arrComments[i] = NULL;
		}
		m_arrComments.clear();
	}
	void CComments::read(const CPath& oPath)
	{
		//don't use this. use read(const CPath& oRootPath, const CPath& oFilePath)
		CPath oRootPath;
		read(oRootPath, oPath);
	}
	void CComments::write(const CPath& oFilePath, const CPath& oDirectory, CContentTypes& oContent) const
	{
	}
	const OOX::FileType CComments::type() const
	{
		return FileTypes::Comments;
	}
	const CPath CComments::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}
	const CPath CComments::DefaultFileName() const
	{
		return type().DefaultFileName();
	}
	void CComments::read(const CPath& oRootPath, const CPath& oFilePath)
	{
		IFileContainer::Read( oRootPath, oFilePath );

		XmlUtils::CXmlLiteReader oReader;

		if ( !oReader.FromFile( oFilePath.GetPath() ) )
			return;

		if ( !oReader.ReadNextNode() )
			return;

		std::wstring sName = oReader.GetName();
		if ( L"w:comments" == sName && !oReader.IsEmptyNode() )
		{
			int nNumberingDepth = oReader.GetDepth();
			while ( oReader.ReadNextSiblingNode( nNumberingDepth ) )
			{
				sName = oReader.GetName();
				if ( L"w:comment" == sName )
				{
					CComment* pComment = new CComment(oReader);

					if ((pComment) && (pComment->m_oId.IsInit()))
					{
						m_mapComments.insert( std::make_pair( pComment->m_oId->GetValue(), m_arrComments.size()));
					}
					m_arrComments.push_back( pComment );
				}
			}
		}
	}

	CDocumentComments::CDocumentComments(OOX::Document *pMain) : CComments(NULL)
	{
		File::m_pMainDocument = pMain;
		CDocx* docx = dynamic_cast<CDocx*>(File::m_pMainDocument);
		if (docx) docx->m_pDocumentComments = this;
	}
	CDocumentComments::CDocumentComments(OOX::Document *pMain, const CPath& oPath) : CComments(NULL, oPath)
	{
		File::m_pMainDocument = pMain;
		CDocx* docx = dynamic_cast<CDocx*>(File::m_pMainDocument);
		if (docx) docx->m_pDocumentComments = this;
	}
	const OOX::FileType CDocumentComments::type() const
	{
		return FileTypes::DocumentComments;
	}
	const CPath CDocumentComments::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}
	const CPath CDocumentComments::DefaultFileName() const
	{
		return type().DefaultFileName();
	}

	CCommentExt::CCommentExt()
	{
	}
	CCommentExt::~CCommentExt()
	{
	}
	void CCommentExt::fromXML(XmlUtils::CXmlNode& oNode)
	{
	}
	std::wstring CCommentExt::toXML() const
	{
		return L"";
	}
	EElementType CCommentExt::getType() const
	{
		return et_w15_commentEx;
	}
	void CCommentExt::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes( oReader );

		if ( !oReader.IsEmptyNode() )
			oReader.ReadTillEnd();
	}
	void CCommentExt::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if     ( oReader, L"w15:paraId",			m_oParaId )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"w15:paraIdParent",	m_oParaIdParent )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"w15:done",			m_oDone )
		WritingElement_ReadAttributes_End( oReader )
	}

	CCommentsExt::CCommentsExt(OOX::Document *pMain) : OOX::File(pMain)//, OOX::IFileContainer(pMain)
	{
		CDocx* docx = dynamic_cast<CDocx*>(File::m_pMainDocument);
		if (docx) docx->m_pCommentsExt = this;
	}
	CCommentsExt::CCommentsExt(OOX::Document *pMain, const CPath& oPath) : OOX::File(pMain)//, OOX::IFileContainer(pMain)
	{
		CDocx* docx = dynamic_cast<CDocx*>(File::m_pMainDocument);
		if (docx) docx->m_pCommentsExt = this;

		read( oPath );
	}
	CCommentsExt::~CCommentsExt()
	{
		for(size_t i = 0, length = m_arrComments.size(); i < length; ++i)
		{
			if (m_arrComments[i]) delete m_arrComments[i];
			m_arrComments[i] = NULL;
		}
		m_arrComments.clear();
	}
	void CCommentsExt::read(const CPath& oPath)
	{
		//don't use this. use read(const CPath& oRootPath, const CPath& oFilePath)
		CPath oRootPath;
		read(oRootPath, oPath);
	}
	void CCommentsExt::write(const CPath& oFilePath, const CPath& oDirectory, CContentTypes& oContent) const
	{
	}
	const OOX::FileType CCommentsExt::type() const
	{
		return FileTypes::CommentsExt;
	}
	const CPath CCommentsExt::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}
	const CPath CCommentsExt::DefaultFileName() const
	{
		return type().DefaultFileName();
	}
	void CCommentsExt::read(const CPath& oRootPath, const CPath& oFilePath)
	{
		//IFileContainer::Read( oRootPath, oFilePath );

		XmlUtils::CXmlLiteReader oReader;

		if ( !oReader.FromFile( oFilePath.GetPath() ) )
			return;

		if ( !oReader.ReadNextNode() )
			return;

		std::wstring sName = oReader.GetName();
		if ( L"w15:commentsEx" == sName && !oReader.IsEmptyNode() )
		{
			int nNumberingDepth = oReader.GetDepth();
			while ( oReader.ReadNextSiblingNode( nNumberingDepth ) )
			{
				sName = oReader.GetName();
				if ( L"w15:commentEx" == sName )
				{
					CCommentExt* pCommentExt = new CCommentExt(oReader);
					if ((pCommentExt) && (pCommentExt->m_oParaId.IsInit()))
					{
						m_mapComments.insert( std::make_pair( pCommentExt->m_oParaId->GetValue(), m_arrComments.size()));
						m_arrComments.push_back( pCommentExt );
					}
				}
			}
		}
	}

	CDocumentCommentsExt::CDocumentCommentsExt(OOX::Document *pMain) : CCommentsExt(NULL)
	{
		File::m_pMainDocument = pMain;
		CDocx* docx = dynamic_cast<CDocx*>(File::m_pMainDocument);
		if (docx) docx->m_pDocumentCommentsExt = this;
	}
	CDocumentCommentsExt::CDocumentCommentsExt(OOX::Document *pMain, const CPath& oPath) : CCommentsExt(NULL, oPath)
	{
		File::m_pMainDocument = pMain;
		CDocx* docx = dynamic_cast<CDocx*>(File::m_pMainDocument);
		if (docx) docx->m_pDocumentCommentsExt = this;
	}
	const OOX::FileType CDocumentCommentsExt::type() const
	{
		return FileTypes::DocumentCommentsExt;
	}

	CCommentExtensible::CCommentExtensible()
	{
	}
	CCommentExtensible::~CCommentExtensible()
	{
	}
	void CCommentExtensible::fromXML(XmlUtils::CXmlNode& oNode)
	{
	}
	std::wstring CCommentExtensible::toXML() const
	{
		return L"";
	}
	EElementType CCommentExtensible::getType() const
	{
		return et_w16_commentExtensible;
	}
	void CCommentExtensible::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes( oReader );

		if ( oReader.IsEmptyNode() )
			return;

		int nParentDepth = oReader.GetDepth();
		while( oReader.ReadNextSiblingNode( nParentDepth ) )
		{
			std::wstring strName = XmlUtils::GetNameNoNS(oReader.GetName());
			if ( L"extLst" == strName )
				m_oExtLst = oReader;
		}
	}
	void CCommentExtensible::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_StartChar_No_NS( oReader )
			WritingElement_ReadAttributes_Read_ifChar     ( oReader, "durableId", m_oDurableId )
			WritingElement_ReadAttributes_Read_else_ifChar( oReader, "dateUtc",	m_oDateUtc )
			WritingElement_ReadAttributes_Read_else_ifChar( oReader, "intelligentPlaceholder", m_oIntelligentPlaceholder )
		WritingElement_ReadAttributes_EndChar_No_NS( oReader )
	}

	CCommentsExtensible::CCommentsExtensible(OOX::Document *pMain) : OOX::File(pMain)//, OOX::IFileContainer(pMain)
	{
		CDocx* docx = dynamic_cast<CDocx*>(File::m_pMainDocument);
		if (docx) docx->m_pCommentsExtensible = this;
	}
	CCommentsExtensible::CCommentsExtensible(OOX::Document *pMain, const CPath& oPath) : OOX::File(pMain)//, OOX::IFileContainer(pMain)
	{
		CDocx* docx = dynamic_cast<CDocx*>(File::m_pMainDocument);
		if (docx) docx->m_pCommentsExtensible = this;

		read( oPath );
	}
	CCommentsExtensible::~CCommentsExtensible()
	{
		for(size_t i = 0, length = m_arrComments.size(); i < length; ++i)
		{
			if (m_arrComments[i]) delete m_arrComments[i];
			m_arrComments[i] = NULL;
		}
		m_arrComments.clear();
	}
	void CCommentsExtensible::read(const CPath& oRootPath, const CPath& oFilePath)
	{
		//IFileContainer::Read( oRootPath, oFilePath );

		XmlUtils::CXmlLiteReader oReader;

		if ( !oReader.FromFile( oFilePath.GetPath() ) )
			return;

		if ( !oReader.ReadNextNode() )
			return;

		const char* sName = XmlUtils::GetNameNoNS(oReader.GetNameChar());
		if (strcmp("commentsExtensible", sName) == 0)
		{
			int nNumberingDepth = oReader.GetDepth();
			while ( oReader.ReadNextSiblingNode( nNumberingDepth ) )
			{
				const char* sNameSub = XmlUtils::GetNameNoNS(oReader.GetNameChar());
				if (strcmp("commentExtensible", sNameSub) == 0)
				{
					m_arrComments.push_back( new CCommentExtensible(oReader) );
				}
			}
		}
	}
	void CCommentsExtensible::read(const CPath& oPath)
	{
		//don't use this. use read(const CPath& oRootPath, const CPath& oFilePath)
		CPath oRootPath;
		read(oRootPath, oPath);
	}
	void CCommentsExtensible::write(const CPath& oFilePath, const CPath& oDirectory, CContentTypes& oContent) const
	{
	}
	const OOX::FileType CCommentsExtensible::type() const
	{
		return FileTypes::CommentsExtensible;
	}
	const CPath CCommentsExtensible::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}
	const CPath CCommentsExtensible::DefaultFileName() const
	{
		return type().DefaultFileName();
	}

	CDocumentCommentsExtensible::CDocumentCommentsExtensible(OOX::Document *pMain) : CCommentsExtensible(NULL)
	{
		File::m_pMainDocument = pMain;
		CDocx* docx = dynamic_cast<CDocx*>(File::m_pMainDocument);
		if (docx) docx->m_pDocumentCommentsExtensible = this;
	}
	CDocumentCommentsExtensible::CDocumentCommentsExtensible(OOX::Document *pMain, const CPath& oPath) : CCommentsExtensible(NULL, oPath)
	{
		File::m_pMainDocument = pMain;
		CDocx* docx = dynamic_cast<CDocx*>(File::m_pMainDocument);
		if (docx) docx->m_pDocumentCommentsExtensible = this;
	}
	const OOX::FileType CDocumentCommentsExtensible::type() const
	{
		return FileTypes::DocumentCommentsExtensible;
	}

	CCommentsUserData::CCommentsUserData(OOX::Document *pMain) : CCommentsExtensible(NULL)
	{
		File::m_pMainDocument = pMain;
		CDocx* docx = dynamic_cast<CDocx*>(File::m_pMainDocument);
		if (docx) docx->m_pCommentsUserData = this;
	}
	CCommentsUserData::CCommentsUserData(OOX::Document *pMain, const CPath& oPath) : CCommentsExtensible(NULL, oPath)
	{
		File::m_pMainDocument = pMain;
		CDocx* docx = dynamic_cast<CDocx*>(File::m_pMainDocument);
		if (docx) docx->m_pCommentsUserData = this;
	}
	const OOX::FileType CCommentsUserData::type() const
	{
		return FileTypes::CommentsUserData;
	}

	CCommentId::CCommentId()
	{
	}
	CCommentId::~CCommentId()
	{
	}
	void CCommentId::fromXML(XmlUtils::CXmlNode& oNode)
	{
	}
	std::wstring CCommentId::toXML() const
	{
		return L"";
	}
	EElementType CCommentId::getType() const
	{
		return et_w16_commentId;
	}
	void CCommentId::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes( oReader );

		if ( !oReader.IsEmptyNode() )
			oReader.ReadTillEnd();
	}
	void CCommentId::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if     ( oReader, L"w16cid:paraId",		m_oParaId )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"w16cid:durableId",	m_oDurableId )
		WritingElement_ReadAttributes_End( oReader )
	}

	CCommentsIds::CCommentsIds(OOX::Document *pMain) : OOX::File(pMain)
	{
		CDocx* docx = dynamic_cast<CDocx*>(File::m_pMainDocument);
		if (docx) docx->m_pCommentsIds = this;
	}
	CCommentsIds::CCommentsIds(OOX::Document *pMain, const CPath& oPath) : OOX::File(pMain)
	{
		CDocx* docx = dynamic_cast<CDocx*>(File::m_pMainDocument);
		if (docx) docx->m_pCommentsIds = this;

		read( oPath );
	}
	CCommentsIds::~CCommentsIds()
	{
		for(size_t i = 0, length = m_arrComments.size(); i < length; ++i)
		{
			if (m_arrComments[i]) delete m_arrComments[i];
			m_arrComments[i] = NULL;
		}
		m_arrComments.clear();
	}
	void CCommentsIds::read(const CPath& oFilePath)
	{
		XmlUtils::CXmlLiteReader oReader;

		if ( !oReader.FromFile( oFilePath.GetPath() ) )
			return;

		if ( !oReader.ReadNextNode() )
			return;

		std::wstring sName = oReader.GetName();
		if ( L"w16cid:commentsIds" == sName && !oReader.IsEmptyNode() )
		{
			int nNumberingDepth = oReader.GetDepth();
			while ( oReader.ReadNextSiblingNode( nNumberingDepth ) )
			{
				sName = oReader.GetName();
				if ( L"w16cid:commentId" == sName )
				{
					CCommentId* pCommentExt = new CCommentId(oReader);
					if (pCommentExt->m_oParaId.IsInit() && pCommentExt->m_oDurableId.IsInit())
					{
						m_arrComments.push_back( pCommentExt );
					}
				}
			}
		}
	}
	void CCommentsIds::write(const CPath& oFilePath, const CPath& oDirectory, CContentTypes& oContent) const
	{
	}
	const OOX::FileType CCommentsIds::type() const
	{
		return FileTypes::CommentsIds;
	}
	const CPath CCommentsIds::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}
	const CPath CCommentsIds::DefaultFileName() const
	{
		return type().DefaultFileName();
	}

	CDocumentCommentsIds::CDocumentCommentsIds(OOX::Document *pMain) : CCommentsIds(NULL)
	{
		File::m_pMainDocument = pMain;
		CDocx* docx = dynamic_cast<CDocx*>(File::m_pMainDocument);
		if (docx) docx->m_pDocumentCommentsIds = this;
	}
	CDocumentCommentsIds::CDocumentCommentsIds(OOX::Document *pMain, const CPath& oPath) : CCommentsIds(NULL, oPath)
	{
		File::m_pMainDocument = pMain;
		CDocx* docx = dynamic_cast<CDocx*>(File::m_pMainDocument);
		if (docx) docx->m_pDocumentCommentsIds = this;
	}
	const OOX::FileType CDocumentCommentsIds::type() const
	{
		return FileTypes::DocumentCommentsIds;
	}

	CPresenceInfo::CPresenceInfo()
	{
	}
	CPresenceInfo::~CPresenceInfo()
	{
	}
	void CPresenceInfo::fromXML(XmlUtils::CXmlNode& oNode)
	{
	}
	std::wstring CPresenceInfo::toXML() const
	{
		return L"";
	}
	EElementType CPresenceInfo::getType() const
	{
		return et_w15_presenceInfo;
	}
	void CPresenceInfo::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes( oReader );

		if ( !oReader.IsEmptyNode() )
			oReader.ReadTillEnd();
	}
	void CPresenceInfo::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_StartChar_No_NS( oReader )
			WritingElement_ReadAttributes_Read_ifChar     ( oReader, "providerId", m_oProviderId )
			WritingElement_ReadAttributes_Read_else_ifChar( oReader, "userId",	m_oUserId )
		WritingElement_ReadAttributes_EndChar_No_NS( oReader )
	}

	CPerson::CPerson()
	{
	}
	CPerson::~CPerson()
	{
	}
	void CPerson::fromXML(XmlUtils::CXmlNode& oNode)
	{
	}
	std::wstring CPerson::toXML() const
	{
		return L"";
	}
	EElementType CPerson::getType() const
	{
		return et_w15_person;
	}
	void CPerson::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes( oReader );

		if ( oReader.IsEmptyNode() )
			return;

		int nParentDepth = oReader.GetDepth();
		while( oReader.ReadNextSiblingNode( nParentDepth ) )
		{
			std::wstring sName = oReader.GetName();
			if ( L"w15:presenceInfo" ==sName )
				m_oPresenceInfo = oReader;
		}
	}
	void CPerson::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if ( oReader, L"w15:author", m_oAuthor )
		WritingElement_ReadAttributes_End( oReader )
	}

	CPeople::CPeople(OOX::Document *pMain) : OOX::File(pMain)
	{
		CDocx* docx = dynamic_cast<CDocx*>(File::m_pMainDocument);
		if (docx) docx->m_pPeople = this;
	}
	CPeople::CPeople(OOX::Document *pMain, const CPath& oPath) : OOX::File(pMain)
	{
		CDocx* docx = dynamic_cast<CDocx*>(File::m_pMainDocument);
		if (docx) docx->m_pPeople = this;

		read( oPath );
	}
	CPeople::~CPeople()
	{
		for(size_t i = 0, length = m_arrPeoples.size() ; i < length; ++i)
			if (m_arrPeoples[i]) delete m_arrPeoples[i];
		m_arrPeoples.clear();
	}
	void CPeople::read(const CPath& oFilePath)
	{
		XmlUtils::CXmlLiteReader oReader;

		if ( !oReader.FromFile( oFilePath.GetPath() ) )
			return;

		if ( !oReader.ReadNextNode() )
			return;

		std::wstring sName = oReader.GetName();
		if ( L"w15:people" == sName && !oReader.IsEmptyNode() )
		{
			int nNumberingDepth = oReader.GetDepth();
			while ( oReader.ReadNextSiblingNode( nNumberingDepth ) )
			{
				sName = oReader.GetName();
				if ( L"w15:person" == sName )
					m_arrPeoples.push_back( new CPerson(oReader) );
			}
		}
	}
	void CPeople::write(const CPath& oFilePath, const CPath& oDirectory, CContentTypes& oContent) const
	{
	}
	const OOX::FileType CPeople::type() const
	{
		return FileTypes::People;
	}
	const CPath CPeople::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}
	const CPath CPeople::DefaultFileName() const
	{
		return type().DefaultFileName();
	}

	CDocumentPeople::CDocumentPeople(OOX::Document *pMain) : CPeople(NULL)
	{
		File::m_pMainDocument = pMain;
		CDocx* docx = dynamic_cast<CDocx*>(File::m_pMainDocument);
		if (docx) docx->m_pDocumentPeople = this;
	}
	CDocumentPeople::CDocumentPeople(OOX::Document *pMain, const CPath& oPath) : CPeople(NULL, oPath)
{
	File::m_pMainDocument = pMain;
	CDocx* docx = dynamic_cast<CDocx*>(File::m_pMainDocument);
	if (docx) docx->m_pDocumentPeople = this;
}
	const OOX::FileType CDocumentPeople::type() const
	{
		return FileTypes::DocumentPeople;
	}

} // namespace OOX

