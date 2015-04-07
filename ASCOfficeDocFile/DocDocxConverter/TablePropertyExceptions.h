#pragma once

#include "PropertyExceptions.h"
#include "ParagraphPropertyExceptions.h"
#include "VirtualStreamReader.h"

namespace DocFileFormat
{
	class TablePropertyExceptions: public PropertyExceptions
	{
	public:
		/// Parses the bytes to retrieve a TAPX
		TablePropertyExceptions(unsigned char* bytes, int size) : PropertyExceptions(bytes, size), m_bSkipShading97 (FALSE)
		{
			//not yet implemented
		}

		virtual ~TablePropertyExceptions()
		{

		}

		/// Extracts the TAPX SPRMs out of a PAPX
		TablePropertyExceptions (ParagraphPropertyExceptions* papx, POLE::Stream* dataStream) : PropertyExceptions()
		{
			VirtualStreamReader oBinReader(dataStream, 0);

			m_bSkipShading97	=	FALSE;

			for (list<SinglePropertyModifier>::iterator oSpmIter = papx->grpprl->begin(); oSpmIter != papx->grpprl->end(); ++oSpmIter)
			{
				if (oSpmIter->OpCode == sprmTDefTableShd || oSpmIter->OpCode == sprmTDefTableShd2nd || 
					oSpmIter->OpCode == sprmTDefTableShd2nd || oSpmIter->OpCode == sprmTDefTableShd3rd)
				{
					m_bSkipShading97 = TRUE;
				}

				if (oSpmIter->Type == TAP)
				{
					grpprl->push_back(*oSpmIter);
				}
				else if ((int)(oSpmIter->OpCode) == sprmPTableProps)
				{
					//there is a native TAP in the data stream
					unsigned int fc = FormatUtils::BytesToUInt32(oSpmIter->Arguments, 0, oSpmIter->argumentsSize);

					//get the size of the following grpprl
                    oBinReader.Seek(fc, 0/* STREAM_SEEK_SET*/);
					unsigned char* sizebytes = oBinReader.ReadBytes(2, true);
					unsigned short grpprlSize = FormatUtils::BytesToUInt16(sizebytes, 0, 2);

					//read the grpprl
					unsigned char* grpprlBytes = oBinReader.ReadBytes(grpprlSize, true);

					//parse the grpprl
					PropertyExceptions externalPx(grpprlBytes, grpprlSize);

					for (list<SinglePropertyModifier>::iterator oIter = externalPx.grpprl->begin(); oIter != externalPx.grpprl->end(); ++oIter)
					{
						if (oIter->Type == TAP)
						{
							grpprl->push_back(*oIter);
						}
					}

					RELEASEARRAYOBJECTS(grpprlBytes);
					RELEASEARRAYOBJECTS(sizebytes);
				}
			}
		}

		inline bool IsSkipShading97 ()
		{
			return m_bSkipShading97;
		}

	private:

		bool	m_bSkipShading97;	//	пропускать правило от Word97 
	};
}
