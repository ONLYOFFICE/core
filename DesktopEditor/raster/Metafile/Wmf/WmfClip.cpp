#include "WmfClip.h"

namespace MetaFile
{
	CWmfClip::CWmfClip()
	{
	}
	CWmfClip::~CWmfClip()
	{
		Clear();
	}
	void CWmfClip::operator=(CWmfClip& oClip)
	{
		Clear();
		for (unsigned int ulIndex = 0; ulIndex < oClip.m_vCommands.size(); ulIndex++)
		{
			CWmfClipCommandBase* pCommand = oClip.m_vCommands.at(ulIndex);
			CWmfClipCommandBase* pNewCommand = NULL;
			switch (pCommand->GetType())
			{
				case WMF_CLIPCOMMAND_INTERSECT:
				{
					CWmfClipCommandIntersect* pI = (CWmfClipCommandIntersect*)pCommand;
					pNewCommand = new CWmfClipCommandIntersect(pI->m_shL, pI->m_shT, pI->m_shR, pI->m_shB);
					break;
				}
				case WMF_CLIPCOMMAND_EXCLUDE:
				{
					CWmfClipCommandExclude* pE = (CWmfClipCommandExclude*)pCommand;
					pNewCommand = new CWmfClipCommandExclude(pE->m_shL, pE->m_shT, pE->m_shR, pE->m_shB, pE->m_oBB);
					break;
				}
			}

			if (pNewCommand)
				m_vCommands.push_back(pNewCommand);
		}
	}
	void CWmfClip::Reset()
	{
		Clear();
	}
	bool CWmfClip::Intersect(short shL, short shT, short shR, short shB)
	{
		CWmfClipCommandBase* pCommand = new CWmfClipCommandIntersect(shL, shT, shR, shB);
		if (!pCommand)
			return false;

		m_vCommands.push_back(pCommand);
		return true;
	}
	bool CWmfClip::Exclude(short shL, short shT, short shR, short shB, TRect oBB)
	{
		CWmfClipCommandBase* pCommand = new CWmfClipCommandExclude(shL, shT, shR, shB, oBB);
		if (!pCommand)
			return false;

		m_vCommands.push_back(pCommand);
		return true;
	}
	void CWmfClip::Clear()
	{
		for (unsigned int ulIndex = 0; ulIndex < m_vCommands.size(); ulIndex++)
		{
			CWmfClipCommandBase* pCommand = m_vCommands.at(ulIndex);
			delete pCommand;
		}
		m_vCommands.clear();
	}
	void CWmfClip::ClipOnRenderer(IOutputDevice* pOutput)
	{
		if (pOutput)
		{
			pOutput->ResetClip();
			for (unsigned int ulIndex = 0; ulIndex < m_vCommands.size(); ulIndex++)
			{
				CWmfClipCommandBase* pCommand = m_vCommands.at(ulIndex);

				switch (pCommand->GetType())
				{
					case WMF_CLIPCOMMAND_INTERSECT:
					{
						CWmfClipCommandIntersect* pIntersect = (CWmfClipCommandIntersect*)pCommand;

						pOutput->StartClipPath(RGN_AND);
						pOutput->MoveTo(pIntersect->m_shL, pIntersect->m_shT);
						pOutput->LineTo(pIntersect->m_shR, pIntersect->m_shT);
						pOutput->LineTo(pIntersect->m_shR, pIntersect->m_shB);
						pOutput->LineTo(pIntersect->m_shL, pIntersect->m_shB);
						pOutput->ClosePath();
						pOutput->EndClipPath(RGN_AND);

						break;
					}
					case WMF_CLIPCOMMAND_EXCLUDE:
					{
						CWmfClipCommandExclude* pExclude = (CWmfClipCommandExclude*)pCommand;

						pOutput->StartClipPath(RGN_AND);

						pOutput->MoveTo(pExclude->m_oBB.nLeft, pExclude->m_oBB.nTop);
						pOutput->LineTo(pExclude->m_oBB.nRight, pExclude->m_oBB.nTop);
						pOutput->LineTo(pExclude->m_oBB.nRight, pExclude->m_oBB.nBottom);
						pOutput->LineTo(pExclude->m_oBB.nLeft, pExclude->m_oBB.nBottom);
						pOutput->ClosePath();

						pOutput->MoveTo(pExclude->m_shL, pExclude->m_shT);
						pOutput->LineTo(pExclude->m_shR, pExclude->m_shT);
						pOutput->LineTo(pExclude->m_shR, pExclude->m_shB);
						pOutput->LineTo(pExclude->m_shL, pExclude->m_shB);
						pOutput->ClosePath();

						pOutput->EndClipPath(RGN_AND);

						break;
					}
				}
			}
		}
	}
}