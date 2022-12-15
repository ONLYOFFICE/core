#ifndef CINTERPRETATORSVGBASE_H
#define CINTERPRETATORSVGBASE_H

#include "../../xml/include/xmlwriter.h"
#include "../../MetaFile.h"

namespace MetaFile
{
	class CInterpretatorSvgBase : public IOutputDevice
	{
		public:
			CInterpretatorSvgBase(IMetaFileBase* pParser, double dWidth, double dHeight);
			virtual ~CInterpretatorSvgBase();

			void SetSize(double dWidth, double dHeight);
			void GetSize(double& dWidth, double& dHeight);
			void UpdateSize();

			void SetXmlWriter(XmlUtils::CXmlWriter* pXmlWriter);
			XmlUtils::CXmlWriter* GetXmlWriter();

			std::wstring GetFile();
			void IncludeSvg(const std::wstring& wsSvg, const TRectD& oRect, const TRectD& oClipRect, TXForm *pTransform);
		private:
			void WriteNode(const std::wstring& wsNodeName, const NodeAttributes& arAttributes, const std::wstring& wsValueNode = L"");
			void WriteNodeBegin(const std::wstring& wsNodeName, const NodeAttributes& arAttributes);
			void WriteNodeEnd(const std::wstring& wsNodeName);
			void WriteText(const std::wstring& wsText, const TPointD& oCoord, const TRect& oBounds = TRect(), const TPointD& oScale = TPointD(1, 1));

			void AddStroke(NodeAttributes &arAttributes) const;
			void AddFill(NodeAttributes &arAttributes, double dWidth = 0, double dHeight = 0);
			void AddTransform(NodeAttributes &arAttributes, TXForm* pTransform = NULL) const;
			void AddClip(NodeAttributes &arAttributes);

			void AddNoneFill(NodeAttributes &arAttributes) const;

			TPointD GetCutPos() const;

			std::wstring CreatePath(const IPath* pPath = NULL, const TXForm* pTransform = NULL);
			std::wstring CreateHatchStyle(unsigned int unHatchStyle, double dWidth, double dHeight);
			std::wstring CreateDibPatternStyle(IBrush *pBrush);
			std::wstring CreatePatternStyle(IBrush *pBrush);
			std::wstring CreateGradient(IBrush *pBrush);

		private:
			TSvgViewport         m_oViewport;
			TPointD              m_oSizeWindow;
			TPointD              m_oScale;

			std::wstring         m_wsLastClipId;

			unsigned int         m_unNumberDefs;
			std::wstring         m_wsDefs;

			IMetaFileBase       *m_pParser;
			XmlUtils::CXmlWriter m_oXmlWriter;

			friend class CEmfInterpretatorSvg;
			friend class CWmfInterpretatorSvg;
	};
}

#endif // CINTERPRETATORSVGBASE_H
