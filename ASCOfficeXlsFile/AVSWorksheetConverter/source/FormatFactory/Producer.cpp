#include "stdafx.h"
#include "Producer.h"
#include <Serializer/XLSSerializer.h>
#include <Serializer/XLSXSerializer.h>
#include <Serializer/XMLSerializer.h>
#include <Converter/XLSConverter.h>
#include <Converter/XLSXConverter.h>


Producer::~Producer()
{
}


Serializer& Producer::getSerializer(const EWS_FORMAT format)
{
	switch(format)
	{
		case ef_XLS:
		{
			static XLS::XLSSerializer prod;
			return prod;	
		}
		case ef_XML:
		{
			static XLS::XMLSerializer prod;
			return prod;	
		}
		case ef_XLSX:
		default:
		{
			static XLSX::XLSXSerializer prod;
			return prod;	
		}
	}
}


Converter& Producer::getConverter(const EWS_FORMAT format)
{
	switch(format)
	{
	case ef_XLS:
		{
			static XLS::XLSConverter prod;
			return prod;	
		}
	case ef_XLSX:
	default:
		{
			static XLSX::XLSXConverter prod;
			return prod;	
		}
	}
}



