/*!	\file IImageMetaData.h
*
*	Описание интерфейсов для работы с метаданными
*
*	Интерфейс IImageMetaDataParser предназначен для обеспечения функциональности
*	получения метаданных разных типов, таких как
*	Exif (http://en.wikipedia.org/wiki/EXIF),
*	IPTC (http://en.wikipedia.org/wiki/IPTC_Information_Interchange_Model),
*	XMP (http://en.wikipedia.org/wiki/Extensible_Metadata_Platform) 
*	и др.
*
*	При помощи метода IImageMetaDataParser::ParseFile или IImageMetaDataParser::ParseStream производится
*	разбор данных и поиск метаданных.
*	Число доступных типов метаданных для разобранного файла возвращается значением IImageMetaDataParser::AvailDataTypeCount.
*	После этого можно получить типы найденных метаданных и интерфейсы для работы с ними при помощи методов
*	IImageMetaDataParser::MetaDataType и IImageMetaDataParser::MetaData соответственно.
*
*	Тип метаданных представляет собой заранее зарезервированное значение, отличное от нуля (например 1 - для Exif, 2 - для IPTC и т.д.).
*	
*	Получив интерфейс через метод IImageMetaDataParser::MetaData, мы можем запросить общий для всех метаданных
*	интерфейс IImageMetaDataCommon. Через него мы можем получить (метод IImageMetaDataCommon::GetCommonXML) простую XML общего вида,
*	одинакового для всех типов метаданных, предназначенную в основном для отображения данных
*	пользователю в виде "имя параметра" - "значение".
*
*	Для установки наименований тегов, возвращаемых в XML, отличной от заданной по умолчанию
*	(например, для осуществления перевода на разные языки), служит метод IImageMetaDataCommon::SetTranslateXML
*	
*	Метод IImageMetaDataCommon::GetXML возвращает XML, специфичную для конкретного типа метаданных. Например, для Exif
*	будут содержаться данные об идентификаторах тегов, их наименованиях, типах, длине и значениях.
*
*	Для каждого конкретного типа метаданных, мы можем запросить интерфейс, унаследованый от IImageMetaDataCommon
*	и специфичный для данного типа.
*/

#ifndef _AVS_IIMAGEMETADATA_INCLUDE_H_
#define _AVS_IIMAGEMETADATA_INCLUDE_H_
#pragma once

/*!	\interface	IImageMetaDataParser
*	\breif	Интерфейс предоставляет функциональность получения метаданных разного типа
*/
[
	object,
	uuid("9C564E31-0ED0-45e3-B447-71E308724130"),
	dual,	helpstring("IImageMetaDataParser Interface"),
	pointer_default(unique)
]
__interface IImageMetaDataParser: IDispatch
{
	/*!	\brief	Извлечение метаданных из файла
	*	
	*	\param[in]	bstrFileName	Имя файла, строка BSTR
	*	\param[in]	dwMode			Режим обработки (зарезервировано)
	*/
	[id(10000 + 1)] HRESULT	ParseFile([in] BSTR bstrFileName, [in] DWORD dwMode);
	
	/*!	\breif	Извлечение метаданных из потока (аналогично методу ParseFile, но чтение осуществляется из потока)
	*
	*	\param[in]	pStream		Поток IStream из которого читаем данные
	*	\param[in]	dwMode		Режим обработки (зарезервировано)
	*	
	*	\remarks	Метод может быть полезен в случае когда мы хотим прочитать метаданные из файла, который
	*				не существует непосредственно на диске (например извлеченный в память thumbnail от raw-файла)
	*/
	[id(10000 + 2)] HRESULT ParseStream([in] IStream * pStream, [in] BSTR bstrFileName);

	/*!	\breif	Получить число доступных типов метаданных
	*
	*	\param	dwCount[out]	Число доступных типов метаданных. В случае если не удалось извлечь никаких метаданных,
	*							возвращаем значение 0
	*/
	[id(10000 + 3), propget] HRESULT AvailDataTypeCount([out, retval] DWORD * dwCount);
	
	/*!	\brief	Получить интерфейс метаданных по порядковому номеру
	*
	*	\param[in]	dwNum	Порядковый номер запрашиваемых метаданных (от 0 до \a AvailDataTypeCount)
	*	\param[out]	pMetaData	Указатель на интерфейс, через который получаем метаданные или NULL в случае ошибки.
	*							В случае успешного завершения, гарантируется, что мы можем запросить интерфейс \a IImageMetaDataCommon - общий для всех метаданных
	*	
	*	\returns	E_INVALIDARG, в случае если указали несуществующий порядковый порядковый номер метаданных;
	*				SUCCEEDED в случае удачного завершения, FAILED - в противном случае
	*/
	[id(10000 + 4)] HRESULT MetaData([in] DWORD dwNum, [out, retval] IUnknown ** pMetaData);

	/*!	\brief	Получить тип метаданных по порядковому номеру
	*
	*	\param[in]	dwNum	Порядковый номер запрашиваемых метаданных (от 0 до \a AvailDataTypeCount)
	*	\param[out]	dwDataType	Идентификатор типа метаданных или ноль в случае ошибки
	*	
	*	\returns	E_INVALIDARG, в случае если указали несуществующий порядковый номер метаданных;
	*				SUCCEEDED в случае удачного завершения, FAILED - в противном случае
	*/
	[id(10000 + 5)] HRESULT MetaDataType([in] DWORD dwNum, [out, retval] DWORD * dwDataType);
	
	/*!	\brief		Установить дополнительное значение, параметр по имени
	*/
	[id(10000 + 100)] HRESULT	SetAdditionalParam([in]BSTR ParamName, [in] VARIANT ParamValue);

	/*!	\brief		Получить дополнительное значение, параметр по имени
	*/
	[id(10000 + 101)] HRESULT	GetAdditionalParam([in]BSTR ParamName, [out, retval] VARIANT* ParamValue);
};

/*!	\interface IImageMetaDataCommon
*	\brief	Общий для всех типов метаданных интерфейс.
*			Предназначен для получения типа метаданных и XML общего вида для всех типов метаданных
*/
[
	object,
	uuid("0EA09BA9-CBBE-460a-8DB0-EE4E34441679"),
	dual,	helpstring("IImageMetaDataCommon Interface"),
	pointer_default(unique)
]
__interface IImageMetaDataCommon: IDispatch
{
	/*!	\breif	Получить тип метаданных
	*/
	[id(10000 + 1), propget] HRESULT	MetaDataType([out, retval] DWORD * dwMode);

	/*!	\breif	Получить XML с метаданными
	*	
	*	\remarks	Формат XML общий для всех типов метаданных (вида 'имя параметра' - 'значение')
	*/
	[id(10000 + 2)] HRESULT	GetCommonXML([in] DWORD dwMode, [out, retval] BSTR * bstrMetaDataXML);

	/*!	\breif	Получить XML с метаданными
	*	
	*	\remarks	Формат XML специфичный для каждого типа метаданных
	*/
	[id(10000 + 3)] HRESULT GetXML([in] DWORD dwMode, [out, retval] BSTR * bstrMetaDataXML);

	/*!	\breif	Установить XML с именованием для конвертации названий тегов
	*	
	*	\param[in]	bstrTranslateXML XML с именованием тегов
	*
	*	\remarks	Метод необходим для того чтобы жестко не зашивать названия выводимых тегов и для
	*				осуществления перевода. Напрмер, значение поля 'ExposureTime' (Exif Tag ID 33434)
	*				в английском варианте будет соответствовать имени параметра 'Exposure time', в русском - 'Время выдержки'
	*/
	[id(10000 + 4)] HRESULT	SetTranslateXML([in] BSTR bstrTranslateXML);

	/*!	\brief		Установить дополнительное значение, параметр по имени
	*/
	[id(10000 + 100)] HRESULT	SetAdditionalParam([in]BSTR ParamName, [in] VARIANT ParamValue);

	/*!	\brief		Получить дополнительное значение, параметр по имени
	*/
	[id(10000 + 101)] HRESULT	GetAdditionalParam([in]BSTR ParamName, [out, retval] VARIANT* ParamValue);
};

/*!	\interface IImageMetaDataExif
*	\brief	Метаданные типа Exif
*	
*	\remarks	Функциональность интерфейса может расширяться (вопрос для обсуждения)
*/
[
	object,
	uuid("D1680997-BC53-4124-8F9A-CCEE05A6D3E7"),
	dual,
	pointer_default(unique)
]
__interface IImageMetaDataExif: IImageMetaDataCommon
{
	[id(1)] HRESULT GetValue([in]USHORT TagId, [out, retval] BSTR * bstrValue);
	[id(2)] HRESULT GetType([in]USHORT TagId, [out, retval] USHORT * nType);
	[id(3)] HRESULT GetCount([in]USHORT TagId, [out, retval] UINT * nCount);
};

/*!	\interface IImageMetaDataIPTC
*	\brief	Метаданные типа IPTC
*	
*	\remarks	Функциональность интерфейса может расширяться (вопрос для обсуждения)
*/
[
	object,
	uuid("6BE1ECC2-B1AE-4e45-A46F-40230909AFC2"),
	dual,
	pointer_default(unique)
]
__interface IImageMetaDataIPTC: IImageMetaDataCommon
{
};


#endif

