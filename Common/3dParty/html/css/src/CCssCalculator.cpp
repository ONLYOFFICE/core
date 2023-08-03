#include "CCssCalculator.h"
#include "CCssCalculator_Private.h"


namespace NSCSS
{
	CCssCalculator::CCssCalculator()
	{
		m_pInternal = new CCssCalculator_Private();
	}

	CCssCalculator::~CCssCalculator()
	{
		delete m_pInternal;
	}

	CCompiledStyle CCssCalculator::GetCompiledStyle(const std::vector<CNode> &arSelectors, const bool& bIsSettings, const UnitMeasure& unitMeasure) const
	{
		return m_pInternal->GetCompiledStyle(arSelectors, bIsSettings, unitMeasure);
	}

	bool CCssCalculator::GetCompiledStyle(CCompiledStyle &oStyle, const std::vector<CNode> &arSelectors, const bool &bIsSettings, const UnitMeasure &unitMeasure) const
	{
		return m_pInternal->GetCompiledStyle(oStyle, arSelectors, bIsSettings, unitMeasure);
	}

	void CCssCalculator::AddStyles(const std::string &sStyle)
	{
		m_pInternal->AddStyles(sStyle);
	}

	void CCssCalculator::AddStyles(const std::wstring &wsStyle)
	{
		m_pInternal->AddStyles(wsStyle);
	}

	void CCssCalculator::AddStylesFromFile(const std::wstring &wsFileName)
	{
		m_pInternal->AddStylesFromFile(wsFileName);
	}

	void CCssCalculator::SetUnitMeasure(const UnitMeasure& nType)
	{
		m_pInternal->SetUnitMeasure(nType);
	}

	void CCssCalculator::SetDpi(const unsigned short int& nValue)
	{
		m_pInternal->SetDpi(nValue);
	}

	void CCssCalculator::SetBodyTree(const CTree &oTree)
	{
		m_pInternal->SetBodyTree(oTree);
	}

	void CCssCalculator::SetSizeSourceWindow(const CSizeWindow &oSizeWindow)
	{
		m_pInternal->SetSizeSourceWindow(oSizeWindow);
	}

	void CCssCalculator::SetSizeDeviceWindow(const CSizeWindow &oSizeWindow)
	{
		m_pInternal->SetSizeDeviceWindow(oSizeWindow);
	}

	CSizeWindow CCssCalculator::GetSizeSourceWindow() const
	{
		return m_pInternal->GetSizeSourceWindow();
	}

	CSizeWindow CCssCalculator::GetSizeDeviceWindow() const
	{
		return m_pInternal->GetSizeDeviceWindow();
	}

	UnitMeasure CCssCalculator::GetUnitMeasure() const
	{
		return m_pInternal->GetUnitMeasure();
	}

	std::wstring CCssCalculator::GetEncoding() const
	{
		return m_pInternal->GetEncoding();
	}

	unsigned short int CCssCalculator::GetDpi() const
	{
		return m_pInternal->GetDpi();
	}

	void CCssCalculator::Clear()
	{
		m_pInternal->Clear();
	}
}
