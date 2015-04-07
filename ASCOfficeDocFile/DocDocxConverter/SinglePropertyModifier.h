#pragma once

#include "../Common/SPRMCodes.h"

namespace DocFileFormat
{
	/// Identifies the type of a SPRM
	typedef enum SprmType
	{
		PAP = 1,
		CHP = 2,
		PIC = 3,
		SEP = 4,
		TAP = 5
	} SprmType;

	class SinglePropertyModifier
	{
	public:
		/// The operation code identifies the property of the 
		/// PAP/CHP/PIC/SEP/TAP which sould be modified
		OperationCode OpCode;
		/// This SPRM requires special handling
		bool fSpec;
		/// The type of the SPRM
		SprmType Type;
		/// The arguments which is applied to the property
		unsigned char* Arguments;
		/// The Arguments size
		unsigned int argumentsSize;

		SinglePropertyModifier();
		/// parses the unsigned char to retrieve a SPRM
		SinglePropertyModifier( unsigned char* bytes, int size );
		SinglePropertyModifier( const SinglePropertyModifier& spm );
		bool operator == ( const SinglePropertyModifier& spm );
		bool operator != ( const SinglePropertyModifier& spm );
		SinglePropertyModifier& operator = ( const SinglePropertyModifier& spm );
		~SinglePropertyModifier();
		/// Get be used to get the size of the sprm's operand.
		/// Returns 0 if the Operation failed and 255 if the size is variable
		static unsigned char GetOperandSize( unsigned char spra );
	};
}
