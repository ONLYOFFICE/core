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

// void Set(const BYTE& n) { m_n = n; }
// BYTE Get() const { return m_n; }

// void Set(const int& n) { m_n = n; }
// int Get() const { return m_n; }

// void Set(const double& d) { m_d = d; }
// double Get() const { return m_d; }

// void Set(const std::wstring& ws) { m_ws = ws; }
// const std::wstring& Get() const { return m_ws; }

class GRAPHICS_DECL CAnnotFieldInfo : public IAdvancedCommand
{
public:
	class CWidgetAnnotPr
	{
	public:
		class CButtonWidgetPr
		{
		public:
			BYTE GetS() const  { return m_nS; }
			BYTE GetTP() const { return m_nTP; }
			BYTE GetSW() const { return m_nSW; }
			BYTE GetStyle()  const { return m_nStyle; }
			int  GetIFFlag() const { return m_nIFFlag; }
			void GetA(double& dA1, double& dA2) const { dA1 = m_dA1; dA2 = m_dA2; }
			const std::wstring& GetCA() const { return m_wsCA; }
			const std::wstring& GetRC() const { return m_wsRC; }
			const std::wstring& GetAC() const { return m_wsAC; }
			const std::wstring& GetAP_N_Yes() const { return m_wsAP_N_Yes; }

			void Read(NSOnlineOfficeBinToPdf::CBufferReader* pReader, BYTE nType, int nFlags);

		private:
			BYTE m_nS;
			BYTE m_nTP;
			BYTE m_nSW;
			BYTE m_nStyle;
			int m_nIFFlag;
			double m_dA1, m_dA2;
			std::wstring m_wsCA;
			std::wstring m_wsRC;
			std::wstring m_wsAC;
			std::wstring m_wsAP_N_Yes;
		};

		class CTextWidgetPr
		{
		public:
			int GetMaxLen() const { return m_nMaxLen; }
			const std::wstring& GetV()  const { return m_wsV; }
			const std::wstring& GetRV() const { return m_wsRV; }

			void Read(NSOnlineOfficeBinToPdf::CBufferReader* pReader, int nFlags, int nWidgetFlag);

		private:
			int m_nMaxLen;
			std::wstring m_wsV;
			std::wstring m_wsRV;
		};

		class CChoiceWidgetPr
		{
		public:
			int GetTI() const { return m_nTI; }
			const std::wstring& GetV() const { return m_wsV; }
			const std::vector< std::pair<std::wstring, std::wstring> >& GetOpt() const { return m_arrOpt; }

			void Read(NSOnlineOfficeBinToPdf::CBufferReader* pReader, int nFlags);

		private:
			int m_nTI;
			std::wstring m_wsV;
			std::vector< std::pair<std::wstring, std::wstring> > m_arrOpt;
		};

		class CSignatureWidgetPr
		{

		};

		class CActionWidget
		{
		public:
			CActionWidget() : pNext(NULL) {}
			~CActionWidget() { RELEASEOBJECT(pNext); }

			BYTE nKind;
			BYTE nFlags;
			BYTE nActionType;
			int  nInt1;
			double dD[4];
			std::wstring wsType;
			std::wstring wsStr1;
			std::vector<std::wstring> arrStr;
			CActionWidget* pNext;
		};

	public:
		CWidgetAnnotPr(BYTE nType);
		~CWidgetAnnotPr();

		BYTE GetQ()    const { return m_nQ; }
		BYTE GetH()    const { return m_nH; }
		BYTE GetType() const { return m_nType; }
		int  GetR()    const { return m_nR; }
		int  GetFlag()      const { return m_nFlag; }
		int  GetFlags()     const { return m_nFlags; }
		int  GetParentID()  const { return m_nParentID; }
		int  GetFontStyle() const { return m_nFontStyle; }
		double GetFontSize()   const { return m_dFS; }
		double GetFontSizeAP() const { return m_dFSAP; }
		const std::wstring& GetTU() const { return m_wsTU; }
		const std::wstring& GetDS() const { return m_wsDS; }
		const std::wstring& GetDV() const { return m_wsDV; }
		const std::wstring& GetT()  const { return m_wsT; }
		const std::wstring& GetFontName()  const { return m_wsFN; }
		const std::vector<double>& GetTC() const { return m_arrTC; }
		const std::vector<double>& GetBC() const { return m_arrBC; }
		const std::vector<double>& GetBG() const { return m_arrBG; }
		const std::vector<CActionWidget*> GetActions() const { return m_arrAction; }

		CButtonWidgetPr*    GetButtonWidgetPr()    { return m_pButtonPr; }
		CTextWidgetPr*      GetTextWidgetPr()      { return m_pTextPr; }
		CChoiceWidgetPr*    GetChoiceWidgetPr()    { return m_pChoicePr; }
		CSignatureWidgetPr* GetSignatureWidgetPr() { return m_pSignaturePr; }

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
		std::vector<double> m_arrTC;
		std::vector<double> m_arrBC;
		std::vector<double> m_arrBG;
		std::vector<CActionWidget*> m_arrAction;

		CButtonWidgetPr*    m_pButtonPr;
		CTextWidgetPr*      m_pTextPr;
		CChoiceWidgetPr*    m_pChoicePr;
		CSignatureWidgetPr* m_pSignaturePr;
	};

	class CMarkupAnnotPr
	{
	public:
		BYTE   GetRT()      const { return m_nRT; }
		int    GetFlag()    const { return m_nFlag; }
		int    GetPopupID() const { return m_nPopupID; }
		int    GetIRTID()   const { return m_nIRTID; }
		double GetCA()      const { return m_dCA; }
		const std::wstring& GetT()    const { return m_wsT; }
		const std::wstring& GetRC()   const { return m_wsRC; }
		const std::wstring& GetCD()   const { return m_wsCD; }
		const std::wstring& GetSubj() const { return m_wsSubj; }

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
	};

	class CTextAnnotPr
	{
	public:
		CTextAnnotPr() : m_bOpen(false), m_nName(2), m_nState(7), m_nStateModel(2) {}

		bool IsOpen()        const { return m_bOpen; }
		BYTE GetName()       const { return m_nName; }
		BYTE GetState()      const { return m_nState; }
		BYTE GetStateModel() const { return m_nStateModel; }

		void Read(NSOnlineOfficeBinToPdf::CBufferReader* pReader, int nFlags);

	private:
		bool m_bOpen;
		BYTE m_nName;
		BYTE m_nState;
		BYTE m_nStateModel;
	};

	class CInkAnnotPr
	{
	public:
		const std::vector< std::vector<double> >& GetInkList() const { return m_arrInkList; }

		void Read(NSOnlineOfficeBinToPdf::CBufferReader* pReader);

	private:
		std::vector< std::vector<double> > m_arrInkList;
	};

	class CLineAnnotPr
	{
	public:
		bool IsCap()    const { return m_bCap; }
		BYTE GetIT()    const { return m_nIT; }
		BYTE GetCP()    const { return m_nCP; }
		double GetLL()  const { return m_dLL; }
		double GetLLE() const { return m_dLLE; }
		double GetLLO() const { return m_dLLO; }
		void GetLE(BYTE& nLE1, BYTE& nLE2) const { nLE1 = m_nLE[0]; nLE2 = m_nLE[1]; }
		void GetL(double& dL1, double& dL2, double& dL3, double& dL4) const { dL1 = m_dL[0]; dL2 = m_dL[1]; dL3 = m_dL[2]; dL4 = m_dL[3]; }
		void GetCO(double& dCO1, double& dCO2) const { dCO1 = m_dCO[0]; dCO2 = m_dCO[1]; }
		const std::vector<double>& GetIC()     const { return m_arrIC; }

		void Read(NSOnlineOfficeBinToPdf::CBufferReader* pReader, int nFlags);

	private:
		bool   m_bCap;
		BYTE   m_nIT;
		BYTE   m_nCP;
		double m_dLL;
		double m_dLLE;
		double m_dLLO;
		BYTE   m_nLE[2];
		double m_dL[4];
		double m_dCO[2];
		std::vector<double> m_arrIC;
	};

	class CTextMarkupAnnotPr
	{
	public:
		BYTE GetSubtype() const { return m_nSubtype; }
		const std::vector<double>& GetQuadPoints() const { return m_arrQuadPoints; }

		void Read(NSOnlineOfficeBinToPdf::CBufferReader* pReader, BYTE nType);

	private:
		BYTE m_nSubtype;
		std::vector<double> m_arrQuadPoints;
	};

	class CSquareCircleAnnotPr
	{
	public:
		BYTE GetSubtype() const { return m_nSubtype; }
		void GetRD(double& dRD1, double& dRD2, double& dRD3, double& dRD4) const { dRD1 = m_dRD[0]; dRD2 = m_dRD[1]; dRD3 = m_dRD[2]; dRD4 = m_dRD[3]; }
		const std::vector<double>& GetIC() const { return m_arrIC; }

		void Read(NSOnlineOfficeBinToPdf::CBufferReader* pReader, BYTE nType, int nFlags);

	private:
		BYTE m_nSubtype;
		double m_dRD[4];
		std::vector<double> m_arrIC;
	};

	class CPolygonLineAnnotPr
	{
	public:
		BYTE GetIT()      const { return m_nIT; }
		BYTE GetSubtype() const { return m_nSubtype; }
		void GetLE(BYTE& nLE1, BYTE& nLE2)       const { nLE1 = m_nLE[0]; nLE2 = m_nLE[1]; }
		const std::vector<double>& GetIC()       const { return m_arrIC; }
		const std::vector<double>& GetVertices() const { return m_arrVertices; }

		void Read(NSOnlineOfficeBinToPdf::CBufferReader* pReader, BYTE nType, int nFlags);

	private:
		BYTE m_nIT;
		BYTE m_nSubtype;
		BYTE m_nLE[2];
		std::vector<double> m_arrIC;
		std::vector<double> m_arrVertices;
	};

	class CPopupAnnotPr
	{
	public:
		bool IsOpen()      const { return m_bOpen; }
		int  GetFlag()     const { return m_nFlag; }
		int  GetParentID() const { return m_nParentID; }

		void Read(NSOnlineOfficeBinToPdf::CBufferReader* pReader);

	private:
		bool m_bOpen;
		int  m_nFlag;
		int  m_nParentID;
	};

	class CFreeTextAnnotPr
	{
	public:
		BYTE GetQ()  const { return m_nQ; }
		BYTE GetIT() const { return m_nIT; }
		BYTE GetLE() const { return m_nLE; }
		const std::wstring& GetDS() const { return m_wsDS; }
		void GetRD(double& dRD1, double& dRD2, double& dRD3, double& dRD4) const { dRD1 = m_dRD[0]; dRD2 = m_dRD[1]; dRD3 = m_dRD[2]; dRD4 = m_dRD[3]; }
		const std::vector<double>& GetCL() const { return m_arrCL; }

		void Read(NSOnlineOfficeBinToPdf::CBufferReader* pReader, int nFlags);

	private:
		BYTE m_nQ;
		BYTE m_nIT;
		BYTE m_nLE;
		std::wstring m_wsDS;
		double m_dRD[4]{};
		std::vector<double> m_arrCL;
	};

	class CCaretAnnotPr
	{
	public:
		BYTE GetSy()  const { return m_nSy; }
		void GetRD(double& dRD1, double& dRD2, double& dRD3, double& dRD4) const { dRD1 = m_dRD[0]; dRD2 = m_dRD[1]; dRD3 = m_dRD[2]; dRD4 = m_dRD[3]; }

		void Read(NSOnlineOfficeBinToPdf::CBufferReader* pReader, int nFlags);

	private:
		BYTE m_nSy;
		double m_dRD[4]{};
	};

private:
	struct CBorder
	{
		BYTE   nType;
		double dWidth;
		double dDashesAlternating;
		double dGaps;
	};

public:
	CAnnotFieldInfo();
	virtual ~CAnnotFieldInfo();

	void SetType(int nType);
	bool IsValid() const;

	void GetBounds(double& dX1, double& dY1, double& dX2, double& dY2) const;
	void   GetBorder(BYTE& nType, double& dWidth, double& dDashesAlternating, double& dGaps);
	int    GetFlag()      const { return m_nFlag; }
	int    GetID()        const { return m_nID; }
	int    GetAnnotFlag() const { return m_nAnnotFlag; }
	int    GetPage()      const { return m_nPage; }
	void   GetBE(BYTE& nS, double& dI) { nS = m_pBE.first; dI = m_pBE.second; }
	const std::wstring& GetNM() const { return m_wsNM; }
	const std::wstring& GetLM() const { return m_wsLM; }
	const std::wstring& GetContents() const { return m_wsContents; }
	const std::vector<double>& GetC() const { return m_arrC; }

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

	CMarkupAnnotPr*       GetMarkupAnnotPr()       { return m_pMarkupPr; }
	CTextAnnotPr*         GetTextAnnotPr()         { return m_pTextPr; }
	CInkAnnotPr*          GetInkAnnotPr()          { return m_pInkPr; }
	CLineAnnotPr*         GetLineAnnotPr()         { return m_pLinePr; }
	CTextMarkupAnnotPr*   GetTextMarkupAnnotPr()   { return m_pTextMarkupPr; }
	CSquareCircleAnnotPr* GetSquareCircleAnnotPr() { return m_pSquareCirclePr; }
	CPolygonLineAnnotPr*  GetPolygonLineAnnotPr()  { return m_pPolygonLinePr; }
	CPopupAnnotPr*        GetPopupAnnotPr()        { return m_pPopupPr; }
	CFreeTextAnnotPr*     GetFreeTextAnnotPr()     { return m_pFreeTextPr; }
	CCaretAnnotPr*        GetCaretAnnotPr()        { return m_pCaretPr; }
	CWidgetAnnotPr*       GetWidgetAnnotPr()       { return m_pWidgetPr; }

	bool Read(NSOnlineOfficeBinToPdf::CBufferReader* pReader, IMetafileToRenderter* pCorrector);

private:
	int          m_nType;
	double       m_dX1;
	double       m_dY1;
	double       m_dX2;
	double       m_dY2;
	int          m_nFlag;
	int          m_nID;
	int          m_nAnnotFlag;
	int          m_nPage;
	std::wstring m_wsNM;
	std::wstring m_wsLM;
	std::wstring m_wsContents;
	std::pair<BYTE, double> m_pBE;
	std::vector<double> m_arrC;
	CBorder      m_oBorder;

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
	CWidgetAnnotPr*       m_pWidgetPr;
};

class GRAPHICS_DECL CAnnotFieldDelete : public IAdvancedCommand
{
public:
	CAnnotFieldDelete();
	virtual ~CAnnotFieldDelete() {}

	int GetID() { return m_nID; }

	bool Read(NSOnlineOfficeBinToPdf::CBufferReader* pReader, IMetafileToRenderter* pCorrector);

private:
	int m_nID;
};

class GRAPHICS_DECL CWidgetsInfo : public IAdvancedCommand
{
private:
	struct CParent
	{
		int nID;
		int nFlags;
		int nParentID;
		std::wstring sName;
		std::wstring sV;
		std::wstring sDV;
	};
public:
	CWidgetsInfo();
	virtual ~CWidgetsInfo();

	bool Read(NSOnlineOfficeBinToPdf::CBufferReader* pReader, IMetafileToRenderter* pCorrector);

private:
	std::vector<std::wstring> m_arrCO;
	std::vector<CParent*> m_arrParents;
};

#endif // _BUILD_ANNOTFIELD_H_
