/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */
#pragma once

#include "../Base/Nullable.h"
#include "../Common/SimpleTypes_Base.h"
#include "WritingElement.h"

//--------------------------------------------------------------------------------
// Ink Markup Language (InkML) W3C Recommendation 20 September 2011
// http://www.w3.org/2003/InkML
//--------------------------------------------------------------------------------

namespace SimpleTypes
{
namespace Ink
{
	enum EChannelType
	{
		channeltypeDecimal = 0,
		channeltypeInteger = 1,
		channeltypeDouble = 2,
		channeltypeBoolean = 3
	};
	DEFINE_SIMPLE_TYPE(CChannelType, EChannelType, channeltypeDecimal)

	enum EChannelOrientation
	{
		orientationPositive = 0,
		orientationNegative = 1
	};
	DEFINE_SIMPLE_TYPE(CChannelOrientation, EChannelOrientation, orientationPositive)

	enum ETraceType
	{
		tracetypePenDown = 0,
		tracetypePenUp = 1,
		tracetypeIndeterminate = 2
	};
	DEFINE_SIMPLE_TYPE(CTraceType, ETraceType, tracetypePenDown)

	enum ETraceContinuation
	{
		continuationBegin = 0,
		continuationEnd = 1,
		continuationMiddle = 2
	};
	DEFINE_SIMPLE_TYPE(CTraceContinuation, ETraceContinuation, continuationBegin)

	enum EMappingType
	{
		mappingtypeIdentity = 0,
		mappingtypeLookup = 1,
		mappingtypeAffine = 2,
		mappingtypeMathML = 3,
		mappingtypeProduct = 4,
		mappingtypeUnknown = 5
	};
	DEFINE_SIMPLE_TYPE(CMappingType, EMappingType, mappingtypeUnknown)

	enum ETableApply
	{
		applyAbsolute = 0,
		applyRelative = 1
	};
	DEFINE_SIMPLE_TYPE(CTableApply, ETableApply, applyAbsolute)

	enum ETableInterpolation
	{
		interpolationFloor = 0,
		interpolationMiddle = 1,
		interpolationCeiling = 2,
		interpolationLinear = 3,
		interpolationCubic = 4
	};
	DEFINE_SIMPLE_TYPE(CTableInterpolation, ETableInterpolation, interpolationFloor)
}// Ink
} // SimpleTypes

namespace OOX
{
	WritingElement* CreateInkElement(const std::wstring& sName);
	WritingElement* CreateInkElement(BYTE nType);
	BYTE GetInkRecordType(EElementType eType);

	//--------------------------------------------------------------------------------
	// annotation
	//--------------------------------------------------------------------------------
	class CInkAnnotation : public WritingElement
	{
	public:
		WritingElement_AdditionMethods(CInkAnnotation)
		CInkAnnotation() {}
		virtual ~CInkAnnotation() {}

		virtual void fromXML(XmlUtils::CXmlNode& node) {}
		virtual std::wstring toXML() const { return L""; }

		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
		virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

		void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
		virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
		virtual EElementType getType() const;

		nullable_string m_sType;
		nullable_string m_sEncoding;

		std::wstring m_sText;
	};
	//--------------------------------------------------------------------------------
	// annotationXML
	//--------------------------------------------------------------------------------
	class CInkAnnotationXML : public WritingElement
	{
	public:
		WritingElement_AdditionMethods(CInkAnnotationXML)
		CInkAnnotationXML() {}
		virtual ~CInkAnnotationXML() {}

		virtual void fromXML(XmlUtils::CXmlNode& node) {}
		virtual std::wstring toXML() const { return L""; }

		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
		virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

		void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
		virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
		virtual EElementType getType() const;

		nullable_string m_sType;
		nullable_string m_sEncoding;
		nullable_string m_sHref;

		std::wstring m_sContent; // any xml
	};
	//--------------------------------------------------------------------------------
	// bind
	//--------------------------------------------------------------------------------
	class CInkBind : public WritingElement
	{
	public:
		WritingElement_AdditionMethods(CInkBind)
		CInkBind() {}
		virtual ~CInkBind() {}

		virtual void fromXML(XmlUtils::CXmlNode& node) {}
		virtual std::wstring toXML() const { return L""; }

		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
		virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

		void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
		virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
		virtual EElementType getType() const;

		nullable_string m_sSource;
		nullable_string m_sTarget;
		nullable_string m_sColumn;
		nullable_string m_sVariable;
	};
	//--------------------------------------------------------------------------------
	// table
	//--------------------------------------------------------------------------------
	class CInkTable : public WritingElement
	{
	public:
		WritingElement_AdditionMethods(CInkTable)
		CInkTable() {}
		virtual ~CInkTable() {}

		virtual void fromXML(XmlUtils::CXmlNode& node) {}
		virtual std::wstring toXML() const { return L""; }

		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
		virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

		void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
		virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
		virtual EElementType getType() const;

		nullable_string m_sId;
		nullable<SimpleTypes::Ink::CTableApply> m_oApply;
		nullable<SimpleTypes::Ink::CTableInterpolation> m_oInterpolation;

		std::wstring m_sData;
	};
	//--------------------------------------------------------------------------------
	// matrix
	//--------------------------------------------------------------------------------
	class CInkMatrix : public WritingElement
	{
	public:
		WritingElement_AdditionMethods(CInkMatrix)
		CInkMatrix() {}
		virtual ~CInkMatrix() {}

		virtual void fromXML(XmlUtils::CXmlNode& node) {}
		virtual std::wstring toXML() const { return L""; }

		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
		virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

		void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
		virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
		virtual EElementType getType() const;

		nullable_string m_sId;

		std::wstring m_sData;
	};
	//--------------------------------------------------------------------------------
	// mapping (bind*, (table | matrix | mapping* | math)?)
	//--------------------------------------------------------------------------------
	class CInkMapping : public WritingElementWithChilds<>
	{
	public:
		WritingElement_AdditionMethods(CInkMapping)
		CInkMapping() {}
		virtual ~CInkMapping() {}

		virtual void fromXML(XmlUtils::CXmlNode& node) {}
		virtual std::wstring toXML() const { return L""; }

		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
		virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

		void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
		virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
		virtual EElementType getType() const;

		nullable_string m_sId;
		nullable<SimpleTypes::Ink::CMappingType> m_oType;
		nullable_string m_sMappingRef;

		std::wstring m_sMathML; // math:math as is
	};
	//--------------------------------------------------------------------------------
	// channel
	//--------------------------------------------------------------------------------
	class CInkChannel : public WritingElement
	{
	public:
		WritingElement_AdditionMethods(CInkChannel)
		CInkChannel() {}
		virtual ~CInkChannel() {}

		virtual void fromXML(XmlUtils::CXmlNode& node) {}
		virtual std::wstring toXML() const { return L""; }

		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
		virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

		void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
		virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
		virtual EElementType getType() const;

		nullable_string m_sId;
		nullable_string m_sName;
		nullable<SimpleTypes::Ink::CChannelType> m_oType;
		nullable_string m_sDefault;
		nullable_string m_sMin;
		nullable_string m_sMax;
		nullable<SimpleTypes::Ink::CChannelOrientation> m_oOrientation;
		nullable_string m_sRespectTo;
		nullable_string m_sUnits;

		nullable<CInkMapping> m_oMapping;
	};
	//--------------------------------------------------------------------------------
	// intermittentChannels
	//--------------------------------------------------------------------------------
	class CInkIntermittentChannels : public WritingElementWithChilds<CInkChannel>
	{
	public:
		WritingElement_AdditionMethods(CInkIntermittentChannels)
		CInkIntermittentChannels() {}
		virtual ~CInkIntermittentChannels() {}

		virtual void fromXML(XmlUtils::CXmlNode& node) {}
		virtual std::wstring toXML() const { return L""; }

		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
		virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

		virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
		virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
		virtual EElementType getType() const;
	};
	//--------------------------------------------------------------------------------
	// traceFormat (channel*, intermittentChannels?)
	//--------------------------------------------------------------------------------
	class CInkTraceFormat : public WritingElementWithChilds<CInkChannel>
	{
	public:
		WritingElement_AdditionMethods(CInkTraceFormat)
		CInkTraceFormat() {}
		virtual ~CInkTraceFormat() {}

		virtual void fromXML(XmlUtils::CXmlNode& node) {}
		virtual std::wstring toXML() const { return L""; }

		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
		virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

		void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
		virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
		virtual EElementType getType() const;

		nullable_string m_sId;

		nullable<CInkIntermittentChannels> m_oIntermittentChannels;
	};
	//--------------------------------------------------------------------------------
	// channelProperty
	//--------------------------------------------------------------------------------
	class CInkChannelProperty : public WritingElement
	{
	public:
		WritingElement_AdditionMethods(CInkChannelProperty)
		CInkChannelProperty() {}
		virtual ~CInkChannelProperty() {}

		virtual void fromXML(XmlUtils::CXmlNode& node) {}
		virtual std::wstring toXML() const { return L""; }

		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
		virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

		void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
		virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
		virtual EElementType getType() const;

		nullable_string m_sChannel;
		nullable_string m_sName;
		nullable_string m_sValue;
		nullable_string m_sUnits;
	};
	//--------------------------------------------------------------------------------
	// channelProperties
	//--------------------------------------------------------------------------------
	class CInkChannelProperties : public WritingElementWithChilds<CInkChannelProperty>
	{
	public:
		WritingElement_AdditionMethods(CInkChannelProperties)
		CInkChannelProperties() {}
		virtual ~CInkChannelProperties() {}

		virtual void fromXML(XmlUtils::CXmlNode& node) {}
		virtual std::wstring toXML() const { return L""; }

		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
		virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

		virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
		virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
		virtual EElementType getType() const;
	};
	//--------------------------------------------------------------------------------
	// sampleRate
	//--------------------------------------------------------------------------------
	class CInkSampleRate : public WritingElement
	{
	public:
		WritingElement_AdditionMethods(CInkSampleRate)
		CInkSampleRate() {}
		virtual ~CInkSampleRate() {}

		virtual void fromXML(XmlUtils::CXmlNode& node) {}
		virtual std::wstring toXML() const { return L""; }

		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
		virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

		void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
		virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
		virtual EElementType getType() const;

		nullable_bool m_bUniform;
		nullable_double m_dValue;
	};
	//--------------------------------------------------------------------------------
	// latency
	//--------------------------------------------------------------------------------
	class CInkLatency : public WritingElement
	{
	public:
		WritingElement_AdditionMethods(CInkLatency)
		CInkLatency() {}
		virtual ~CInkLatency() {}

		virtual void fromXML(XmlUtils::CXmlNode& node) {}
		virtual std::wstring toXML() const { return L""; }

		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
		virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

		void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
		virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
		virtual EElementType getType() const;

		nullable_double m_dValue;
	};
	//--------------------------------------------------------------------------------
	// activeArea
	//--------------------------------------------------------------------------------
	class CInkActiveArea : public WritingElement
	{
	public:
		WritingElement_AdditionMethods(CInkActiveArea)
		CInkActiveArea() {}
		virtual ~CInkActiveArea() {}

		virtual void fromXML(XmlUtils::CXmlNode& node) {}
		virtual std::wstring toXML() const { return L""; }

		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
		virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

		void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
		virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
		virtual EElementType getType() const;

		nullable_string m_sSize;
		nullable_double m_dHeight;
		nullable_double m_dWidth;
		nullable_string m_sUnits;
	};
	//--------------------------------------------------------------------------------
	// srcProperty
	//--------------------------------------------------------------------------------
	class CInkSrcProperty : public WritingElement
	{
	public:
		WritingElement_AdditionMethods(CInkSrcProperty)
		CInkSrcProperty() {}
		virtual ~CInkSrcProperty() {}

		virtual void fromXML(XmlUtils::CXmlNode& node) {}
		virtual std::wstring toXML() const { return L""; }

		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
		virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

		void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
		virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
		virtual EElementType getType() const;

		nullable_string m_sName;
		nullable_string m_sValue;
		nullable_string m_sUnits;
	};
	//--------------------------------------------------------------------------------
	// inkSource (traceFormat, sampleRate?, latency?, activeArea?, srcProperty*, channelProperties?)
	//--------------------------------------------------------------------------------
	class CInkSource : public WritingElementWithChilds<CInkSrcProperty>
	{
	public:
		WritingElement_AdditionMethods(CInkSource)
		CInkSource() {}
		virtual ~CInkSource() {}

		virtual void fromXML(XmlUtils::CXmlNode& node) {}
		virtual std::wstring toXML() const { return L""; }

		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
		virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

		void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
		virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
		virtual EElementType getType() const;

		nullable_string m_sId;
		nullable_string m_sManufacturer;
		nullable_string m_sModel;
		nullable_string m_sSerialNo;
		nullable_string m_sSpecificationRef;
		nullable_string m_sDescription;

		nullable<CInkTraceFormat> m_oTraceFormat;
		nullable<CInkSampleRate> m_oSampleRate;
		nullable<CInkLatency> m_oLatency;
		nullable<CInkActiveArea> m_oActiveArea;
		nullable<CInkChannelProperties> m_oChannelProperties;
	};
	//--------------------------------------------------------------------------------
	// brushProperty (annotation | annotationXML)*
	//--------------------------------------------------------------------------------
	class CInkBrushProperty : public WritingElementWithChilds<>
	{
	public:
		WritingElement_AdditionMethods(CInkBrushProperty)
		CInkBrushProperty() {}
		virtual ~CInkBrushProperty() {}

		virtual void fromXML(XmlUtils::CXmlNode& node) {}
		virtual std::wstring toXML() const { return L""; }

		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
		virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

		void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
		virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
		virtual EElementType getType() const;

		nullable_string m_sName;
		nullable_string m_sValue;
		nullable_string m_sUnits;
	};
	//--------------------------------------------------------------------------------
	// brush (annotation | annotationXML | brushProperty)*
	//--------------------------------------------------------------------------------
	class CInkBrush : public WritingElementWithChilds<>
	{
	public:
		WritingElement_AdditionMethods(CInkBrush)
		CInkBrush() {}
		virtual ~CInkBrush() {}

		virtual void fromXML(XmlUtils::CXmlNode& node) {}
		virtual std::wstring toXML() const { return L""; }

		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
		virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

		void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
		virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
		virtual EElementType getType() const;

		nullable_string m_sId;
		nullable_string m_sBrushRef;
	};
	//--------------------------------------------------------------------------------
	// timestamp
	//--------------------------------------------------------------------------------
	class CInkTimestamp : public WritingElement
	{
	public:
		WritingElement_AdditionMethods(CInkTimestamp)
		CInkTimestamp() {}
		virtual ~CInkTimestamp() {}

		virtual void fromXML(XmlUtils::CXmlNode& node) {}
		virtual std::wstring toXML() const { return L""; }

		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
		virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

		void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
		virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
		virtual EElementType getType() const;

		nullable_string m_sId;
		nullable_double m_dTime;
		nullable_string m_sTimestampRef;
		nullable_string m_sTimeString;
		nullable_double m_dTimeOffset;
	};
	//--------------------------------------------------------------------------------
	// canvas (traceFormat?)
	//--------------------------------------------------------------------------------
	class CInkCanvas : public WritingElement
	{
	public:
		WritingElement_AdditionMethods(CInkCanvas)
		CInkCanvas() {}
		virtual ~CInkCanvas() {}

		virtual void fromXML(XmlUtils::CXmlNode& node) {}
		virtual std::wstring toXML() const { return L""; }

		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
		virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

		void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
		virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
		virtual EElementType getType() const;

		nullable_string m_sId;
		nullable_string m_sTraceFormatRef;

		nullable<CInkTraceFormat> m_oTraceFormat;
	};
	//--------------------------------------------------------------------------------
	// canvasTransform (mapping, mapping?)
	//--------------------------------------------------------------------------------
	class CInkCanvasTransform : public WritingElementWithChilds<CInkMapping>
	{
	public:
		WritingElement_AdditionMethods(CInkCanvasTransform)
		CInkCanvasTransform() {}
		virtual ~CInkCanvasTransform() {}

		virtual void fromXML(XmlUtils::CXmlNode& node) {}
		virtual std::wstring toXML() const { return L""; }

		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
		virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

		void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
		virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
		virtual EElementType getType() const;

		nullable_string m_sId;
		nullable_bool m_bInvertible;
	};
	//--------------------------------------------------------------------------------
	// context (canvas?, canvasTransform?, traceFormat?, inkSource?, brush?, timestamp?)
	//--------------------------------------------------------------------------------
	class CInkContext : public WritingElement
	{
	public:
		WritingElement_AdditionMethods(CInkContext)
		CInkContext() {}
		virtual ~CInkContext() {}

		virtual void fromXML(XmlUtils::CXmlNode& node) {}
		virtual std::wstring toXML() const { return L""; }

		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
		virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

		void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
		virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
		virtual EElementType getType() const;

		nullable_string m_sId;
		nullable_string m_sContextRef;
		nullable_string m_sCanvasRef;
		nullable_string m_sCanvasTransformRef;
		nullable_string m_sTraceFormatRef;
		nullable_string m_sInkSourceRef;
		nullable_string m_sBrushRef;
		nullable_string m_sTimestampRef;

		nullable<CInkCanvas> m_oCanvas;
		nullable<CInkCanvasTransform> m_oCanvasTransform;
		nullable<CInkTraceFormat> m_oTraceFormat;
		nullable<CInkSource> m_oInkSource;
		nullable<CInkBrush> m_oBrush;
		nullable<CInkTimestamp> m_oTimestamp;
	};
	//--------------------------------------------------------------------------------
	// trace
	//--------------------------------------------------------------------------------
	class CInkTrace : public WritingElement
	{
	public:
		WritingElement_AdditionMethods(CInkTrace)
		CInkTrace() {}
		virtual ~CInkTrace() {}

		virtual void fromXML(XmlUtils::CXmlNode& node) {}
		virtual std::wstring toXML() const { return L""; }

		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
		virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

		void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
		virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
		virtual EElementType getType() const;

		nullable_string m_sId;
		nullable<SimpleTypes::Ink::CTraceType> m_oType;
		nullable<SimpleTypes::Ink::CTraceContinuation> m_oContinuation;
		nullable_string m_sPriorRef;
		nullable_string m_sContextRef;
		nullable_string m_sBrushRef;
		nullable_double m_dDuration;
		nullable_double m_dTimeOffset;

		std::wstring m_sData; // points as is
	};
	//--------------------------------------------------------------------------------
	// traceGroup (annotation | annotationXML | trace | traceGroup)*
	//--------------------------------------------------------------------------------
	class CInkTraceGroup : public WritingElementWithChilds<>
	{
	public:
		WritingElement_AdditionMethods(CInkTraceGroup)
		CInkTraceGroup() {}
		virtual ~CInkTraceGroup() {}

		virtual void fromXML(XmlUtils::CXmlNode& node) {}
		virtual std::wstring toXML() const { return L""; }

		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
		virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

		void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
		virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
		virtual EElementType getType() const;

		nullable_string m_sId;
		nullable_string m_sContextRef;
		nullable_string m_sBrushRef;
	};
	//--------------------------------------------------------------------------------
	// traceView (annotation | annotationXML | traceView)*
	//--------------------------------------------------------------------------------
	class CInkTraceView : public WritingElementWithChilds<>
	{
	public:
		WritingElement_AdditionMethods(CInkTraceView)
		CInkTraceView() {}
		virtual ~CInkTraceView() {}

		virtual void fromXML(XmlUtils::CXmlNode& node) {}
		virtual std::wstring toXML() const { return L""; }

		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
		virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

		void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
		virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
		virtual EElementType getType() const;

		nullable_string m_sId;
		nullable_string m_sContextRef;
		nullable_string m_sTraceDataRef;
		nullable_string m_sFrom;
		nullable_string m_sTo;
	};
	//--------------------------------------------------------------------------------
	// definitions (brush | canvas | canvasTransform | context | inkSource | mapping |
	//					timestamp | trace | traceFormat | traceGroup | traceView)*
	//--------------------------------------------------------------------------------
	class CInkDefinitions : public WritingElementWithChilds<>
	{
	public:
		WritingElement_AdditionMethods(CInkDefinitions)
		CInkDefinitions() {}
		virtual ~CInkDefinitions() {}

		virtual void fromXML(XmlUtils::CXmlNode& node) {}
		virtual std::wstring toXML() const { return L""; }

		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
		virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

		virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
		virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
		virtual EElementType getType() const;
	};
} // namespace OOX
