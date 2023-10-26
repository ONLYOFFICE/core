/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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

#include "./../WrapperWritingElement.h"
#include "NvPicPr.h"
#include "Fills/BlipFill.h"
#include "SpPr.h"
#include "ShapeStyle.h"

namespace OOX
{
	class OleObject;
}
namespace PPTX
{
	namespace Limit
	{
		DEFINE_LIMIT_BASE(OLEDrawAspectType)

		DEFINE_LIMIT_BASE(OLEType)

		DEFINE_LIMIT_BASE(OLEUpdateMode)

	} // namespace Limit
} // namespace PPTX

namespace PPTX
{
	namespace Logic
	{
		class Shape;
		class COLEObject : public WrapperWritingElement
		{
		public:
			virtual OOX::EElementType getType () const;
			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;
			
			virtual void toXmlWriter (NSBinPptxRW::CXmlWriter* pWriter) const;
			
			virtual void toPPTY (NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY (NSBinPptxRW::CBinaryFileReader* pReader);
			
			virtual void FillParentPointersForChilds();
			
			bool isValid();
			
			smart_ptr<OOX::OleObject>	GetOleObject(const OOX::RId& oRId, OOX::IFileContainer* pRels)const;
			std::wstring				GetOleData	(const std::wstring& sFilePath)const;

			nullable_limit<Limit::OLEDrawAspectType>m_oDrawAspect;
			nullable<OOX::RId>						m_oId;
			nullable_string							m_sObjectId;
			nullable_string							m_sProgId;
			nullable_string							m_sShapeId;
			nullable_limit<Limit::OLEType>			m_oType;
			nullable_limit<Limit::OLEUpdateMode>	m_oUpdateMode;

	//internal
			nullable_string						m_sData;
			
			smart_ptr<OOX::OleObject>			m_OleObjectFile;
		
			nullable_int						m_oDxaOrig;
			nullable_int						m_oDyaOrig;
			nullable_bool						m_oShowAsIcon;
			nullable_string						m_oName;
			nullable_bool						m_oMoveWithCells;
			nullable_bool						m_oSizeWithCells;

			nullable_string						m_sAlternateContenteXml;
		private:
			bool ReadXlsxBin(NSBinPptxRW::CBinaryFileReader* pReader, const std::wstring& sDstEmbedded, BYTE* pData, long length);
			bool ReadDocxBin(NSBinPptxRW::CBinaryFileReader* pReader, const std::wstring& sDstEmbedded, BYTE* pData, long length);			
			bool ReadOoxmlZip(NSBinPptxRW::CBinaryFileReader* pReader, const std::wstring& sDstEmbedded, BYTE* pData, long length);
		};

		class Pic : public WrapperWritingElement
		{
		public:
			Pic(std::wstring ns = L"p");
			virtual ~Pic();

			virtual OOX::EElementType getType () const;

			explicit Pic(XmlUtils::CXmlNode& node);
			const Pic& operator =(XmlUtils::CXmlNode& node);
			
			explicit Pic(XmlUtils::CXmlLiteReader& oReader);
			const Pic& operator =(XmlUtils::CXmlLiteReader& oReader);

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void fromXMLOle(XmlUtils::CXmlNode& node);

			virtual void GetRect(Aggplus::RECT& pRect)const;
			
			virtual std::wstring GetFullPicName()const;
			smart_ptr<OOX::File> GetMediaLink(NSBinPptxRW::CBinaryFileWriter* pWriter = NULL)const;
			
			DWORD GetFill(UniFill& fill)const;
			DWORD GetLine(Ln& line)const;
			
			double GetStTrim () const;
			double GetEndTrim () const;
			long GetRefId() const;

			void FillLevelUp();
			void Merge(Pic& pic, bool bIsSlidePlaceholder = false);

			void SetLevelUpElement( Shape* p){m_pLevelUp = p;};

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			
			virtual std::wstring toXML() const;
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;
			void toXmlWriterVML(NSBinPptxRW::CXmlWriter* pWriter, smart_ptr<PPTX::Theme>& oTheme, smart_ptr<PPTX::Logic::ClrMap>& oClrMap, bool in_group = false);
			
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
//----------------------------------------------------------------------
			Shape*					m_pLevelUp;
			NvPicPr					nvPicPr;
			BlipFill				blipFill;
			SpPr					spPr;
			nullable<ShapeStyle>	style;
			nullable_string			macro;
			//internal
			nullable<COLEObject>	oleObject; 

			std::wstring			m_namespace;
			nullable_string			m_sClientDataXml;

		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

