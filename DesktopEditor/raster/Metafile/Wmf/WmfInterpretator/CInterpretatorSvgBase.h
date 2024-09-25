#ifndef CINTERPRETATORSVGBASE_H
#define CINTERPRETATORSVGBASE_H

#include "../../xml/include/xmlwriter.h"
#include "../../Common/MetaFileTypes.h"
#include "../../MetaFile.h"

namespace MetaFile
{
	class CHatchGenerator
	{
	public:
		CHatchGenerator();

		void SetSize(double dWidth, double dHeight);
		void SetStyleId(unsigned int unStyleId, unsigned int unNumber);
		void SetStroke(double dWidth, int nColor, unsigned char chAlpha = 255);
		void SetBackground(int nColor, unsigned char chAlpha = 255);

		bool GenerateHatch();

		std::wstring GetPatternId() const;
		std::wstring GetPattern();
	private:
		void AddLine(const TPointD& oPoint1, const TPointD& oPoint2);
		void AddLines(const std::vector<std::pair<TPointD, TPointD>>& arPoints);
		void AddPoint(const TPointD& oPoint);
		void AddPoints(const std::vector<TPointD>& arPoints);

		bool GenerateStartPattern();
		void GenerateBackground();
		void GenerateEndPattern();

		int          m_nHatchStyle;
		unsigned int m_unNumber;

		double        m_dStrokeWidth;
		int           m_nStrokeColor;
		unsigned char m_chStrokeAlpha;

		double m_dWidth;
		double m_dHeight;

		int           m_nBackgroundColor;
		unsigned char m_chBackgroundAlpha;

		NSStringUtils::CStringBuilder m_oStringBuilder;
	};

	class CSvgClip
	{
	public:
		CSvgClip();

		void Reset();

		void BeginClip();
		void CloseClip();

		bool StartedClip() const;
		bool Empty()       const;

		void AddClipValue(const std::wstring& wsId, const std::wstring& wsValue, int nClipMode = RGN_AND);

		inline std::wstring GetClip()   const;
		inline std::wstring GetClipId() const;
	private:
		struct TClipValue
		{
			std::wstring m_wsId;
			std::wstring m_wsValue;
			int          m_nClipMode;
		};

		std::vector<TClipValue> m_arValues;

		bool m_bStartClip;
	};

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
		void WriteText(const std::wstring& wsText, const TPointD& oCoord, const TRectL& oBounds = TRectL(), const TPointD& oScale = TPointD(1, 1), const std::vector<double>& arDx = {});

		void DrawBitmap(double dX, double dY, double dW, double dH, BYTE* pBuffer, unsigned int unWidth, unsigned int unHeight) override;

		void ResetClip() override;
		void IntersectClip(const TRectD& oClip) override;
		void ExcludeClip(const TRectD& oClip, const TRectD& oBB) override;
		void PathClip(const CPath& oPath, int nClipMode, TXForm* pTransform = NULL) override;
		void StartClipPath(unsigned int unMode, int nFillMode = -1) override {};
		void EndClipPath(unsigned int unMode) override {};

		void AddStroke(NodeAttributes &arAttributes) const;
		void AddFill(NodeAttributes &arAttributes, double dWidth = 0, double dHeight = 0);
		void AddTransform(NodeAttributes &arAttributes, TXForm* pTransform = NULL) const;
		void AddClip();
		bool OpenClip();
		void CloseClip();

		void AddNoneFill(NodeAttributes &arAttributes) const;

		TPointD GetCutPos() const;

		std::wstring CreatePath(const CPath& oPath, const TXForm* pTransform = NULL);
		std::wstring CreateHatchStyle(unsigned int unHatchStyle, double dWidth, double dHeight);
		std::wstring CreateDibPatternStyle(const IBrush* pBrush);
		std::wstring CreatePatternStyle(const IBrush* pBrush);
		std::wstring CreateGradient(const IBrush *pBrush);
	private:
		TSvgViewport         m_oViewport;
		TPointD              m_oSizeWindow;

		unsigned int         m_unNumberDefs;
		std::wstring         m_wsDefs;

		IMetaFileBase        *m_pParser;

		XmlUtils::CXmlWriter *m_pXmlWriter;
		bool                 m_bExternXmlWriter;

		bool                 m_bUpdatedClip;
		CSvgClip             m_oClip;

		friend class CEmfInterpretatorSvg;
		friend class CWmfInterpretatorSvg;
	};

	std::wstring CalculateColor(unsigned int unColor, BYTE uchAlpha);
	std::wstring CalculateColor(BYTE uchRed, BYTE uchGreen, BYTE uchBlue, BYTE uchAlpha);
}

#endif // CINTERPRETATORSVGBASE_H
