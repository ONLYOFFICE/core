// AVSMPEG2VideoFormat.h : Declaration of the CAVSMPEG2VideoFormat

#pragma once
#include "resource.h"       // main symbols
//#include "ATLDefine.h"
//#include "
#include "AVSVideoFormat.h"

class CAVSMPEG2VideoFormat;
// IAVSMPEG2VideoFormat
[ object, uuid("F6EB57E2-EADD-4D7D-A8C3-3D53023E8DC4"), dual, pointer_default(unique)]
__interface IAVSMPEG2VideoFormat : IAVSVideoFormat
{
	[id(104000 + 1), propget] HRESULT MPEGVersion([out, retval] long* pVal);
	[id(104000 + 1), propput] HRESULT MPEGVersion([in] long newVal);
	 
	[id(104000 + 2), propget] HRESULT Distance_I_I([out, retval] long* pVal);
	[id(104000 + 2), propput] HRESULT Distance_I_I([in] long newVal);
	[id(104000 + 3), propget] HRESULT Distance_I_P([out, retval] long* pVal);
	[id(104000 + 3), propput] HRESULT Distance_I_P([in] long newVal);
	 
	[id(104000 + 4), propget] HRESULT PullDown([out, retval] long* pVal);
	[id(104000 + 4), propput] HRESULT PullDown([in] long newVal);
	//[id(104000 + 5), propget] HRESULT ChromaFormat([out, retval] long* pVal);
	//[id(104000 + 5), propput] HRESULT ChromaFormat([in] long newVal);
	[id(104000 + 6), propget] HRESULT ConstantBitrate([out, retval] VARIANT_BOOL* pVal);
	[id(104000 + 6), propput] HRESULT ConstantBitrate([in] VARIANT_BOOL newVal);
	[id(104000 + 7), propget] HRESULT FastMotionCompensationLevel([out, retval] long* pVal);
	[id(104000 + 7), propput] HRESULT FastMotionCompensationLevel([in] long newVal);
	 
	[id(104000 + 8), propget] HRESULT ProfileID([out, retval] long* pVal);
	[id(104000 + 8), propput] HRESULT ProfileID([in] long newVal);
	[id(104000 + 9), propget] HRESULT LevelID([out, retval] long* pVal);
	[id(104000 + 9), propput] HRESULT LevelID([in] long newVal);
	[id(104000 + 10), propget] HRESULT LowDelay([out, retval] VARIANT_BOOL* pVal);
	[id(104000 + 10), propput] HRESULT LowDelay([in] VARIANT_BOOL newVal);
	[id(104000 + 11), propget] HRESULT RepeatFirstField([out, retval] VARIANT_BOOL* pVal);
	[id(104000 + 11), propput] HRESULT RepeatFirstField([in] VARIANT_BOOL newVal);
	[id(104000 + 12), propget] HRESULT FieldPicture([out, retval] VARIANT_BOOL* pVal);
	[id(104000 + 12), propput] HRESULT FieldPicture([in] VARIANT_BOOL newVal);
	 
	[id(104000 + 13), propget] HRESULT InterlacingEnabled([out, retval] VARIANT_BOOL* pVal);
	[id(104000 + 13), propput] HRESULT InterlacingEnabled([in] VARIANT_BOOL newVal);
	[id(104000 + 14), propget] HRESULT TffEnabled([out, retval] VARIANT_BOOL* pVal);
	[id(104000 + 14), propput] HRESULT TffEnabled([in] VARIANT_BOOL newVal);
	 
	[id(104000 + 15), propget] HRESULT QuantI([out, retval] long* pVal);
	[id(104000 + 15), propput] HRESULT QuantI([in] long newVal);
	[id(104000 + 16), propget] HRESULT QuantP([out, retval] long* pVal);
	[id(104000 + 16), propput] HRESULT QuantP([in] long newVal);
	[id(104000 + 17), propget] HRESULT QuantB([out, retval] long* pVal);
	[id(104000 + 17), propput] HRESULT QuantB([in] long newVal);
	[id(104000 + 18), propget] HRESULT minQuantI([out, retval] long* pVal);
	[id(104000 + 18), propput] HRESULT minQuantI([in] long newVal);
	[id(104000 + 19), propget] HRESULT maxQuantI([out, retval] long* pVal);
	[id(104000 + 19), propput] HRESULT maxQuantI([in] long newVal);
	[id(104000 + 20), propget] HRESULT minQuantP([out, retval] long* pVal);
	[id(104000 + 20), propput] HRESULT minQuantP([in] long newVal);
	[id(104000 + 21), propget] HRESULT maxQuantP([out, retval] long* pVal);
	[id(104000 + 21), propput] HRESULT maxQuantP([in] long newVal);
	[id(104000 + 22), propget] HRESULT minQuantB([out, retval] long* pVal);
	[id(104000 + 22), propput] HRESULT minQuantB([in] long newVal);
	[id(104000 + 23), propget] HRESULT maxQuantB([out, retval] long* pVal);
	[id(104000 + 23), propput] HRESULT maxQuantB([in] long newVal);
	 
	[id(104000 + 24), propget] HRESULT UseQI([out, retval] VARIANT_BOOL* pVal);
	[id(104000 + 24), propput] HRESULT UseQI([in] VARIANT_BOOL newVal);
	[id(104000 + 25), propget] HRESULT UseQP([out, retval] VARIANT_BOOL* pVal);
	[id(104000 + 25), propput] HRESULT UseQP([in] VARIANT_BOOL newVal);
	[id(104000 + 26), propget] HRESULT UseQB([out, retval] VARIANT_BOOL* pVal);
	[id(104000 + 26), propput] HRESULT UseQB([in] VARIANT_BOOL newVal);
	 
	[id(104000 + 27), propget] HRESULT DisplayWidth([out, retval] long* pVal);
	[id(104000 + 27), propput] HRESULT DisplayWidth([in] long newVal);
	[id(104000 + 28), propget] HRESULT DisplayHeight([out, retval] long* pVal);
	[id(104000 + 28), propput] HRESULT DisplayHeight([in] long newVal);

	[id(104000 + 29), propget] HRESULT VBVBufferSize([out, retval] long* pVal);
	[id(104000 + 29), propput] HRESULT VBVBufferSize([in] long newVal);

	[id(104000 + 30), propget] HRESULT ColorDescription([out, retval] VARIANT_BOOL* pVal);
	[id(104000 + 30), propput] HRESULT ColorDescription([in] VARIANT_BOOL newVal);
	[id(104000 + 31), propget] HRESULT ColorPrimaries([out, retval] long* pVal);
	[id(104000 + 31), propput] HRESULT ColorPrimaries([in] long newVal);
	[id(104000 + 32), propget] HRESULT TransferCharacteristics([out, retval] long* pVal);
	[id(104000 + 32), propput] HRESULT TransferCharacteristics([in] long newVal);
	[id(104000 + 33), propget] HRESULT MatrixCoefficients([out, retval] long* pVal);
	[id(104000 + 33), propput] HRESULT MatrixCoefficients([in] long newVal);
	

	[id(104000 + 35), propget] HRESULT IntraQuantizerMatrix([out, retval, satype("BYTE")] SAFEARRAY** pVal);
	[id(104000 + 35), propput] HRESULT IntraQuantizerMatrix([in, satype("BYTE")] SAFEARRAY* newVal);

	[id(104000 + 37), propget] HRESULT NonIntraQuantizerMatrix([out, retval, satype("BYTE")] SAFEARRAY** pVal);
	[id(104000 + 37), propput] HRESULT NonIntraQuantizerMatrix([in, satype("BYTE")] SAFEARRAY* newVal);

	[id(104000 + 38), propget] HRESULT SequenceHeader([out, retval, satype("BYTE")] SAFEARRAY** pVal);
	[id(104000 + 38), propput] HRESULT SequenceHeader([in, satype("BYTE")] SAFEARRAY* newVal);
};

template <class CoT, class T>
class CAVSMPEG2VideoFormatImpl : public CAVSVideoFormatImpl<CoT, T>
{
public:
	CAVSMPEG2VideoFormatImpl()
	{
		m_lMediaType = MEDIAFORMAT_MPEG2;
		SetDefaultProperties();
	}
	~CAVSMPEG2VideoFormatImpl()
	{
		RELEASEARRAY(m_pIntraQuantizerMatrix);
		RELEASEARRAY(m_pNonIntraQuantizerMatrix);
		RELEASEARRAY(m_pSeqHeader);
	}
	STDMETHOD(CreateDuplicate)(IAVSMediaFormat** pMediaFormat)
	{
		if (NULL==pMediaFormat)
			return S_OK;

		CAVSVideoFormatImpl<CoT,T>::CreateDuplicate(pMediaFormat);
		if (NULL==(*pMediaFormat))
			return S_OK;
		IAVSMPEG2VideoFormat* pAVSMPEG2Format=NULL;
		(*pMediaFormat)->QueryInterface(__uuidof(IAVSMPEG2VideoFormat), (void**)&pAVSMPEG2Format);
		if (NULL==pAVSMPEG2Format)
		{
			(*pMediaFormat)->Release();
			*pMediaFormat = NULL;
			return S_OK;
		}

		pAVSMPEG2Format->put_MPEGVersion(m_lMPEGVersion);
		pAVSMPEG2Format->put_Distance_I_I(m_lDistance_I_I);
		pAVSMPEG2Format->put_Distance_I_P(m_lDistance_I_P);

		pAVSMPEG2Format->put_PullDown(m_lPullDown);
		//pAVSMPEG2Format->put_ChromaFormat(m_lChromaFormat);
		pAVSMPEG2Format->put_ConstantBitrate((m_bConstantBitrate ? VARIANT_TRUE : VARIANT_FALSE));
		pAVSMPEG2Format->put_FastMotionCompensationLevel(m_lFastMotionCompensationLevel);

		pAVSMPEG2Format->put_ProfileID(m_lProfileID);
		pAVSMPEG2Format->put_LevelID(m_lLevelID);
		pAVSMPEG2Format->put_LowDelay((m_bLowDelay ? VARIANT_TRUE : VARIANT_FALSE));
		pAVSMPEG2Format->put_RepeatFirstField((m_bRepeatFirstField ? VARIANT_TRUE : VARIANT_FALSE));
		pAVSMPEG2Format->put_FieldPicture((m_bFieldPicture ? VARIANT_TRUE : VARIANT_FALSE));

		pAVSMPEG2Format->put_InterlacingEnabled((m_bInterlacingEnabled ? VARIANT_TRUE : VARIANT_FALSE));
		pAVSMPEG2Format->put_TffEnabled((m_bTffEnabled ? VARIANT_TRUE : VARIANT_FALSE));

		pAVSMPEG2Format->put_QuantI(m_lQuantI);
		pAVSMPEG2Format->put_QuantP(m_lQuantP);
		pAVSMPEG2Format->put_QuantB(m_lQuantB);
		pAVSMPEG2Format->put_minQuantI(m_lminQuantI);
		pAVSMPEG2Format->put_maxQuantI(m_lmaxQuantI);
		pAVSMPEG2Format->put_minQuantP(m_lminQuantP);
		pAVSMPEG2Format->put_maxQuantP(m_lmaxQuantP);
		pAVSMPEG2Format->put_minQuantB(m_lminQuantB);
		pAVSMPEG2Format->put_maxQuantB(m_lmaxQuantB);

		pAVSMPEG2Format->put_UseQI((m_bUseQI ? VARIANT_TRUE : VARIANT_FALSE));
		pAVSMPEG2Format->put_UseQP((m_bUseQP ? VARIANT_TRUE : VARIANT_FALSE));
		pAVSMPEG2Format->put_UseQB((m_bUseQB ? VARIANT_TRUE : VARIANT_FALSE));

		pAVSMPEG2Format->put_DisplayWidth(m_lDisplayWidth);
		pAVSMPEG2Format->put_DisplayHeight(m_lDisplayHeight);

		pAVSMPEG2Format->put_VBVBufferSize(m_lVBVBufferSize);

		pAVSMPEG2Format->put_ColorDescription(m_bColorDescription);
		pAVSMPEG2Format->put_ColorPrimaries(m_lColourPrimaries);
		pAVSMPEG2Format->put_TransferCharacteristics(m_lTransferCharacteristics);
		pAVSMPEG2Format->put_MatrixCoefficients(m_lMatrixCoefficients);

		pAVSMPEG2Format->put_IntraQuantizerMatrix(m_pIntraQuantizerMatrix);
		pAVSMPEG2Format->put_NonIntraQuantizerMatrix(m_pNonIntraQuantizerMatrix);
		pAVSMPEG2Format->put_SequenceHeader(m_pSeqHeader);
		pAVSMPEG2Format->Release();
		return S_OK;
	}
	STDMETHOD(SetDefaultProperties)(void)
	{
		CAVSVideoFormatImpl<CoT, T>::SetDefaultProperties();
		m_ulfccHandler = MPG2Handler;
		m_lMPEGVersion = 2;
		m_lDistance_I_I = 15;
		m_lDistance_I_P = 3;
		
		m_lPullDown = 0;
		//m_lChromaFormat = 1;
		m_bConstantBitrate = VARIANT_TRUE;
		m_lFastMotionCompensationLevel = 0;
		
		m_lProfileID = 4;
		m_lLevelID = 8;
		m_bLowDelay = VARIANT_FALSE;
		m_bRepeatFirstField = VARIANT_FALSE;
		m_bFieldPicture = VARIANT_FALSE;
		
		m_bInterlacingEnabled = VARIANT_FALSE;
		m_bTffEnabled = VARIANT_FALSE;
		
		m_lQuantI = 10;
		m_lQuantP = 14;
		m_lQuantB = 20;
		m_lminQuantI = 2;
		m_lmaxQuantI = 62;
		m_lminQuantP = 2;
		m_lmaxQuantP = 62;
		m_lminQuantB = 2;
		m_lmaxQuantB = 62;
		
		m_bUseQI = VARIANT_TRUE;
		m_bUseQP = VARIANT_TRUE;
		m_bUseQB = VARIANT_TRUE;
		
		m_lDisplayWidth = 0;
		m_lDisplayHeight = 0;

		m_lVBVBufferSize = 0;
		m_bColorDescription = VARIANT_FALSE;
		m_lColourPrimaries = 0;
		m_lTransferCharacteristics = 0;
		m_lMatrixCoefficients = 0;

		m_pIntraQuantizerMatrix = NULL;
		m_pNonIntraQuantizerMatrix = NULL;
		m_pSeqHeader = NULL;
		return S_OK;
	}

	STDMETHOD(SetPropertiesByFormat)(IAVSMediaFormat* pMediaFormat)
	{
		CAVSVideoFormatImpl<CoT, T>::SetPropertiesByFormat(pMediaFormat);
		if (pMediaFormat!=NULL)
		{
			IAVSMPEG2VideoFormat*	pAVSMPEG2Format=NULL;
			pMediaFormat->QueryInterface(__uuidof(IAVSMPEG2VideoFormat), (void**)&pAVSMPEG2Format);
			if (pAVSMPEG2Format!=NULL)
			{
				pAVSMPEG2Format->get_MPEGVersion(&m_lMPEGVersion);
				pAVSMPEG2Format->get_Distance_I_I(&m_lDistance_I_I);
				pAVSMPEG2Format->get_Distance_I_P(&m_lDistance_I_P);

				pAVSMPEG2Format->get_PullDown(&m_lPullDown);
				//pAVSMPEG2Format->get_ChromaFormat(&m_lChromaFormat);
				pAVSMPEG2Format->get_ConstantBitrate(&m_bConstantBitrate);
				pAVSMPEG2Format->get_FastMotionCompensationLevel(&m_lFastMotionCompensationLevel);

				pAVSMPEG2Format->get_ProfileID(&m_lProfileID);
				pAVSMPEG2Format->get_LevelID(&m_lLevelID);
				pAVSMPEG2Format->get_LowDelay(&m_bLowDelay);
				pAVSMPEG2Format->get_RepeatFirstField(&m_bRepeatFirstField);
				pAVSMPEG2Format->get_FieldPicture(&m_bFieldPicture);

				pAVSMPEG2Format->get_InterlacingEnabled(&m_bInterlacingEnabled);
				pAVSMPEG2Format->get_TffEnabled(&m_bTffEnabled);

				pAVSMPEG2Format->get_QuantI(&m_lQuantI);
				pAVSMPEG2Format->get_QuantP(&m_lQuantP);
				pAVSMPEG2Format->get_QuantB(&m_lQuantB);
				pAVSMPEG2Format->get_minQuantI(&m_lminQuantI);
				pAVSMPEG2Format->get_maxQuantI(&m_lmaxQuantI);
				pAVSMPEG2Format->get_minQuantP(&m_lminQuantP);
				pAVSMPEG2Format->get_maxQuantP(&m_lmaxQuantP);
				pAVSMPEG2Format->get_minQuantB(&m_lminQuantB);
				pAVSMPEG2Format->get_maxQuantB(&m_lmaxQuantB);

				pAVSMPEG2Format->get_UseQI(&m_bUseQI);
				pAVSMPEG2Format->get_UseQP(&m_bUseQP);
				pAVSMPEG2Format->get_UseQB(&m_bUseQB);

				pAVSMPEG2Format->get_DisplayWidth(&m_lDisplayWidth);
				pAVSMPEG2Format->get_DisplayHeight(&m_lDisplayHeight);

				pAVSMPEG2Format->get_VBVBufferSize(&m_lVBVBufferSize);

				pAVSMPEG2Format->get_ColorDescription(&m_bColorDescription);
				pAVSMPEG2Format->get_ColorPrimaries(&m_lColourPrimaries);
				pAVSMPEG2Format->get_TransferCharacteristics(&m_lTransferCharacteristics);
				pAVSMPEG2Format->get_MatrixCoefficients(&m_lMatrixCoefficients);

				pAVSMPEG2Format->get_IntraQuantizerMatrix(&m_pIntraQuantizerMatrix);
				pAVSMPEG2Format->get_NonIntraQuantizerMatrix(&m_pNonIntraQuantizerMatrix);
				pAVSMPEG2Format->get_SequenceHeader(&m_pSeqHeader);
				RELEASEINTERFACE(pAVSMPEG2Format);
			}
		}
		return S_OK;
	}
	STDMETHOD(SetAdditionalParam)(BSTR ParamName, VARIANT ParamValue)
	{
		//Здесь проверяем параметры относящиеся к AVSMPEG2VideoFormat
		return CAVSVideoFormatImpl<CoT, T>::SetAdditionalParam(ParamName, ParamValue);
	}
	STDMETHOD(GetAdditionalParam)(BSTR ParamName, VARIANT* ParamValue)
	{
		//Здесь проверяем параметры относящиеся к AVSMPEG2VideoFormat
		return CAVSVideoFormatImpl<CoT, T>::GetAdditionalParam(ParamName, ParamValue);
	}
	//////////////////////////////////////
	LINK_PROPERTY_LONG(MPEGVersion,	m_lMPEGVersion)
	 
	LINK_PROPERTY_LONG(Distance_I_I, m_lDistance_I_I)
	LINK_PROPERTY_LONG(Distance_I_P, m_lDistance_I_P)
		 
	LINK_PROPERTY_LONG(PullDown, m_lPullDown)
//	LINK_PROPERTY_LONG(ChromaFormat, m_lChromaFormat)
	LINK_PROPERTY_VARIANT_BOOL(ConstantBitrate, m_bConstantBitrate)
	LINK_PROPERTY_LONG(FastMotionCompensationLevel, m_lFastMotionCompensationLevel)
	 
	LINK_PROPERTY_LONG(ProfileID, m_lProfileID)
	LINK_PROPERTY_LONG(LevelID, m_lLevelID)
	LINK_PROPERTY_VARIANT_BOOL(LowDelay, m_bLowDelay)
	LINK_PROPERTY_VARIANT_BOOL(RepeatFirstField, m_bRepeatFirstField)
	LINK_PROPERTY_VARIANT_BOOL(FieldPicture, m_bFieldPicture)
	 
	LINK_PROPERTY_VARIANT_BOOL(InterlacingEnabled, m_bInterlacingEnabled)
	LINK_PROPERTY_VARIANT_BOOL(TffEnabled, m_bTffEnabled)
		 

	LINK_PROPERTY_LONG(QuantI, m_lQuantI)
	LINK_PROPERTY_LONG(QuantP, m_lQuantP)
	LINK_PROPERTY_LONG(QuantB, m_lQuantB)

	LINK_PROPERTY_LONG(minQuantI, m_lminQuantI)
	LINK_PROPERTY_LONG(maxQuantI, m_lmaxQuantI)
	LINK_PROPERTY_LONG(minQuantP, m_lminQuantP)
	LINK_PROPERTY_LONG(maxQuantP, m_lmaxQuantP)
	LINK_PROPERTY_LONG(minQuantB, m_lminQuantB)
	LINK_PROPERTY_LONG(maxQuantB, m_lmaxQuantB)
	 
	LINK_PROPERTY_VARIANT_BOOL(UseQI, m_bUseQI)
	LINK_PROPERTY_VARIANT_BOOL(UseQP, m_bUseQP)
	LINK_PROPERTY_VARIANT_BOOL(UseQB, m_bUseQB)

	LINK_PROPERTY_LONG(DisplayWidth, m_lDisplayWidth)
	LINK_PROPERTY_LONG(DisplayHeight, m_lDisplayHeight)

	LINK_PROPERTY_LONG(VBVBufferSize, m_lVBVBufferSize);

	LINK_PROPERTY_VARIANT_BOOL(ColorDescription, m_bColorDescription);
	LINK_PROPERTY_LONG(ColorPrimaries, m_lColourPrimaries);
	LINK_PROPERTY_LONG(TransferCharacteristics, m_lTransferCharacteristics);
	LINK_PROPERTY_LONG(MatrixCoefficients, m_lMatrixCoefficients);

	LINK_PROPERTY_SAFEARRAY(IntraQuantizerMatrix, m_pIntraQuantizerMatrix);
	LINK_PROPERTY_SAFEARRAY(NonIntraQuantizerMatrix, m_pNonIntraQuantizerMatrix);
	LINK_PROPERTY_SAFEARRAY(SequenceHeader, m_pSeqHeader);

protected:
//Encoder
	long m_lPullDown;
	VARIANT_BOOL m_bConstantBitrate;
	long m_lFastMotionCompensationLevel;
	
	long m_lDistance_I_I;
	long m_lDistance_I_P;

	long m_lQuantI;
	long m_lQuantP;
	long m_lQuantB;
	long m_lminQuantI;
	long m_lmaxQuantI;
	long m_lminQuantP;
	long m_lmaxQuantP;
	long m_lminQuantB;
	long m_lmaxQuantB;
	
	VARIANT_BOOL	m_bUseQI;
	VARIANT_BOOL	m_bUseQP;
	VARIANT_BOOL	m_bUseQB;

	VARIANT_BOOL	m_bFieldPicture;

//Decoder
	long			m_lProfileID;
	long			m_lLevelID;
	long			m_lDisplayWidth;
	long			m_lDisplayHeight;
	VARIANT_BOOL 	m_bLowDelay;//SEQ_FLAG_LOW_DELAY
//
	long			m_lMPEGVersion;	//SEQ_FLAG_MPEG2
	VARIANT_BOOL	m_bInterlacingEnabled;	//Progressive Sequnce/frame
//Picture
	VARIANT_BOOL	m_bRepeatFirstField;
	VARIANT_BOOL	m_bTffEnabled;

//From Decoder
	long			m_lVBVBufferSize;
	VARIANT_BOOL	m_bColorDescription;
	long			m_lColourPrimaries;
	long			m_lTransferCharacteristics;
	long			m_lMatrixCoefficients;

	SAFEARRAY*		m_pIntraQuantizerMatrix;
	SAFEARRAY*		m_pNonIntraQuantizerMatrix;

	SAFEARRAY*		m_pSeqHeader;
};

// CAVSMPEG2VideoFormat

[coclass, uuid("ABB081C7-9765-4225-BC71-0B3F521D143F"), threading(apartment), vi_progid("AVSMediaFormatSettings3.AVSMPEG2VideoFo"), progid("AVSMediaFormatSettings3.AVSMPEG2Video.1"), version(1.0)]
class ATL_NO_VTABLE CAVSMPEG2VideoFormat : public CAVSMPEG2VideoFormatImpl<CAVSMPEG2VideoFormat, IAVSMPEG2VideoFormat>
{
};

