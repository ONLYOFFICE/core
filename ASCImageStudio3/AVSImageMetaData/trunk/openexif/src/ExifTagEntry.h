/* 
* The contents of this file are subject to the IBM Common Public
* License Version 0.5 (the "License"); you may not use this file 
* except in compliance with the License. You may obtain a copy of 
* the License at http://oss.software.ibm.com/developerworks/
* opensource/license-cpl.html
* 
* The Original Code is the Open Exif Toolkit,
* released November 13, 2000.
*
* The Initial Developer of the Original Code is Eastman 
* Kodak Company. Portions created by Kodak are 
* Copyright (C) 2001 Eastman Kodak Company. All 
* Rights Reserved. 
*
* Creation Date: 07/14/2001
*
* Original Author: 
* George Sotak george.sotak@kodak.com 
*
* Contributor(s): 
* Ricardo Rosario ricardo.rosario@kodak.com
*/ 

#ifndef _EXIF_TAG_ENTRY_H_
#define _EXIF_TAG_ENTRY_H_


#include "ExifTypeDefs.h"
#include <iostream>
#include <vector>
#include <string>

#include "..\..\common\utils.h"

namespace Exif 
{

	//:
	// This abstract class is the base class for representing the tag entry of an Exif image.
	// It contains three pieces of inforamtion: tag that identified the field, field type
	// and the count of number of values.
	//
	class EXIF_DECL ExifTagEntry
	{
	public:

		// Destructor
		virtual ~ExifTagEntry() {}

		// Create a copy of the ExifTagEntry object.
		//!return: Pointer to the copy.
		virtual ExifTagEntry* clone( void ) const = 0;

		// Get the tag number.
		//!return: Tag number
		exiftag_t getTagNum( void ) const
		{ return mTagNum; }

		// Get the field type.
		//!return: Field type.
		ttype_t getType( void ) const
		{ return mType; }

		// Get the number of values.
		//!return: Reference to the number of values.
		exif_int32& getCount( void )
		{ return mCount; }

		// Get the number of values.
		//!return: Number of values.
		exif_int32 getCount( void ) const
		{ return mCount; }

		// Set the number of values.
		//!param: cnt - Number of values
		void setCount( exif_int32 cnt )
		{ mCount = cnt; return; }

		// Print the tag entry.
		//!param: c - Output stream.
		//!return: Output stream.
		virtual std::ostream& print(std::ostream& c = std::cout) const = 0;
		virtual std::wostream & PrintXMLEntry(std::wostream& c) const = 0;
		virtual std::wostream & PrintXMLEntryEx(std::wostream& c, std::wstring & category = std::wstring(L""), unsigned int makernoteid = 0) const = 0;
		virtual std::ostream & PrintValue(std::ostream& c) const = 0;
		virtual std::wostream & PrintValue(std::wostream& c) const = 0;

	protected:
		ExifTagEntry(exiftag_t num, ttype_t typ )
			: mTagNum(num), mType(typ)
		{}
		ExifTagEntry(exiftag_t num, ttype_t typ, exif_int32 cnt )
			: mTagNum(num), mType(typ), mCount(cnt)
		{}

		exiftag_t     mTagNum;
		ttype_t    mType;
		exif_int32      mCount;
	} ;


	template <class S>
	inline S& ExifPrintValue(S& c, const unsigned char& v) { return c << (int)v; }

	template <class S>
	inline S& ExifPrintValue(S& c, const char& v) { return c << (int)v; }

	template <class S>
	inline S& ExifPrintValue(S& c, const unsigned short& v) { return c << v; }

	template <class S>
	inline S& ExifPrintValue(S& c, const short& v) { return c << v; }

	template <class S>
	inline S& ExifPrintValue(S& c, const unsigned int& v) { return c << v; }

	template <class S>
	inline S& ExifPrintValue(S& c, const int& v) { return c << v; }

	template <class S>
	inline S& ExifPrintValue(S& c, const unsigned long& v) { return c << v; }

	template <class S>
	inline S& ExifPrintValue(S& c, const long& v) { return c << v; }

	template <class S>
	inline S& ExifPrintValue(S& c, const float& v) { return c << v; }

	template <class S>
	inline S& ExifPrintValue(S& c, const double& v) { return c << v; }

	template <class S>
	inline S& ExifPrintValue(S& c, const std::string& v) { return c << v ; }

	template <class S>
	inline S& ExifPrintValue(S& c, const std::wstring& v) { return c << v ; }

	inline std::wostream & ExifPrintValue(std::wostream & c, const std::string& v)
	{
		return c << AVS::Utils::__StoW(v.c_str());
	}

	// ostream implementation for vector. we want to avoid using operator "<<"
	// that will conflict the one in ekc
	template <class S, class TYPE>
	inline S& ExifPrintValue(S& c, const std::vector<TYPE>& v)
	{
		int i = v.size();
		c << "[" ;
		for (int j=0; j<i; j++) 
		{
			c << (j==0?"":",");
			ExifPrintValue(c, v[j]);
		}
		c << "]";   
		return c;
	}

	//:
	// This template class derives from ExifTagEntry to represent the tag entry of the
	// specified type. 
	//
	template<class Type>
	class EXIF_DECL ExifTagEntryT : public ExifTagEntry
	{
	public:

		// Create a ExifTagEntry object with the specified tag number and type.
		//!param: num - Tag number.
		//!param: typ - Field type.
		ExifTagEntryT( exiftag_t num, ttype_t typ )
			: ExifTagEntry( num, typ )
		{}

		// Create a ExifTagEntry object with the specified tag number, type, count and value.
		//!param: num - Tag number.
		//!param: typ - Field type.
		//!param: cnt - Number of values
		//!param: value - Field value
		ExifTagEntryT( exiftag_t num, ttype_t typ, exif_int32 cnt, Type value )
			: ExifTagEntry( num, typ, cnt ), mValue(value)
		{}

		// Copy constructor
		//!param: entry - Source to copy
		ExifTagEntryT(const ExifTagEntryT<Type>& entry)
			: ExifTagEntry(entry.mTagNum, entry.mType, entry.mCount), mValue(entry.getValue())
		{}

		// Destructor
		virtual ~ExifTagEntryT() {}

		// Create a copy of the ExifTagEntry object.
		//!return: Pointer to the copy.
		virtual ExifTagEntry* clone( void ) const
		{ return new ExifTagEntryT<Type>( *this ); }

		// Get value.
		//!return: Reference to the value.
		Type& getValue() 
		{ return mValue; }

		// Get value.
		//!return: Reference to the value.
		const Type& getValue( void ) const
		{ return mValue; }

		// Set value.
		//!param: sv - Field value.
		void setValue( const Type& sv )
		{ mValue = sv; return; }

		// Print the tag entry.
		//!param: c - Output stream.
		//!return: Output stream.
		virtual std::ostream& print(std::ostream& c = std::cout) const
		{
			c << mTagNum << '\t' << mType << '\t' << mCount << '\t';
			ExifPrintValue(c, mValue);
			return c;
		}


		virtual std::wostream & PrintXMLEntry(std::wostream & c) const
		{
			c << L"<Tag>";
			c << L"<ExifTag" << L" tagid=\"" << mTagNum << L"\"" <<
				L" type=\"" << mType << L"\""
				<< L" count=\"" << mCount << L"\"" << L">";
			ExifPrintValue(c, mValue);
			c << L"</ExifTag>";
			c << L"</Tag>";
			return c;
		}

		virtual std::wostream & PrintXMLEntryEx(std::wostream& c, std::wstring &category = std::wstring(L""), unsigned int makernoteid = 0) const
		{
			c << L"<Tag>";

			c << L"<ExifTag" << L" tagid=\"" << mTagNum << L"\"" <<
				L" type=\"" << mType << L"\""
				<< L" count=\"" << mCount << L"\"";

			if (category.length() > 0)
			{
				c << L" category=\"" << category << L"\"";
			}

			if (makernoteid != 0)
			{
				c << L" categoryid=\"" << makernoteid << L"\"";
			}
			c<< L">";
			ExifPrintValue(c, mValue);
			c << L"</ExifTag>";
			c << L"</Tag>";
			return c;
		}

		virtual std::ostream & PrintValue(std::ostream& c) const
		{
			ExifPrintValue(c, mValue);
			return c;
		}

		virtual std::wostream & PrintValue(std::wostream& c) const
		{
			ExifPrintValue(c, mValue);
			return c;
		}


	private:

		Type mValue;

	} ;

#ifdef EXIF_EXPLICIT

	template class EXIF_DECL ExifTagEntryT<vector<char> >;

	template class EXIF_DECL ExifTagEntryT<char>;

	template class EXIF_DECL ExifTagEntryT<vector<unsigned char> >;

	template class EXIF_DECL ExifTagEntryT<unsigned char>;

	template class EXIF_DECL ExifTagEntryT<vector<short> >;

	template class EXIF_DECL ExifTagEntryT<short>;

	template class EXIF_DECL ExifTagEntryT<vector<unsigned short> >;

	template class EXIF_DECL ExifTagEntryT<unsigned short>;

	template class EXIF_DECL ExifTagEntryT<vector<int> >;

	template class EXIF_DECL ExifTagEntryT<int>;

	template class EXIF_DECL ExifTagEntryT<vector<unsigned int> >;

	template class EXIF_DECL ExifTagEntryT<unsigned int>;

	template class EXIF_DECL ExifTagEntryT<vector<bool> >;

	template class EXIF_DECL ExifTagEntryT<bool>;

	template class EXIF_DECL ExifTagEntryT<vector<float> >;

	template class EXIF_DECL ExifTagEntryT<float>;

	template class EXIF_DECL ExifTagEntryT<vector<double> >;

	template class EXIF_DECL ExifTagEntryT<double>;

	template class EXIF_DECL ExifTagEntryT<vector<std::string> >;

	template class EXIF_DECL ExifTagEntryT<std::string>;

#endif
}
#endif // _EXIF_TAG_ENTRY_H_
