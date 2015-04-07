/*! \file	ImageRawOptions.h
 *	\brief	Настройки raw-конвертера
 *
 *	\author	Nikolay.Pogorskiy@avsmedia.net
 *	\details
 *	Пример XML:	
 *
 *			<ImageRaw3 mode="0">
 *				<SimpleMode ver="1">
 *					<CameraWhiteBalance use="0"/>
 *					<AutoWhiteBalance use="0">
 *					<AverageGrayBox use="0">
 *						<X max="0">0</X>
 *						<Y max="0">0</Y>
 *						<W max="0">0</W>
 *						<H max="0">0</H>
 *					</AverageGrayBox>
 *					<CustomWhiteBalance use="0">
 *						<R min="0" max="0">0.0</R>
 *						<G1 min="0" max="0">0.0</G1>
 *						<B min="0" max="0">0.0</B>
 *						<G2 min="0" max="0">0.0</G2>
 *					</CustomWhiteBalance>
 *					<PresetWhiteBalance use="0" max="8">0</PresetWhiteBalance>
 *					<Temperature use="0">
 *						<Temperature min="0" max="0"></Temperature>
 *						<Green min="0" max="0"></Green>
 *					</Temperature>
 *					<CameraMatrix use="0"/>
 *					<CorrectChromaticAberration use="0">
 *						<R min="0" max="0">0.0</R>
 *						<B min="0" max="0">0.0</B>
 *					</CorrectChromaticAberration>
 *					<DarknessLevel use="0" min="0" max="0">-1</DarknessLevel>
 *					<SaturationLevel use="0" min="0" max="0">-1</SaturationLevel>
 *					<DenoisingThreshold use="0" max="0.0">0.0</DenoisingThreshold>
 *					<HighlightMode use="0" max="8">0</HighlightMode>
 *					<NoAutoBrighten use="0"/>
 *					<AdjustBrightness use="0" min="0" max="0">1.0</AdjustBrightness>
 *					<InterpolationQuality use="0" max="3">3</InterpolationQuality>
 *					<MedianFilterPasses use="0" max="0">0</MedianFilterPasses>
 *				</SimpleMode>
 *			</ImageRaw3>
 *
 */
 
#pragma once

#include "stdio.h"
#include "XMLWrapper.h"
#include "comutil.h"



#define IMAGERAW_MAX_CHANNEL_MUL	9.0
#define	IMAGERAW_MIN_CHANNEL_MUL	1.0

/*!
 *	\brief Вспомогательная функция для установки значения атрибута XML-узла
 */
template <class T>
void SetAttrib(XML::XMLNodeWrapper & node,BSTR name,T value)
{
	VARIANT variant;
	variant.vt = VT_BSTR;
	variant.bstrVal = _bstr_t(value).copy();
	node.SetAttributeValue(name,variant);
	::SysFreeString(variant.bstrVal);
}

//#include "atlcomcli.h"

/*!
 *	\brief		Настройки для режима "simple"
 *	
 *	\details	Класс для хранения настроек и обестпечения загрузки и сохранения в xml
 */
class ImageRaw3SimpleModeOptions
{
public:
	/*!
	*	\brief Пользовательские значения баланса белого
	*/

	struct SCustomWhiteBalance
	{
		float r,g1,b,g2;
		float max_r,max_g1,max_b,max_g2;
		float min_r,min_g1,min_b,min_g2;
		SCustomWhiteBalance(float r_=0.0,float g1_=0.0,float b_=0.0,float g2_=0.0):r(r_),g1(g1_),b(b_),g2(g2_)
		{
			max_r = 9.0; max_g1 = 9.0; max_g2 = 9.0; max_b = 9.0;
			min_r = 1.0; min_g1 = 1.0; min_g2 = 1.0; min_b = 1.0;
		};
		void Reset()
		{
			max_r = 9.0; max_g1 = 9.0; max_g2 = 9.0; max_b = 9.0;
			min_r = 1.0; min_g1 = 1.0; min_g2 = 1.0; min_b = 1.0;
			r = g1 = b = g2 = 0.0;
		}
		
	};

	/*!
	*	\brief Границы серой области для установки баланса белого
	*/
	struct SAverageGrayBox
	{
		int x,y,w,h;
		int max_x,max_y,max_w,max_h;
		SAverageGrayBox(int x_ =0, int max_x_ =0
						,int y_=0, int max_y_ =0
						,int w_=0, int max_w_ =0
						,int h_=0, int max_h_ =0)
										:x(x_),y(y_),w(w_),h(h_),max_x(max_x_),max_y(max_y_),max_w(max_w_),max_h(max_h_)
		{
		};

		void Reset()
		{
			x = y = w = h = 0;
			max_x = max_y = max_w = max_h = 0;
		}
		
	};

	/*!
	*	\brief	Значения для коррекции хроматических аберраций
	*/
	struct SCorrectChromaticAberration
	{
		float r,b;
		float min_r,min_b;
		float max_r,max_b;
		SCorrectChromaticAberration(float r_=0.0,float min_r_ =0.0,float max_r_ =0.0, float b_=0.0, float min_b_ = 0.0, float max_b_ = 0.0):
		r(r_),b(b_),max_r(max_r_),max_b(max_b_),min_r(min_r_),min_b(min_b_)
		{
		}

		void Reset()
		{
			r = b = 0.0;
			min_r = 0.0;
			max_r = 0.0;
			min_b = 0.0;
			max_b = 0.0;
		}
		
	};

	/*!
	 *	\brief	Пользовательские значения цветовой температуры и уровня зеленого
	 */
	struct STemperature
	{
		int temperature;
		float green;
		int max_temperature,min_temperature;
		float max_green,min_green;
		STemperature(int temperature_,float green_):temperature(temperature_),green(green_)
		{
			max_temperature = 15000;
			min_temperature = 2000;
			max_green = 2.5;
			min_green = 0.2;
		};
		void Reset()
		{
			temperature = 0;
			green = 0.0;
			max_temperature = 15000;
			min_temperature = 2000;
			max_green = 2.5;
			min_green = 0.2;
		}
	};

	int							Version;				//!<	Версия настроек
	bool						UseCameraWhiteBalance;	//!<	Баланс белого камеры 
	bool						UseAutoWhiteBalance;	//!<	Автоматический баланс белого
	
	bool						UseAverageGrayBox;		//!<	Использования серой области для установки баланса белого
	SAverageGrayBox				AverageGrayBox;			//!<	Серая область для установки баланса белого
	
	bool						UseCustomWhiteBalance;	//!<	Использование пользовательских значения для установки баланса белого
	SCustomWhiteBalance			CustomWhiteBalance;		//!<	Пользовательские значения баланса белого
	
	bool						UsePresetWhiteBalance;	//!<	Использование предустановленных настроек баланса белого
	int							PresetWhiteBalance;		//!<	Предустановленные настройки баланса белого
	int							max_PresetWhiteBalance;	//!<	Максимальное значение параметра PresetWhiteBalance
	
	bool						UseTemperature;			//!<	Использование пользовательских значений цветовой температуры и уровня зеленого
	STemperature				Temperature;			//!<	Пользовательские значения цветовой температуры и уровня зеленого
	
	bool						UseCameraMatrix;		//!<	use camera matrix
	
	bool						UseCorrectChromaticAberration;	//!<	Коррекция хроматических аберраций
	SCorrectChromaticAberration	CorrectChromaticAberration;		//!<	Значения для коррекции хроматических аберраций

	bool						UseDarknessLevel;		//!<	Установка уровня черного
	int							DarknessLevel;			//!<	Значение уровня черного
	int							max_DarknessLevel;		//!<	Максимальное значение параметра
	int							min_DarknessLevel;		//!<	Минимальное значение параметра

	bool						UseSaturationLevel;		//!<	Установка уровня насыщенности
	int							SaturationLevel;		//!<	Уровень насыщенности
	int							max_SaturationLevel;	//!<	Максимальное значение параметра \a SaturationLevel
	int							min_SaturationLevel;	//!<	Минимальное значение параметра \a SaturationLevel

	bool						UseDenoisingThreshold;	//!<	Использование подавления шумов
	float						DenoisingThreshold;		//!<	Значение порога для подавления шумов
	float						max_DenoisingThreshold;	//!<	Максимальное значение параметра \a DenoisingThreshold

	bool						UseHighlightMode;		//!<	Обработка пересвеченных областей
	int							HighlightMode;			//!<	Режим обработки пересвеченных областей
	int							max_HighlightMode;		//!<	Максимальное значение параметра HighlightMode

	bool						NoAutoBrighten;			//!<	no auto brighten

	bool						UseAdjustBrightness;	//!<	Установка параметра яркости
	float						AdjustBrightness;		//!<	Значение параметра яркости
	float						min_AdjustBrightness;	//!<	Минимальное значение параметра \a AdjustBrightness
	float						max_AdjustBrightness;	//!<	Максимальное значение параметра \a AdjustBrightness

	bool						UseInterpolationQuality;	//!<	Использование параметра режима интерполяции
	int							InterpolationQuality;		//!<	Режим интерполяции
	int							max_InterpolationQuality;	//!<	Максимальное значение параметра \a InterpolationQuality

	bool						UseMedianFilterPasses;	//!<	Использование медианного фильтра
	int							MedianFilterPasses;		//!<	Число проходов медианного фильтра
	int							max_MedianFilterPasses;	//!<	Максимальное значение параметра \a MedianFilterPasses


public:
	//! Конструктор класса
	ImageRaw3SimpleModeOptions():
		Version(1),
		UseCameraWhiteBalance(false),
		UseAutoWhiteBalance(false),
		UseAverageGrayBox(false),
		AverageGrayBox(0,0,0,0),
		UseCustomWhiteBalance(false),
		CustomWhiteBalance(0.0,0.0,0.0,0.0),
		UsePresetWhiteBalance(false),
		PresetWhiteBalance(0),
		max_PresetWhiteBalance(0),
		UseTemperature(false),
		Temperature(0,0.0),
		UseCameraMatrix(false),
		UseCorrectChromaticAberration(false),
		CorrectChromaticAberration(0.0,0.0),
		UseDarknessLevel(false),
		min_DarknessLevel(0),
		max_DarknessLevel(0),
		DarknessLevel(-1),
		UseSaturationLevel(false),
		min_SaturationLevel(0),
		max_SaturationLevel(0),
		SaturationLevel(-1),
		UseDenoisingThreshold(false),
		DenoisingThreshold(0.0),
		max_DenoisingThreshold(0.0),
		UseHighlightMode(false),
		max_HighlightMode(0),
		HighlightMode(0),
		NoAutoBrighten(false),
		UseAdjustBrightness(false),
		min_AdjustBrightness(0.0),
		max_AdjustBrightness(0.0),
		AdjustBrightness(1.0),
		UseInterpolationQuality(false),
		max_InterpolationQuality(3),
		InterpolationQuality(3),
		UseMedianFilterPasses(false),
		max_MedianFilterPasses(0),
		MedianFilterPasses(0)
	{
	}

	//!	Сброс всех параметров на значения по умолчанию
	void Reset()
	{
		Version = 1;
		UseCameraWhiteBalance = false;
		UseAutoWhiteBalance = false;
		UseAverageGrayBox = false;
		AverageGrayBox.Reset();
		UseCustomWhiteBalance = false;
		CustomWhiteBalance.Reset();
		UsePresetWhiteBalance = false;
		PresetWhiteBalance = 0;
		max_PresetWhiteBalance = 0;
		UseTemperature = false;
		Temperature.Reset();
		UseCameraMatrix = false;
		UseCorrectChromaticAberration = false;
		CorrectChromaticAberration.Reset();
		UseDarknessLevel = false;
		max_DarknessLevel = 0;
		min_DarknessLevel = 0;
		DarknessLevel = -1;
		UseSaturationLevel = false;
		SaturationLevel = -1;
		max_SaturationLevel = 0;
		min_SaturationLevel = 0;
		UseDenoisingThreshold = false;
		max_DenoisingThreshold = 0.0;
		DenoisingThreshold = 0.0;
		UseHighlightMode = false;
		max_HighlightMode = 0;
		HighlightMode = 0;
		NoAutoBrighten = false;
		UseAdjustBrightness = false;
		AdjustBrightness = 1.0;
		max_AdjustBrightness = 0.0;
		min_AdjustBrightness = 0.0;
		UseInterpolationQuality = false;
		InterpolationQuality = 3;
		UseMedianFilterPasses = false;
		MedianFilterPasses = 0;
	}

	//!	Скопировать XML с параметрами в строку BSTR
	HRESULT CopyXMLTo(BSTR * xml)
	{
		HRESULT hr;
		XML::XMLDocumentWrapper xmlDoc;

		if (!xml)
			return E_POINTER;

		*xml = NULL;
		if FAILED(hr = SetXMLDoc(xmlDoc))
			return hr;
		
		if (!(xmlDoc.GetXML(xml)))
			return E_FAIL;
		
		return S_OK;	
	}

	//!	Загрузить параметры из строки, содержащей xml
	HRESULT OpenXML(BSTR xml)
	{
		if (!xml)
			return E_POINTER;
		XML::XMLDocumentWrapper xmlDoc;
		
		if (!xmlDoc.LoadXML(xml))
			return E_INVALIDARG;
		
		return ParseXMLDoc(xmlDoc);
	}

	//!	Загрузить параметры из XML-файла
	HRESULT OpenFile(BSTR filename)
	{
		HRESULT hr;
		if (!filename)
			return E_POINTER;
		
		XML::XMLDocumentWrapper xmlDoc;

		BSTR bsFileName = ::SysAllocString(filename);
		if (!xmlDoc.Load(bsFileName))
			hr = E_INVALIDARG;
		else
			hr = S_OK;
		::SysFreeString(bsFileName);
		if FAILED(hr)
			return hr;

		return ParseXMLDoc(xmlDoc);
	}

	//!	Сохранить параметры в XML-файл
	HRESULT SaveFile(BSTR filename)
	{
		HRESULT hr;
		XML::XMLDocumentWrapper xmlDoc;

		if (!filename)
			return E_POINTER;

		if FAILED(hr = SetXMLDoc(xmlDoc))
			return hr;

		BSTR bsFileName = ::SysAllocString(filename);
		if (!(xmlDoc.Save(bsFileName)))
			hr = E_FAIL;
		else
			hr = S_OK;
		::SysFreeString(bsFileName);

		return hr;
	}

protected:
	//!	Преобразовать параметры в структуру XML::XMLDocumentWrappe
	HRESULT SetXMLDoc(XML::XMLDocumentWrapper & xmlDoc);
	//!	Заполнить поля класса из структуры XML::XMLDocumentWrapper
	HRESULT ParseXMLDoc(XML::XMLDocumentWrapper & xmlDoc);
};