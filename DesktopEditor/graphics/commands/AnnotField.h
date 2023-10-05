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

		};

		class CTextWidgetPr
		{

		};

		class CChoiceWidgetPr
		{

		};

		class CSignatureWidgetPr
		{

		};

	private:
		class CActionWidget
		{

		};

	public:
		void SetQ(const BYTE& nQ) { m_nQ = nQ; }
		void SetH(const BYTE& nH) { m_nH = nH; }
		void SetR(const int& nR)  { m_nR = nR; }
		void SetTU(const std::wstring& wsTU) { m_wsTU = wsTU; }
		void SetDS(const std::wstring& wsDS) { m_wsDS = wsDS; }
		void SetDV(const std::wstring& wsDV) { m_wsDV = wsDV; }
		void SetT (const std::wstring& wsT)  { m_wsT = wsT; }
		void SetTC(const std::vector<double>& arrTC) { m_arrTC = arrTC; }
		void SetBC(const std::vector<double>& arrBC) { m_arrBC = arrBC; }
		void SetBG(const std::vector<double>& arrBG) { m_arrBG = arrBG; }

		BYTE GetQ() const { return m_nQ; }
		BYTE GetH() const { return m_nH; }
		int  GetR() const { return m_nR; }
		const std::wstring& GetTU() const { return m_wsTU; }
		const std::wstring& GetDS() const { return m_wsDS; }
		const std::wstring& GetDV() const { return m_wsDV; }
		const std::wstring& GetT()  const { return m_wsT; }
		const std::vector<double>& GetTC() const { return m_arrTC; }
		const std::vector<double>& GetBC() const { return m_arrBC; }
		const std::vector<double>& GetBG() const { return m_arrBG; }

		CButtonWidgetPr*    GetButtonWidgetPr()    { return m_pButtonPr; }
		CTextWidgetPr*      GetTextWidgetPr()      { return m_pTextPr; }
		CChoiceWidgetPr*    GetChoiceWidgetPr()    { return m_pChoicePr; }
		CSignatureWidgetPr* GetSignatureWidgetPr() { return m_pSignaturePr; }

	private:
		BYTE m_nQ;
		BYTE m_nH;
		int m_nR;
		std::wstring m_wsTU;
		std::wstring m_wsDS;
		std::wstring m_wsDV;
		std::wstring m_wsT;
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
		void SetRT(const BYTE& nRT)              { m_nRT      = nRT; }
		void SetFlag(const int& nFlag)           { m_nFlag    = nFlag; }
		void SetPopupID(const int& nPopupID)     { m_nPopupID = nPopupID; }
		void SetIRTID(const int& nIRTID)         { m_nIRTID   = nIRTID; }
		void SetCA(const double& dCA)            { m_dCA      = dCA; }
		void SetT(const std::wstring& wsT)       { m_wsT      = wsT; }
		void SetRC(const std::wstring& wsRC)     { m_wsRC     = wsRC; }
		void SetCD(const std::wstring& wsCD)     { m_wsCD     = wsCD; }
		void SetSubj(const std::wstring& wsSubj) { m_wsSubj   = wsSubj; }

		BYTE   GetRT()      const { return m_nRT; }
		int    GetFlag()    const { return m_nFlag; }
		int    GetPopupID() const { return m_nPopupID; }
		int    GetIRTID()   const { return m_nIRTID; }
		double GetCA()      const { return m_dCA; }
		const std::wstring& GetT()    const { return m_wsT; }
		const std::wstring& GetRC()   const { return m_wsRC; }
		const std::wstring& GetCD()   const { return m_wsCD; }
		const std::wstring& GetSubj() const { return m_wsSubj; }

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

		void SetOpen(bool bOpen)             { m_bOpen       = bOpen; }
		void SetName(BYTE nName)             { m_nName       = nName; }
		void SetState(BYTE nState)           { m_nState      = nState; }
		void SetStateModel(BYTE nStateModel) { m_nStateModel = nStateModel; }

		bool IsOpen()        const { return m_bOpen; }
		BYTE GetName()       const { return m_nName; }
		BYTE GetState()      const { return m_nState; }
		BYTE GetStateModel() const { return m_nStateModel; }

	private:
		bool m_bOpen;
		BYTE m_nName;
		BYTE m_nState;
		BYTE m_nStateModel;
	};

	class CInkAnnotPr
	{
	public:
		void SetInkList(const std::vector< std::vector<double> >& arrInkList) { m_arrInkList = arrInkList; }

		const std::vector< std::vector<double> >& GetInkList() const { return m_arrInkList; }

	private:
		std::vector< std::vector<double> > m_arrInkList;
	};

	class CLineAnnotPr
	{
	public:
		void SetCap(bool bCap)          { m_bCap = bCap; }
		void SetIT(const BYTE& nIT)     { m_nIT  = nIT; }
		void SetCP(const BYTE& nCP)     { m_nCP  = nCP; }
		void SetLL(const double& dLL)   { m_dLL  = dLL; }
		void SetLLE(const double& dLLE) { m_dLLE = dLLE; }
		void SetLLO(const double& dLLO) { m_dLLO = dLLO; }
		void SetLE(const BYTE& nLE1, const BYTE& nLE2) { m_nLE[0] = nLE1; m_nLE[1] = nLE2; }
		void SetL(const double& dL1, const double& dL2, const double& dL3, const double& dL4)
		{ m_dL[0] = dL1; m_dL[1] = dL2; m_dL[2] = dL3; m_dL[3] = dL4; }
		void SetCO(const double& dCO1, const double& dCO2) { m_dCO[0] = dCO1; m_dCO[1] = dCO2; }
		void SetIC(const std::vector<double>& arrIC)       { m_arrIC = arrIC; }

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
		void SetSubtype(const BYTE& nSubtype) { m_nSubtype = nSubtype; }
		void SetQuadPoints(const std::vector<double>& arrQuadPoints) { m_arrQuadPoints = arrQuadPoints; }

		BYTE GetSubtype() const { return m_nSubtype; }
		const std::vector<double>& GetQuadPoints() const { return m_arrQuadPoints; }

	private:
		BYTE m_nSubtype;
		std::vector<double> m_arrQuadPoints;
	};

	class CSquareCircleAnnotPr
	{
	public:
		void SetSubtype(const BYTE& nSubtype) { m_nSubtype = nSubtype; }
		void SetRD(const double& dRD1, const double& dRD2, const double& dRD3, const double& dRD4)
		{ m_dRD[0] = dRD1; m_dRD[1] = dRD2; m_dRD[2] = dRD3; m_dRD[3] = dRD4; }
		void SetIC(const std::vector<double>& arrIC) { m_arrIC = arrIC; }

		BYTE GetSubtype() const { return m_nSubtype; }
		void GetRD(double& dRD1, double& dRD2, double& dRD3, double& dRD4) const { dRD1 = m_dRD[0]; dRD2 = m_dRD[1]; dRD3 = m_dRD[2]; dRD4 = m_dRD[3]; }
		const std::vector<double>& GetIC() const { return m_arrIC; }

	private:
		BYTE m_nSubtype;
		double m_dRD[4];
		std::vector<double> m_arrIC;
	};

	class CPolygonLineAnnotPr
	{
	public:
		void SetIT(const BYTE& nIT)           { m_nIT      = nIT; }
		void SetSubtype(const BYTE& nSubtype) { m_nSubtype = nSubtype; }
		void SetLE(const BYTE& nLE1, const BYTE& nLE2) { m_nLE[0] = nLE1; m_nLE[1] = nLE2; }
		void SetIC(const std::vector<double>& arrIC)   { m_arrIC  = arrIC; }
		void SetVertices(const std::vector<double>& arrVertices) { m_arrVertices = arrVertices; }

		BYTE GetIT()      const { return m_nIT; }
		BYTE GetSubtype() const { return m_nSubtype; }
		void GetLE(BYTE& nLE1, BYTE& nLE2)       const { nLE1 = m_nLE[0]; nLE2 = m_nLE[1]; }
		const std::vector<double>& GetIC()       const { return m_arrIC; }
		const std::vector<double>& GetVertices() const { return m_arrVertices; }

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
		void SetOpen(bool bOpen)               { m_bOpen     = bOpen; }
		void SetFlag(const int& nFlag)         { m_nFlag     = nFlag; }
		void SetParentID(const int& nParentID) { m_nParentID = nParentID; }

		bool IsOpen()      const { return m_bOpen; }
		int  GetFlag()     const { return m_nFlag; }
		int  GetParentID() const { return m_nParentID; }

	private:
		bool m_bOpen;
		int  m_nFlag;
		int  m_nParentID;
	};

	class CFreeTextAnnotPr
	{
	public:
		void SetQ(const BYTE& nQ)   { m_nQ  = nQ; }
		void SetIT(const BYTE& nIT) { m_nIT = nIT; }
		void SetLE(const BYTE& nLE) { m_nLE = nLE; }
		void SetDS(const std::wstring& wsDS) { m_wsDS = wsDS; }
		void SetRD(const double& dRD1, const double& dRD2, const double& dRD3, const double& dRD4)
		{ m_dRD[0] = dRD1; m_dRD[1] = dRD2; m_dRD[2] = dRD3; m_dRD[3] = dRD4; }
		void SetCL(const std::vector<double>& arrCL) { m_arrCL = arrCL; }

		BYTE GetQ()  const { return m_nQ; }
		BYTE GetIT() const { return m_nIT; }
		BYTE GetLE() const { return m_nLE; }
		const std::wstring& GetDS() const { return m_wsDS; }
		void GetRD(double& dRD1, double& dRD2, double& dRD3, double& dRD4) const { dRD1 = m_dRD[0]; dRD2 = m_dRD[1]; dRD3 = m_dRD[2]; dRD4 = m_dRD[3]; }
		const std::vector<double>& GetCL() const { return m_arrCL; }

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
		void SetSy(const BYTE& nSy) { m_nSy  = nSy; }
		void SetRD(const double& dRD1, const double& dRD2, const double& dRD3, const double& dRD4)
		{ m_dRD[0] = dRD1; m_dRD[1] = dRD2; m_dRD[2] = dRD3; m_dRD[3] = dRD4; }

		BYTE GetSy()  const { return m_nSy; }
		void GetRD(double& dRD1, double& dRD2, double& dRD3, double& dRD4) const { dRD1 = m_dRD[0]; dRD2 = m_dRD[1]; dRD3 = m_dRD[2]; dRD4 = m_dRD[3]; }

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

	virtual void SetType(int nType);
	bool IsValid() const;

	virtual void SetBounds(const double& dX1, const double& dY1, const double& dX2, const double& dY2);
	void SetBorder(BYTE nType, double dWidth, double dDashesAlternating, double dGaps);
	void SetFlag(const int& nFlag)           { m_nFlag      = nFlag; }
	void SetID(const int& nID)               { m_nID        = nID; }
	void SetAnnotFlag(const int& nAnnotFlag) { m_nAnnotFlag = nAnnotFlag; }
	void SetPage(const int& nPage)           { m_nPage      = nPage; }
	void SetBE(BYTE nS, const double& dI)    { m_pBE.first = nS; m_pBE.second = dI; }
	void SetNM(const std::wstring& wsNM)     { m_wsNM      = wsNM; }
	void SetLM(const std::wstring& wsLM)     { m_wsLM      = wsLM; }
	void SetContents(const std::wstring& wsContents) { m_wsContents = wsContents; }
	void SetC(const std::vector<double>& arrC)       { m_arrC       = arrC; }

	virtual void GetBounds(double& dX1, double& dY1, double& dX2, double& dY2) const;
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

	bool isWidget()       const;
	bool isMarkup()       const;
	bool IsText()         const;
	bool IsInk()          const;
	bool IsLine()         const;
	bool IsTextMarkup()   const;
	bool IsSquareCircle() const;
	bool IsPolygonLine()  const;
	bool IsPopup()        const;
	bool IsFreeText()     const;
	bool IsCaret()        const;

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
	CWidgetAnnotPr*        GetWidgetAnnotPr()        { return m_pWidgetPr; }

	bool Read(NSOnlineOfficeBinToPdf::CBufferReader* pReader, IMetafileToRenderter* pCorrector);

protected:
	int          m_nType;
	double       m_dX1;
	double       m_dY1;
	double       m_dX2;
	double       m_dY2;

private:
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

#endif // _BUILD_ANNOTFIELD_H_
