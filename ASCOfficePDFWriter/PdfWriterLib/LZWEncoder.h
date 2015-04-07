#ifndef _LZW_ENCODER_H
#define _LZW_ENCODER_H

#pragma once 

#include "../../common/File.h"


//-----------------------------------------------------------------------------------------------------
// CEncoderLZW
//-----------------------------------------------------------------------------------------------------

#define LZW_NO_ERROR            0
#define LZW_BAD_FILE_NAME       1
#define LZW_BAD_ARGUMENT        2
#define LZW_BAD_MEM_ALLOC       3
#define LZW_BAD_DATA_CODE       4
#define LZW_DICTIONARY_OVERFLOW 5

typedef struct SDic_Val 
{ 
	unsigned int unCharacter;
	unsigned int unCode;
	SDic_Val    *pLeft_Ptr;
	SDic_Val    *pRight_Ptr;
} TDic_Val, *PDic_Val;

#define CHAR_DIC_VAL(  pDic) ((*(pDic)).unCharacter)
#define CODE_DIC_VAL(  pDic) ((*(pDic)).unCode     )
#define PLEFT_DIC_VAL( pDic) ((*(pDic)).pLeft_Ptr  )
#define PRIGHT_DIC_VAL(pDic) ((*(pDic)).pRight_Ptr )

#define TYPE_GIF_ENCODING

#define EXP2_DIC_MAX  12  // 2^EXP2_DIC_MAX gives the maximum word counter in the dictionary during *all* the compressions.
                          //   Possible values: 3 to 25.
                          //   Attention: Beyond 12, you can have some errors of memory allocation
                          //   depending on your compiler and your computer.

class CEncoderLZW
{
public:

	CEncoderLZW(const BYTE* pBuffer, long lSize, BOOL bCopyBuffer = FALSE)
	{
		m_nError = LZW_NO_ERROR;

		m_ulVal_to_read          = 0;
		m_ulVal_to_write         = 0;
		m_unBit_counter_to_read  = 0;
		m_unBit_counter_to_write = 0;

		m_bDeleteBuffer = bCopyBuffer;

		m_lSize = lSize;
		m_lPos  = 0;

		if ( bCopyBuffer )
		{
			m_pBuffer = new BYTE[lSize];
			if ( !m_pBuffer )
			{
				m_lSize = 0;
				m_bDeleteBuffer = FALSE;
				m_nError = LZW_BAD_MEM_ALLOC;

				return;
			}
			::memcpy( m_pBuffer, pBuffer, lSize );
		}
		else
			m_pBuffer = (BYTE *)pBuffer;
	}
	~CEncoderLZW()
	{
		if ( m_bDeleteBuffer )
			delete []m_pBuffer;

        m_oFile.CloseFile();
	}
    bool Encode(wchar_t *wsFilePath)
    {
        m_oFile.CreateFileW(wsFilePath);
        //m_pFile = _wfopen( wsFilePath, _T("wb+") );

        Encode_lzw();

        return (!CheckError());
    }
private:

    void Write_Byte(unsigned char unChar)
    {
//        if ( m_pFile )
//            ::fputc( unChar, m_pFile );
        m_oFile.WriteFile(&unChar,1);
    }
	unsigned char Read_Byte()
	{
		if ( m_lPos == 18559 )
			int k = 10;
		if ( m_lPos >= m_lSize )
			return EOF;

		return (unsigned char)m_pBuffer[m_lPos++];
	}
	bool End_Of_Data()
	{
		if ( m_lPos > m_lSize - 1 )
			return true;

		return false;
	}

	bool CheckError()
	{
		if ( LZW_NO_ERROR != m_nError )
			return true;

		return false;
	}

	void Init_Encode_Dictionary1()
	{ 
		if ( CheckError() )
			return;

		m_unIndex_dic_max = 1 << 12; // Attention: Possible values: 2^3 to 2^EXP2_DIC_MAX.
		m_unInput_bit_counter = 8;   // Attention: Possible values: 1 to EXP2_DIC_MAX-1
								     // (usually, for pictures, set up to 1, or 4, or 8, in the case
								     // of monochrome, or 16-colors, or 256-colors picture). */
		if ( m_unInput_bit_counter == 1 )
			m_unBit_counter_min_encoding = 3;
		else 
			m_unBit_counter_min_encoding = m_unInput_bit_counter + 1;

		m_unInitialization_code = 1 << ( m_unBit_counter_min_encoding - 1 );

#ifdef TYPE_GIF_ENCODING
		m_unEnd_information_code = m_unInitialization_code + 1;
#else
		m_unEnd_information_code = m_unInitialization_code - 1;
#endif

		register unsigned int unIndex;
		for ( unIndex = 0; unIndex <= m_unEnd_information_code; unIndex++ )
		{ 
			if ( ( m_aoEncode_dictionary[unIndex] = (PDic_Val)malloc( sizeof(TDic_Val) ) ) == NULL )
			{
				while (unIndex)
				{ 
					unIndex--;
					free( m_aoEncode_dictionary[unIndex] );
				}
				m_nError = LZW_BAD_MEM_ALLOC;
				return;
			}
			CHAR_DIC_VAL( m_aoEncode_dictionary[unIndex] )   = unIndex;
			CODE_DIC_VAL( m_aoEncode_dictionary[unIndex] )   = unIndex;
			PLEFT_DIC_VAL( m_aoEncode_dictionary[unIndex] )  = NULL;
			PRIGHT_DIC_VAL( m_aoEncode_dictionary[unIndex] ) = NULL;
		}
		for ( ; unIndex < m_unIndex_dic_max; unIndex++ )
			m_aoEncode_dictionary[unIndex]=NULL;

		m_unIndex_dic = m_unEnd_information_code + 1;
		m_unBit_counter_encoding = m_unBit_counter_min_encoding;
	}

	void Init_Encode_Dictionary2()
	{
		if ( CheckError() )
			return;

		for ( register unsigned int unIndex = 0; unIndex < m_unIndex_dic_max; unIndex++ )
		{
			if ( m_aoEncode_dictionary[unIndex] )
			{
				PLEFT_DIC_VAL( m_aoEncode_dictionary[unIndex]  ) = NULL;
				PRIGHT_DIC_VAL( m_aoEncode_dictionary[unIndex] ) = NULL;
			}
		}
		m_unIndex_dic = m_unEnd_information_code + 1;
		m_unBit_counter_encoding = m_unBit_counter_min_encoding;
	}
	void Remove_Encode_Dictionary()
	{ 
		if ( CheckError() )
			return;

		for ( register unsigned int unIndex = 0; ( unIndex < m_unIndex_dic_max ) && ( m_aoEncode_dictionary[unIndex] != NULL) ; unIndex++ )
			free( m_aoEncode_dictionary[unIndex] );
	}
	PDic_Val Find_node(PDic_Val pCurrent_Node, unsigned int unSymbol)
	{
		if ( CheckError() )
			return NULL;

		PDic_Val pNew_Node;

		if ( PLEFT_DIC_VAL( pCurrent_Node ) == NULL )
			return pCurrent_Node;
		else 
		{ 
			pNew_Node = PLEFT_DIC_VAL( pCurrent_Node );
			while ( ( CHAR_DIC_VAL( pNew_Node ) != unSymbol ) && ( PRIGHT_DIC_VAL( pNew_Node ) != NULL ) )
				pNew_Node = PRIGHT_DIC_VAL( pNew_Node );
			
			return pNew_Node;
		}
	}
	void Add_node(PDic_Val pCurrent_Node, PDic_Val pNew_Node, unsigned int unSymbol)
	{ 
		if ( CheckError() )
			return;

		if ( m_aoEncode_dictionary[m_unIndex_dic] == NULL )
		{ 
			if ( ( m_aoEncode_dictionary[m_unIndex_dic] = (PDic_Val)malloc( sizeof(TDic_Val) ) ) == NULL )
			{ 
				Remove_Encode_Dictionary();
				m_nError = LZW_BAD_MEM_ALLOC;
				return;
			}
			CODE_DIC_VAL( m_aoEncode_dictionary[m_unIndex_dic]   ) = m_unIndex_dic;
			PLEFT_DIC_VAL( m_aoEncode_dictionary[m_unIndex_dic]  ) = NULL;
			PRIGHT_DIC_VAL( m_aoEncode_dictionary[m_unIndex_dic] ) = NULL;
		}
		CHAR_DIC_VAL( m_aoEncode_dictionary[m_unIndex_dic] ) = unSymbol;

		if ( pCurrent_Node == pNew_Node )
			PLEFT_DIC_VAL( pNew_Node ) = m_aoEncode_dictionary[m_unIndex_dic];
		else 
			PRIGHT_DIC_VAL( pNew_Node ) = m_aoEncode_dictionary[m_unIndex_dic];
		m_unIndex_dic++;

		if ( (unsigned)m_unIndex_dic == (unsigned)( 1 << m_unBit_counter_encoding ) )
			m_unBit_counter_encoding++;
	}
	bool Dictionary_sature()
	{
		return m_unIndex_dic == m_unIndex_dic_max - 1;
	} 
	void Write_code_lr(unsigned int unValue)
	{
		if ( CheckError() )
			return;

		m_ulVal_to_write = ( m_ulVal_to_write << m_unBit_counter_encoding ) | unValue;
		m_unBit_counter_to_write += m_unBit_counter_encoding;

		while ( m_unBit_counter_to_write >= 8 )
		{ 
			m_unBit_counter_to_write -= 8;
			Write_Byte( (unsigned char) ( m_ulVal_to_write >> m_unBit_counter_to_write ) );
			m_ulVal_to_write &= ( ( 1 << m_unBit_counter_to_write ) - 1 );
		}
	}

	void Complete_Encoding_lr()
	{ 
		if ( CheckError() )
			return;

		if ( m_unBit_counter_to_write > 0 )
			Write_Byte( (unsigned char)( m_ulVal_to_write << ( 8 - m_unBit_counter_to_write ) ) );
		
		m_ulVal_to_write = m_unBit_counter_to_write = 0;
	}
	void Write_code_rl(unsigned int unValue)
	{ 
		if ( CheckError() )
			return;

		m_ulVal_to_write |= ( (unsigned long int)unValue ) << m_unBit_counter_to_write;
		m_unBit_counter_to_write += m_unBit_counter_encoding;
		
		while ( m_unBit_counter_to_write >= 8 )
        { 
			m_unBit_counter_to_write -= 8;
			Write_Byte( (unsigned char)( m_ulVal_to_write & 0xFF ) );
			m_ulVal_to_write = ( m_ulVal_to_write >> 8 )&( ( 1 << m_unBit_counter_to_write ) - 1 );
        }
	}
	void Complete_encoding_rl()
	{ 
		if ( CheckError() )
			return;

		if ( m_unBit_counter_to_write > 0 )
			Write_Byte( (unsigned char)m_ulVal_to_write );
		
		m_ulVal_to_write = m_unBit_counter_to_write = 0;
	}
	unsigned int Read_Input()
	{
		if ( CheckError() )
			return 0;

		while ( m_unBit_counter_to_read < m_unInput_bit_counter )
		{ 
			m_ulVal_to_read = ( m_ulVal_to_read << 8 ) | Read_Byte();
			m_unBit_counter_to_read += 8;
		}

		m_unBit_counter_to_read -= m_unInput_bit_counter;
		
		unsigned int unRead_code = m_ulVal_to_read >> m_unBit_counter_to_read;
		m_ulVal_to_read &= ( ( 1 << m_unBit_counter_to_read ) - 1 );
		return unRead_code;
	}

	bool End_Input()
	{
		return ( m_unBit_counter_to_read < m_unInput_bit_counter ) && End_Of_Data();
	}
	int Encode_lzw()
	{ 
		if ( CheckError() )
			return 0;

		PDic_Val pCurrent_node, pNew_node;
		unsigned int unSymbol;

		if ( !End_Input() )
		{ 
			Init_Encode_Dictionary1();
#ifdef TYPE_GIF_ENCODING
			Write_code_lr( m_unInitialization_code );
#endif
			pCurrent_node = m_aoEncode_dictionary[ Read_Input() ];

			while ( !End_Input() )
			{ 
				unSymbol = Read_Input();
				pNew_node = Find_node( pCurrent_node, unSymbol );
				
				if ( ( pNew_node != pCurrent_node ) && ( CHAR_DIC_VAL( pNew_node ) == unSymbol ) )
					pCurrent_node = pNew_node;
				else 
				{ 
					Write_code_lr( CODE_DIC_VAL( pCurrent_node ) );
					Add_node( pCurrent_node, pNew_node, unSymbol );

					if ( Dictionary_sature() )
					{
#ifdef TYPE_GIF_ENCODING
						Write_code_lr( m_unInitialization_code );
#endif
						Init_Encode_Dictionary2();
					}
					pCurrent_node = m_aoEncode_dictionary[ unSymbol ];
				}
			}

			Write_code_lr( CODE_DIC_VAL( pCurrent_node ) );
#ifdef TYPE_GIF_ENCODING
			Write_code_lr( m_unEnd_information_code );
#endif
			Complete_Encoding_lr();
			Remove_Encode_Dictionary();
		}
		return 0;
	}


private:

	BYTE *m_pBuffer;
	long  m_lPos;
	long  m_lSize;
	BOOL  m_bDeleteBuffer;

    NSFile::CFileBinary m_oFile;
	int   m_nError;


	unsigned long m_ulVal_to_read;
	unsigned long m_ulVal_to_write;
	unsigned char m_unBit_counter_to_read;
	unsigned char m_unBit_counter_to_write;

	unsigned int  m_unIndex_dic;                // Word counter already known in the dictionary.
	unsigned char m_unBit_counter_encoding;     // Bit counter in the encoding.
	unsigned int  m_unIndex_dic_max;            // index_dic_max gives the maximum word counter in the dictionary during *one* compression. This constant is restricted to the range of end_information_code to 2^EXP2_DIC_MAX.
    unsigned char m_unInput_bit_counter;        // Bit counter for each data in input. With input_bit_counter=1, we can compress/decompress monochrome pictures and with input_bit_counter=8, we can handle 256-colors pictures or any kind of files.
	unsigned char m_unBit_counter_min_encoding; // Bit counter to encode 'initialization_code'.
	unsigned int  m_unInitialization_code;
    unsigned int  m_unEnd_information_code;     // initialization_code and end_information_code are both consecutive coming up just after the last known word in the initial dictionary.
	
	PDic_Val      m_aoEncode_dictionary[1<<EXP2_DIC_MAX];

};

#endif /* _LZW_ENCODER_H */
