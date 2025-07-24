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
#ifndef _BUILD_ANNOTFIELD_H_
#define _BUILD_ANNOTFIELD_H_

#include "../config.h"
#include "../MetafileToRendererReader.h"
class IMetafileToRenderter;

class GRAPHICS_DECL CAnnotFieldInfo : public IAdvancedCommand
{
public:
	enum EAnnotType
	{
		Unknown = -1,
		Text = 0,
		Link = 1,
		FreeText = 2,
		Line = 3,
		Square = 4,
		Circle = 5,
		PolygonLine = 6,
		PolyLine = 7,
		Highlight = 8,
		Underline = 9,
		Squiggly = 10,
		Strikeout = 11,
		Stamp = 12,
		Caret = 13,
		Ink = 14,
		Popup = 15,
		FileAttachment = 16,
		Widget = 26,
		WidgetPushButton = 27,
		WidgetRadioButton = 28,
		WidgetCheckBox = 29,
		WidgetText = 30,
		WidgetCombobox = 31,
		WidgetListbox = 32,
		WidgetSignature = 33
	};

	class GRAPHICS_DECL CWidgetAnnotPr
	{
	public:
		class GRAPHICS_DECL CButtonWidgetPr
		{
		public:
			BYTE GetS()      const;
			BYTE GetTP()     const;
			BYTE GetSW()     const;
			BYTE GetStyle()  const;
			int  GetIFFlag() const;
			int  GetI()      const;
			int  GetRI()     const;
			int  GetIX()     const;
			void GetA(double& dA1, double& dA2);
			const std::wstring& GetV();
			const std::wstring& GetCA();
			const std::wstring& GetRC();
			const std::wstring& GetAC();
			const std::wstring& GetAP_N_Yes();

			void Read(NSOnlineOfficeBinToPdf::CBufferReader* pReader, BYTE nType, int nFlags);

		private:
			BYTE m_nS;
			BYTE m_nTP;
			BYTE m_nSW;
			BYTE m_nStyle;
			int m_nIFFlag;
			int m_nI;
			int m_nRI;
			int m_nIX;
			double m_dA1, m_dA2;
			std::wstring m_wsV;
			std::wstring m_wsCA;
			std::wstring m_wsRC;
			std::wstring m_wsAC;
			std::wstring m_wsAP_N_Yes;
		};

		class GRAPHICS_DECL CTextWidgetPr
		{
		public:
			int GetMaxLen() const;
			const std::wstring& GetV();
			const std::wstring& GetRV();
			const std::wstring& GetAPV();
			BYTE* GetRender(LONG& nLen);

			void Read(NSOnlineOfficeBinToPdf::CBufferReader* pReader, int nFlags, int nWidgetFlag);

		private:
			int m_nMaxLen;
			std::wstring m_wsV;
			std::wstring m_wsRV;
			std::wstring m_wsAPV;
			LONG         m_nRenderLen;
			BYTE*        m_pRender;
		};

		class GRAPHICS_DECL CChoiceWidgetPr
		{
		public:
			int GetTI() const;
			const std::wstring& GetV();
			const std::wstring& GetAPV();
			const std::vector<int>& GetI();
			const std::vector<std::wstring>& GetArrV();
			const std::vector< std::pair<std::wstring, std::wstring> >& GetOpt();
			BYTE* GetRender(LONG& nLen);

			void Read(NSOnlineOfficeBinToPdf::CBufferReader* pReader, int nFlags);

		private:
			int m_nTI;
			std::wstring m_wsV;
			std::wstring m_wsAPV;
			std::vector<int> m_arrI;
			std::vector<std::wstring> m_arrV;
			std::vector< std::pair<std::wstring, std::wstring> > m_arrOpt;
			LONG         m_nRenderLen;
			BYTE*        m_pRender;
		};

		class GRAPHICS_DECL CSignatureWidgetPr
		{

		};

		class GRAPHICS_DECL CActionWidget
		{
		public:
			CActionWidget();
			~CActionWidget();

			BYTE nKind;
			BYTE nFlags;
			BYTE nActionType;
			int  nInt1;
			double dD[4]{};
			std::wstring wsType;
			std::wstring wsStr1;
			std::vector<std::wstring> arrStr;
			CActionWidget* pNext;
		};

		CWidgetAnnotPr(BYTE nType);
		~CWidgetAnnotPr();

		BYTE GetQ()    const;
		BYTE GetH()    const;
		BYTE GetType() const;
		int  GetR()    const;
		int  GetFlag()      const;
		int  GetFlags()     const;
		int  GetParentID()  const;
		int  GetFontStyle() const;
		double GetFontSize()   const;
		double GetFontSizeAP() const;
		const std::wstring& GetTU();
		const std::wstring& GetDS();
		const std::wstring& GetDV();
		const std::wstring& GetT();
		const std::wstring& GetFontName();
		const std::wstring& GetFontKey();
		const std::vector<double>& GetTC();
		const std::vector<double>& GetBC();
		const std::vector<double>& GetBG();
		const std::vector<CActionWidget*>& GetActions();

		CButtonWidgetPr*    GetButtonWidgetPr();
		CTextWidgetPr*      GetTextWidgetPr();
		CChoiceWidgetPr*    GetChoiceWidgetPr();
		CSignatureWidgetPr* GetSignatureWidgetPr();

		void Read(NSOnlineOfficeBinToPdf::CBufferReader* pReader, BYTE nType);

	private:
		BYTE m_nQ;
		BYTE m_nH;
		BYTE m_nType;
		int m_nR;
		int m_nFlag;
		int m_nFlags;
		int m_nParentID;
		int m_nFontStyle;
		double m_dFS;
		double m_dFSAP;
		std::wstring m_wsTU;
		std::wstring m_wsDS;
		std::wstring m_wsDV;
		std::wstring m_wsT;
		std::wstring m_wsFN;
		std::wstring m_wsFK;
		std::vector<double> m_arrTC;
		std::vector<double> m_arrBC;
		std::vector<double> m_arrBG;
		std::vector<CActionWidget*> m_arrAction;

		CButtonWidgetPr*    m_pButtonPr;
		CTextWidgetPr*      m_pTextPr;
		CChoiceWidgetPr*    m_pChoicePr;
		CSignatureWidgetPr* m_pSignaturePr;
	};

	class GRAPHICS_DECL CMarkupAnnotPr
	{
	public:
		struct GRAPHICS_DECL CFontData
		{
			BYTE nAlignment;
			int nFontFlag;
			double dFontSise;
			double dVAlign;
			double dColor[3];
			std::wstring sFontFamily;
			std::wstring sActualFont;
			std::wstring sText;
		};

		virtual ~CMarkupAnnotPr();

		BYTE   GetRT()      const;
		int    GetFlag()    const;
		int    GetPopupID() const;
		int    GetIRTID()   const;
		double GetCA()      const;
		const std::wstring& GetT();
		const std::wstring& GetCD();
		const std::wstring& GetSubj();
		const std::vector<CFontData*>& GetRC();

		void Read(NSOnlineOfficeBinToPdf::CBufferReader* pReader, int nFlags);

	private:
		BYTE         m_nRT;
		int          m_nFlag;
		int          m_nPopupID;
		int          m_nIRTID;
		double       m_dCA;
		std::wstring m_wsT;
		std::wstring m_wsRC;
		std::wstring m_wsCD;
		std::wstring m_wsSubj;
		std::vector<CFontData*> m_arrRC;
	};

	class GRAPHICS_DECL CTextAnnotPr
	{
	public:
		CTextAnnotPr();

		bool IsOpen()        const;
		BYTE GetName()       const;
		BYTE GetState()      const;
		BYTE GetStateModel() const;

		void Read(NSOnlineOfficeBinToPdf::CBufferReader* pReader, int nFlags);

	private:
		bool m_bOpen;
		BYTE m_nName;
		BYTE m_nState;
		BYTE m_nStateModel;
	};

	class GRAPHICS_DECL CInkAnnotPr
	{
	public:
		const std::vector< std::vector<double> >& GetInkList();

		void Read(NSOnlineOfficeBinToPdf::CBufferReader* pReader);

	private:
		std::vector< std::vector<double> > m_arrInkList;
	};

	class GRAPHICS_DECL CLineAnnotPr
	{
	public:
		bool IsCap()    const;
		BYTE GetIT()    const;
		BYTE GetCP()    const;
		double GetLL()  const;
		double GetLLE() const;
		double GetLLO() const;
		void GetLE(BYTE& nLE1, BYTE& nLE2);
		void GetL(double& dL1, double& dL2, double& dL3, double& dL4);
		void GetCO(double& dCO1, double& dCO2);
		const std::vector<double>& GetIC();

		void Read(NSOnlineOfficeBinToPdf::CBufferReader* pReader, int nFlags);

	private:
		bool   m_bCap;
		BYTE   m_nIT;
		BYTE   m_nCP;
		double m_dLL;
		double m_dLLE;
		double m_dLLO;
		BYTE   m_nLE[2]{};
		double m_dL[4]{};
		double m_dCO[2]{};
		std::vector<double> m_arrIC;
	};

	class GRAPHICS_DECL CTextMarkupAnnotPr
	{
	public:
		BYTE GetSubtype() const;
		const std::vector<double>& GetQuadPoints();

		void Read(NSOnlineOfficeBinToPdf::CBufferReader* pReader, BYTE nType);

	private:
		BYTE m_nSubtype;
		std::vector<double> m_arrQuadPoints;
	};

	class GRAPHICS_DECL CSquareCircleAnnotPr
	{
	public:
		BYTE GetSubtype() const;
		void GetRD(double& dRD1, double& dRD2, double& dRD3, double& dRD4);
		const std::vector<double>& GetIC();

		void Read(NSOnlineOfficeBinToPdf::CBufferReader* pReader, BYTE nType, int nFlags);

	private:
		BYTE m_nSubtype;
		double m_dRD[4]{};
		std::vector<double> m_arrIC;
	};

	class GRAPHICS_DECL CPolygonLineAnnotPr
	{
	public:
		BYTE GetIT()      const;
		BYTE GetSubtype() const;
		void GetLE(BYTE& nLE1, BYTE& nLE2);
		const std::vector<double>& GetIC();
		const std::vector<double>& GetVertices();

		void Read(NSOnlineOfficeBinToPdf::CBufferReader* pReader, BYTE nType, int nFlags);

	private:
		BYTE m_nIT;
		BYTE m_nSubtype;
		BYTE m_nLE[2]{};
		std::vector<double> m_arrIC;
		std::vector<double> m_arrVertices;
	};

	class GRAPHICS_DECL CPopupAnnotPr
	{
	public:
		bool IsOpen()      const;
		int  GetFlag()     const;
		int  GetParentID() const;

		void Read(NSOnlineOfficeBinToPdf::CBufferReader* pReader);

	private:
		bool m_bOpen;
		int  m_nFlag;
		int  m_nParentID;
	};

	class GRAPHICS_DECL CFreeTextAnnotPr
	{
	public:
		BYTE GetQ()  const;
		BYTE GetIT() const;
		BYTE GetLE() const;
		int GetRotate();
		const std::wstring& GetDS();
		void GetRD(double& dRD1, double& dRD2, double& dRD3, double& dRD4);
		const std::vector<double>& GetCL();
		const std::vector<double>& GetIC();

		void Read(NSOnlineOfficeBinToPdf::CBufferReader* pReader, int nFlags);

	private:
		BYTE m_nQ;
		BYTE m_nIT;
		BYTE m_nLE;
		int m_nRotate;
		std::wstring m_wsDS;
		double m_dRD[4]{};
		std::vector<double> m_arrCL;
		std::vector<double> m_arrIC;
	};

	class GRAPHICS_DECL CCaretAnnotPr
	{
	public:
		BYTE GetSy() const;
		void GetRD(double& dRD1, double& dRD2, double& dRD3, double& dRD4);

		void Read(NSOnlineOfficeBinToPdf::CBufferReader* pReader, int nFlags);

	private:
		BYTE m_nSy;
		double m_dRD[4]{};
	};

	class GRAPHICS_DECL CStampAnnotPr
	{
	public:
		double GetRotate();
		const std::wstring& GetName();
		void GetInRect(double& dRD1, double& dRD2, double& dRD3, double& dRD4);

		void Read(NSOnlineOfficeBinToPdf::CBufferReader* pReader, int nFlags);

	private:
		double m_nRotate;
		std::wstring m_wsName;
		double m_dInRect[4]{};
	};

	CAnnotFieldInfo();
	virtual ~CAnnotFieldInfo();

	void CreateMarkup();
	void SetType(int nType);
	EAnnotType GetType();

	void  GetBounds(double& dX1, double& dY1, double& dX2, double& dY2);
	void  GetBorder(BYTE& nType, double& dWidth, std::vector<double>& arrDash);
	int   GetFlag()      const;
	int   GetID()        const;
	int   GetAnnotFlag() const;
	int   GetPage()      const;
	int   GetCopyAP()    const;
	void  GetBE(BYTE& nS, double& dI);
	BYTE* GetRender(LONG& nLen);
	const std::wstring& GetNM();
	const std::wstring& GetLM();
	const std::wstring& GetOUserID();
	const std::wstring& GetOMetadata();
	const std::wstring& GetContents();
	const std::vector<double>& GetC();

	bool IsWidget()          const;
	bool IsButtonWidget()    const;
	bool IsTextWidget()      const;
	bool IsChoiceWidget()    const;
	bool IsSignatureWidget() const;
	bool IsMarkup()          const;
	bool IsText()            const;
	bool IsInk()             const;
	bool IsLine()            const;
	bool IsTextMarkup()      const;
	bool IsSquareCircle()    const;
	bool IsPolygonLine()     const;
	bool IsPopup()           const;
	bool IsFreeText()        const;
	bool IsCaret()           const;
	bool IsStamp()           const;

	CMarkupAnnotPr*       GetMarkupAnnotPr();
	CTextAnnotPr*         GetTextAnnotPr();
	CInkAnnotPr*          GetInkAnnotPr();
	CLineAnnotPr*         GetLineAnnotPr();
	CTextMarkupAnnotPr*   GetTextMarkupAnnotPr();
	CSquareCircleAnnotPr* GetSquareCircleAnnotPr();
	CPolygonLineAnnotPr*  GetPolygonLineAnnotPr();
	CPopupAnnotPr*        GetPopupAnnotPr();
	CFreeTextAnnotPr*     GetFreeTextAnnotPr();
	CCaretAnnotPr*        GetCaretAnnotPr();
	CStampAnnotPr*        GetStampAnnotPr();
	CWidgetAnnotPr*       GetWidgetAnnotPr();

	bool Read(NSOnlineOfficeBinToPdf::CBufferReader* pReader, IMetafileToRenderter* pCorrector);

private:
	struct CBorder
	{
		BYTE   nType;
		double dWidth;
		std::vector<double> arrDash;
	};

	EAnnotType   m_nType;
	double       m_dX1;
	double       m_dY1;
	double       m_dX2;
	double       m_dY2;
	int          m_nFlag;
	int          m_nID;
	int          m_nAnnotFlag;
	int          m_nPage;
	int          m_nCopyAP;
	std::wstring m_wsNM;
	std::wstring m_wsLM;
	std::wstring m_wsOUserID;
	std::wstring m_wsOMetadata;
	std::wstring m_wsContents;
	std::pair<BYTE, double> m_pBE;
	std::vector<double> m_arrC;
	CBorder      m_oBorder;
	LONG         m_nRenderLen;
	BYTE*        m_pRender;

	CMarkupAnnotPr*       m_pMarkupPr;
	CTextAnnotPr*         m_pTextPr;
	CInkAnnotPr*          m_pInkPr;
	CLineAnnotPr*         m_pLinePr;
	CTextMarkupAnnotPr*   m_pTextMarkupPr;
	CSquareCircleAnnotPr* m_pSquareCirclePr;
	CPolygonLineAnnotPr*  m_pPolygonLinePr;
	CPopupAnnotPr*        m_pPopupPr;
	CFreeTextAnnotPr*     m_pFreeTextPr;
	CCaretAnnotPr*        m_pCaretPr;
	CStampAnnotPr*        m_pStampPr;
	CWidgetAnnotPr*       m_pWidgetPr;
};

class GRAPHICS_DECL CAnnotFieldDelete : public IAdvancedCommand
{
public:
	CAnnotFieldDelete();
	virtual ~CAnnotFieldDelete();

	int GetID();

	bool Read(NSOnlineOfficeBinToPdf::CBufferReader* pReader, IMetafileToRenderter* pCorrector);

private:
	int m_nID;
};

class GRAPHICS_DECL CWidgetsInfo : public IAdvancedCommand
{
public:
	struct CParent
	{
		int nID;
		int nFlags;
		int nMaxLen;
		int nParentID;
		int nFieldFlag;
		std::wstring sName;
		std::wstring sV;
		std::wstring sDV;
		std::wstring sTU;
		std::vector<int> arrI;
		std::vector<std::wstring> arrV;
		std::vector<CAnnotFieldInfo::CWidgetAnnotPr::CActionWidget*> arrAction;
		std::vector< std::pair<std::wstring, std::wstring> > arrOpt;
	};

	CWidgetsInfo();
	virtual ~CWidgetsInfo();

	const std::vector< std::pair<int, int> >& GetCO();
	const std::vector<std::wstring>& GetButtonImg();
	const std::vector<CParent*>& GetParents();

	void ChangeCO(int i, int nNum, int nGen);

	bool Read(NSOnlineOfficeBinToPdf::CBufferReader* pReader, IMetafileToRenderter* pCorrector);

private:
	std::vector< std::pair<int, int> > m_arrCO;
	std::vector<std::wstring> m_arrButtonImg;
	std::vector<CParent*> m_arrParents;
};

#endif // _BUILD_ANNOTFIELD_H_
