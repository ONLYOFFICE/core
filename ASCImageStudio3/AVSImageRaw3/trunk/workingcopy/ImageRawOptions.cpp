/*! \file	ImageRawOptions.cpp
 *	\brief	Настройки raw-конвертера
 *
 *	\author	Nikolay.Pogorskiy@avsmedia.net
 */

#include "stdafx.h"
#include "ImageRawOptions.h"

#pragma comment(lib,"comsuppw.lib");


/*!
 *	\brief Вспомогательная функция для получения значения атрибута use, границ и параметра
 */
bool GetParamsByNameI(XML::XMLNodeWrapper & node, BSTR name,bool & use, int & value, int & min, int & max)
{
	XML::XMLNodeWrapper tmpNode = node.FindNode(name);
	if (!tmpNode)
		return false;

	BSTR bsVal = NULL; 
	tmpNode.GetAttributeValue(OLESTR("use"),&bsVal,OLESTR("1"));
	if (!wcscmp(_bstr_t(bsVal),_bstr_t(L"true")))
		use = true;
	else
		use = (bool)(_wtoi(bsVal));

	::SysFreeString(bsVal);

	tmpNode.GetAttributeValue(OLESTR("min"),&bsVal,OLESTR("0"));
	min = (int)(_wtoi(bsVal));
	::SysFreeString(bsVal);

	tmpNode.GetAttributeValue(OLESTR("max"),&bsVal,OLESTR("0"));
	max = (int)(_wtoi(bsVal));
	::SysFreeString(bsVal);

	tmpNode.GetText(&bsVal);
	value = _wtoi(bsVal);
	::SysFreeString(bsVal);
	return true;
}

/*!
 *	\brief Вспомогательная функция для получения значения атрибута use и параметра
 */
bool GetParamsByNameI(XML::XMLNodeWrapper & node, BSTR name,bool & use, int & value)
{
	XML::XMLNodeWrapper tmpNode = node.FindNode(name);
	if (!tmpNode)
		return false;

	BSTR bsVal = NULL;
	tmpNode.GetAttributeValue(OLESTR("use"),&bsVal,OLESTR("1"));
	if (!wcscmp(_bstr_t(bsVal),_bstr_t(L"true")))
		use = true;
	else
		use = (bool)(_wtoi(bsVal));

	::SysFreeString(bsVal);
	tmpNode.GetText(&bsVal);
	value = _wtoi(bsVal);
	::SysFreeString(bsVal);
	return true;
}

void SetAttrib(XML::XMLNodeWrapper & node,BSTR name,float value)
{
	VARIANT variant;
	variant.vt = VT_R4;
	variant.fltVal = value;
	node.SetAttributeValue(name,variant);
}

void SetAttrib(XML::XMLNodeWrapper & node,BSTR name,bool value)
{
	VARIANT variant;
	variant.vt = VT_BOOL;
	variant.boolVal = (VARIANT_BOOL)value;
	
	node.SetAttributeValue(name,variant);
	
}

void SetAttrib(XML::XMLNodeWrapper & node,BSTR name,int value)
{
	VARIANT variant;
	variant.vt = VT_I4;
	variant.intVal = value;
	node.SetAttributeValue(name,variant);

}

void SetAttrib(XML::XMLNodeWrapper & node,BSTR name,BSTR value)
{
	VARIANT variant;
	variant.vt = VT_BSTR;
	variant.bstrVal = ::SysAllocString(value);
	node.SetAttributeValue(name,variant);
	::SysFreeString(variant.bstrVal);
}



/*!
 *	Заменяем запятую на точку в строковой записи float-числа
 */

double __wtof(const wchar_t * _Str)
{
	double _res;
	wchar_t * floatingpoint, *_copy = _wcsdup(_Str);
	if (!_copy)
		return 0;
	floatingpoint = wcschr(_copy,L',');
	if (floatingpoint)
		*floatingpoint = L'.';
	_res = _wtof(_copy);
	free(_copy);
	return _res;
}

/*!
 *	\brief Вспомогательная функция для получения значения атрибута use, границ и параметра
 */
bool GetParamsByNameF(XML::XMLNodeWrapper & node, BSTR name,bool & use, float & value, float & min, float & max)
{
	XML::XMLNodeWrapper tmpNode = node.FindNode(name);
	if (!tmpNode)
		return false;

	BSTR bsVal = NULL;
	tmpNode.GetAttributeValue(OLESTR("use"),&bsVal,OLESTR("1"));
	
	if (!wcscmp(_bstr_t(bsVal),_bstr_t(L"true")))
		use = true;
	else
		use = (bool)(_wtoi(bsVal));


	::SysFreeString(bsVal);

	tmpNode.GetAttributeValue(OLESTR("min"),&bsVal,OLESTR("0"));
	min = (float)(__wtof(bsVal));
	::SysFreeString(bsVal);

	tmpNode.GetAttributeValue(OLESTR("max"),&bsVal,OLESTR("0"));
	max = (float)(__wtof(bsVal));
	::SysFreeString(bsVal);


	tmpNode.GetText(&bsVal);
	value = __wtof(bsVal);
	::SysFreeString(bsVal);
	return true;
}

/*!
 *	\brief Вспомогательная функция для получения значения атрибута use, границ и параметра
 */
bool GetParamsByNameF(XML::XMLNodeWrapper & node, BSTR name,bool & use, float & value)
{
	XML::XMLNodeWrapper tmpNode = node.FindNode(name);
	if (!tmpNode)
		return false;

	BSTR bsVal = NULL;
	tmpNode.GetAttributeValue(OLESTR("use"),&bsVal,OLESTR("1"));
	
	if (!wcscmp(_bstr_t(bsVal),_bstr_t(L"true")))
		use = true;
	else
		use = (bool)(_wtoi(bsVal));


	::SysFreeString(bsVal);
	tmpNode.GetText(&bsVal);
	value = __wtof(bsVal);
	::SysFreeString(bsVal);
	return true;
}


/*!
 *	\brief Вспомогательная функция для получения значения атрибута use
 */
bool GetUseByName(XML::XMLNodeWrapper & node, BSTR name,bool & use)
{
	XML::XMLNodeWrapper tmpNode = node.FindNode(name);
	if (!tmpNode)
		return false;

	BSTR bsVal = NULL;
	tmpNode.GetAttributeValue(OLESTR("use"),&bsVal,OLESTR("1"));

	if (!wcscmp(_bstr_t(bsVal),_bstr_t(L"true")))
		use = true;
	else
		use = (bool)(_wtoi(bsVal));


	::SysFreeString(bsVal);
	return true;
}

void SetText_(XML::XMLNodeWrapper & node,float val)
{
	char buf_[255];
	_gcvt_s(buf_,255,val,5);
	node.SetText(_bstr_t(buf_));
}

void SetText_(XML::XMLNodeWrapper & node,int val)
{
	char buf_[255];
	_itoa_s(val,buf_,255,10);
	node.SetText(_bstr_t(buf_));
}

void SetText_(XML::XMLNodeWrapper & node,BSTR val)
{
	node.SetText(_bstr_t(val));
}

void SetText_(XML::XMLNodeWrapper & node,bool val)
{
	if (val)
		node.SetText(_bstr_t(L"1"));
	else
		node.SetText(_bstr_t(L"0"));
}




#define SET_MAX_ATTR(VAL)	SetAttrib(tmpNode,OLESTR("max"),(VAL))
#define SET_MIN_ATTR(VAL)	SetAttrib(tmpNode,OLESTR("min"),(VAL))

#define SET_MAX_ATTR2(VAL)	SetAttrib(tmpNode2,OLESTR("max"),(VAL))
#define SET_MIN_ATTR2(VAL)	SetAttrib(tmpNode2,OLESTR("min"),(VAL))

#define SET_TEXT2(VAL)		SetText_(tmpNode2,(VAL));//tmpNode2.SetText(_bstr_t(VAL))
#define SET_TEXT(VAL)		SetText_(tmpNode,(VAL));//tmpNode.SetText(_bstr_t(VAL))

#define _SUBNODE(VAL)		tmpNode2 = tmpNode.AppendNode(VAL);
#define _NODE(VAL,USE)		tmpNode2 = tmpNode = paramRoot.AppendNode(VAL); SetAttrib(tmpNode,OLESTR("use"),(int)(USE));



HRESULT ImageRaw3SimpleModeOptions::SetXMLDoc(XML::XMLDocumentWrapper & xmlDoc)
{
//	XML::XMLDocumentWrapper xmlDoc;
	XML::XMLNodeWrapper		tmpNode,tmpNode2,paramRoot;
	
	xmlDoc.Clear();
	tmpNode = xmlDoc.AppendNode(OLESTR("ImageRaw3"));
	SetAttrib(tmpNode,OLESTR("mode"),0);
	SetAttrib(tmpNode,OLESTR("xmlns"),OLESTR("http://tempuri.org/ImageRaw3Options.xsd"));

	paramRoot = tmpNode.AppendNode(OLESTR("SimpleMode"));
	SetAttrib(paramRoot,OLESTR("ver"),Version);

	_NODE(OLESTR("CameraWhiteBalance"),UseCameraWhiteBalance);
	_NODE(OLESTR("AutoWhiteBalance"),UseAutoWhiteBalance);
	_NODE(OLESTR("AverageGrayBox"),UseAverageGrayBox);

	_SUBNODE(OLESTR("X"));
	SET_MAX_ATTR2(AverageGrayBox.max_x);
	SET_TEXT2(AverageGrayBox.x);
	
	_SUBNODE(OLESTR("Y"));
	SET_MAX_ATTR2(AverageGrayBox.max_y);
	SET_TEXT2(AverageGrayBox.y);
	
	_SUBNODE(OLESTR("W"));
	SET_MAX_ATTR2(AverageGrayBox.max_w);
	SET_TEXT2(AverageGrayBox.w);
	
	_SUBNODE(OLESTR("H"));
	SET_MAX_ATTR2(AverageGrayBox.max_h);
	SET_TEXT2(AverageGrayBox.h);

	_NODE(OLESTR("CustomWhiteBalance"),UseCustomWhiteBalance);
	
	_SUBNODE(OLESTR("R"));
	SET_MAX_ATTR2(CustomWhiteBalance.max_r);
	SET_MIN_ATTR2(CustomWhiteBalance.min_r);
	SET_TEXT2(CustomWhiteBalance.r);

	_SUBNODE(OLESTR("G1"));
	SET_MAX_ATTR2(CustomWhiteBalance.max_g1);
	SET_MIN_ATTR2(CustomWhiteBalance.min_g1);
	SET_TEXT2(CustomWhiteBalance.g1);

	_SUBNODE(OLESTR("B"));
	SET_MAX_ATTR2(CustomWhiteBalance.max_b);
	SET_MIN_ATTR2(CustomWhiteBalance.min_b);
	SET_TEXT2(CustomWhiteBalance.b);

	_SUBNODE(OLESTR("G2"));
	SET_MAX_ATTR2(CustomWhiteBalance.max_g2);
	SET_MIN_ATTR2(CustomWhiteBalance.min_g2);
	SET_TEXT2(CustomWhiteBalance.g2);

	_NODE(OLESTR("PresetWhiteBalance"),UsePresetWhiteBalance);
	SET_MAX_ATTR(max_PresetWhiteBalance);
	SET_TEXT(PresetWhiteBalance);

	_NODE(OLESTR("Temperature"),UseTemperature);
	
	_SUBNODE(OLESTR("TemperatureValue"));
	SET_MAX_ATTR2(Temperature.max_temperature);
	SET_MIN_ATTR2(Temperature.min_temperature);
	SET_TEXT2(Temperature.temperature);
	
	_SUBNODE(OLESTR("Green"));
	SET_MAX_ATTR2(Temperature.max_green);
	SET_MIN_ATTR2(Temperature.min_green);
	SET_TEXT2(Temperature.green);


	_NODE(OLESTR("CameraMatrix"),UseCameraMatrix);
	
	_NODE(OLESTR("CorrectChromaticAberration"),UseCorrectChromaticAberration);
	_SUBNODE(OLESTR("R"));
	SET_MAX_ATTR2(CorrectChromaticAberration.max_r);
	SET_MIN_ATTR2(CorrectChromaticAberration.min_r);
	SET_TEXT2(CorrectChromaticAberration.r);

	_SUBNODE(OLESTR("B"));
	SET_MAX_ATTR2(CorrectChromaticAberration.max_b);
	SET_MIN_ATTR2(CorrectChromaticAberration.min_b);
	SET_TEXT2(CorrectChromaticAberration.b);

	_NODE(OLESTR("DarknessLevel"),UseDarknessLevel);
	SET_MAX_ATTR(max_DarknessLevel);
	SET_MIN_ATTR(min_DarknessLevel);
	SET_TEXT(DarknessLevel);


	_NODE(OLESTR("SaturationLevel"),(UseSaturationLevel));
	SET_MAX_ATTR(max_SaturationLevel);
	SET_MIN_ATTR(min_SaturationLevel);
	SET_TEXT(SaturationLevel);

	_NODE(OLESTR("DenoisingThreshold"),UseDenoisingThreshold);
	SET_MAX_ATTR(max_DenoisingThreshold);
	SET_TEXT(DenoisingThreshold);

	_NODE(OLESTR("HighlightMode"),UseHighlightMode);
	SET_MIN_ATTR(min_HighlightMode);
	SET_MAX_ATTR(max_HighlightMode);
	SET_TEXT(HighlightMode);

	_NODE(OLESTR("NoAutoBrighten"),NoAutoBrighten);

	_NODE(OLESTR("AdjustBrightness"),UseAdjustBrightness);
	SET_MAX_ATTR(max_AdjustBrightness);
	SET_MIN_ATTR(min_AdjustBrightness);
	SET_TEXT(AdjustBrightness);

	_NODE(OLESTR("InterpolationQuality"),UseInterpolationQuality);
	SET_MAX_ATTR(max_InterpolationQuality);
	SET_TEXT(InterpolationQuality);

	_NODE(OLESTR("MedianFilterPasses"),UseMedianFilterPasses);
	SET_MAX_ATTR(max_MedianFilterPasses);
	SET_TEXT(MedianFilterPasses);

	return S_OK;
}

#undef SET_MAX_ATTR	
#undef SET_MIN_ATTR	

#undef SET_MAX_ATTR2
#undef SET_MIN_ATTR2

#undef SET_TEXT2
#undef SET_TEXT	

#undef _SUBNODE	
#undef _NODE	


HRESULT ImageRaw3SimpleModeOptions::ParseXMLDoc(XML::XMLDocumentWrapper & xmlDoc)
{
	XML::XMLNodeWrapper tmpNode,tmpNode2;
	XML::XMLNodeListWrapper tmpList;
	BSTR bsName = NULL,bsTmp = NULL;

	HRESULT hr;
	bool	br;
	float tmpF;
	int tmpI;

	if (!xmlDoc)
		return E_FAIL;

	tmpNode = xmlDoc.GetRootNode();
	tmpNode.GetName(&bsName);
	if (_bstr_t(OLESTR("ImageRaw3")) != _bstr_t(bsName))
		return E_FAIL;
	::SysFreeString(bsName);

	bsTmp = NULL;
	br = tmpNode.GetAttributeValue(OLESTR("mode"),&bsTmp);
	int mode = _wtoi(bsTmp);
	::SysFreeString(bsTmp);

	if (0 != mode)
		return E_NOTIMPL;

	if (!(tmpNode = tmpNode.FindNode(OLESTR("SimpleMode"))))
		return E_FAIL;

	bsTmp = NULL;
	tmpNode.GetAttributeValue(OLESTR("ver"),&bsTmp);
	int ver = _wtoi(bsTmp);
	::SysFreeString(bsTmp);

	if (ver<1)
		return E_NOTIMPL;

	this->Version = ver;

	GetUseByName(tmpNode,OLESTR("CameraWhiteBalance"),UseCameraWhiteBalance);
	GetUseByName(tmpNode,OLESTR("AutoWhiteBalance"),UseAutoWhiteBalance);

	if (GetUseByName(tmpNode,OLESTR("AverageGrayBox"),UseAverageGrayBox))
	{
		tmpNode2 = tmpNode.FindNode(OLESTR("AverageGrayBox"));
		bool tmpB;
		int tmpI;
		GetParamsByNameI(tmpNode2,OLESTR("X"),tmpB,AverageGrayBox.x,tmpI,AverageGrayBox.max_x);
		GetParamsByNameI(tmpNode2,OLESTR("Y"),tmpB,AverageGrayBox.y,tmpI,AverageGrayBox.max_y);
		GetParamsByNameI(tmpNode2,OLESTR("W"),tmpB,AverageGrayBox.w,tmpI,AverageGrayBox.max_w);
		GetParamsByNameI(tmpNode2,OLESTR("H"),tmpB,AverageGrayBox.h,tmpI,AverageGrayBox.max_h);
	}
	/**/

	if (GetUseByName(tmpNode,OLESTR("CustomWhiteBalance"),UseCustomWhiteBalance))
	{
		tmpNode2 = tmpNode.FindNode(OLESTR("CustomWhiteBalance"));
		bool tmpB;
		GetParamsByNameF(tmpNode2,OLESTR("G1"),tmpB,CustomWhiteBalance.g1,CustomWhiteBalance.min_g1,CustomWhiteBalance.max_g1);
		GetParamsByNameF(tmpNode2,OLESTR("B"),tmpB,CustomWhiteBalance.b,CustomWhiteBalance.min_b,CustomWhiteBalance.max_b);
		GetParamsByNameF(tmpNode2,OLESTR("G2"),tmpB,CustomWhiteBalance.g2,CustomWhiteBalance.min_g2,CustomWhiteBalance.max_g2);
		GetParamsByNameF(tmpNode2,OLESTR("R"),tmpB,CustomWhiteBalance.r,CustomWhiteBalance.min_r,CustomWhiteBalance.max_r);		
	}
	/**/

	GetParamsByNameI(tmpNode,OLESTR("PresetWhiteBalance"),UsePresetWhiteBalance,PresetWhiteBalance,tmpI,max_PresetWhiteBalance);

	if (GetUseByName(tmpNode,OLESTR("Temperature"),UseTemperature))
	{
		tmpNode2 = tmpNode.FindNode(OLESTR("Temperature"));
		bool tmpB;
		GetParamsByNameF(tmpNode2,OLESTR("TemperatureValue"),tmpB,Temperature.temperature,Temperature.min_temperature,Temperature.max_temperature);
		GetParamsByNameF(tmpNode2,OLESTR("Green"),tmpB,Temperature.green,Temperature.min_green,Temperature.max_green);
	}

	GetUseByName(tmpNode,OLESTR("CameraMatrix"),UseCameraMatrix);
	if (GetUseByName(tmpNode,OLESTR("CorrectChromaticAberration"),UseCorrectChromaticAberration))
	{
		tmpNode2 = tmpNode.FindNode(OLESTR("CorrectChromaticAberration"));
		bool tmpB;
		GetParamsByNameF(tmpNode2,OLESTR("R"),tmpB,CorrectChromaticAberration.r,CorrectChromaticAberration.min_r,CorrectChromaticAberration.max_r);
		GetParamsByNameF(tmpNode2,OLESTR("B"),tmpB,CorrectChromaticAberration.b,CorrectChromaticAberration.min_b,CorrectChromaticAberration.max_b);
	}
	/**/

	GetParamsByNameI(tmpNode,OLESTR("DarknessLevel"),UseDarknessLevel,DarknessLevel,min_DarknessLevel,max_DarknessLevel);
	GetParamsByNameI(tmpNode,OLESTR("SaturationLevel"),UseSaturationLevel,SaturationLevel,min_SaturationLevel,max_SaturationLevel);
	
	GetParamsByNameF(tmpNode,OLESTR("DenoisingThreshold"),UseDenoisingThreshold,DenoisingThreshold,tmpF,max_DenoisingThreshold);
	GetParamsByNameI(tmpNode,OLESTR("HighlightMode"),UseHighlightMode,HighlightMode,tmpI,max_HighlightMode);
	GetUseByName(tmpNode,OLESTR("NoAutoBrighten"),NoAutoBrighten);
	GetParamsByNameF(tmpNode,OLESTR("AdjustBrightness"),UseAdjustBrightness,AdjustBrightness,min_AdjustBrightness,max_AdjustBrightness);
	GetParamsByNameI(tmpNode,OLESTR("InterpolationQuality"),UseInterpolationQuality,InterpolationQuality,tmpI,max_InterpolationQuality);
	GetParamsByNameI(tmpNode,OLESTR("MedianFilterPasses"),UseMedianFilterPasses,MedianFilterPasses,tmpI,max_MedianFilterPasses);

	return S_OK;
}