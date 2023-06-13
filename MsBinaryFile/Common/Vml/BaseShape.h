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

#include "VmlPath.h"
#include "../../../OOXML/Common/SimpleTypes_Base.h"

#include <boost/shared_ptr.hpp>

namespace ODRAW
{
    using namespace NSBaseShape;
    class CHandle_
    {
    public:
        std::wstring position;
        std::wstring xrange;
        std::wstring yrange;
        std::wstring switchHandle;
        std::wstring polar;
        std::wstring radiusrange;

		CHandle_();
		CHandle_& operator =(const CHandle_& oSrc);
    };

	class CBaseShape;
	typedef boost::shared_ptr<CBaseShape> CBaseShapePtr;

    class CBaseShape
    {
    public:
		std::vector<long>					m_arAbsMaxAdjustments;
		std::vector<long>					m_arAdjustments;
        std::vector<double>					m_arGuides;
		long								m_eJoin;
        bool								m_bConcentricFill;
        bool								m_bCustomShape;
        std::vector<SimpleTypes::CPoint>	m_arConnectors;
        std::vector<long>					m_arConnectorAngles;

        std::vector<Aggplus::RECT>			m_arTextRects;

        std::vector<CHandle_>				m_arHandles;

        std::wstring						m_strTransformXml;

        std::wstring						m_strPath;
        std::wstring						m_strRect;

        int									m_lLimoX;
        int									m_lLimoY;

        CPath								m_oPath;

		CBaseShape();

        virtual bool LoadFromXML(const std::wstring& xml)               = 0;
        virtual bool LoadFromXML(XmlUtils::CXmlNode& xmlNode)           = 0;
        virtual bool LoadAdjustValuesList(const std::wstring& xml)		= 0;
        virtual bool LoadGuidesList(const std::wstring& xml)            = 0;
        virtual bool LoadAdjustHandlesList(const std::wstring& xml)		= 0;
        virtual bool LoadConnectorsList(const std::wstring& xml)		= 0;
        virtual bool LoadTextRect(const std::wstring& xml)				= 0;
        virtual bool LoadPathList(const std::wstring& xml)				= 0;
        virtual bool SetAdjustment(long index, long value)				= 0;
        virtual void ReCalculate()										= 0;

        virtual std::wstring ToXML(CGeomShapeInfo& GeomInfo, double StartTime, double EndTime, CBrush& Brush, CPen& Pen) = 0;
		virtual void ToRenderer(IRenderer* pRenderer, CGeomShapeInfo& pGeomInfo, double dStartTime, double dEndTime, CPen& pPen, CBrush& pFore);

		virtual void SetWidthHeightLogic(const double& dWidth, const double& dHeight);
		virtual void GetWidthHeightLogic(double& dWidth, double& dHeight);

		virtual void AddGuide(const std::wstring& strGuide);

        static CBaseShapePtr CreateByType(ClassType ClassType, int ShapeType);

        virtual const ClassType GetClassType()const				= 0;
        bool SetType(ClassType ClassType, int ShapeType);

		virtual bool SetProperties(CBaseShape* Shape);
		virtual bool SetToDublicate(CBaseShape* Shape);
    };
}
