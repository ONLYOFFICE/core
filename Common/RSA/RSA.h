#pragma once

#include "VLong.h"

using namespace VLong;
namespace RSA
{
	const static CVLong<DWORD> g_coOne = 1;
	static const size_t g_cnBufferGrow = 200;
	static LPCSTR g_cpszSeparator = "<>";
	class CPrimeFactory
	{
		size_t m_nNP;
		LPDWORD m_pData;
	public:
		CPrimeFactory()
		{
			m_nNP = 0;
			size_t nNP = 200;
			m_pData = new DWORD[nNP];

			// Initialise pl
			size_t nSS = 8*nNP; // Rough estimate to fill pl
			char *pBitTest = new char[nSS+1]; // one extra to stop search
			for (size_t i=0; i<=nSS; i++)
				pBitTest[i] = 1;
			DWORD dwP = 2;
			while (TRUE)
			{
				// skip composites
				while (0==pBitTest[dwP]) 
					dwP++;
				if (dwP == nSS )
					break;
				m_pData[m_nNP] = dwP;
				m_nNP++;
				if (m_nNP == nNP )
					break;
				// cross off multiples
				DWORD dwTemp = dwP*2;
				while (dwTemp < nSS)
				{
					pBitTest[dwTemp] = 0;
					dwTemp += dwP;
				}
				dwP++;
			}
			delete []pBitTest;
		}
		virtual ~CPrimeFactory()
		{
			delete []m_pData;
		}
		CVLong<DWORD> FindPrime(CVLong<DWORD> &oStart)
		{
			DWORD dwSS = 1000; // should be enough unless we are unlucky
			char *pBitSet = new char[dwSS]; // bitset of candidate primes
			size_t nTestedCount = 0;
			while (TRUE)
			{
				memset(pBitSet, 1, dwSS);
				for (DWORD i=0; i<m_nNP; i++)
				{
					DWORD dwP = m_pData[i];
					DWORD dwR = (DWORD)(oStart % (CVLong<DWORD>)dwP); // not as fast as it should be - could do with special routine
					if (0!=dwR) 
						dwR = dwP - dwR;
					// cross off multiples of p
					while (dwR < dwSS )
					{
						pBitSet[dwR] = 0;
						dwR += dwP;
					}
				}
				// now test candidates
				for (DWORD i=0; i<dwSS; i++)
				{
					if (0!=pBitSet[i])
					{
						nTestedCount++;
						if (IsProbablePrime(oStart))
						{
							delete []pBitSet;
							return oStart;
						}
					}
					oStart += 1;
				}
			}
			delete []pBitSet;
		}

		//
		static BOOL IsProbablePrime(const CVLong<DWORD> &oPrime)
		{
			// Test based on Fermats theorem a**(p-1) = 1 mod p for prime p
			// For 1000 bit numbers this can take quite a while
			const size_t nRep = 4;
			const CVLong<DWORD> arAny[nRep] = {2,3,5,7};
			const CVLong<DWORD> oOne(1);
			for (size_t i=0; i<nRep; i++)
			{
				if (oOne != CMontgomery<DWORD>::ModExp(arAny[i], oPrime - 1, oPrime))
					return FALSE;
			}
			return TRUE;
		}
	};

	class CRSAKeyGenerator
	{
	protected:
		CVLong<DWORD> m_oMod;
		CVLong<DWORD> m_oExp;
		CVLong<DWORD> m_oP;
		CVLong<DWORD> m_oQ;
		void CalculatePublicKey()
		{
			m_oMod = m_oP*m_oQ;
			m_oExp = 50001; // must be odd since p-1 and q-1 are even
			while ( (CVLong<DWORD>::GCD(m_oP - g_coOne, m_oExp) != g_coOne)	|| 
					(CVLong<DWORD>::GCD(m_oQ - g_coOne, m_oExp) != g_coOne))
				m_oExp += 2;
		}
	public:
		LPSTR ExportPrivateKey()
		{	
			LPSTR pszP = m_oP.ExportToBase64(); size_t nPStrLenght = strlen(pszP);
			LPSTR pszQ = m_oQ.ExportToBase64(); size_t nQStrLenght = strlen(pszQ);
			LPSTR pszRet = new char[nPStrLenght + nQStrLenght + 2 + 1];
			pszRet[0] = '\0';
			strcat(pszRet, pszP);
			delete []pszP;
			strcat(pszRet, g_cpszSeparator);
			strcat(pszRet, pszQ);
			delete []pszQ;
			size_t nRetStrLenght = strlen(pszRet);
			for (int i=0; i<(int)nRetStrLenght - 1; i++)
			{
				if (('\r' == pszRet[i])&&('\n' == pszRet[i+1]))
				{
					memcpy(pszRet, pszRet + 2, nRetStrLenght - i - 2);
					nRetStrLenght -= 2;
					i -= 2;
				}				
			}
			return pszRet;			
		}
		LPSTR ExportPublicKey()
		{	
			LPSTR pszMod = m_oMod.ExportToBase64(); size_t nModStrLenght = strlen(pszMod);
			LPSTR pszExp = m_oExp.ExportToBase64(); size_t nExpStrLenght = strlen(pszExp);
			LPSTR pszRet = new char[nModStrLenght + nExpStrLenght + 2 + 1];
			pszRet[0] = '\0';
			strcat(pszRet, pszMod);
			delete []pszMod;
			strcat(pszRet, g_cpszSeparator);
			strcat(pszRet, pszExp);
			delete []pszExp;
			size_t nRetStrLenght = strlen(pszRet);
			for (int i=0; i<(int)nRetStrLenght - 1; i++)
			{
				if (('\r' == pszRet[i])&&('\n' == pszRet[i+1]))
				{
					memcpy(pszRet + i, pszRet + i + 2, nRetStrLenght - i - 2);
					nRetStrLenght -= 2;
					i -= 2;
				}				
			}
			return pszRet;			
		}
		void Create(LPBYTE pKey1, size_t nSizeKey1, LPBYTE pKey2, size_t nSizeKey2)
		{
			CPrimeFactory oPF;
			m_oP = oPF.FindPrime(CVLong<DWORD>::FromBuffer(pKey1, nSizeKey1));
			m_oQ = oPF.FindPrime(CVLong<DWORD>::FromBuffer(pKey2, nSizeKey2));
			if (m_oP > m_oQ) 
			{ 
				CVLong<DWORD> oTmp = m_oP; 
				m_oP = m_oQ; 
				m_oP = oTmp;
			}
			CalculatePublicKey();
		}
	};

	class CRSAEncryptor
	{
	protected:

		CVLong<DWORD> m_oMod;
		CVLong<DWORD> m_oExp;
		CVLong<DWORD> Encrypt(const CVLong<DWORD>& oVal) // Requires 0 <= oVal < m_oMod
		{
			return CMontgomery<DWORD>::ModExp(oVal, m_oExp, m_oMod);
		}
	public:
		BOOL ImportPublicKey(LPCSTR pszBase64)
		{			
			LPCSTR pszExp = strstr(pszBase64, g_cpszSeparator);
			if (NULL==pszExp)
				return FALSE;
			int nModStrLength = (int)(pszExp - pszBase64);
			LPSTR pszMod = new char [nModStrLength + 1];
			memcpy(pszMod, pszBase64, nModStrLength);
			pszMod[nModStrLength] = '\0';
			m_oMod.ImportFromBase64(pszMod);
			m_oExp.ImportFromBase64(pszExp + 2);
			delete [] pszMod;
#ifdef _DEBUG
			//ATLTRACE("ImportPublicKey\n");

			LPSTR pszTemp = m_oMod.ExportToBase64();
			//ATLTRACE("m_oMod = %s\n", pszTemp);
			delete [] pszTemp;
			pszTemp = m_oExp.ExportToBase64();
			//ATLTRACE("m_oExp = %s\n", pszTemp);
			delete [] pszTemp;
#endif
			return (((CVLong<DWORD>)0!=m_oMod)&&((CVLong<DWORD>)0!=m_oExp));
		}
		//
		BOOL Encrypt(LPBYTE pSrcBuffer, size_t nSrcSize, LPBYTE &pDstBuffer, size_t &nDstSize)
		{
			if ((0==nSrcSize)||(NULL==pSrcBuffer))
				return FALSE;

			size_t nDstBufferSize = nSrcSize + 1024;
			pDstBuffer = new BYTE[nDstBufferSize];
			if (NULL==pDstBuffer)
				return FALSE;
			LPBYTE pDstBufferPtr = pDstBuffer;
			nDstSize = 0;
			CVLong<DWORD> oKoef = 256;
			for (size_t i=0; i<nSrcSize;)
			{
				CVLong<DWORD> oSrcVal = 0;
				size_t j = i;
				for (; j < nSrcSize; j++)
				{
					CVLong<DWORD> oTemp = oSrcVal*oKoef + (CVLong<DWORD>)pSrcBuffer[j];
					if (oTemp>=m_oMod)
						break;
					oSrcVal = oTemp;
				}
				CVLong<DWORD> oDstVal = Encrypt(oSrcVal);
				i = j;
				size_t nDstValSize = oDstVal.GetByteRealDataSize();
				if (nDstSize + nDstValSize>nDstBufferSize)
				{
					LPBYTE pDstBufferOld = pDstBuffer;
					pDstBuffer = new BYTE[nDstBufferSize + g_cnBufferGrow];
					if (NULL==pDstBuffer)
					{
						delete [] pDstBufferOld;
						return FALSE;
					}
					memcpy(pDstBuffer, pDstBufferOld, nDstBufferSize);
					delete [] pDstBufferOld;
					nDstBufferSize += g_cnBufferGrow;
					pDstBufferPtr += nDstSize;					
				}
				oDstVal.ToBuffer(pDstBufferPtr);
				pDstBufferPtr += nDstValSize;
				nDstSize += nDstValSize;
			}
			return TRUE;
		}
		LPSTR Encrypt(LPCSTR pszSrc)
		{			
			LPBYTE pEncBuffer = NULL; size_t nEncBufferSize = 0;
			if (!Encrypt((LPBYTE)pszSrc, strlen(pszSrc), pEncBuffer, nEncBufferSize))
				return "";

			int nStrSize = Base64EncodeGetRequiredLength((int)nEncBufferSize);
			LPSTR pStrData = new char[nStrSize + 1];
			BOOL bSuccess = Base64Encode(pEncBuffer, (int)nEncBufferSize, pStrData, &nStrSize);
			pStrData[nStrSize] = '\0';
			delete [] pEncBuffer;
			return pStrData;
		}
	};

	class CRSADecryptor
	{
	protected:
		CVLong<DWORD> m_oMod;
		CVLong<DWORD> m_oExp;
		CVLong<DWORD> m_oP;
		CVLong<DWORD> m_oQ;
		void CalculatePublicKey()
		{
			m_oMod = m_oP*m_oQ;
			m_oExp = 50001; // must be odd since p-1 and q-1 are even
			while ( (CVLong<DWORD>::GCD(m_oP - g_coOne, m_oExp) != g_coOne)	|| 
					(CVLong<DWORD>::GCD(m_oQ - g_coOne, m_oExp) != g_coOne))
				m_oExp += 2;
		}
		CVLong<DWORD> Decrypt(CVLong<DWORD>& oVal)
		{
			// Calculate values for performing decryption
			// These could be cached, but the calculation is quite fast			
			CVLong<DWORD> oD = CVLong<DWORD>::ModInv(m_oExp, (m_oP - g_coOne)*(m_oQ - g_coOne));
			CVLong<DWORD> oU = CVLong<DWORD>::ModInv(m_oP, m_oQ);
			CVLong<DWORD> oDP = oD % (m_oP - g_coOne);
			CVLong<DWORD> oDQ = oD % (m_oQ - g_coOne);

			// Apply chinese remainder theorem
			CVLong<DWORD> oA = CMontgomery<DWORD>::ModExp(oVal % m_oP, oDP, m_oP);
			CVLong<DWORD> oB = CMontgomery<DWORD>::ModExp(oVal % m_oQ, oDQ, m_oQ);
			if (oB < oA) 
				oB += m_oQ;
			return oA + m_oP * ( ((oB - oA) * oU) % m_oQ);
		}
	public:
		BOOL ImportPrivateKey(LPCSTR pszBase64)
		{
			LPCSTR pszQ = strstr(pszBase64, g_cpszSeparator);
			if (NULL==pszQ)
				return FALSE;
			int nPStrLength = (int)(pszQ - pszBase64);
			LPSTR pszP = new char [nPStrLength + 1];
			memcpy(pszP, pszBase64, nPStrLength);
			pszP[nPStrLength] = '\0';
			m_oP.ImportFromBase64(pszP);
			m_oQ.ImportFromBase64(pszQ + 2);
			delete [] pszP;

			if (((CVLong<DWORD>)0==m_oP)&&((CVLong<DWORD>)0==m_oQ))
				return FALSE;
			CalculatePublicKey();
#ifdef _DEBUG
			//ATLTRACE("ImportPrivateKey\n");
			LPBYTE pTemp = new BYTE[1024];
			m_oP.ToBuffer(pTemp);
			//ATLTRACE("m_oP = ");
			for (size_t i=0; i<m_oP.GetByteRealDataSize(); i++)
			{	
				//ATLTRACE("0x%02X, ", pTemp[i]);
			}
			//ATLTRACE("\n");
			
			m_oQ.ToBuffer(pTemp);
			//ATLTRACE("m_oQ = ");
			for (size_t i=0; i<m_oQ.GetByteRealDataSize(); i++)
			{	
				//ATLTRACE("0x%02X, ", pTemp[i]);
			}
			//ATLTRACE("\n");

			m_oMod.ToBuffer(pTemp);
			//ATLTRACE("m_oMod = ");
			for (size_t i=0; i<m_oMod.GetByteRealDataSize(); i++)
			{	
				//ATLTRACE("0x%02X, ", pTemp[i]);
			}
			//ATLTRACE("\n");
			
			m_oExp.ToBuffer(pTemp);
			//ATLTRACE("m_oExp = ");
			for (size_t i=0; i<m_oExp.GetByteRealDataSize(); i++)
			{	
				//ATLTRACE("0x%02X, ", pTemp[i]);
			}
			//ATLTRACE("\n");
			delete [] pTemp;
			//ATLTRACE("m_oP   = %s\n", m_oP.ExportToBase64());
			//ATLTRACE("m_oQ   = %s\n", m_oQ.ExportToBase64());
			//ATLTRACE("m_oMod = %s\n", m_oMod.ExportToBase64());
			//ATLTRACE("m_oExp = %s\n", m_oExp.ExportToBase64());
#endif
			return TRUE;
		}
		//		
		void Create(LPBYTE pKey1, size_t nSizeKey1, LPBYTE pKey2, size_t nSizeKey2)
		{
			CPrimeFactory oPF;
			m_oP = oPF.FindPrime(CVLong<DWORD>::FromBuffer(pKey1, nSizeKey1));
			m_oQ = oPF.FindPrime(CVLong<DWORD>::FromBuffer(pKey2, nSizeKey2));
			if (m_oP > m_oQ) 
			{ 
				CVLong<DWORD> oTmp = m_oP; 
				m_oP = m_oQ; 
				m_oP = oTmp;
			}
			CalculatePublicKey();
		}
		BOOL Decrypt(LPBYTE pSrcBuffer, size_t nSrcSize, LPBYTE &pDstBuffer, size_t &nDstSize)
		{
			if ((0==nSrcSize)||(NULL==pSrcBuffer))
				return FALSE;

			size_t nDstBufferSize = nSrcSize + 1024;
			pDstBuffer = new BYTE[nDstBufferSize];
			if (NULL==pDstBuffer)
				return FALSE;
			LPBYTE pDstBufferPtr = pDstBuffer;
			size_t nDstBufferCursor = 0;
			CVLong<DWORD> oKoef = 256;
			for (size_t i=0; i<nSrcSize; )
			{
				CVLong<DWORD> oSrcVal = 0;
				size_t j = i;
				for (; j < nSrcSize; j++)
				{
					CVLong<DWORD> oTemp = oSrcVal*oKoef + (CVLong<DWORD>)pSrcBuffer[j];
					if (oTemp>=m_oMod)
						break;
					oSrcVal = oTemp;
				}
				CVLong<DWORD> oDstVal = Decrypt(oSrcVal);
				i = j;
				size_t nDstValSize = oDstVal.GetByteRealDataSize();
				if (nDstBufferCursor + nDstValSize>nDstBufferSize)
				{
					LPBYTE pDstBufferOld = pDstBuffer;
					pDstBuffer = new BYTE[nDstBufferSize + g_cnBufferGrow];
					if (NULL==pDstBuffer)
					{
						delete [] pDstBufferOld;
						return FALSE;
					}
					memcpy(pDstBuffer, pDstBufferOld, nDstBufferSize);
					delete [] pDstBufferOld;
					nDstBufferSize += g_cnBufferGrow;
					pDstBufferPtr += nDstBufferCursor;					
				}
				oDstVal.ToBuffer(pDstBufferPtr);
				pDstBufferPtr += nDstValSize;
				nDstBufferCursor += nDstValSize;
			}
			nDstSize = nDstBufferCursor;
			return TRUE;
		}
		LPSTR Decrypt(LPCSTR pszBase64)
		{
			int nBase64StrLen = (int)strlen(pszBase64);
			int nEncSize;
			nEncSize = Base64DecodeGetRequiredLength(nBase64StrLen);
			LPBYTE pEncBuffer = new BYTE[nEncSize];
			if (NULL==pEncBuffer)
				return "";
			if (!Base64Decode(pszBase64, nBase64StrLen, pEncBuffer, &nEncSize))
			{
				delete []pEncBuffer;
				return "";
			}
			LPBYTE pDecBuffer = NULL;
			size_t nDecSize = 0;
			if (!Decrypt(pEncBuffer, nEncSize, pDecBuffer, nDecSize))
			{
				delete []pEncBuffer;
				return "";
			}
			
			LPSTR pStr = new char[nDecSize+1];
			memcpy((LPBYTE)pStr, pDecBuffer, nDecSize);
			pStr[nDecSize] = '\0';
			delete [] pEncBuffer;
			delete [] pDecBuffer;
			return pStr;
		}
	};
};