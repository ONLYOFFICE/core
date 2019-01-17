/*
 * (c) Copyright Ascensio System SIA 2010-2019
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
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
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
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "MemoryUtils.h"
#include "Object.h"
#include "Dict.h"
#include "Stream.h"
#include "Function.h"

namespace PdfReader
{
	//------------------------------------------------------------------------
	// Function
	//------------------------------------------------------------------------

	Function::Function()
	{
	}

	Function::~Function()
	{
	}

	Function *Function::Parse(Object *pFuncObject)
	{
		Dict *pDict;

		// Функция задается либо как словарь, либо поток, либо как тождественная функция
		if (pFuncObject->IsStream())
		{
			pDict = pFuncObject->StreamGetDict();
		}
		else if (pFuncObject->IsDict())
		{
			pDict = pFuncObject->GetDict();
		}
		else if (pFuncObject->IsName("Identity"))
		{
			return new IdentityFunction();
		}
		else
		{
			// TO DO: Error "Expected function dictionary or stream"
			return NULL;
		}

		Object oTemp;
		if (!pDict->Search("FunctionType", &oTemp)->IsInt())
		{
			// TO DO: Error "Function type is missing or wrong type"
			oTemp.Free();
			return NULL;
		}
		int nFuncType = oTemp.GetInt();
		oTemp.Free();

		Function *pFunc = NULL;
		if (0 == nFuncType) // Sampled function
		{
			pFunc = new SampledFunction(pFuncObject, pDict);
		}
		else if (2 == nFuncType) // Exponential interpolation function
		{
			pFunc = new ExponentialFunction(pFuncObject, pDict);
		}
		else if (3 == nFuncType) // Stitching function
		{
			pFunc = new StitchingFunction(pFuncObject, pDict);
		}
		else if (4 == nFuncType) // PostScript calculator function
		{
			pFunc = new PostScriptFunction(pFuncObject, pDict);
		}
		else
		{
			// TO DO: Error "Unimplemented function type"
			return NULL;
		}
		if (!pFunc->IsValid())
		{
			delete pFunc;
			return NULL;
		}

		return pFunc;
	}

	bool Function::Initialize(Dict *pDict)
	{
		Object oArray;
		// Domain (Всегда должен присутствовать)
		if (!pDict->Search("Domain", &oArray)->IsArray())
		{
			// TO DO: Error "Function is missing domain"
			oArray.Free();
			return false;
		}
		m_nInputDim = oArray.ArrayGetLength() / 2;

		if (m_nInputDim > funcMaxInputs)
		{
			// TO DO: Error "Functions with more than funcMaxInputs inputs are unsupported"
			oArray.Free();
			return false;
		}
		for (int nIndex = 0; nIndex < m_nInputDim; ++nIndex)
		{
			Object oTemp;
			oArray.ArrayGet(2 * nIndex, &oTemp);
			if (!oTemp.IsNum())
			{
				// TO DO: Error "Illegal value in function domain array"
				oTemp.Free();
				oArray.Free();
				return false;
			}
			m_arrDomain[nIndex][0] = oTemp.GetNum();
			oTemp.Free();
			oArray.ArrayGet(2 * nIndex + 1, &oTemp);
			if (!oTemp.IsNum())
			{
				// TO DO: Error "Illegal value in function domain array"
				oTemp.Free();
				oArray.Free();
				return false;
			}
			m_arrDomain[nIndex][1] = oTemp.GetNum();
			oTemp.Free();
		}
		oArray.Free();

		// Range (Этого поля может и не быть)
		m_bHasRange = false;
		m_nOutputDim = 0;
		if (pDict->Search("Range", &oArray)->IsArray())
		{
			m_bHasRange = true;
			m_nOutputDim = oArray.ArrayGetLength() / 2;
			if (m_nOutputDim > funcMaxOutputs)
			{
				// TO DO: Error "Functions with more than funcMaxOutputs outputs are unsupported"
				oArray.Free();
				return false;
			}
			for (int nIndex = 0; nIndex < m_nOutputDim; ++nIndex)
			{
				Object oTemp;
				oArray.ArrayGet(2 * nIndex, &oTemp);
				if (!oTemp.IsNum())
				{
					// TO DO: Error "Illegal value in function range array"
					oTemp.Free();
					oArray.Free();
					return false;
				}
				m_arrRange[nIndex][0] = oTemp.GetNum();
				oTemp.Free();
				oArray.ArrayGet(2 * nIndex + 1, &oTemp);
				if (!oTemp.IsNum())
				{
					// TO DO: Error "Illegal value in function range array"
					oTemp.Free();
					oArray.Free();
					return false;
				}
				m_arrRange[nIndex][1] = oTemp.GetNum();
				oTemp.Free();
			}
		}
		oArray.Free();

		return true;
	}

	//------------------------------------------------------------------------
	// IdentityFunction
	//------------------------------------------------------------------------

	IdentityFunction::IdentityFunction()
	{
		// Заполняем произвольными значениями
		m_nInputDim = funcMaxInputs;
		m_nOutputDim = funcMaxOutputs;

		for (int nIndex = 0; nIndex < funcMaxInputs; ++nIndex)
		{
			m_arrDomain[nIndex][0] = 0;
			m_arrDomain[nIndex][1] = 1;
		}
		m_bHasRange = false;
	}

	IdentityFunction::~IdentityFunction()
	{
	}

	void IdentityFunction::Transform(double *pInput, double *pOutput)
	{
		for (int nIndex = 0; nIndex < funcMaxOutputs; ++nIndex)
		{
			pOutput[nIndex] = pInput[nIndex]; // тождественная же функция :)
		}
	}

	//------------------------------------------------------------------------
	// SampledFunction
	//------------------------------------------------------------------------

	SampledFunction::SampledFunction(Object *pFuncObject, Dict *pDict)
	{
		m_pSamples = NULL;
		m_pBuffer = NULL;
		m_bValid = false;

		if (!Initialize(pDict))
		{
			return;
		}
		if (!m_bHasRange)
		{
			// TO DO: Error "Type 0 function is missing range"
			return;
		}
		if (m_nInputDim > sampledFuncMaxInputs)
		{
			// TO DO: Error "Sampled functions with more than sampledFuncMaxInputs inputs are unsupported"
			return;
		}

		m_pBuffer = (double *)MemUtilsMallocArray(1 << m_nInputDim, sizeof(double));

		if (!pFuncObject->IsStream())
		{
			// TO DO: Error "Type 0 function isn't a stream"
			return;
		}
		Stream *pStream = pFuncObject->GetStream();

		// Size
		Object oArray;
		if (!pDict->Search("Size", &oArray)->IsArray() || oArray.ArrayGetLength() != m_nInputDim)
		{
			// TO DO: Error "Function has missing or invalid size array"
			oArray.Free();
			return;
		}
		for (int nIndex = 0; nIndex < m_nInputDim; ++nIndex)
		{
			Object oTemp;
			oArray.ArrayGet(nIndex, &oTemp);
			if (!oTemp.IsInt())
			{
				// TO DO: Error "Illegal value in function size array"
				oTemp.Free();
				oArray.Free();
				return;
			}
			m_arrSize[nIndex] = oTemp.GetInt();
			oTemp.Free();
		}
		oArray.Free();

		m_arrIndexMult[0] = m_nOutputDim;
		for (int nIndex = 1; nIndex < m_nInputDim; ++nIndex)
		{
			m_arrIndexMult[nIndex] = m_arrIndexMult[nIndex - 1] * m_arrSize[nIndex - 1];
		}

		// BitsPerSample
		if (!pDict->Search("BitsPerSample", &oArray)->IsInt())
		{
			// TO DO: Error "Function has missing or invalid BitsPerSample"
			oArray.Free();
			return;
		}
		int nSampleBits = oArray.GetInt();
		double dSampleMult = 1.0 / (pow(2.0, (double)nSampleBits) - 1);
		oArray.Free();

		// TO DO: Сделать чтение поля Order, и реализовать функцию Transform

		// Encode
		if (pDict->Search("Encode", &oArray)->IsArray() && oArray.ArrayGetLength() == 2 * m_nInputDim)
		{
			for (int nIndex = 0; nIndex < m_nInputDim; ++nIndex)
			{
				Object oTemp;
				oArray.ArrayGet(2 * nIndex, &oTemp);
				if (!oTemp.IsNum())
				{
					// TO DO: Error "Illegal value in function encode array"
					oTemp.Free();
					oArray.Free();
					return;
				}
				m_arrEncoder[nIndex][0] = oTemp.GetNum();
				oTemp.Free();

				oArray.ArrayGet(2 * nIndex + 1, &oTemp);
				if (!oTemp.IsNum())
				{
					// TO DO: Error "Illegal value in function encode array"
					oTemp.Free();
					oArray.Free();
					return;
				}
				m_arrEncoder[nIndex][1] = oTemp.GetNum();
				oTemp.Free();
			}
		}
		else
		{
			for (int nIndex = 0; nIndex < m_nInputDim; ++nIndex)
			{
				m_arrEncoder[nIndex][0] = 0;
				m_arrEncoder[nIndex][1] = m_arrSize[nIndex] - 1;
			}
		}
		oArray.Free();

		for (int nIndex = 0; nIndex < m_nInputDim; ++nIndex)
		{
			m_arrInputMult[nIndex] = (m_arrEncoder[nIndex][1] - m_arrEncoder[nIndex][0]) / (m_arrDomain[nIndex][1] - m_arrDomain[nIndex][0]);
		}

		// Decode
		if (pDict->Search("Decode", &oArray)->IsArray() && oArray.ArrayGetLength() == 2 * m_nOutputDim)
		{
			for (int nIndex = 0; nIndex < m_nOutputDim; ++nIndex)
			{
				Object oTemp;
				oArray.ArrayGet(2 * nIndex, &oTemp);
				if (!oTemp.IsNum())
				{
					// TO DO: Error "Illegal value in function decode array"
					oTemp.Free();
					oArray.Free();
					return;
				}
				m_arrDecoder[nIndex][0] = oTemp.GetNum();
				oTemp.Free();

				oArray.ArrayGet(2 * nIndex + 1, &oTemp);
				if (!oTemp.IsNum())
				{
					// TO DO: Error "Illegal value in function decode array"
					oTemp.Free();
					oArray.Free();
					return;
				}
				m_arrDecoder[nIndex][1] = oTemp.GetNum();
				oTemp.Free();
			}
		}
		else
		{
			for (int nIndex = 0; nIndex < m_nOutputDim; ++nIndex)
			{
				m_arrDecoder[nIndex][0] = m_arrRange[nIndex][0];
				m_arrDecoder[nIndex][1] = m_arrRange[nIndex][1];
			}
		}
		oArray.Free();

		// Samples
		m_nSamplesCount = m_nOutputDim;
		for (int nIndex = 0; nIndex < m_nInputDim; ++nIndex)
			m_nSamplesCount *= m_arrSize[nIndex];
		m_pSamples = (double *)MemUtilsMallocArray(m_nSamplesCount, sizeof(double));

		unsigned int unBuf = 0;
		int nBitsCount = 0;
		unsigned int unBitMask = (1 << nSampleBits) - 1;
		pStream->Reset();
		for (int nIndex = 0; nIndex < m_nSamplesCount; ++nIndex)
		{
			unsigned int unValue = 0;
			if (nSampleBits == 8)
			{
				unValue = pStream->GetChar();
			}
			else if (nSampleBits == 16)
			{
				unValue = pStream->GetChar();
				unValue = (unValue << 8) + pStream->GetChar();
			}
			else if (nSampleBits == 32)
			{
				unValue = pStream->GetChar();
				unValue = (unValue << 8) + pStream->GetChar();
				unValue = (unValue << 8) + pStream->GetChar();
				unValue = (unValue << 8) + pStream->GetChar();
			}
			else
			{
				while (nBitsCount < nSampleBits)
				{
					unBuf = (unBuf << 8) | (pStream->GetChar() & 0xff);
					nBitsCount += 8;
				}
				unValue = (unBuf >> (nBitsCount - nSampleBits)) & unBitMask;
				nBitsCount -= nSampleBits;
			}
			m_pSamples[nIndex] = (double)unValue * dSampleMult;
		}
		pStream->Close();

		m_bValid = true;
		return;
	}

	SampledFunction::~SampledFunction()
	{
		MemUtilsFree(m_pSamples);
		MemUtilsFree(m_pBuffer);
	}

	SampledFunction::SampledFunction(SampledFunction *pFunc)
	{
		memcpy(this, pFunc, sizeof(SampledFunction));
		m_pSamples = (double *)MemUtilsMallocArray(m_nSamplesCount, sizeof(double));
		memcpy(m_pSamples, pFunc->m_pSamples, m_nSamplesCount * sizeof(double));
		m_pBuffer = (double *)MemUtilsMallocArray(1 << m_nInputDim, sizeof(double));
	}

	void SampledFunction::Transform(double *pInput, double *pOutput)
	{
		int    arrE[funcMaxInputs][2];
		double arrEFrac0[funcMaxInputs];
		double arrEFrac1[funcMaxInputs];

		for (int nIndex = 0; nIndex < m_nInputDim; ++nIndex)
		{
			double dValue = (pInput[nIndex] - m_arrDomain[nIndex][0]) * m_arrInputMult[nIndex] + m_arrEncoder[nIndex][0];
			if (dValue < 0)
			{
				dValue = 0;
			}
			else if (dValue > m_arrSize[nIndex] - 1)
			{
				dValue = m_arrSize[nIndex] - 1;
			}
			arrE[nIndex][0] = (int)dValue;
			if ((arrE[nIndex][1] = arrE[nIndex][0] + 1) >= m_arrSize[nIndex])
			{
				arrE[nIndex][1] = arrE[nIndex][0];
			}
			arrEFrac1[nIndex] = dValue - arrE[nIndex][0];
			arrEFrac0[nIndex] = 1 - arrEFrac1[nIndex];
		}

		for (int nI = 0; nI < m_nOutputDim; ++nI)
		{
			for (int nJ = 0; nJ < (1 << m_nInputDim); ++nJ)
			{
				int nIdx = nI;
				for (int nK = 0, t = nJ; nK < m_nInputDim; ++nK, t >>= 1)
				{
					nIdx += m_arrIndexMult[nK] * (arrE[nK][t & 1]);
				}
				m_pBuffer[nJ] = m_pSamples[nIdx];
			}

			int nT = 0;
			for (int nJ = 0, nT = (1 << m_nInputDim); nJ < m_nInputDim; ++nJ, nT >>= 1)
			{
				for (int nK = 0; nK < nT; nK += 2)
				{
					m_pBuffer[nK >> 1] = arrEFrac0[nJ] * m_pBuffer[nK] + arrEFrac1[nJ] * m_pBuffer[nK + 1];
				}
			}

			pOutput[nI] = m_pBuffer[0] * (m_arrDecoder[nI][1] - m_arrDecoder[nI][0]) + m_arrDecoder[nI][0];
			if (pOutput[nI] < m_arrRange[nI][0])
			{
				pOutput[nI] = m_arrRange[nI][0];
			}
			else if (pOutput[nI] > m_arrRange[nI][1])
			{
				pOutput[nI] = m_arrRange[nI][1];
			}
		}
	}

	//------------------------------------------------------------------------
	// ExponentialFunction
	//------------------------------------------------------------------------

	ExponentialFunction::ExponentialFunction(Object *pFuncObject, Dict *pDict)
	{
		m_bValid = false;

		if (!Initialize(pDict))
		{
			return;
		}
		if (m_nInputDim != 1)
		{
			// TO DO: Error "Exponential function with more than one input"
			return;
		}

		// C0
		Object oArray;
		if (pDict->Search("C0", &oArray)->IsArray())
		{
			if (m_bHasRange && oArray.ArrayGetLength() != m_nOutputDim)
			{
				// TO DO: Error "Function's C0 array is wrong length"
				oArray.Free();
				return;
			}
			m_nOutputDim = oArray.ArrayGetLength();
			for (int nIndex = 0; nIndex < m_nOutputDim; ++nIndex)
			{
				Object oTemp;
				oArray.ArrayGet(nIndex, &oTemp);
				if (!oTemp.IsNum())
				{
					// TO DO: Error "Illegal value in function C0 array"
					oArray.Free();
					oTemp.Free();
					return;
				}
				m_arrC0[nIndex] = oTemp.GetNum();
				oTemp.Free();
			}
		}
		else
		{
			if (m_bHasRange && m_nOutputDim != 1)
			{
				// TO DO: Error "Function's C0 array is wrong length"
				oArray.Free();
				return;
			}
			m_nOutputDim = 1;
			m_arrC0[0] = 0;
		}
		oArray.Free();

		// C1
		if (pDict->Search("C1", &oArray)->IsArray())
		{
			if (oArray.ArrayGetLength() != m_nOutputDim)
			{
				// TO DO: Error "Function's C1 array is wrong length"
				oArray.Free();
				return;
			}
			for (int nIndex = 0; nIndex < m_nOutputDim; ++nIndex)
			{
				Object oTemp;
				oArray.ArrayGet(nIndex, &oTemp);
				if (!oTemp.IsNum())
				{
					// TO DO: Error "Illegal value in function C1 array"
					oTemp.Free();
					oArray.Free();
					return;
				}
				m_arrC1[nIndex] = oTemp.GetNum();
				oTemp.Free();
			}
		}
		else
		{
			if (m_nOutputDim != 1)
			{
				// TO DO: Error "Function's C1 array is wrong length"
				oArray.Free();
				return;
			}
			m_arrC1[0] = 1;
		}
		oArray.Free();

		// N (exponent)
		if (!pDict->Search("N", &oArray)->IsNum())
		{
			// TO DO: Error "Function has missing or invalid N"
			oArray.Free();
			return;
		}
		m_dN = oArray.GetNum();
		oArray.Free();

		m_bValid = true;
		return;
	}

	ExponentialFunction::~ExponentialFunction()
	{
	}

	ExponentialFunction::ExponentialFunction(ExponentialFunction *pFunc)
	{
		memcpy(this, pFunc, sizeof(ExponentialFunction));
	}

	void ExponentialFunction::Transform(double *pInput, double *pOutput)
	{
		double dX = 0;

		if (pInput[0] < m_arrDomain[0][0])
		{
			dX = m_arrDomain[0][0];
		}
		else if (pInput[0] > m_arrDomain[0][1])
		{
			dX = m_arrDomain[0][1];
		}
		else
		{
			dX = pInput[0];
		}
		for (int nIndex = 0; nIndex < m_nOutputDim; ++nIndex)
		{
			pOutput[nIndex] = m_arrC0[nIndex] + pow(dX, m_dN) * (m_arrC1[nIndex] - m_arrC0[nIndex]);
			if (m_bHasRange)
			{
				if (pOutput[nIndex] < m_arrRange[nIndex][0])
				{
					pOutput[nIndex] = m_arrRange[nIndex][0];
				}
				else if (pOutput[nIndex] > m_arrRange[nIndex][1])
				{
					pOutput[nIndex] = m_arrRange[nIndex][1];
				}
			}
		}
		return;
	}

	//------------------------------------------------------------------------
	// StitchingFunction
	//------------------------------------------------------------------------

	StitchingFunction::StitchingFunction(Object *pFuncObject, Dict *pDict)
	{
		m_bValid = false;
		m_ppFuncs = NULL;
		m_arrBounds = NULL;
		m_arrEncode = NULL;
		m_arrScale = NULL;

		if (!Initialize(pDict))
		{
			return;
		}

		if (m_nInputDim != 1)
		{
			// TO DO: Error "Stitching function with more than one input"
			return;
		}

		// Functions
		Object oArray;
		if (!pDict->Search("Functions", &oArray)->IsArray())
		{
			// TO DO: Error "Missing 'Functions' entry in stitching function"
			oArray.Free();
			return;
		}

		m_nCount = oArray.ArrayGetLength();
		m_ppFuncs   = (Function**)MemUtilsMallocArray(m_nCount, sizeof(Function *));
		m_arrBounds =   (double *)MemUtilsMallocArray(m_nCount + 1, sizeof(double));
		m_arrEncode =   (double *)MemUtilsMallocArray(2 * m_nCount, sizeof(double));
		m_arrScale  =   (double *)MemUtilsMallocArray(m_nCount, sizeof(double));

		for (int nIndex = 0; nIndex < m_nCount; ++nIndex)
		{
			m_ppFuncs[nIndex] = NULL;
		}

		for (int nIndex = 0; nIndex < m_nCount; ++nIndex)
		{
			Object oTemp;
			if (!(m_ppFuncs[nIndex] = Function::Parse(oArray.ArrayGet(nIndex, &oTemp))))
			{
				oTemp.Free();
				oArray.Free();
				return;
			}

			if (nIndex > 0 && (m_ppFuncs[nIndex]->GetInputSize() != 1 || m_ppFuncs[nIndex]->GetOutputSize() != m_ppFuncs[0]->GetOutputSize()))
			{
				// TO DO: Error "Incompatible subfunctions in stitching function"
				oTemp.Free();
				oArray.Free();
				return;
			}
			oTemp.Free();
		}
		oArray.Free();

		// Bounds
		if (!pDict->Search("Bounds", &oArray)->IsArray() || oArray.ArrayGetLength() != m_nCount - 1)
		{
			// TO DO: Error "Missing or invalid 'Bounds' entry in stitching function"
			oArray.Free();
			return;
		}
		m_arrBounds[0] = m_arrDomain[0][0];
		for (int nIndex = 1; nIndex < m_nCount; ++nIndex)
		{
			Object oTemp;
			if (!oArray.ArrayGet(nIndex - 1, &oTemp)->IsNum())
			{
				// TO DO: Error "Invalid type in 'Bounds' array in stitching function"
				oTemp.Free();
				oArray.Free();
				return;
			}
			m_arrBounds[nIndex] = oTemp.GetNum();
			oTemp.Free();
		}
		m_arrBounds[m_nCount] = m_arrDomain[0][1];
		oArray.Free();

		// Encode
		if (!pDict->Search("Encode", &oArray)->IsArray() || oArray.ArrayGetLength() != 2 * m_nCount)
		{
			// TO DO: Error "Missing or invalid 'Encode' entry in stitching function"
			oArray.Free();
			return;
		}

		for (int nIndex = 0; nIndex < 2 * m_nCount; ++nIndex)
		{
			Object oTemp;
			if (!oArray.ArrayGet(nIndex, &oTemp)->IsNum())
			{
				// TO DO: Error "Invalid type in 'Encode' array in stitching function"
				oTemp.Free();
				oArray.Free();
				return;
			}
			m_arrEncode[nIndex] = oTemp.GetNum();
			oTemp.Free();
		}
		oArray.Free();

		for (int nIndex = 0; nIndex < m_nCount; ++nIndex)
		{
			if (m_arrBounds[nIndex] == m_arrBounds[nIndex + 1])
			{
				// Чтобы избежать деление на 0, в данном случае функция с номером nIndex не будет использоваться
				m_arrScale[nIndex] = 0;
			}
			else
			{
				m_arrScale[nIndex] = (m_arrEncode[2 * nIndex + 1] - m_arrEncode[2 * nIndex]) / (m_arrBounds[nIndex + 1] - m_arrBounds[nIndex]);
			}
		}

		m_bValid = true;
		return;
	}

	StitchingFunction::StitchingFunction(StitchingFunction *pFunc)
	{
		m_nCount = pFunc->m_nCount;
		m_ppFuncs = (Function **)MemUtilsMallocArray(m_nCount, sizeof(Function *));

		for (int nIndex = 0; nIndex < m_nCount; ++nIndex)
		{
			m_ppFuncs[nIndex] = pFunc->m_ppFuncs[nIndex]->Copy();
		}

		m_arrBounds = (double *)MemUtilsMallocArray(m_nCount + 1, sizeof(double));
		memcpy(m_arrBounds, pFunc->m_arrBounds, (m_nCount + 1) * sizeof(double));

		m_arrEncode = (double *)MemUtilsMallocArray(2 * m_nCount, sizeof(double));
		memcpy(m_arrEncode, pFunc->m_arrEncode, 2 * m_nCount * sizeof(double));

		m_arrScale = (double *)MemUtilsMallocArray(m_nCount, sizeof(double));
		memcpy(m_arrScale, pFunc->m_arrScale, m_nCount * sizeof(double));

		m_bValid = true;
	}

	StitchingFunction::~StitchingFunction()
	{
		if (m_ppFuncs)
		{
			for (int nIndex = 0; nIndex < m_nCount; ++nIndex)
			{
				delete m_ppFuncs[nIndex];
			}
		}
		MemUtilsFree(m_ppFuncs);
		MemUtilsFree(m_arrBounds);
		MemUtilsFree(m_arrEncode);
		MemUtilsFree(m_arrScale);
	}

	void StitchingFunction::Transform(double *pInput, double *pOutput)
	{
		double dX;

		if (pInput[0] < m_arrDomain[0][0])
		{
			dX = m_arrDomain[0][0];
		}
		else if (pInput[0] > m_arrDomain[0][1])
		{
			dX = m_arrDomain[0][1];
		}
		else
		{
			dX = pInput[0];
		}

		int nIndex = 0;
		for (nIndex = 0; nIndex < m_nCount - 1; ++nIndex)
		{
			if (dX < m_arrBounds[nIndex + 1])
			{
				break;
			}
		}
		dX = m_arrEncode[2 * nIndex] + (dX - m_arrBounds[nIndex]) * m_arrScale[nIndex];
		m_ppFuncs[nIndex]->Transform(&dX, pOutput);
	}

	//------------------------------------------------------------------------
	// PostScriptFunction
	//------------------------------------------------------------------------

	enum PSOperations
	{
		psOperationAbs,
		psOperationAdd,
		psOperationAnd,
		psOperationAtan,
		psOperationBitshift,
		psOperationCeiling,
		psOperationCopy,
		psOperationCos,
		psOperationCvi,
		psOperationCvr,
		psOperationDiv,
		psOperationDup,
		psOperationEq,
		psOperationExch,
		psOperationExp,
		psOperationFalse,
		psOperationFloor,
		psOperationGe,
		psOperationGt,
		psOperationIdiv,
		psOperationIndex,
		psOperationLe,
		psOperationLn,
		psOperationLog,
		psOperationLt,
		psOperationMod,
		psOperationMul,
		psOperationNe,
		psOperationNeg,
		psOperationNot,
		psOperationOr,
		psOperationPop,
		psOperationRoll,
		psOperationRound,
		psOperationSin,
		psOperationSqrt,
		psOperationSub,
		psOperationTrue,
		psOperationTruncate,
		psOperationXor,
		psOperationIf,
		psOperationIfelse,
		psOperationReturn
	};

	// Список идет в алфавитном порядке. Номер элемента здесь, соответствует
	// операции в перечисляемом типе PSOperations
	char *c_sPSOperationNames[] =
	{
		"abs",
		"add",
		"and",
		"atan",
		"bitshift",
		"ceiling",
		"copy",
		"cos",
		"cvi",
		"cvr",
		"div",
		"dup",
		"eq",
		"exch",
		"exp",
		"false",
		"floor",
		"ge",
		"gt",
		"idiv",
		"index",
		"le",
		"ln",
		"log",
		"lt",
		"mod",
		"mul",
		"ne",
		"neg",
		"not",
		"or",
		"pop",
		"roll",
		"round",
		"sin",
		"sqrt",
		"sub",
		"true",
		"truncate",
		"xor"
	};

#define PSOperationsCount (sizeof(c_sPSOperationNames) / sizeof(char *))

	enum PSObjectType
	{
		psBool,
		psInt,
		psReal,
		psOperator,
		psBlock
	};

	// В потоке, операторы 'if'/'ifelse' занимают три слота.
	//
	//         +---------------------------------+
	//         | psOperator: If / Ifelse         |
	//         +---------------------------------+
	//         | psBlock: указатель = <A>        |
	//         +---------------------------------+
	//         | psBlock: указатель = <B>        |
	//         +---------------------------------+
	//         | if                              |
	//         | ...                             |
	//         | psOperator: Return              |
	//         +---------------------------------+
	//     <A> | else                            |
	//         | ...                             |
	//         | psOperator: Return              |
	//         +---------------------------------+
	//     <B> | ...                             |
	//
	// Если оператор = 'if', указатель <A> все задан, но не используется

	struct PSObject
	{
		PSObjectType     eType;
		union
		{
			bool         uBool;      // boolean (stack only)
			int          uInt;       // integer (stack and code)
			double       uReal;      // real (stack and code)
			PSOperations uOperation; // operator (code only)
			int          uBlock;     // if/ifelse block pointer (code only)
		};
	};

#define psStackSize 100

	class PSStack
	{
	public:

		PSStack()
		{
			m_nStackSize = psStackSize;
		}
		void   PushBool(bool bBool)
		{
			if (CheckOverflow())
			{
				m_arrStack[--m_nStackSize].eType = psBool;
				m_arrStack[m_nStackSize].uBool = bBool;
			}
		}

		void   PushInt(int nInteger)
		{
			if (CheckOverflow())
			{
				m_arrStack[--m_nStackSize].eType = psInt;
				m_arrStack[m_nStackSize].uInt    = nInteger;
			}
		}

		void   PushReal(double dReal)
		{
			if (CheckOverflow())
			{
				m_arrStack[--m_nStackSize].eType = psReal;
				m_arrStack[m_nStackSize].uReal   = dReal;
			}
		}

		bool   PopBool()
		{
			if (CheckUnderflow() && CheckType(psBool, psBool))
			{
				return m_arrStack[m_nStackSize++].uBool;
			}
			return false;
		}

		int    PopInt()
		{
			if (CheckUnderflow() && CheckType(psInt, psInt))
			{
				return m_arrStack[m_nStackSize++].uInt;
			}
			return 0;
		}

		double PopNum()
		{
			if (CheckUnderflow() && CheckType(psInt, psReal))
			{
				double dReturn = (m_arrStack[m_nStackSize].eType == psInt) ? (double)m_arrStack[m_nStackSize].uInt : m_arrStack[m_nStackSize].uReal;
				++m_nStackSize;
				return dReturn;
			}
			return 0;
		}

		bool Empty()
		{
			return m_nStackSize == psStackSize;
		}
		bool TopIsInt()
		{
			return m_nStackSize < psStackSize && m_arrStack[m_nStackSize].eType == psInt;
		}
		bool TopTwoAreInts()
		{
			return m_nStackSize < psStackSize - 1 && m_arrStack[m_nStackSize].eType == psInt && m_arrStack[m_nStackSize + 1].eType == psInt;
		}
		bool TopIsReal()
		{
			return m_nStackSize < psStackSize && m_arrStack[m_nStackSize].eType == psReal;
		}
		bool TopTwoAreNums()
		{
			return m_nStackSize < psStackSize - 1 && (m_arrStack[m_nStackSize].eType == psInt || m_arrStack[m_nStackSize].eType == psReal) && (m_arrStack[m_nStackSize + 1].eType == psInt || m_arrStack[m_nStackSize + 1].eType == psReal);
		}

		void Copy(int nCount)
		{
			if (m_nStackSize + nCount > psStackSize)
			{
				// TO DO: Error "Stack underflow in PostScript function"
				return;
			}
			if (!CheckOverflow(nCount))
			{
				return;
			}
			for (int nIndex = m_nStackSize + nCount - 1; nIndex >= m_nStackSize; --nIndex)
			{
				m_arrStack[nIndex - nCount] = m_arrStack[nIndex];
			}
			m_nStackSize -= nCount;
		}

		void Roll(int n, int j)
		{
			int i, k;

			if (j >= 0)
			{
				j %= n;
			}
			else
			{
				j = -j % n;
				if (j != 0)
				{
					j = n - j;
				}
			}
			if (n <= 0 || j == 0)
			{
				return;
			}
			for (i = 0; i < j; ++i)
			{
				PSObject oPSObject = m_arrStack[m_nStackSize];
				for (k = m_nStackSize; k < m_nStackSize + n - 1; ++k)
				{
					m_arrStack[k] = m_arrStack[k + 1];
				}
				m_arrStack[m_nStackSize + n - 1] = oPSObject;
			}
		}

		void Index(int nIndex)
		{
			if (!CheckOverflow())
			{
				return;
			}
			--m_nStackSize;
			m_arrStack[m_nStackSize] = m_arrStack[m_nStackSize + 1 + nIndex];
		}

		void Pop()
		{
			if (!CheckUnderflow())
			{
				return;
			}
			++m_nStackSize;
		}


	private:

		bool CheckOverflow(int nCount = 1)
		{
			if (m_nStackSize - nCount < 0)
			{
				// TO DO: Error "Stack overflow in PostScript function"
				return false;
			}
			return true;
		}

		bool CheckUnderflow()
		{
			if (m_nStackSize == psStackSize)
			{
				// TO DO: Error "Stack underflow in PostScript function"
				return false;
			}
			return true;
		}

		bool CheckType(PSObjectType eType1, PSObjectType eType2)
		{
			if (m_arrStack[m_nStackSize].eType != eType1 && m_arrStack[m_nStackSize].eType != eType2)
			{
				// TO DO: Error "Type mismatch in PostScript function"
				return false;
			}
			return true;
		}

	private:

		PSObject m_arrStack[psStackSize];
		int      m_nStackSize;
	};

	PostScriptFunction::PostScriptFunction(Object *pFuncObject, Dict *pDict)
	{
		m_pCode = NULL;
		m_nCodeSize = 0;
		m_bValid = false;

		if (!Initialize(pDict))
		{
			return;
		}
		if (!m_bHasRange)
		{
			// TO DO: Error "Type 4 function is missing range"
			return;
		}

		if (!pFuncObject->IsStream())
		{
			// TO DO: Error "Type 4 function isn't a stream"
			return;
		}
		Stream *pStream = pFuncObject->GetStream();

		//----- parse the function
		m_seCodeString = new StringExt();
		pStream->Reset();

		StringExt *seToken;
		if (!(seToken = GetToken(pStream)) || seToken->Compare("{"))
		{
			// TO DO: Error "Expected '{' at start of PostScript function"
			MemUtilsFree(seToken);
			pStream->Close();
			return;
		}
		if (seToken)
			delete seToken;
		int nCodePos = 0;
		if (!ParseCode(pStream, &nCodePos))
		{
			pStream->Close();
			return;
		}
		pStream->Close();

		m_bValid = true;
	}

	PostScriptFunction::PostScriptFunction(PostScriptFunction *pFunc)
	{
		memcpy(this, pFunc, sizeof(PostScriptFunction));
		m_pCode = (PSObject *)MemUtilsMallocArray(m_nCodeSize, sizeof(PSObject));
		memcpy(m_pCode, pFunc->m_pCode, m_nCodeSize * sizeof(PSObject));
		m_seCodeString = pFunc->m_seCodeString->Copy();
	}

	PostScriptFunction::~PostScriptFunction()
	{
		MemUtilsFree(m_pCode);
		if (m_seCodeString)
			delete m_seCodeString;
	}

	void PostScriptFunction::Transform(double *pInput, double *pOutput)
	{
		PSStack *pStack = new PSStack();
		if (!pStack)
			return;

		for (int nIndex = 0; nIndex < m_nInputDim; ++nIndex)
		{
			pStack->PushReal(pInput[nIndex]);
		}
		Exec(pStack, 0);

		for (int nIndex = m_nOutputDim - 1; nIndex >= 0; --nIndex)
		{
			pOutput[nIndex] = pStack->PopNum();
			if (pOutput[nIndex] < m_arrRange[nIndex][0])
			{
				pOutput[nIndex] = m_arrRange[nIndex][0];
			}
			else if (pOutput[nIndex] > m_arrRange[nIndex][1])
			{
				pOutput[nIndex] = m_arrRange[nIndex][1];
			}
		}
		delete pStack;
	}

	bool PostScriptFunction::ParseCode(Stream *pStream, int *pnCodePos)
	{
		StringExt *seToken;

		while (1)
		{
			if (!(seToken = GetToken(pStream)))
			{
				// TO DO: Error "Unexpected end of PostScript function stream"
				return false;
			}
			if (!seToken)
				return false;

			char *pCurChar = seToken->GetBuffer();

			if (isdigit(*pCurChar) || *pCurChar == '.' || *pCurChar == '-')
			{
				bool bReal = false;
				for (++pCurChar; *pCurChar; ++pCurChar)
				{
					if (*pCurChar == '.')
					{
						bReal = true;
						break;
					}
				}
				ResizeCode(*pnCodePos);
				if (bReal)
				{
					m_pCode[*pnCodePos].eType = psReal;
					m_pCode[*pnCodePos].uReal = atof(seToken->GetBuffer());
				}
				else
				{
					m_pCode[*pnCodePos].eType = psInt;
					m_pCode[*pnCodePos].uInt = atoi(seToken->GetBuffer());
				}
				++*pnCodePos;
				delete seToken;
			}
			else if (!seToken->Compare("{"))
			{
				delete seToken;
				int nOperatorPos = *pnCodePos;
				int nElsePos = 0;
				*pnCodePos += 3;
				ResizeCode(nOperatorPos + 2);
				if (!ParseCode(pStream, pnCodePos))
				{
					return false;
				}
				if (!(seToken = GetToken(pStream)))
				{
					// TO DO: Error "Unexpected end of PostScript function stream"
					return false;
				}
				if (!seToken->Compare("{"))
				{
					nElsePos = *pnCodePos;
					if (!ParseCode(pStream, pnCodePos))
					{
						return false;
					}
					delete seToken;
					if (!(seToken = GetToken(pStream)))
					{
						// TO DO: Error "Unexpected end of PostScript function stream"
						return false;
					}
				}
				else
				{
					nElsePos = -1;
				}
				if (!seToken->Compare("if"))
				{
					if (nElsePos >= 0)
					{
						// TO DO: Error "Got 'if' operator with two blocks in PostScript function"
						return false;
					}
					m_pCode[nOperatorPos].eType = psOperator;
					m_pCode[nOperatorPos].uOperation = psOperationIf;
					m_pCode[nOperatorPos + 2].eType = psBlock;
					m_pCode[nOperatorPos + 2].uBlock = *pnCodePos;
				}
				else if (!seToken->Compare("ifelse"))
				{
					if (nElsePos < 0)
					{
						// TO DO: Error "Got 'ifelse' operator with one blocks in PostScript function"
						return false;
					}
					m_pCode[nOperatorPos].eType = psOperator;
					m_pCode[nOperatorPos].uOperation = psOperationIfelse;
					m_pCode[nOperatorPos + 1].eType = psBlock;
					m_pCode[nOperatorPos + 1].uBlock = nElsePos;
					m_pCode[nOperatorPos + 2].eType = psBlock;
					m_pCode[nOperatorPos + 2].uBlock = *pnCodePos;
				}
				else
				{
					// TO DO: Error "Expected if/ifelse operator in PostScript function"
					delete seToken;
					return false;
				}
				delete seToken;
			}
			else if (!seToken->Compare("}"))
			{
				delete seToken;
				ResizeCode(*pnCodePos);
				m_pCode[*pnCodePos].eType = psOperator;
				m_pCode[*pnCodePos].uOperation = psOperationReturn;
				++*pnCodePos;
				break;
			}
			else
			{
				int nFirst = -1;
				int nSecond = PSOperationsCount;
				// invariant: psOpNames[a] < tok < psOpNames[b]
				int nCompareRes = 0;
				while (nSecond - nFirst > 1)
				{
					int nMiddle = (nFirst + nSecond) / 2;
					nCompareRes = seToken->Compare(c_sPSOperationNames[nMiddle]);
					if (nCompareRes > 0)
					{
						nFirst = nMiddle;
					}
					else if (nCompareRes < 0)
					{
						nSecond = nMiddle;
					}
					else
					{
						nFirst = nSecond = nMiddle;
					}
				}
				if (nCompareRes != 0)
				{
					// TO DO: Error "Unknown operator in PostScript function"
					delete seToken;
					return false;
				}
				delete seToken;
				ResizeCode(*pnCodePos);
				m_pCode[*pnCodePos].eType = psOperator;
				m_pCode[*pnCodePos].uOperation = (PSOperations)nFirst;
				++*pnCodePos;
			}
		}
		return true;
	}

	StringExt *PostScriptFunction::GetToken(Stream *pStream)
	{
		int nChar = 0;

		StringExt *seResult = new StringExt();
		bool bComment = false;

		while (1)
		{
			if ((nChar = pStream->GetChar()) == EOF)
			{
				break;
			}
			m_seCodeString->Append(nChar);
			if (bComment)
			{
				if (nChar == '\x0a' || nChar == '\x0d')
				{
					bComment = false;
				}
			}
			else if (nChar == '%')
			{
				bComment = true;
			}
			else if (!isspace(nChar))
			{
				break;
			}
		}
		if (nChar == '{' || nChar == '}')
		{
			seResult->Append((char)nChar);
		}
		else if (isdigit(nChar) || nChar == '.' || nChar == '-')
		{
			while (1)
			{
				seResult->Append((char)nChar);
				nChar = pStream->LookChar();
				if (nChar == EOF || !(isdigit(nChar) || nChar == '.' || nChar == '-'))
				{
					break;
				}
				pStream->GetChar();
				m_seCodeString->Append(nChar);
			}
		}
		else
		{
			while (1)
			{
				seResult->Append((char)nChar);
				nChar = pStream->LookChar();
				if (nChar == EOF || !isalnum(nChar))
				{
					break;
				}
				pStream->GetChar();
				m_seCodeString->Append(nChar);
			}
		}
		return seResult;
	}

	void PostScriptFunction::ResizeCode(int nNewSize)
	{
		if (nNewSize >= m_nCodeSize)
		{
			m_nCodeSize += 64;
			m_pCode = (PSObject *)MemUtilsReallocArray(m_pCode, m_nCodeSize, sizeof(PSObject));
		}
	}

	void PostScriptFunction::Exec(PSStack *pStack, int codePtr)
	{
		int nInteger1 = 0, nInteger2 = 0;
		double dReal1 = 0, dReal2 = 0;
		bool bBool1 = false, bBool2 = false;

		while (1)
		{
			switch (m_pCode[codePtr].eType)
			{
				case psInt:
				pStack->PushInt(m_pCode[codePtr++].uInt);
				break;
				case psReal:
				pStack->PushReal(m_pCode[codePtr++].uReal);
				break;
				case psOperator:
				switch (m_pCode[codePtr++].uOperation)
				{
					case psOperationAbs:
					if (pStack->TopIsInt())
					{
						pStack->PushInt(abs(pStack->PopInt()));
					}
					else
					{
						pStack->PushReal(fabs(pStack->PopNum()));
					}
					break;
					case psOperationAdd:
					if (pStack->TopTwoAreInts())
					{
						nInteger2 = pStack->PopInt();
						nInteger1 = pStack->PopInt();
						pStack->PushInt(nInteger1 + nInteger2);
					}
					else
					{
						dReal2 = pStack->PopNum();
						dReal1 = pStack->PopNum();
						pStack->PushReal(dReal1 + dReal2);
					}
					break;
					case psOperationAnd:
					if (pStack->TopTwoAreInts())
					{
						nInteger2 = pStack->PopInt();
						nInteger1 = pStack->PopInt();
						pStack->PushInt(nInteger1 & nInteger2);
					}
					else
					{
						bBool2 = pStack->PopBool();
						bBool1 = pStack->PopBool();
						pStack->PushBool(bBool1 && bBool2);
					}
					break;
					case psOperationAtan:
					dReal2 = pStack->PopNum();
					dReal1 = pStack->PopNum();
					pStack->PushReal(atan2(dReal1, dReal2));
					break;
					case psOperationBitshift:
					nInteger2 = pStack->PopInt();
					nInteger1 = pStack->PopInt();
					if (nInteger2 > 0)
					{
						pStack->PushInt(nInteger1 << nInteger2);
					}
					else if (nInteger2 < 0)
					{
						pStack->PushInt((int)((unsigned int)nInteger1 >> nInteger2));
					}
					else
					{
						pStack->PushInt(nInteger1);
					}
					break;
					case psOperationCeiling:
					if (!pStack->TopIsInt())
					{
						pStack->PushReal(ceil(pStack->PopNum()));
					}
					break;
					case psOperationCopy:
					pStack->Copy(pStack->PopInt());
					break;
					case psOperationCos:
					pStack->PushReal(cos(pStack->PopNum()));
					break;
					case psOperationCvi:
					if (!pStack->TopIsInt())
					{
						pStack->PushInt((int)pStack->PopNum());
					}
					break;
					case psOperationCvr:
					if (!pStack->TopIsReal())
					{
						pStack->PushReal(pStack->PopNum());
					}
					break;
					case psOperationDiv:
					dReal2 = pStack->PopNum();
					dReal1 = pStack->PopNum();
					pStack->PushReal(dReal1 / dReal2);
					break;
					case psOperationDup:
					pStack->Copy(1);
					break;
					case psOperationEq:
					if (pStack->TopTwoAreInts())
					{
						nInteger2 = pStack->PopInt();
						nInteger1 = pStack->PopInt();
						pStack->PushBool(nInteger1 == nInteger2);
					}
					else if (pStack->TopTwoAreNums())
					{
						dReal2 = pStack->PopNum();
						dReal1 = pStack->PopNum();
						pStack->PushBool(dReal1 == dReal2);
					}
					else
					{
						bBool2 = pStack->PopBool();
						bBool1 = pStack->PopBool();
						pStack->PushBool(bBool1 == bBool2);
					}
					break;
					case psOperationExch:
					pStack->Roll(2, 1);
					break;
					case psOperationExp:
					dReal2 = pStack->PopNum();
					dReal1 = pStack->PopNum();
					pStack->PushReal(pow(dReal1, dReal2));
					break;
					case psOperationFalse:
					pStack->PushBool(false);
					break;
					case psOperationFloor:
					if (!pStack->TopIsInt())
					{
						pStack->PushReal(floor(pStack->PopNum()));
					}
					break;
					case psOperationGe:
					if (pStack->TopTwoAreInts())
					{
						nInteger2 = pStack->PopInt();
						nInteger1 = pStack->PopInt();
						pStack->PushBool(nInteger1 >= nInteger2);
					}
					else
					{
						dReal2 = pStack->PopNum();
						dReal1 = pStack->PopNum();
						pStack->PushBool(dReal1 >= dReal2);
					}
					break;
					case psOperationGt:
					if (pStack->TopTwoAreInts())
					{
						nInteger2 = pStack->PopInt();
						nInteger1 = pStack->PopInt();
						pStack->PushBool(nInteger1 > nInteger2);
					}
					else
					{
						dReal2 = pStack->PopNum();
						dReal1 = pStack->PopNum();
						pStack->PushBool(dReal1 > dReal2);
					}
					break;
					case psOperationIdiv:
					nInteger2 = pStack->PopInt();
					nInteger1 = pStack->PopInt();
					pStack->PushInt(nInteger1 / nInteger2);
					break;
					case psOperationIndex:
					pStack->Index(pStack->PopInt());
					break;
					case psOperationLe:
					if (pStack->TopTwoAreInts())
					{
						nInteger2 = pStack->PopInt();
						nInteger1 = pStack->PopInt();
						pStack->PushBool(nInteger1 <= nInteger2);
					}
					else
					{
						dReal2 = pStack->PopNum();
						dReal1 = pStack->PopNum();
						pStack->PushBool(dReal1 <= dReal2);
					}
					break;
					case psOperationLn:
					pStack->PushReal(log(pStack->PopNum()));
					break;
					case psOperationLog:
					pStack->PushReal(log10(pStack->PopNum()));
					break;
					case psOperationLt:
					if (pStack->TopTwoAreInts())
					{
						nInteger2 = pStack->PopInt();
						nInteger1 = pStack->PopInt();
						pStack->PushBool(nInteger1 < nInteger2);
					}
					else
					{
						dReal2 = pStack->PopNum();
						dReal1 = pStack->PopNum();
						pStack->PushBool(dReal1 < dReal2);
					}
					break;
					case psOperationMod:
					nInteger2 = pStack->PopInt();
					nInteger1 = pStack->PopInt();
					pStack->PushInt(nInteger1 % nInteger2);
					break;
					case psOperationMul:
					if (pStack->TopTwoAreInts())
					{
						nInteger2 = pStack->PopInt();
						nInteger1 = pStack->PopInt();
						//~ should check for out-of-range, and push a real instead
						pStack->PushInt(nInteger1 * nInteger2);
					}
					else
					{
						dReal2 = pStack->PopNum();
						dReal1 = pStack->PopNum();
						pStack->PushReal(dReal1 * dReal2);
					}
					break;
					case psOperationNe:
					if (pStack->TopTwoAreInts())
					{
						nInteger2 = pStack->PopInt();
						nInteger1 = pStack->PopInt();
						pStack->PushBool(nInteger1 != nInteger2);
					}
					else if (pStack->TopTwoAreNums())
					{
						dReal2 = pStack->PopNum();
						dReal1 = pStack->PopNum();
						pStack->PushBool(dReal1 != dReal2);
					}
					else
					{
						bBool2 = pStack->PopBool();
						bBool1 = pStack->PopBool();
						pStack->PushBool(bBool1 != bBool2);
					}
					break;
					case psOperationNeg:
					if (pStack->TopIsInt())
					{
						pStack->PushInt(-pStack->PopInt());
					}
					else
					{
						pStack->PushReal(-pStack->PopNum());
					}
					break;
					case psOperationNot:
					if (pStack->TopIsInt())
					{
						pStack->PushInt(~pStack->PopInt());
					}
					else
					{
						pStack->PushBool(!pStack->PopBool());
					}
					break;
					case psOperationOr:
					if (pStack->TopTwoAreInts())
					{
						nInteger2 = pStack->PopInt();
						nInteger1 = pStack->PopInt();
						pStack->PushInt(nInteger1 | nInteger2);
					}
					else
					{
						bBool2 = pStack->PopBool();
						bBool1 = pStack->PopBool();
						pStack->PushBool(bBool1 || bBool2);
					}
					break;
					case psOperationPop:
					pStack->Pop();
					break;
					case psOperationRoll:
					nInteger2 = pStack->PopInt();
					nInteger1 = pStack->PopInt();
					pStack->Roll(nInteger1, nInteger2);
					break;
					case psOperationRound:
					if (!pStack->TopIsInt())
					{
						dReal1 = pStack->PopNum();
						pStack->PushReal((dReal1 >= 0) ? floor(dReal1 + 0.5) : ceil(dReal1 - 0.5));
					}
					break;
					case psOperationSin:
					pStack->PushReal(sin(pStack->PopNum()));
					break;
					case psOperationSqrt:
					pStack->PushReal(sqrt(pStack->PopNum()));
					break;
					case psOperationSub:
					if (pStack->TopTwoAreInts())
					{
						nInteger2 = pStack->PopInt();
						nInteger1 = pStack->PopInt();
						pStack->PushInt(nInteger1 - nInteger2);
					}
					else
					{
						dReal2 = pStack->PopNum();
						dReal1 = pStack->PopNum();
						pStack->PushReal(dReal1 - dReal2);
					}
					break;
					case psOperationTrue:
					pStack->PushBool(true);
					break;
					case psOperationTruncate:
					if (!pStack->TopIsInt())
					{
						dReal1 = pStack->PopNum();
						pStack->PushReal((dReal1 >= 0) ? floor(dReal1) : ceil(dReal1));
					}
					break;
					case psOperationXor:
					if (pStack->TopTwoAreInts())
					{
						nInteger2 = pStack->PopInt();
						nInteger1 = pStack->PopInt();
						pStack->PushInt(nInteger1 ^ nInteger2);
					}
					else
					{
						bBool2 = pStack->PopBool();
						bBool1 = pStack->PopBool();
						pStack->PushBool(bBool1 ^ bBool2);
					}
					break;
					case psOperationIf:
					bBool1 = pStack->PopBool();
					if (bBool1)
					{
						Exec(pStack, codePtr + 2);
					}
					codePtr = m_pCode[codePtr + 1].uBlock;
					break;
					case psOperationIfelse:
					bBool1 = pStack->PopBool();
					if (bBool1)
					{
						Exec(pStack, codePtr + 2);
					}
					else
					{
						Exec(pStack, m_pCode[codePtr].uBlock);
					}
					codePtr = m_pCode[codePtr + 1].uBlock;
					break;
					case psOperationReturn:
					return;
				}
				break;
				default:
				// TO DO: Error "Internal: bad object in PostScript function code"
				break;
			}
		}
	}
}