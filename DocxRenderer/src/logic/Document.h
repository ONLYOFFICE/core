﻿#pragma once
#include "../../../DesktopEditor/common/Directory.h"

#include "Page.h"
#include "managers/ImageManager.h"
#include "managers/FontStyleManager.h"
#include "managers/ParagraphStyleManager.h"


namespace NSDocxRenderer
{
	class CDocument
	{
	public:
		Aggplus::CGraphicsPathSimpleConverter m_oSimpleGraphicsConverter;

		NSFonts::IApplicationFonts* m_pAppFonts       {nullptr};
		NSFonts::IFontManager*      m_pBaseFontManager{nullptr};
		NSStructures::CFont	        m_oInstalledFont;

		CImageManager          m_oImageManager;
		CFontStyleManager      m_oFontStyleManager;
		CParagraphStyleManager m_oParagraphStyleManager;
		CFontManager           m_oFontManager;
		CFontSelector          m_oFontSelector;

		CPage m_oCurrentPage;
		LONG  m_lCurrentCommandType {0};

		double m_dWidth  {0.0};
		double m_dHeight {0.0};
		double m_dDpiX   {72.0};
		double m_dDpiY   {72.0};

		std::wstring m_strTempDirectory {L""};
		std::wstring m_strDstFilePath;

		LONG m_lPageNum {0};
		LONG m_lNumberPages{0};

		bool m_bIsDisablePageCommand {false}; // disable commands inside draw function

		std::map<LONG, NSStringUtils::CStringBuilder*> m_mapXmlString;

	public:
		CDocument(IRenderer* pRenderer, NSFonts::IApplicationFonts* pFonts);
		~CDocument();

	public:
		HRESULT NewPage();
		HRESULT get_Height(double* dHeight);
		HRESULT put_Height(double dHeight);
		HRESULT get_Width(double* dWidth);
		HRESULT put_Width(double dWidth);
		HRESULT get_DpiX(double* dDpiX);
		HRESULT get_DpiY(double* dDpiY);

		HRESULT get_PenColor(LONG* lColor);
		HRESULT put_PenColor(LONG lColor);
		HRESULT get_PenAlpha(LONG* lAlpha);
		HRESULT put_PenAlpha(LONG lAlpha);
		HRESULT get_PenSize(double* dSize);
		HRESULT put_PenSize(double dSize);
		HRESULT get_PenDashStyle(BYTE* val);
		HRESULT put_PenDashStyle(BYTE val);
		HRESULT get_PenLineStartCap(BYTE* val);
		HRESULT put_PenLineStartCap(BYTE val);
		HRESULT get_PenLineEndCap(BYTE* val);
		HRESULT put_PenLineEndCap(BYTE val);
		HRESULT get_PenLineJoin(BYTE* val);
		HRESULT put_PenLineJoin(BYTE val);
		HRESULT get_PenDashOffset(double* val);
		HRESULT put_PenDashOffset(double val);
		HRESULT get_PenAlign(LONG* val);
		HRESULT put_PenAlign(LONG val);
		HRESULT get_PenMiterLimit(double* val);
		HRESULT put_PenMiterLimit(double val);
		HRESULT PenDashPattern(double* pPattern, LONG lCount);

		HRESULT get_BrushType(LONG* lType);
		HRESULT put_BrushType(LONG lType);
		HRESULT get_BrushColor1(LONG* lColor);
		HRESULT put_BrushColor1(LONG lColor);
		HRESULT get_BrushAlpha1(LONG* lAlpha);
		HRESULT put_BrushAlpha1(LONG lAlpha);
		HRESULT get_BrushColor2(LONG* lColor);
		HRESULT put_BrushColor2(LONG lColor);
		HRESULT get_BrushAlpha2(LONG* lAlpha);
		HRESULT put_BrushAlpha2(LONG lAlpha);
		HRESULT get_BrushTexturePath(std::wstring* sPath);
		HRESULT put_BrushTexturePath(const std::wstring& sPath);
		HRESULT get_BrushTextureMode(LONG* lMode);
		HRESULT put_BrushTextureMode(LONG lMode);
		HRESULT get_BrushTextureAlpha(LONG* lTxAlpha);
		HRESULT put_BrushTextureAlpha(LONG lTxAlpha);
		HRESULT get_BrushLinearAngle(double* dAngle);
		HRESULT put_BrushLinearAngle(double dAngle);
		HRESULT BrushRect(bool val, double left, double top, double width, double height);
		HRESULT BrushBounds(const double& dLeft, const double& dTop, const double& dWidth, const double& dHeight);
		HRESULT put_BrushGradientColors(LONG* pColors, double* pPositions, LONG lCount);
		HRESULT put_BrushGradInfo(void* pGradInfo);

		HRESULT get_FontName(std::wstring* sName);
		HRESULT put_FontName(std::wstring sName);
		HRESULT get_FontPath(std::wstring* sPath);
		HRESULT put_FontPath(std::wstring sPath);
		HRESULT get_FontSize(double* dSize);
		HRESULT put_FontSize(double dSize);
		HRESULT get_FontStyle(LONG* lStyle);
		HRESULT put_FontStyle(LONG lStyle);
		HRESULT get_FontStringGID(INT* bGID);
		HRESULT put_FontStringGID(INT bGID);
		HRESULT get_FontCharSpace(double* dSpace);
		HRESULT put_FontCharSpace(double dSpace);
		HRESULT get_FontFaceIndex(int* lFaceIndex);
		HRESULT put_FontFaceIndex(const int& lFaceIndex);

		HRESULT get_ShadowDistanceX(double* val);
		HRESULT put_ShadowDistanceX(double val);
		HRESULT get_ShadowDistanceY(double* val);
		HRESULT put_ShadowDistanceY(double val);
		HRESULT get_ShadowBlurSize(double* val);
		HRESULT put_ShadowBlurSize(double val);
		HRESULT get_ShadowColor(LONG* val);
		HRESULT put_ShadowColor(LONG val);
		HRESULT get_ShadowAlpha(LONG* val);
		HRESULT put_ShadowAlpha(LONG val);
		HRESULT get_ShadowVisible(INT* val);
		HRESULT put_ShadowVisible(INT val);

		HRESULT get_EdgeVisible(LONG* val);
		HRESULT put_EdgeVisible(LONG val);
		HRESULT get_EdgeColor(LONG* val);
		HRESULT put_EdgeColor(LONG val);
		HRESULT get_EdgeAlpha(LONG* val);
		HRESULT put_EdgeAlpha(LONG val);
		HRESULT get_EdgeDist(double* val);
		HRESULT put_EdgeDist(double val);

		HRESULT CommandDrawTextPrivate(const int* pUnicodes, const int* pGids, int nCount, const double& dX, const double& dY, const double& dW, const double& dH, const double& dBaseLineOffset = 0);
		HRESULT CommandDrawTextCHAR(const int& lUnicode, const double& dX, const double& dY, const double& dW, const double& dH);
		HRESULT CommandDrawTextExCHAR(const int& lUnicode, const int& lGid, const double& dX, const double& dY, const double& dW, const double& dH);
		virtual HRESULT CommandDrawText(const std::wstring& wsUnicodeText, const double& dX, const double& dY, const double& dW, const double& dH);
		virtual HRESULT CommandDrawTextEx(const std::wstring& wsUnicodeText, const unsigned int* pGids, const unsigned int nGidsCount, const double& dX, const double& dY, const double& dW, const double& dH);

		HRESULT BeginCommand(DWORD lType);
		HRESULT EndCommand(DWORD lType);

		HRESULT PathCommandMoveTo(double fX, double fY);
		HRESULT PathCommandLineTo(double fX, double fY);
		HRESULT PathCommandLinesTo(double* pPoints, LONG lCount);
		HRESULT PathCommandCurveTo(double fX1, double fY1, double fX2, double fY2, double fX3, double fY3);
		HRESULT PathCommandCurvesTo(double* pPoints, LONG lCount);
		HRESULT PathCommandArcTo(double fX, double fY, double fWidth, double fHeight, double fStartAngle, double fSweepAngle);
		HRESULT PathCommandClose();
		HRESULT PathCommandEnd();
		HRESULT DrawPath(long nType);
		HRESULT PathCommandStart();
		HRESULT PathCommandGetCurrentPoint(double* fX, double* fY);

		HRESULT PathCommandTextCHAR(const int& lUnicode, const double& dX, const double& dY, const double& dW, const double& dH);
		HRESULT PathCommandTextExCHAR(const int& lUnicode, const int& lGid, const double& dX, const double& dY, const double& dW, const double& dH);
		HRESULT PathCommandText(const std::wstring& wsUnicodeText, const double& dX, const double& dY, const double& dW, const double& dH);
		HRESULT PathCommandTextEx(const std::wstring& wsUnicodeText, const unsigned int* pGids, const unsigned int nGidsCount, const double& dX, const double& dY, const double& dW, const double& dH);

		HRESULT GetCommandParams(double* dAngle, double* dLeft, double* dTop, double* dWidth, double* dHeight, DWORD* lFlags);
		HRESULT SetCommandParams(double dAngle, double dLeft, double dTop, double dWidth, double dHeight, DWORD lFlags);

		HRESULT DrawImage(IGrObject* pImage, double fX, double fY, double fWidth, double fHeight);
		HRESULT DrawImageFromFile(const std::wstring& sVal, double fX, double fY, double fWidth, double fHeight);

		HRESULT SetTransform(double dA, double dB, double dC, double dD, double dE, double dF);
		HRESULT GetTransform(double *pdA, double *pdB, double *pdC, double *pdD, double *pdE, double *pdF);
		HRESULT ResetTransform(void);
		HRESULT get_ClipMode(LONG* plMode);
		HRESULT put_ClipMode(LONG lMode);

	protected:
		void ApplyTransform(double d1, double d2, double d3, double d4, double d5, double d6);
		void ApplyTransform2(double dAngle, double dLeft, double dTop, double dWidth, double dHeight, DWORD lFlags);
		void _SetFont();
	public:

		void Init(bool bIsClearStreams = true);
		void Clear();

#ifndef DISABLE_FULL_DOCUMENT_CREATION
		void BuildDocumentXml();
		void BuildDocumentXmlRels();
		void BuildFontTableXml();
		void BuildStylesXml();

		void CreateTemplates();
		void Write();
#endif
	};
}
