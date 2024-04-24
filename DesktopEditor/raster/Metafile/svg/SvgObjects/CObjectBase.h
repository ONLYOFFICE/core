/*
 * (c) Copyright Ascensio System SIA 2010-2024
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

#ifndef COBJECTBASE_H
#define COBJECTBASE_H

#include "../../../../../Common/3dParty/html/css/src/CNode.h"
#include "../../../../../Common/3dParty/html/css/src/StaticFunctions.h"
#include "../../../../xml/include/xmlutils.h"
#include "../../../../graphics/IRenderer.h"
#include "../../../common/IGrObject.h"
#include "../SvgTypes.h"

class CSvgFile;

namespace SVG
{
	struct TSvgStyles
	{
		SvgColor     m_oFill;
		TStroke      m_oStroke;
		SvgDigit     m_oOpacity;

		TSvgStyles& operator+=(const TSvgStyles& oSvgStyles);
	};
	
	struct TSvgTransformation
	{
		SvgTransform m_oTransform;
		TClip        m_oClip;
		SvgColor     m_oMask;
		SvgDigit     m_oOpacity;
		bool         m_bDraw;
	};

	enum ObjectType
	{
		RendererObject,
		AppliedObject
	};

	class CObject : public IGrObject
	{
	public:
		CObject(const NSCSS::CNode& oData);
		CObject(XmlUtils::CXmlNode& oNode);
		virtual ~CObject();

		virtual ObjectType GetType() const = 0;

		void SetData(const std::wstring wsStyles, unsigned short ushLevel, bool bHardMode = false);

		virtual void SetData(const std::map<std::wstring, std::wstring>& mAttributes, unsigned short ushLevel, bool bHardMode = false) = 0;

		void SetTransform(const std::map<std::wstring, std::wstring>& mAttributes, unsigned short ushLevel, bool bHardMode = false);
		void SetClip(const std::map<std::wstring, std::wstring>& mAttributes, unsigned short ushLevel, bool bHardMode = false);
		void SetMask(const std::map<std::wstring, std::wstring>& mAttributes, unsigned short ushLevel, bool bHardMode = false);
		void SetDisplay(const std::map<std::wstring, std::wstring>& mAttributes, unsigned short ushLevel, bool bHardMode = false);
		void SetOpacity(const std::map<std::wstring, std::wstring>& mAttributes, unsigned short ushLevel, bool bHardMode = false);
		
		std::wstring GetId() const;
		virtual std::vector<NSCSS::CNode> GetFullPath() const;

	private:
		bool ApplyTransform(IRenderer* pRenderer, const SvgTransform* pTransform, Aggplus::CMatrix& oOldMatrix) const;
		bool ApplyClip(IRenderer* pRenderer, const TClip* pClip, const CSvgFile *pFile, const TBounds& oBounds) const;
		bool ApplyMask(IRenderer* pRenderer, const SvgColor* pMask, const CSvgFile *pFile, const TBounds& oBounds) const;

		bool ApplyDef(IRenderer* pRenderer, const CSvgFile *pFile, const std::wstring& wsUrl, const TBounds& oBounds) const;

		void SetNodeData(XmlUtils::CXmlNode& oNode);

		friend class CRenderedObject;
		friend class CAppliedObject;

		friend class CUse;
		friend class CLine;
		friend class CRect;
		friend class CPath;
		friend class CText;
		friend class CTSpan;
		friend class CImage;
		friend class CCircle;
		friend class CPolygon;
		friend class CEllipse;
		friend class CPolyline;
		friend class CGraphicsContainer;

		friend class CClipPath;

		NSCSS::CNode       m_oXmlNode;
		TSvgTransformation m_oTransformtaion;
	};

	enum CommandeMode
	{
		CommandeModeDraw,
		CommandeModeClip,
		CommandeModeMask
	};

	class CRenderedObject : public CObject
	{
	public:
		CRenderedObject(const NSCSS::CNode& oData, CRenderedObject* pParent = NULL);
		CRenderedObject(XmlUtils::CXmlNode& oNode, CRenderedObject* pParent = NULL);
		virtual ~CRenderedObject();

		ObjectType GetType() const override;

		virtual void SetData(const std::map<std::wstring, std::wstring>& mAttributes, unsigned short ushLevel, bool bHardMode = false) override;

		virtual bool Draw(IRenderer* pRenderer, const CSvgFile *pFile, CommandeMode oMode = CommandeModeDraw, const TSvgStyles* pStyles = NULL) const = 0;

		virtual TBounds GetBounds() const = 0;

		std::vector<NSCSS::CNode> GetFullPath() const override;
	private:
		void SetDefaultStyles();

		void SetStroke(const std::map<std::wstring, std::wstring>& mAttributes, unsigned short ushLevel, bool bHardMode = false);
		void SetFill(const std::map<std::wstring, std::wstring>& mAttributes, unsigned short ushLevel, bool bHardMode = false);

		bool StartPath(IRenderer* pRenderer, const CSvgFile *pFile, Aggplus::CMatrix& oOldTransform, CommandeMode oMode = CommandeModeDraw) const;
		void EndPath(IRenderer* pRenderer, const CSvgFile *pFile, const Aggplus::CMatrix& oOldTransform, CommandeMode oMode = CommandeModeDraw, const TSvgStyles* pOtherStyles = NULL) const;

		virtual void ApplyStyle(IRenderer* pRenderer, const TSvgStyles* pStyles, const CSvgFile *pFile, int& nTypePath) const;

		bool ApplyStroke(IRenderer* pRenderer, const TStroke* pStroke, bool bUseDefault = false) const;
		bool ApplyFill(IRenderer* pRenderer, const SvgColor* pFill, const CSvgFile *pFile, bool bUseDefault = false) const;
		bool ApplyOpacity(IRenderer* pRenderer, const SvgDigit* pOpacity) const;

		friend class CUse;
		friend class CLine;
		friend class CRect;
		friend class CPath;
		friend class CMask;
		friend class CTSpan;
		friend class CImage;
		friend class CCircle;
		friend class CPolygon;
		friend class CEllipse;
		friend class CPolyline;
		friend class CTextPath;
		friend class CClipPath;
		friend class CGraphicsContainer;

		TSvgStyles       m_oStyles;
		CRenderedObject *m_pParent;
	};

	class CAppliedObject : public CObject
	{
	public:
		CAppliedObject(XmlUtils::CXmlNode& oNode);
		virtual ~CAppliedObject();

		ObjectType GetType() const override;

		virtual bool Apply(IRenderer* pRenderer, const CSvgFile *pFile, const TBounds &oObjectBounds) = 0;
	};
}


#endif // COBJECTBASE_H
