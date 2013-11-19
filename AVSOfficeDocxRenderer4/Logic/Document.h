#pragma once
#include "Page.h"

#include <stdio.h>

namespace NSDocxRenderer
{
	static CString scXmlShortNodeEnd			=	L"\"/>";
	static CString scXmlNodeEnd					=	L"\">";

	static CString scXmlNodeFontNodeBegin		=	L"<w:font w:name=\"";
	static CString scXmlNodeFontPanoseBegin		=	L"<w:panose1 w:val=\"";
	static CString scXmlNodeFontCharsetBegin	=	L"<w:charset w:val=\"";
	static CString scXmlNodeFontFamilyBegin		=	L"<w:family w:val=\"";
	static CString scXmlNodeFontPitchTrue		=	L"<w:pitch w:val=\"fixed\" />";
	static CString scXmlNodeFontPitchFalse		=	L"<w:pitch w:val=\"variable\" />";
	static CString scXmlNodeFontEnd				=	L"</w:font>";



	static CString g_string_imageRID_png		=	L"<Relationship Id=\"rId%d\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/image\" Target=\"media/image%d.png\"/>";
	static CString g_string_imageRID_jpg		=	L"<Relationship Id=\"rId%d\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/image\" Target=\"media/image%d.jpg\"/>";


	static CString g_string_SIG					=	L"<w:sig w:usb0=\"%08x\" w:usb1=\"%08x\" w:usb2=\"%08x\" w:usb3=\"%08x\" w:csb0=\"%08x\" w:csb1=\"%08x\"/>";

	static CString g_string_DocumentStart		=	L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?>\
													 <w:document \
													 xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" \
													 xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" \
													 xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" \
													 xmlns:pic=\"http://schemas.openxmlformats.org/drawingml/2006/picture\" \
													 xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" \
													 xmlns:o=\"urn:schemas-microsoft-com:office:office\" \
													 xmlns:v=\"urn:schemas-microsoft-com:vml\" \
													 xmlns:w10=\"urn:schemas-microsoft-com:office:word\">\
													 <w:body>";

	static CString g_string_DocumentEnd			=	L"</w:body></w:document>";

	class CDocument
	{
	protected:
		enum ECommands
		{
			CommandNone = 0,
			CommandNewPage,
			CommandHeight,
			CommandWidth,

			CommandPen,
			CommandPenColor,
			CommandPenAlpha,
			CommandPenSize,
			CommandPenDashStyle,
			CommandPenLineStartCap,
			CommandPenLineEndCap,
			CommandPenLineJoin,
			CommandPenDashOffset,
			CommandPenAlign,
			CommandPenMiterLimit,
			CommandPenDashPattern,

			CommandBrush,
			CommandBrushType,
			CommandBrushColor1,
			CommandBrushAlpha1,
			CommandBrushColor2,
			CommandBrushAlpha2,
			CommandBrushTexturePath,
			CommandBrushTextureMode,
			CommandBrushTextureAlpha,
			CommandBrushLinearAngle,
			CommandBrushRect,

			CommandFont,
			CommandFontName,
			CommandFontPath,
			CommandFontSize,
			CommandFontStyle,
			CommandFontStringGID,
			CommandFontCharSpace,

			CommandShadow,
			CommandShadowDistanceX,
			CommandShadowDistanceY,
			CommandShadowBlurSize,
			CommandShadowColor,
			CommandShadowAlpha,
			CommandShadowVisible,

			CommandEdgeText,
			CommandEdgeVisible,
			CommandEdgeColor,
			CommandEdgeAlpha,
			CommandEdgeDist,

			CommandCommandDrawText,
			CommandCommandDrawTextEx,

			CommandBeginCommand,
			CommandEndCommand,

			CommandPathCommandMoveTo,
			CommandPathCommandLineTo,
			CommandPathCommandLinesTo,
			CommandPathCommandCurveTo,
			CommandPathCommandCurvesTo,
			CommandPathCommandArcTo,
			CommandPathCommandClose,
			CommandPathCommandEnd,
			CommandDrawPath,
			CommandPathCommandStart,
			CommandPathCommandText,
			CommandPathCommandTextEx,

			CommandSetCommandParams,

			CommandDrawImage,
			CommandDrawImageFromFile,

			CommandSetTransform,
			CommandResetTransform,

			CommandClipMode,

			CommandTextAssociationType
		};

		class DataWriter
		{
			BYTE* m_pBuffer;

		public:
			DataWriter( BYTE* pBuffer )
			{
				m_pBuffer = pBuffer;
			}

			void Write( ECommands data )
			{
				memcpy( m_pBuffer, &data, sizeof(ECommands) );
				m_pBuffer += sizeof(ECommands);
			}

			void Write( short data )
			{
				memcpy( m_pBuffer, &data, sizeof(short) );
				m_pBuffer += sizeof(short);
			}

			void Write( int data )
			{
				memcpy( m_pBuffer, &data, sizeof(int) );
				m_pBuffer += sizeof(int);
			}
			void Write( long data )
			{
				memcpy( m_pBuffer, &data, sizeof(long) );
				m_pBuffer += sizeof(long);
			}

			void Write( double data )
			{
				memcpy( m_pBuffer, &data, sizeof(double) );
				m_pBuffer += sizeof(double);
			}

			void Write( BYTE data )
			{
				*m_pBuffer = data;
				m_pBuffer += sizeof(BYTE);
			}

			void Write( WORD data )
			{
				memcpy( m_pBuffer, &data, sizeof(WORD) );
				m_pBuffer += sizeof(WORD);
			}

			void Write( DWORD data )
			{
				memcpy( m_pBuffer, &data, sizeof(DWORD) );
				m_pBuffer += sizeof(DWORD);
			}

			void Write( size_t data )
			{
				memcpy( m_pBuffer, &data, sizeof(size_t) );
				m_pBuffer += sizeof(size_t);
			}

			void Write( BSTR data )
			{
				memcpy( m_pBuffer, &data, sizeof(BSTR) );
				m_pBuffer += sizeof(BSTR);
			}

			void Write( IUnknown* data )
			{
				memcpy( m_pBuffer, &data, sizeof(IUnknown*) );
				m_pBuffer += sizeof(IUnknown*);
			}

			void Write( SAFEARRAY* data )
			{
				memcpy( m_pBuffer, &data, sizeof(SAFEARRAY*) );
				m_pBuffer += sizeof(SAFEARRAY*);
			}

		};

		class DataReader
		{
			BYTE* m_pBuffer;

		public:
			DataReader( BYTE* pBuffer )
			{
				m_pBuffer = pBuffer;
			}

			ECommands CommandId()
			{
				ECommands data;
				memcpy( &data, m_pBuffer, sizeof(ECommands) );
				m_pBuffer += sizeof(ECommands);
				return data;
			}

			short Short()
			{
				short data;
				memcpy( &data, m_pBuffer, sizeof(short) );
				m_pBuffer += sizeof(short);
				return data;
			}

			int Int()
			{
				int data;
				memcpy( &data, m_pBuffer, sizeof(int) );
				m_pBuffer += sizeof(int);
				return data;
			}

			long Long()
			{
				long data;
				memcpy( &data, m_pBuffer, sizeof(long) );
				m_pBuffer += sizeof(long);
				return data;
			}

			double Double()
			{
				double data;
				memcpy( &data, m_pBuffer, sizeof(double) );
				m_pBuffer += sizeof(double);
				return data;
			}

			BYTE Byte()
			{
				return *m_pBuffer++;
			}

			WORD Word()
			{
				WORD data;
				memcpy( &data, m_pBuffer, sizeof(WORD) );
				m_pBuffer += sizeof(WORD);
				return data;
			}

			DWORD Dword()
			{
				DWORD data;
				memcpy( &data, m_pBuffer, sizeof(DWORD) );
				m_pBuffer += sizeof(DWORD);
				return data;
			}

			size_t SizeT()
			{
				size_t data;
				memcpy( &data, m_pBuffer, sizeof(size_t) );
				m_pBuffer += sizeof(size_t);
				return data;
			}

			BSTR Bstr()
			{
				BSTR data;
				memcpy( &data, m_pBuffer, sizeof(BSTR) );
				m_pBuffer += sizeof(BSTR);
				return data;
			}

			IUnknown* Interface()
			{
				IUnknown* data;
				memcpy( &data, m_pBuffer, sizeof(IUnknown*) );
				m_pBuffer += sizeof(IUnknown*);
				return data;
			}

			SAFEARRAY* SafeArray()
			{
				SAFEARRAY* data;
				memcpy( &data, m_pBuffer, sizeof(SAFEARRAY*) );
				m_pBuffer += sizeof(SAFEARRAY*);
				return data;
			}

			void SkipData( size_t nDataSize )
			{
				m_pBuffer += nDataSize;
			}

			BYTE* GetBuffer() const
			{
				return m_pBuffer;
			}
		};

		class DataStorage
		{
			BYTE* m_pBuffer;
			size_t m_nSize;
			size_t m_nAllocSize;

		public:
			DataStorage()
			{
				m_pBuffer = NULL;
				m_nSize = 0;
				m_nAllocSize = 0;
			}

			DataStorage( const DataStorage& other )
			{
				m_pBuffer = NULL;
				m_nSize = 0;
				m_nAllocSize = 0;

				if( other.m_pBuffer )
				{
					m_pBuffer = new BYTE[other.m_nAllocSize];
					if( m_pBuffer )
					{
						memcpy( m_pBuffer, other.m_pBuffer, other.m_nSize );

						m_nSize = other.m_nSize;
						m_nAllocSize = other.m_nAllocSize;
					}
				}
			}

			DataStorage& operator = (const DataStorage& other)
			{
				Release();

				if( other.m_pBuffer )
				{
					m_pBuffer = new BYTE[other.m_nAllocSize];
					if( m_pBuffer )
					{
						memcpy( m_pBuffer, other.m_pBuffer, other.m_nSize );

						m_nSize = other.m_nSize;
						m_nAllocSize = other.m_nAllocSize;
					}
				}

				return *this;
			}

			~DataStorage()
			{
				if( m_pBuffer )
					delete [] m_pBuffer;
			}


			void Release()
			{
				if( m_pBuffer )
				{
					delete [] m_pBuffer;

					m_pBuffer = NULL;
					m_nSize = 0;
					m_nAllocSize = 0;
				}
			}

			BYTE* AddSection( size_t nSize )
			{
				nSize += sizeof(size_t);

				size_t nNewSize = m_nSize + nSize;
				if( nNewSize > m_nAllocSize )
				{
					nNewSize += 65536;

					BYTE* pNewBuffer = new BYTE[nNewSize];
					if( !pNewBuffer )
						return NULL;

					if( m_pBuffer )
					{
						memcpy( pNewBuffer, m_pBuffer, m_nSize );
						delete [] m_pBuffer;
					}

					m_pBuffer = pNewBuffer;
					m_nAllocSize = nNewSize;
				}

				memcpy( m_pBuffer + m_nSize, &nSize, sizeof(size_t) );

				BYTE* pRetData = m_pBuffer + m_nSize + sizeof(size_t);
				m_nSize += nSize;
				return pRetData;
			}

			size_t GetSize() const
			{
				return m_nSize;
			}

			BYTE* GetBuffer() const
			{
				return m_pBuffer;
			}

		};
		class StorageDynamicData
		{
			ATL::CSimpleArray<IUnknown*>  m_arrInterfaces;
			ATL::CSimpleArray<BSTR>       m_arrBstrs;
			ATL::CSimpleArray<SAFEARRAY*> m_arrSafeArrays;

		private:
			StorageDynamicData( const StorageDynamicData& other )
			{
			}

			StorageDynamicData& operator = (const StorageDynamicData& other)
			{
			}

		public:
			StorageDynamicData()
			{
			}

			~StorageDynamicData()
			{
				Release();
			}

			void Release()
			{
				ReleaseInterfaces();
				ReleaseBstrs();
				ReleaseSafeArrays();
			}

			IUnknown* AddInterface( IUnknown* pInterface )
			{
				if( pInterface )
				{
					if( m_arrInterfaces.Add( pInterface ) )
					{
						pInterface->AddRef();
						return pInterface;
					}
				}

				return NULL;
			}

			BSTR AddBstr( BSTR bstr )
			{
				if( bstr )
				{
					BSTR copy = SysAllocStringLen( bstr, (UINT)wcslen( bstr ) );
					if( copy )
					{
						if( m_arrBstrs.Add( copy ) )
						{
							return copy;
						}

						::SysFreeString( copy );
					}
				}

				return NULL;
			}

			SAFEARRAY* AddSafeArray( SAFEARRAY* pSafeArray )
			{
				if( pSafeArray )
				{
					SAFEARRAY* copy = NULL;
					SafeArrayCopy( pSafeArray, &copy );
					if( copy )
					{
						if( m_arrSafeArrays.Add( copy ) )
						{
							return copy;
						}

						SafeArrayDestroy( copy );
					}
				}

				return NULL;
			}

			void ReleaseInterfaces()
			{
				for( int i = 0; i < m_arrInterfaces.m_nSize; i++ )
				{
					m_arrInterfaces.m_aT[i]->Release();
				}

				m_arrInterfaces.RemoveAll();
			}

			void ReleaseBstrs()
			{
				for( int i = 0; i < m_arrBstrs.m_nSize; i++ )
				{
					SysFreeString( m_arrBstrs.m_aT[i] );
				}

				m_arrBstrs.RemoveAll();
			}

			void ReleaseSafeArrays()
			{
				for( int i = 0; i < m_arrSafeArrays.m_nSize; i++ )
				{
					SafeArrayDestroy( m_arrSafeArrays.m_aT[i] );
				}

				m_arrSafeArrays.RemoveAll();
			}
		};


	public:
		CDocument() : m_oWriter()
		{
			m_pSimpleGraphicsConverter  = NULL;
			m_lCurrentCommandType		= 0;

			m_pFontManager				= NULL;
			m_punkRenderer				= NULL;

			m_dWidth	= 0;
			m_dHeight	= 0;

			m_dDpiX		= 72;
			m_dDpiY		= 72;

			m_strTempDirectory			= _T("");
			m_lPagesCount				= 0;

			m_bRecordingCommands        = true;
			m_bIsNeedPDFTextAnalyzer	= false;
		}
		AVSINLINE void Clear()
		{
			m_lClipMode = 0;
			RELEASEINTERFACE(m_punkRenderer);
			RELEASEINTERFACE(m_pSimpleGraphicsConverter);
			RELEASEINTERFACE(m_pFontManager);
		}

		~CDocument()
		{
			m_lClipMode = 0;
			RELEASEINTERFACE(m_pSimpleGraphicsConverter);
			RELEASEINTERFACE(m_pFontManager);
		}


		void SetTextAssociationType( TextAssociationType type )
		{
			if( m_bRecordingCommands )
			{
				WriteCommand( CommandTextAssociationType, (int)type );
			}

			m_oCurrentPage.m_eTextAssociationType = type;
		}

		void WriteTestTable()
		{
			//CTable table;
			//table.AddColumn( 500 );
			//table.AddColumn( 1500 );
			//table.AddColumn( 850 );
			//table.AddRow();
			//table.m_lX = 1000;
			//table.m_lY = 1000;
			//table.ToXml( m_oWriter );

			//m_oDocumentStream.WriteStringUTF8(m_oWriter.GetData());
			//m_oWriter.ClearNoAttack();
		}


	protected:
		void WriteCommand( ECommands id )
		{
			BYTE* pBuffer = m_oStorageCommands.AddSection( sizeof id );
			if( pBuffer )
			{
				DataWriter data( pBuffer );
				data.Write( id );
			}
		}

		void WriteCommand( ECommands id, LONG param )
		{
			BYTE* pBuffer = m_oStorageCommands.AddSection( sizeof id + sizeof param );
			if( pBuffer )
			{
				DataWriter data( pBuffer );
				data.Write( id );
				data.Write( param );
			}
		}

		void WriteCommand( ECommands id, double param )
		{
			BYTE* pBuffer = m_oStorageCommands.AddSection( sizeof id + sizeof param );
			if( pBuffer )
			{
				DataWriter data( pBuffer );
				data.Write( id );
				data.Write( param );
			}
		}

		void WriteCommand( ECommands id, BOOL param )
		{
			BYTE* pBuffer = m_oStorageCommands.AddSection( sizeof id + sizeof param );
			if( pBuffer )
			{
				DataWriter data( pBuffer );
				data.Write( id );
				data.Write( param );
			}
		}

		void WriteCommand( ECommands id, BYTE param )
		{
			BYTE* pBuffer = m_oStorageCommands.AddSection( sizeof id + sizeof param );
			if( pBuffer )
			{
				DataWriter data( pBuffer );
				data.Write( id );
				data.Write( param );
			}
		}

		void WriteCommand( ECommands id, DWORD param )
		{
			BYTE* pBuffer = m_oStorageCommands.AddSection( sizeof id + sizeof param );
			if( pBuffer )
			{
				DataWriter data( pBuffer );
				data.Write( id );
				data.Write( param );
			}
		}

		void WriteCommand( ECommands id, BSTR param )
		{
			param = m_oStorageDynamicData.AddBstr( param );

			BYTE* pBuffer = m_oStorageCommands.AddSection( sizeof id + sizeof param );
			if( pBuffer )
			{
				DataWriter data( pBuffer );
				data.Write( id );
				data.Write( param );
			}
		}

		void WriteCommand( ECommands id, SAFEARRAY* param )
		{
			param = m_oStorageDynamicData.AddSafeArray( param );

			BYTE* pBuffer = m_oStorageCommands.AddSection( sizeof id + sizeof param );
			if( pBuffer )
			{
				DataWriter data( pBuffer );
				data.Write( id );
				data.Write( param );
			}
		}


		void PrintCommands()
		{
			size_t nTotalSize = m_oStorageCommands.GetSize();
			if( !nTotalSize )
				return;

			static int g_nFileNumber;

			CString szFileName;
			szFileName.AppendFormat( _T("d:\\TestTables\\com_%d.txt"), g_nFileNumber++ );

			FILE* pFile = ::_tfopen( szFileName.GetBuffer(), _T("wb") );
			if( !pFile )
				return;


			DataReader reader( m_oStorageCommands.GetBuffer() );

			size_t nCurSize = 0;
			do
			{
				size_t nSectionSize = reader.SizeT();
				nCurSize += nSectionSize;

				ECommands CommandId = reader.CommandId();
				switch( CommandId )
				{
				case CommandNewPage:            _ftprintf( pFile, _T("NewPage:         page = %d\r\n"), reader.Long() ); break;
				case CommandHeight:             _ftprintf( pFile, _T("Height:          height = %d\r\n"), reader.Double() ); break;
				case CommandWidth:              _ftprintf( pFile, _T("Width:           width = %d\r\n"), reader.Double() ); break;

				case CommandPen:                _ftprintf( pFile, _T("SetPen:          xml = \"%s\"\r\n"), reader.Bstr() ); break;
				case CommandPenColor:           _ftprintf( pFile, _T("PenColor:        color = %x\r\n"), reader.Long() ); break;
				case CommandPenAlpha:           _ftprintf( pFile, _T("PenAlpha:        alpha = %x\r\n"), reader.Long() ); break;
				case CommandPenSize:            _ftprintf( pFile, _T("PenSize:         size = %f\r\n"), reader.Double() ); break;
				case CommandPenDashStyle:       _ftprintf( pFile, _T("PenDashStyle:    style = %d\r\n"), (int)reader.Byte() ); break;
				case CommandPenLineStartCap:    _ftprintf( pFile, _T("PenLineStartCap: cap = %d\r\n"), (int)reader.Byte() ); break;
				case CommandPenLineEndCap:      _ftprintf( pFile, _T("PenLineEndCap:   cap = %d\r\n"), (int)reader.Byte() ); break;
				case CommandPenLineJoin:        _ftprintf( pFile, _T("PenLineJoin:     join = %d\r\n"), (int)reader.Byte() ); break;
				case CommandPenDashOffset:      _ftprintf( pFile, _T("PenDashOffset:   offset = %f\r\n"), reader.Double() ); break;
				case CommandPenAlign:           _ftprintf( pFile, _T("PenAlign:        align = %d\r\n"), reader.Long() ); break;
				case CommandPenMiterLimit:      _ftprintf( pFile, _T("PenMiterLimit:   limit = %d\r\n"), reader.Double() ); break;
				case CommandPenDashPattern:     _ftprintf( pFile, _T("PenDashPattern:  pattern = %p\r\n"), reader.SafeArray() ); break;

				case CommandBrush:              _ftprintf( pFile, _T("SetBrush:        xml = \"%s\"\r\n"), reader.Bstr() ); break;
				case CommandBrushType:          _ftprintf( pFile, _T("BrushType:       type = %d\r\n"), reader.Long() ); break;
				case CommandBrushColor1:        _ftprintf( pFile, _T("BrushColor1:     color = %x\r\n"), reader.Long() ); break;
				case CommandBrushAlpha1:        _ftprintf( pFile, _T("BrushAlpha1:     alpha = %x\r\n"), reader.Long() ); break;
				case CommandBrushColor2:        _ftprintf( pFile, _T("BrushColor2:     color = %x\r\n"), reader.Long() ); break;
				case CommandBrushAlpha2:        _ftprintf( pFile, _T("BrushAlpha2:     alpha = %x\r\n"), reader.Long() ); break;
				case CommandBrushTexturePath:   _ftprintf( pFile, _T("BrushTexturePath: path = \"%s\"\r\n"), reader.Bstr() ); break;
				case CommandBrushTextureMode:   _ftprintf( pFile, _T("BrushTextureMode: mode = %d\r\n"), reader.Long() ); break;
				case CommandBrushTextureAlpha:  _ftprintf( pFile, _T("BrushTextureAlpha: alpha = %d\r\n"), reader.Long() ); break;
				case CommandBrushLinearAngle:   _ftprintf( pFile, _T("BrushLinearAngle: angle = %f\r\n"), reader.Double() ); break;
				case CommandBrushRect:          _ftprintf( pFile, _T("BrushRect:       rect = %d, left = %f, top = %f, width = %f, height = %f\r\n"), reader.Int(), reader.Double(), reader.Double(), reader.Double(), reader.Double() ); break;

				case CommandFont:               _ftprintf( pFile, _T("SetFont:         xml = \"%s\"\r\n"), reader.Bstr() ); break;
				case CommandFontName:           _ftprintf( pFile, _T("FontName:        name = \"%s\"\r\n"), reader.Bstr() ); break;
				case CommandFontPath:           _ftprintf( pFile, _T("FontPath:        path = \"%s\"\r\n"), reader.Bstr() ); break;
				case CommandFontSize:           _ftprintf( pFile, _T("FontSize:        size = %f\r\n"), reader.Double() ); break;
				case CommandFontStyle:          _ftprintf( pFile, _T("FontStyle:       style = %d\r\n"), reader.Long() ); break;
				case CommandFontStringGID:      _ftprintf( pFile, _T("FontStringGID:   gid = %d\r\n"), reader.Int() ); break;
				case CommandFontCharSpace:      _ftprintf( pFile, _T("FontCharSpace:   space = %f\r\n"), reader.Double() ); break;

				case CommandShadow:             _ftprintf( pFile, _T("SetShadow:       xml = \"%s\"\r\n"), reader.Bstr() ); break;
				case CommandShadowDistanceX:    _ftprintf( pFile, _T("ShadowDistanceX: x = %f\r\n"), reader.Double() ); break;
				case CommandShadowDistanceY:    _ftprintf( pFile, _T("ShadowDistanceY: y = %f\r\n"), reader.Double() ); break;
				case CommandShadowBlurSize:     _ftprintf( pFile, _T("ShadowBlurSize:  size = %f\r\n"), reader.Double() ); break;
				case CommandShadowColor:        _ftprintf( pFile, _T("ShadowColor:     color = %x\r\n"), reader.Long() ); break;
				case CommandShadowAlpha:        _ftprintf( pFile, _T("ShadowAlpha:     alpha = %x\r\n"), reader.Long() ); break;
				case CommandShadowVisible:      _ftprintf( pFile, _T("ShadowVisible:   visible = %d\r\n"), reader.Int() ); break;

				case CommandEdgeText:           _ftprintf( pFile, _T("SetEdgeText:     xml = \"%s\"\r\n"), reader.Bstr() ); break;
				case CommandEdgeVisible:        _ftprintf( pFile, _T("EdgeVisible:     visible = %d\r\n"), reader.Long() ); break;
				case CommandEdgeColor:          _ftprintf( pFile, _T("EdgeColor:       color = %x\r\n"), reader.Long() ); break;
				case CommandEdgeAlpha:          _ftprintf( pFile, _T("EdgeAlpha:       alpha = %x\r\n"), reader.Long() ); break;
				case CommandEdgeDist:           _ftprintf( pFile, _T("EdgeDist:        distance = %f\r\n"), reader.Double() ); break;

				case CommandCommandDrawText:    _ftprintf( pFile, _T("CommandDrawText: text = \"%s\", x = %f, y = %f, width = %f, height = %f, baseline_offset = %f\r\n"), reader.Bstr(), reader.Double(), reader.Double(), reader.Double(), reader.Double(), reader.Double() ); break;
				case CommandCommandDrawTextEx:  _ftprintf( pFile, _T("CommandDrawTextEx: text = \"%s\", GID = \"%s\", source_code = \"%s\", x = %f, y = %f, width = %f, height = %f, baseline_offset = %f, flags = %d\r\n"), reader.Bstr(), reader.Bstr(), reader.Bstr(), reader.Double(), reader.Double(), reader.Double(), reader.Double(), reader.Double(), reader.Dword() ); break;

				case CommandBeginCommand:       _ftprintf( pFile, _T("BeginCommand:    id = %d\r\n"), reader.Dword() ); break;
				case CommandEndCommand:         _ftprintf( pFile, _T("EndCommand:      id = %d\r\n"), reader.Dword() ); break;

				case CommandPathCommandMoveTo:  _ftprintf( pFile, _T("PathCommandMoveTo: x = %f, y = %f\r\n"), reader.Double(), reader.Double() ); break;
				case CommandPathCommandLineTo:  _ftprintf( pFile, _T("PathCommandLineTo: x = %f, y = %f\r\n"), reader.Double(), reader.Double() ); break;
				case CommandPathCommandLinesTo: _ftprintf( pFile, _T("PathCommandLinesTo: ptr = %p\r\n"), reader.SafeArray() ); break;
				case CommandPathCommandCurveTo: _ftprintf( pFile, _T("PathCommandCurveTo: x1 = %f, y1 = %f, x2 = %f, y2 = %f, x3 = %f, y3 = %f\r\n"), reader.Double(), reader.Double(), reader.Double(), reader.Double(), reader.Double(), reader.Double() ); break;
				case CommandPathCommandCurvesTo:_ftprintf( pFile, _T("PathCommandCurvesTo: ptr = %p\r\n"), reader.SafeArray() ); break;
				case CommandPathCommandArcTo:   _ftprintf( pFile, _T("PathCommandArcTo: x = %f, y = %f, width = %f, height = %f, start_angle = %f, sweep_angle = %f\r\n"), reader.Double(), reader.Double(), reader.Double(), reader.Double(), reader.Double(), reader.Double() ); break;
				case CommandPathCommandClose:   _ftprintf( pFile, _T("PathCommandClose: \r\n")); break;
				case CommandPathCommandEnd:     _ftprintf( pFile, _T("PathCommandEnd: \r\n")); break;
				case CommandDrawPath:           _ftprintf( pFile, _T("DrawPath:        id = %d\r\n"), reader.Long() ); break;
				case CommandPathCommandStart:   _ftprintf( pFile, _T("PathCommandStart: \r\n")); break;
				case CommandPathCommandText:    _ftprintf( pFile, _T("PathCommandText: text = \"%s\", x = %f, y = %f, width = %f, height = %f, baseline_offset = %f\r\n"), reader.Bstr(), reader.Double(), reader.Double(), reader.Double(), reader.Double(), reader.Double() ); break;
				case CommandPathCommandTextEx:  _ftprintf( pFile, _T("PathCommandTextEx: text = \"%s\", GID = \"%s\", source_code = \"%s\", x = %f, y = %f, width = %f, height = %f, baseline_offset = %f, flags = %d\r\n"), reader.Bstr(), reader.Bstr(), reader.Bstr(), reader.Double(), reader.Double(), reader.Double(), reader.Double(), reader.Double(), reader.Dword() ); break;

				case CommandSetCommandParams:   _ftprintf( pFile, _T("SetCommandParams: angle = %f, x = %f, y = %f, width = %f, height = %f, flags = %d\r\n"), reader.Double(), reader.Double(), reader.Double(), reader.Double(), reader.Double(), reader.Dword() ); break;

				case CommandDrawImage:          _ftprintf( pFile, _T("DrawImage:        interface = %p, x = %f, y = %f, width = %f, height = %f\r\n"), reader.Interface(), reader.Double(), reader.Double(), reader.Double(), reader.Double() ); break;
				case CommandDrawImageFromFile:  _ftprintf( pFile, _T("DrawImageFromFile: path  =\"%s\", x = %f, y = %f, width = %f, height = %f\r\n"), reader.Bstr(), reader.Double(), reader.Double(), reader.Double(), reader.Double() ); break;

				case CommandSetTransform:       _ftprintf( pFile, _T("SetTransform:     a = %f, b = %f, c = %f, d = %f, e = %f, f = %f\r\n"), reader.Double(), reader.Double(), reader.Double(), reader.Double(), reader.Double(), reader.Double() ); break;
				case CommandResetTransform:     _ftprintf( pFile, _T("ResetTransform: \r\n")); break;

				case CommandClipMode:           _ftprintf( pFile, _T("ClipMode:         mode = %d\r\n"), reader.Long() ); break;
				case CommandTextAssociationType:_ftprintf( pFile, _T("SetTextAssociationType: type = %d\r\n"), reader.Int() ); break;

				default:
					reader.SkipData( nSectionSize - sizeof(size_t) - sizeof(ECommands) );
				}
			} while( nCurSize < nTotalSize );

			fclose( pFile );
		}
		void DetectTables()
		{
			m_oTableDetector.Clear();

			size_t nTotalSize = m_oStorageCommands.GetSize();
			if(0 == nTotalSize)
				return;

			int state = 0;
			ULONG_PTR pos = 0;
			int num = 0;

			NSDocxRenderer::CMatrix matrix;

			DataReader reader( m_oStorageCommands.GetBuffer() );
			size_t nCurSize = 0;

			int nCommandOp						=	0;
			Data::CBufferItemRef* pRefCommand	=	NULL;
			Analyzer::CPoint oMove;
			Analyzer::CPoint oEndPt;
			Analyzer::CLines* pLines			=	NULL;

			do
			{
				size_t nSectionSize = reader.SizeT();

				ECommands CommandId = reader.CommandId();
				switch(CommandId)
				{
				case CommandSetTransform: 
					matrix.SetElements(reader.Double(), reader.Double(), reader.Double(), reader.Double(), reader.Double(), reader.Double());
					break;

				case CommandResetTransform:
					matrix.Reset();
					break;

				/*

				//case CommandCommandDrawText:    
				//	{
				//		ULONG_PTR nBegin	=	(ULONG_PTR)(reader.GetBuffer() - sizeof(size_t) - sizeof(ECommands)) - (ULONG_PTR)(m_oStorageCommands.GetBuffer());

				//		double dX			=	reader.Double();
				//		double dY			=	reader.Double();
				//		double dWidth		=	reader.Double();
				//		double dHeight		=	reader.Double();

				//		m_oCollector.AddText(new Analyzer::CTextItemRef(reader.Bstr(), 
				//			dX,  
				//			dY,  
				//			dWidth,
				//			dHeight,
				//			reader.Double(), nBegin, nBegin + nSectionSize));
				//	}
				//	break;

				//case CommandCommandDrawTextEx:    
				//	{
				//		ULONG_PTR nBegin	=	(ULONG_PTR)(reader.GetBuffer() - sizeof(size_t) - sizeof(ECommands)) - (ULONG_PTR)(m_oStorageCommands.GetBuffer());

				//		double dX			=	reader.Double();
				//		double dY			=	reader.Double();
				//		double dWidth		=	reader.Double();
				//		double dHeight		=	reader.Double();

				//		matrix.TransformPoint(dX, dY);

				//		m_oCollector.AddText(new Analyzer::CTextItemRef(reader.Bstr(), reader.Bstr(),reader.Bstr(),
				//			dX, 
				//			dY, 
				//			dWidth,
				//			dHeight,
				//			reader.Double(), reader.Dword(), nBegin, nBegin + nSectionSize));
				//	}
				//	break;
					
				*/

				case CommandPathCommandMoveTo:
					{
						if (0 == nCommandOp)
						{
							nCommandOp					=	3;
							if (NULL == pRefCommand)
								pRefCommand				=	new Data::CBufferItemRef();

							if (pRefCommand)
							{
								pRefCommand->m_nBegin	=	(ULONG_PTR)(reader.GetBuffer() - sizeof(size_t) - sizeof(ECommands)) - (ULONG_PTR)(m_oStorageCommands.GetBuffer());

								oMove					=	Analyzer::CPoint(reader.Double(), reader.Double());
								oEndPt					=	oMove;

								double dX				=	oMove.GetX();
								double dY				=	oMove.GetY();

								matrix.TransformPoint(dX, dY);

								oMove					=	Analyzer::CPoint(dX, dY);
							}
						}
						else
						{
							nCommandOp					=	0;
							reader.SkipData(nSectionSize - sizeof(size_t) - sizeof(ECommands));
						}

						// ATLTRACE (L"MOVE : (%f, %f)\n", oMove.GetX(), oMove.GetY());
					}

					//if( state != 0 )
					//{
					//	state = 0;
					//	reader.SkipData( nSectionSize - sizeof(size_t) - sizeof(ECommands) );
					//}
					//else
					//{
					//	state = 3;
					//	pos = (ULONG_PTR)(reader.GetBuffer() - sizeof(size_t) - sizeof(ECommands)) - (ULONG_PTR)(m_oStorageCommands.GetBuffer());
					//	pt[0][1] = reader.Double();
					//	pt[0][0] = reader.Double();
					//	num = 1;
					//}

					break;

				case CommandPathCommandLineTo:
					{
						if(nCommandOp != 3)
						{
							nCommandOp		=	0;
							reader.SkipData(nSectionSize - sizeof(size_t) - sizeof(ECommands));
						}
						else
						{
							if (NULL == pLines)
								pLines		=	new Analyzer::CLines(pRefCommand);

							if (pLines)
							{
								oEndPt		=	Analyzer::CPoint(reader.Double(), reader.Double());

								double dX	=	oEndPt.GetX();
								double dY	=	oEndPt.GetY();

								matrix.TransformPoint(dX, dY);

								oEndPt		=	Analyzer::CPoint(dX, dY);

								if ( (oMove.GetX() == oEndPt.GetX()) && fabs(oMove.GetY() - oEndPt.GetY()) < 1.0 )
								{
									//ATLTRACE (L"BAD LINE\n");
									continue;
								}

								if ( (oMove.GetY() == oEndPt.GetY()) && fabs(oMove.GetX() - oEndPt.GetX()) < 1.0 )
								{
									//ATLTRACE (L"BAD LINE\n");
									continue;
								}

								pLines->Add(oMove, oEndPt, pRefCommand);

								ATLTRACE (L"LINE :  (%f, %f) - (%f, %f)\n", oMove.GetX(), oMove.GetY(), oEndPt.GetX(), oEndPt.GetY());

								oMove		=	oEndPt;
							}
						}

						//if( state != 3 || num > 4 )
						//{
						//	state = 0;
						//	reader.SkipData( nSectionSize - sizeof(size_t) - sizeof(ECommands) );
						//}
						//else
						//{
						//	pt[num][1] = reader.Double();
						//	pt[num][0] = reader.Double();
						//	num++;
						//}
					}
					break;

				case CommandPathCommandClose:
					{
						if (nCommandOp == 3) // && num == 5)
						{
							//if( pt[0][0] == pt[4][0] && pt[0][1] == pt[4][1] )
							//{
							//for( int i = 0; i < 4; i++ )
							//{
							//	matrix.TransformPoint( pt[i][0], pt[i][1] );
							//}

							pRefCommand->m_nEnd	=	(ULONG_PTR)(reader.GetBuffer() - sizeof(size_t) - sizeof(ECommands)) - (ULONG_PTR)(m_oStorageCommands.GetBuffer());
							if (m_oCollector.GetLines().AddLines(pLines))
							{
								pLines			=	NULL;
								pRefCommand		=	NULL;

								//ATLTRACE (L"CLOSE\n");
							}
							else
							{
								RELEASEOBJECT(pLines);
								RELEASEOBJECT(pRefCommand);
							}

						}

						nCommandOp	=	0;

						//if( state == 3 && num == 5 )
						//{
						//	if( pt[0][0] == pt[4][0] && pt[0][1] == pt[4][1] )
						//	{
						//		for( int i = 0; i < 4; i++ )
						//		{
						//			matrix.TransformPoint( pt[i][0], pt[i][1] );
						//		}

						//		CBlock* pBlock = m_oTableDetector.AddBlock( pt );
						//		if( pBlock )
						//		{
						//			pBlock->m_id = pos;
						//			pBlock->m_color = m_oBrush.Color1;
						//			pBlock->m_alpha = m_oBrush.Alpha1;
						//		}
						//	}
						//}
						//state = 0;
					}
					break;

				default:
					{
						nCommandOp	=	0;
						reader.SkipData( nSectionSize - sizeof(size_t) - sizeof(ECommands) );
					}
					{
					//	state = 0;
					//	reader.SkipData( nSectionSize - sizeof(size_t) - sizeof(ECommands) );
					}
				}

				nCurSize += nSectionSize;
			}
			while(nCurSize < nTotalSize);

			m_oTableDetector.Calculate();

			if (m_oCollector.Normalize())
			{
				CleanAssociatedCommands ();			//	зачищаем команды которые образуют таблицы
			}
		}

		void CleanAssociatedCommands ()			//	зачищаем команды которые образуют таблицы
		{
			BYTE* pData	=	m_oStorageCommands.GetBuffer();

			/*

			//for (size_t i = 0; i < m_oCollector.GetTextItems(); ++i)
			//{
			//	Analyzer::CTextItemRef* pItem = m_oCollector.GetTextItem(i);
			//	if (pItem)
			//	{
			//		if (pItem->IsInTable())
			//		{
			//			DataReader oReader (pData + pItem->GetBufBegin());
			//			
			//			size_t nSize = oReader.SizeT();

			//			DataWriter oWriter(oReader.GetBuffer());
			//			oWriter.Write(CommandNone);
			//		}
			//	}				
			//}
			
			*/

			for (size_t i = 0; i < m_oCollector.GetLines().GetBBS().GetCount(); ++i)
			{
				Analyzer::CBoundingBox oBox			=	m_oCollector.GetLines().GetBBS().GetBox(i);
				const Analyzer::CSortLines& oLines	=	oBox.GetInnerLines();
				for (size_t j = 0; j < oLines.GetCount(); ++j)
				{
					const Analyzer::CLine* pLine	=	oLines.Get (j);
					if (pLine)
					{
						if (pLine->IsAdd())
						{
							Data::CBufferItemRef* m_pBufRef = pLine->GetItemRef ();
							if (m_pBufRef)
							{		
								unsigned long bufPos	=	m_pBufRef->GetBufBegin();
								while (bufPos < m_pBufRef->m_nEnd) 
								{
									DataReader oReader (pData + bufPos);

									size_t nSize = oReader.SizeT();

									DataWriter oWriter(oReader.GetBuffer());
									oWriter.Write(CommandNone);

									bufPos	+=	( nSize - sizeof(size_t) ) + sizeof(ECommands);
								}								
							}
						}
					}
				}
			}
		}

		void ProcessCommand ()
		{
			PrintCommands();

			DetectTables();
			
			m_oCurrentPage.SetTables(&m_oCollector);

			size_t nTotalSize = m_oStorageCommands.GetSize();
			if( !nTotalSize )
				return;

			DataReader reader( m_oStorageCommands.GetBuffer() );

			m_bRecordingCommands = false;

			int marker = 0;

			size_t nCurSize = 0;
			do
			{
				size_t nSectionSize = reader.SizeT();
				nCurSize += nSectionSize;

				ECommands CommandId = reader.CommandId();
				switch( CommandId )
				{
				//case CommandNewPage:			NewPage(); break;
				case CommandHeight:             put_Height( reader.Double() ); break;
				case CommandWidth:              put_Width( reader.Double() ); break;

				case CommandPen:                SetPen( reader.Bstr() ); break;
				case CommandPenColor:           put_PenColor( reader.Long() ); break;
				case CommandPenAlpha:           put_PenAlpha( reader.Long() ); break;
				case CommandPenSize:            put_PenSize( reader.Double() ); break;
				case CommandPenDashStyle:       put_PenDashStyle( reader.Byte() ); break;
				case CommandPenLineStartCap:    put_PenLineStartCap( reader.Byte() ); break;
				case CommandPenLineEndCap:      put_PenLineEndCap( reader.Byte() ); break;
				case CommandPenLineJoin:        put_PenLineJoin( reader.Byte() ); break;
				case CommandPenDashOffset:      put_PenDashOffset( reader.Double() ); break;
				case CommandPenAlign:           put_PenAlign( reader.Long() ); break;
				case CommandPenMiterLimit:      put_PenMiterLimit( reader.Double() ); break;
				case CommandPenDashPattern:     PenDashPattern( reader.SafeArray() ); break;

				case CommandBrush:              SetBrush( reader.Bstr() ); break;
				case CommandBrushType:          put_BrushType( reader.Long() ); break;
				case CommandBrushColor1:        put_BrushColor1( reader.Long() ); break;
				case CommandBrushAlpha1:        put_BrushAlpha1( reader.Long() ); break;
				case CommandBrushColor2:        put_BrushColor2( reader.Long() ); break;
				case CommandBrushAlpha2:        put_BrushAlpha2( reader.Long() ); break;
				case CommandBrushTexturePath:   put_BrushTexturePath( reader.Bstr() ); break;
				case CommandBrushTextureMode:   put_BrushTextureMode( reader.Long() ); break;
				case CommandBrushTextureAlpha:  put_BrushTextureAlpha( reader.Long() ); break;
				case CommandBrushLinearAngle:   put_BrushLinearAngle( reader.Double() ); break;
				case CommandBrushRect:          BrushRect( reader.Int(), reader.Double(), reader.Double(), reader.Double(), reader.Double() ); break;

				case CommandFont:               SetFont( reader.Bstr() ); break;
				case CommandFontName:           put_FontName( reader.Bstr() ); break;
				case CommandFontPath:           put_FontPath( reader.Bstr() ); break;
				case CommandFontSize:           put_FontSize( reader.Double() ); break;
				case CommandFontStyle:          put_FontStyle( reader.Long() ); break;
				case CommandFontStringGID:      put_FontStringGID( reader.Int() ); break;
				case CommandFontCharSpace:      put_FontCharSpace( reader.Double() ); break;

				case CommandShadow:             SetShadow( reader.Bstr() ); break;
				case CommandShadowDistanceX:    put_ShadowDistanceX( reader.Double() ); break;
				case CommandShadowDistanceY:    put_ShadowDistanceY( reader.Double() ); break;
				case CommandShadowBlurSize:     put_ShadowBlurSize( reader.Double() ); break;
				case CommandShadowColor:        put_ShadowColor( reader.Long() ); break;
				case CommandShadowAlpha:        put_ShadowAlpha( reader.Long() ); break;
				case CommandShadowVisible:      put_ShadowVisible( reader.Int() ); break;

				case CommandEdgeText:           SetEdgeText( reader.Bstr() ); break;
				case CommandEdgeVisible:        put_EdgeVisible( reader.Long() ); break;
				case CommandEdgeColor:          put_EdgeColor( reader.Long() ); break;
				case CommandEdgeAlpha:          put_EdgeAlpha( reader.Long() ); break;
				case CommandEdgeDist:           put_EdgeDist( reader.Double() ); break;

				case CommandCommandDrawText: 
					{
						CommandDrawText( reader.Bstr(), reader.Double(), reader.Double(), reader.Double(), reader.Double(), reader.Double() ); 
					}
					break;

				case CommandCommandDrawTextEx : 
					{
						CommandDrawTextEx(reader.Bstr(), reader.Bstr(), reader.Bstr(), reader.Double(), reader.Double(), reader.Double(), reader.Double(), reader.Double(), reader.Dword());
					}
					break;

				case CommandBeginCommand:       BeginCommand( reader.Dword() ); break;
				case CommandEndCommand:         
					{
						EndCommand( reader.Dword() ); 

						//if( !marker )
						//{
						//	marker = 1;

						//	if( m_oTableDetector.GetCountTables() )
						//	{
						//		NSDocxRenderer::CMatrix matrix = m_oTransform;
						//		m_oTransform.Reset();

						//		ResetTransform();
						//		put_BrushColor1( 0xe0 );
						//		put_BrushColor2( 0xe0 );
						//		put_BrushAlpha1( 255 );
						//		put_BrushAlpha2( 255 );
						//		
						//		for( CTable* pTable = m_oTableDetector.GetFirstTable(); pTable != NULL; pTable = m_oTableDetector.GetNextTable() )
						//		{
						//			BeginCommand( 4 );
						//			PathCommandEnd(); 
						//			PathCommandMoveTo( pTable->GetX(), pTable->GetY() );
						//			PathCommandLineTo( pTable->GetX() + pTable->GetWidth(), pTable->GetY() );
						//			PathCommandLineTo( pTable->GetX() + pTable->GetWidth(), pTable->GetY() + pTable->GetHeight() );
						//			PathCommandLineTo( pTable->GetX(), pTable->GetY() + pTable->GetHeight() );
						//			PathCommandLineTo( pTable->GetX(), pTable->GetY() );
						//			PathCommandClose(); 
						//			DrawPath( 256 );
						//			EndCommand( 4 );
						//		}

						//		m_oTransform = matrix;
						//	}
						//}

						break;
					}

				case CommandPathCommandMoveTo: 
					{
						PathCommandMoveTo( reader.Double(), reader.Double() ); 
					}
					break;

				case CommandPathCommandLineTo: 
					{
						PathCommandLineTo( reader.Double(), reader.Double() );
					}
					break;

				case CommandPathCommandLinesTo: PathCommandLinesTo( reader.SafeArray() ); break;
				case CommandPathCommandCurveTo: PathCommandCurveTo( reader.Double(), reader.Double(), reader.Double(), reader.Double(), reader.Double(), reader.Double() ); break;
				case CommandPathCommandCurvesTo:PathCommandCurvesTo( reader.SafeArray() ); break;
				case CommandPathCommandArcTo:   PathCommandArcTo( reader.Double(), reader.Double(), reader.Double(), reader.Double(), reader.Double(), reader.Double() ); break;
				case CommandPathCommandClose:   PathCommandClose(); break;
				case CommandPathCommandEnd:     PathCommandEnd(); break;
				case CommandDrawPath:           DrawPath( reader.Long() ); break;
				case CommandPathCommandStart:   PathCommandStart(); break;
				case CommandPathCommandText:    PathCommandText( reader.Bstr(), reader.Double(), reader.Double(), reader.Double(), reader.Double(), reader.Double() ); break;
				case CommandPathCommandTextEx:  PathCommandTextEx( reader.Bstr(), reader.Bstr(), reader.Bstr(), reader.Double(), reader.Double(), reader.Double(), reader.Double(), reader.Double(), reader.Dword() ); break;

				case CommandSetCommandParams:   SetCommandParams( reader.Double(), reader.Double(), reader.Double(), reader.Double(), reader.Double(), reader.Dword() ); break;

				case CommandDrawImage:          DrawImage( reader.Interface(), reader.Double(), reader.Double(), reader.Double(), reader.Double() ); break;
				case CommandDrawImageFromFile:  DrawImageFromFile( reader.Bstr(), reader.Double(), reader.Double(), reader.Double(), reader.Double() ); break;

				case CommandSetTransform:       SetTransform( reader.Double(), reader.Double(), reader.Double(), reader.Double(), reader.Double(), reader.Double() ); break;
				case CommandResetTransform:     ResetTransform(); break;

				case CommandClipMode:           put_ClipMode( reader.Long() ); break;
				case CommandTextAssociationType:SetTextAssociationType( (TextAssociationType)reader.Int() ); break;
				case CommandNone:
					{
						reader.SkipData( nSectionSize - sizeof(size_t) - sizeof(ECommands) );
					}
					break;

				default:
					reader.SkipData( nSectionSize - sizeof(size_t) - sizeof(ECommands) );
				}
			} while( nCurSize < nTotalSize );

			m_bRecordingCommands = true;

			m_oStorageCommands.Release();
			m_oStorageDynamicData.Release();
		}


	public:
		AVSINLINE HRESULT NewPage()
		{
			ProcessCommand();

			if (0 != m_lPagesCount)
			{
				m_oCurrentPage.WriteSectionToFile(false, m_oWriter);
				m_oDocumentStream.WriteStringUTF8(m_oWriter.GetData());
				m_oWriter.ClearNoAttack();
			}

			if( m_bRecordingCommands )
			{
				WriteCommand( CommandNewPage, m_lPagesCount );
			}

			WriteTestTable();

			m_oPen.SetDefaultParams();
			m_oBrush.SetDefaultParams();
			m_oFont.SetDefaultParams();
			m_oShadow.SetDefaultParams();
			m_oEdge.SetDefaultParams();

			m_oTransform.Reset();

			++m_lPagesCount;
			m_oCurrentPage.Clear();

			return S_OK;
		}
		AVSINLINE HRESULT get_Height(double* dHeight)
		{
			*dHeight = m_dHeight;
			return S_OK;
		}
		AVSINLINE HRESULT put_Height(double dHeight)
		{
			if( m_bRecordingCommands )
			{
				WriteCommand( CommandHeight, dHeight );
			}

			m_dHeight					= dHeight;
			m_oCurrentPage.m_dHeight	= dHeight;
			return S_OK;
		}
		AVSINLINE HRESULT get_Width(double* dWidth)
		{
			*dWidth = m_dWidth;
			return S_OK;
		}
		AVSINLINE HRESULT put_Width(double dWidth)
		{
			if( m_bRecordingCommands )
			{
				WriteCommand( CommandWidth, dWidth );
			}

			m_dWidth					= dWidth;
			m_oCurrentPage.m_dWidth		= dWidth;
			return S_OK;
		}
		AVSINLINE HRESULT get_DpiX(double* dDpiX)
		{
			*dDpiX = m_dDpiX;
			return S_OK;
		}
		AVSINLINE HRESULT get_DpiY(double* dDpiY)
		{
			*dDpiY = m_dDpiY;
			return S_OK;
		}
		//-------- Функции для задания настроек текста ----------------------------------------------
		// pen --------------------------------------------------------------------------------------
		AVSINLINE HRESULT SetPen(BSTR bsXML)
		{
			if( m_bRecordingCommands )
			{
				WriteCommand( CommandPen, bsXML );
			}

			m_oPen.FromXmlString( CString(bsXML) );
			return S_OK;
		}
		AVSINLINE HRESULT get_PenColor(LONG* lColor)
		{
			*lColor = m_oPen.Color;
			return S_OK;
		}
		AVSINLINE HRESULT put_PenColor(LONG lColor)
		{
			if( m_bRecordingCommands )
			{
				WriteCommand( CommandPenColor, lColor );
			}

			m_oPen.Color = lColor;
			return S_OK;
		}
		AVSINLINE HRESULT get_PenAlpha(LONG* lAlpha)
		{
			*lAlpha = m_oPen.Alpha;
			return S_OK;
		}
		AVSINLINE HRESULT put_PenAlpha(LONG lAlpha)
		{
			if( m_bRecordingCommands )
			{
				WriteCommand( CommandPenAlpha, lAlpha );
			}

			m_oPen.Alpha = lAlpha;
			return S_OK;
		}
		AVSINLINE HRESULT get_PenSize(double* dSize)
		{
			*dSize = m_oPen.Size;
			return S_OK;
		}
		AVSINLINE HRESULT put_PenSize(double dSize)
		{
			if( m_bRecordingCommands )
			{
				WriteCommand( CommandPenSize, dSize );
			}

			m_oPen.Size = dSize;
			return S_OK;
		}
		AVSINLINE HRESULT get_PenDashStyle(BYTE* val)
		{
			*val = m_oPen.DashStyle;
			return S_OK;
		}
		AVSINLINE HRESULT put_PenDashStyle(BYTE val)
		{
			if( m_bRecordingCommands )
			{
				WriteCommand( CommandPenDashStyle, val );
			}

			m_oPen.DashStyle = val;
			return S_OK;
		}
		AVSINLINE HRESULT get_PenLineStartCap(BYTE* val)
		{
			*val = m_oPen.LineStartCap;
			return S_OK;
		}
		AVSINLINE HRESULT put_PenLineStartCap(BYTE val)
		{
			if( m_bRecordingCommands )
			{
				WriteCommand( CommandPenLineStartCap, val );
			}

			m_oPen.LineStartCap = val;
			return S_OK;
		}
		AVSINLINE HRESULT get_PenLineEndCap(BYTE* val)
		{
			*val = m_oPen.LineEndCap;
			return S_OK;
		}
		AVSINLINE HRESULT put_PenLineEndCap(BYTE val)
		{
			if( m_bRecordingCommands )
			{
				WriteCommand( CommandPenLineEndCap, val );
			}

			m_oPen.LineEndCap = val;
			return S_OK;
		}
		AVSINLINE HRESULT get_PenLineJoin(BYTE* val)
		{
			*val = m_oPen.LineJoin;
			return S_OK;
		}
		AVSINLINE HRESULT put_PenLineJoin(BYTE val)
		{
			if( m_bRecordingCommands )
			{
				WriteCommand( CommandPenLineJoin, val );
			}

			m_oPen.LineJoin = val;
			return S_OK;
		}
		AVSINLINE HRESULT get_PenDashOffset(double* val)
		{
			*val = m_oPen.DashOffset;
			return S_OK;
		}
		AVSINLINE HRESULT put_PenDashOffset(double val)
		{
			if( m_bRecordingCommands )
			{
				WriteCommand( CommandPenDashOffset, val );
			}

			m_oPen.DashOffset = val;
			return S_OK;
		}
		AVSINLINE HRESULT get_PenAlign(LONG* val)
		{
			*val = m_oPen.Align;
			return S_OK;
		}
		AVSINLINE HRESULT put_PenAlign(LONG val)
		{
			if( m_bRecordingCommands )
			{
				WriteCommand( CommandPenAlign, val );
			}

			m_oPen.Align = val;
			return S_OK;
		}
		AVSINLINE HRESULT get_PenMiterLimit(double* val)
		{
			*val = m_oPen.MiterLimit;
			return S_OK;
		}
		AVSINLINE HRESULT put_PenMiterLimit(double val)
		{
			if( m_bRecordingCommands )
			{
				WriteCommand( CommandPenMiterLimit, val );
			}

			m_oPen.MiterLimit = val;
			return S_OK;
		}
		AVSINLINE HRESULT PenDashPattern(SAFEARRAY* pPattern)
		{
			if (!pPattern)
				return S_OK;

			if( m_bRecordingCommands )
			{
				WriteCommand( CommandPenDashPattern, pPattern );
			}

			m_oPen.SetDashPattern( (double*)pPattern->pvData, (int)pPattern->rgsabound[0].cElements );

			return S_OK;
		}

		// brush ------------------------------------------------------------------------------------
		AVSINLINE HRESULT SetBrush(BSTR bsXML)
		{
			if( m_bRecordingCommands )
			{
				WriteCommand( CommandBrush, bsXML );
			}

			m_oBrush.FromXmlString( CString(bsXML) );
			return S_OK;
		}
		AVSINLINE HRESULT get_BrushType(LONG* lType)
		{
			*lType = m_oBrush.Type;
			return S_OK;
		}
		AVSINLINE HRESULT put_BrushType(LONG lType)
		{
			if( m_bRecordingCommands )
			{
				WriteCommand( CommandBrushType, lType );
			}

			m_oBrush.Type = lType;
			return S_OK;
		}
		AVSINLINE HRESULT get_BrushColor1(LONG* lColor)
		{
			*lColor = m_oBrush.Color1;
			return S_OK;
		}
		AVSINLINE HRESULT put_BrushColor1(LONG lColor)
		{
			if( m_bRecordingCommands )
			{
				WriteCommand( CommandBrushColor1, lColor );
			}

			m_oBrush.Color1 = lColor;
			return S_OK;
		}
		AVSINLINE HRESULT get_BrushAlpha1(LONG* lAlpha)
		{
			*lAlpha = m_oBrush.Alpha1;
			return S_OK;
		}
		AVSINLINE HRESULT put_BrushAlpha1(LONG lAlpha)
		{
			if( m_bRecordingCommands )
			{
				WriteCommand( CommandBrushAlpha1, lAlpha );
			}

			m_oBrush.Alpha1 = lAlpha;
			return S_OK;
		}
		AVSINLINE HRESULT get_BrushColor2(LONG* lColor)
		{
			*lColor = m_oBrush.Color2;
			return S_OK;
		}
		AVSINLINE HRESULT put_BrushColor2(LONG lColor)
		{
			if( m_bRecordingCommands )
			{
				WriteCommand( CommandBrushColor2, lColor );
			}

			m_oBrush.Color2 = lColor;
			return S_OK;
		}
		AVSINLINE HRESULT get_BrushAlpha2(LONG* lAlpha)
		{
			*lAlpha = m_oBrush.Alpha2;
			return S_OK;
		}
		AVSINLINE HRESULT put_BrushAlpha2(LONG lAlpha)
		{
			if( m_bRecordingCommands )
			{
				WriteCommand( CommandBrushAlpha2, lAlpha );
			}

			m_oBrush.Alpha2 = lAlpha;
			return S_OK;
		}
		AVSINLINE HRESULT get_BrushTexturePath(BSTR* bsPath)
		{
			*bsPath = m_oBrush.TexturePath.AllocSysString();
			return S_OK;
		}
		AVSINLINE HRESULT put_BrushTexturePath(BSTR bsPath)
		{
			if( m_bRecordingCommands )
			{
				WriteCommand( CommandBrushTexturePath, bsPath );
			}

			m_oBrush.TexturePath = bsPath;
			return S_OK;
		}
		AVSINLINE HRESULT get_BrushTextureMode(LONG* lMode)
		{
			*lMode = m_oBrush.TextureMode;
			return S_OK;
		}
		AVSINLINE HRESULT put_BrushTextureMode(LONG lMode)
		{
			if( m_bRecordingCommands )
			{
				WriteCommand( CommandBrushTextureMode, lMode );
			}

			m_oBrush.TextureMode = lMode;
			return S_OK;
		}
		AVSINLINE HRESULT get_BrushTextureAlpha(LONG* lTxAlpha)
		{
			*lTxAlpha = m_oBrush.TextureAlpha;
			return S_OK;
		}
		AVSINLINE HRESULT put_BrushTextureAlpha(LONG lTxAlpha)
		{
			if( m_bRecordingCommands )
			{
				WriteCommand( CommandBrushTextureAlpha, lTxAlpha );
			}

			m_oBrush.TextureAlpha = lTxAlpha;
			return S_OK;
		}
		AVSINLINE HRESULT get_BrushLinearAngle(double* dAngle)
		{
			*dAngle = m_oBrush.LinearAngle;
			return S_OK;
		}
		AVSINLINE HRESULT put_BrushLinearAngle(double dAngle)
		{
			if( m_bRecordingCommands )
			{
				WriteCommand( CommandBrushLinearAngle, dAngle );
			}

			m_oBrush.LinearAngle = dAngle;
			return S_OK;
		}
		AVSINLINE HRESULT BrushRect(BOOL val, double left, double top, double width, double height)
		{
			if( m_bRecordingCommands )
			{
				BYTE* pBuffer = m_oStorageCommands.AddSection( sizeof ECommands + sizeof BOOL + sizeof(double) * 4 );
				if( pBuffer )
				{
					DataWriter data( pBuffer );
					data.Write( CommandBrushRect );
					data.Write( height );
					data.Write( width );
					data.Write( top );
					data.Write( left );
					data.Write( val );
				}
			}

			m_oBrush.Rectable = val;
			m_oBrush.Rect.X = (float)left;
			m_oBrush.Rect.Y = (float)top;
			m_oBrush.Rect.Width  = (float)width;
			m_oBrush.Rect.Height = (float)height;	

			return S_OK;
		}
		// font -------------------------------------------------------------------------------------
		AVSINLINE HRESULT SetFont(BSTR bsXML)
		{
			if( m_bRecordingCommands )
			{
				WriteCommand( CommandFont, bsXML );
			}

			m_oFont.FromXmlString( CString(bsXML) );
			return S_OK;
		}
		AVSINLINE HRESULT get_FontName(BSTR* bsName)
		{
			*bsName = m_oFont.Name.AllocSysString();
			return S_OK;
		}
		AVSINLINE HRESULT put_FontName(BSTR bsName)
		{
			if( m_bRecordingCommands )
			{
				WriteCommand( CommandFontName, bsName );
			}

			m_oFont.Name = bsName;
			return S_OK;
		}
		AVSINLINE HRESULT get_FontPath(BSTR* bsName)
		{
			*bsName = m_oFont.Path.AllocSysString();
			return S_OK;
		}
		AVSINLINE HRESULT put_FontPath(BSTR bsName)
		{
			if( m_bRecordingCommands )
			{
				WriteCommand( CommandFontPath, bsName );
			}

			m_oFont.Path = bsName;
			return S_OK;
		}
		AVSINLINE HRESULT get_FontSize(double* dSize)
		{
			*dSize = m_oFont.Size;
			return S_OK;
		}
		AVSINLINE HRESULT put_FontSize(double dSize)
		{
			if( m_bRecordingCommands )
			{
				WriteCommand( CommandFontSize, dSize );
			}

			m_oFont.Size = dSize;
			return S_OK;
		}
		AVSINLINE HRESULT get_FontStyle(LONG* lStyle)
		{
			*lStyle = m_oFont.GetStyle();
			return S_OK;
		}
		AVSINLINE HRESULT put_FontStyle(LONG lStyle)
		{
			if( m_bRecordingCommands )
			{
				WriteCommand( CommandFontStyle, lStyle );
			}

			m_oFont.SetStyle(lStyle);
			return S_OK;
		}
		AVSINLINE HRESULT get_FontStringGID(BOOL* bGID)
		{
			*bGID = m_oFont.StringGID;
			return S_OK;
		}
		AVSINLINE HRESULT put_FontStringGID(BOOL bGID)
		{
			if( m_bRecordingCommands )
			{
				WriteCommand( CommandFontStringGID, bGID );
			}

			m_oFont.StringGID = bGID;
			return S_OK;
		}
		AVSINLINE HRESULT get_FontCharSpace(double* dSpace)
		{
			*dSpace = m_oFont.CharSpace;
			return S_OK;
		}
		AVSINLINE HRESULT put_FontCharSpace(double dSpace)
		{
			if( m_bRecordingCommands )
			{
				WriteCommand( CommandFontCharSpace, dSpace );
			}

			m_oFont.CharSpace = dSpace;
			return S_OK;
		}
		// shadow -----------------------------------------------------------------------------------
		AVSINLINE HRESULT SetShadow(BSTR bsXML)
		{
			if( m_bRecordingCommands )
			{
				WriteCommand( CommandShadow, bsXML );
			}

			m_oShadow.FromXmlString( CString(bsXML) );
			return S_OK;
		}
		AVSINLINE HRESULT get_ShadowDistanceX(double* val)
		{
			*val = m_oShadow.DistanceX;
			return S_OK;
		}
		AVSINLINE HRESULT put_ShadowDistanceX(double val)
		{
			if( m_bRecordingCommands )
			{
				WriteCommand( CommandShadowDistanceX, val );
			}

			m_oShadow.DistanceX = val;
			return S_OK;
		}
		AVSINLINE HRESULT get_ShadowDistanceY(double* val)
		{
			*val = m_oShadow.DistanceY;
			return S_OK;
		}
		AVSINLINE HRESULT put_ShadowDistanceY(double val)
		{
			if( m_bRecordingCommands )
			{
				WriteCommand( CommandShadowDistanceY, val );
			}

			m_oShadow.DistanceY = val;
			return S_OK;
		}
		AVSINLINE HRESULT get_ShadowBlurSize(double* val)
		{
			*val = m_oShadow.BlurSize;
			return S_OK;
		}
		AVSINLINE HRESULT put_ShadowBlurSize(double val)
		{
			if( m_bRecordingCommands )
			{
				WriteCommand( CommandShadowBlurSize, val );
			}

			m_oShadow.BlurSize = val;
			return S_OK;
		}
		AVSINLINE HRESULT get_ShadowColor(LONG* val)
		{
			*val = m_oShadow.Color;
			return S_OK;
		}
		AVSINLINE HRESULT put_ShadowColor(LONG val)
		{
			if( m_bRecordingCommands )
			{
				WriteCommand( CommandShadowColor, val );
			}

			m_oShadow.Color = val;
			return S_OK;
		}
		AVSINLINE HRESULT get_ShadowAlpha(LONG* val)
		{
			*val = m_oShadow.Alpha;
			return S_OK;
		}
		AVSINLINE HRESULT put_ShadowAlpha(LONG val)
		{
			if( m_bRecordingCommands )
			{
				WriteCommand( CommandShadowAlpha, val );
			}

			m_oShadow.Alpha = val;
			return S_OK;
		}
		AVSINLINE HRESULT get_ShadowVisible(BOOL* val)
		{
			*val = m_oShadow.Visible;
			return S_OK;
		}
		AVSINLINE HRESULT put_ShadowVisible(BOOL val)
		{
			if( m_bRecordingCommands )
			{
				WriteCommand( CommandShadowVisible, val );
			}

			m_oShadow.Visible = val;
			return S_OK;
		}
		// edge -------------------------------------------------------------------------------------
		AVSINLINE HRESULT SetEdgeText(BSTR bsXML)
		{
			if( m_bRecordingCommands )
			{
				WriteCommand( CommandEdgeText, bsXML );
			}

			m_oEdge.FromXmlString( CString(bsXML) );
			return S_OK;
		}
		AVSINLINE HRESULT get_EdgeVisible(LONG* val)
		{
			*val = m_oEdge.Visible;
			return S_OK;
		}
		AVSINLINE HRESULT put_EdgeVisible(LONG val)
		{
			if( m_bRecordingCommands )
			{
				WriteCommand( CommandEdgeVisible, val );
			}

			m_oEdge.Visible = val;
			return S_OK;
		}
		AVSINLINE HRESULT get_EdgeColor(LONG* val)
		{
			*val = m_oEdge.Color;
			return S_OK;
		}
		AVSINLINE HRESULT put_EdgeColor(LONG val)
		{
			if( m_bRecordingCommands )
			{
				WriteCommand( CommandEdgeColor, val );
			}

			m_oEdge.Color = val;
			return S_OK;
		}
		AVSINLINE HRESULT get_EdgeAlpha(LONG* val)
		{
			*val = m_oEdge.Alpha;
			return S_OK;
		}
		AVSINLINE HRESULT put_EdgeAlpha(LONG val)
		{
			if( m_bRecordingCommands )
			{
				WriteCommand( CommandEdgeAlpha, val );
			}

			m_oEdge.Alpha = val;
			return S_OK;
		}
		AVSINLINE HRESULT get_EdgeDist(double* val)
		{
			*val = m_oEdge.Dist;
			return S_OK;
		}
		AVSINLINE HRESULT put_EdgeDist(double val)
		{
			if( m_bRecordingCommands )
			{
				WriteCommand( CommandEdgeDist, val );
			}

			m_oEdge.Dist = val;
			return S_OK;
		}

		//-------- Функции для вывода текста --------------------------------------------------------
		AVSINLINE HRESULT CommandDrawText(BSTR bsText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset)
		{
			if( m_bRecordingCommands )
			{
				bsText = m_oStorageDynamicData.AddBstr( bsText );

				BYTE* pBuffer = m_oStorageCommands.AddSection( sizeof ECommands + sizeof(BSTR) + sizeof(double) * 5 );
				if( pBuffer )
				{
					DataWriter data( pBuffer );
					data.Write( CommandCommandDrawText );
					data.Write( fBaseLineOffset );
					data.Write( fHeight );
					data.Write( fWidth );
					data.Write( fY );
					data.Write( fX );
					data.Write( bsText );
				}
				return S_OK;
			}

			double dAngleMatrix = m_oTransform.z_Rotation();
			if (abs(dAngleMatrix) > 1)
			{
				PathCommandEnd();
				BeginCommand(c_nPathType);
				PathCommandText(bsText, fX, fY, fWidth, fHeight, fBaseLineOffset);
				DrawPath(c_nWindingFillMode);
				EndCommand(c_nPathType);
				PathCommandEnd();
				return S_OK;
			}

			m_oCurrentPage.WriteText(bsText, NULL, fX, fY, fWidth, fHeight, fBaseLineOffset, m_bIsNeedPDFTextAnalyzer);
			return S_OK;
		}
		AVSINLINE HRESULT CommandDrawTextEx(BSTR bsUnicodeText, BSTR bsGidText, BSTR bsSourceCodeText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset, DWORD lFlags)
		{
			if( m_bRecordingCommands )
			{
				bsUnicodeText = m_oStorageDynamicData.AddBstr( bsUnicodeText );
				bsGidText = m_oStorageDynamicData.AddBstr( bsGidText );
				bsSourceCodeText = m_oStorageDynamicData.AddBstr( bsSourceCodeText );

				BYTE* pBuffer = m_oStorageCommands.AddSection( sizeof ECommands + sizeof(BSTR) * 3 + sizeof(double) * 5 + sizeof(DWORD) );
				if( pBuffer )
				{
					DataWriter data( pBuffer );
					data.Write( CommandCommandDrawTextEx );
					data.Write( lFlags );
					data.Write( fBaseLineOffset );
					data.Write( fHeight );
					data.Write( fWidth );
					data.Write( fY );
					data.Write( fX );
					data.Write( bsSourceCodeText );
					data.Write( bsGidText );
					data.Write( bsUnicodeText );
				}
				return S_OK;
			}

			double dAngleMatrix = m_oTransform.z_Rotation();
			if (abs(dAngleMatrix) > 1)
			{
				PathCommandEnd();
				BeginCommand(c_nPathType);
				PathCommandTextEx(bsUnicodeText, bsGidText, bsSourceCodeText, fX, fY, fWidth, fHeight, fBaseLineOffset, lFlags);
				DrawPath(c_nWindingFillMode);
				EndCommand(c_nPathType);
				PathCommandEnd();
				return S_OK;
			}

			m_oCurrentPage.WriteText(bsUnicodeText, bsGidText, fX, fY, fWidth, fHeight, fBaseLineOffset, m_bIsNeedPDFTextAnalyzer);
		
			return S_OK;
		}
		//-------- Маркеры для команд ---------------------------------------------------------------
		AVSINLINE HRESULT BeginCommand(DWORD lType)
		{
			if( m_bRecordingCommands )
			{
				WriteCommand( CommandBeginCommand, lType );
				return S_OK;
			}

			m_lCurrentCommandType = (LONG)lType;
			m_oCurrentPage.m_lCurrentCommand	= m_lCurrentCommandType;

			return S_OK;
		}
		AVSINLINE HRESULT EndCommand(DWORD lType)
		{
			if( m_bRecordingCommands )
			{
				WriteCommand( CommandEndCommand, lType );
				return S_OK;
			}

			m_lCurrentCommandType				= -1;
			m_oCurrentPage.m_lCurrentCommand	= m_lCurrentCommandType;

			if (c_nPageType == lType)
			{
				// нужно записать страницу в файл
				m_oCurrentPage.Build();
				m_oCurrentPage.Write(m_oWriter);
			}
			else if (c_nPathType == lType)
			{
				m_oCurrentPage.End();
			}

			return S_OK;
		}
		//-------- Функции для работы с Graphics Path -----------------------------------------------
		AVSINLINE HRESULT PathCommandMoveTo(double fX, double fY)
		{
			if( m_bRecordingCommands )
			{
				BYTE* pBuffer = m_oStorageCommands.AddSection( sizeof ECommands + sizeof(double) * 2 );
				if( pBuffer )
				{
					DataWriter data( pBuffer );
					data.Write( CommandPathCommandMoveTo );
					data.Write( fY );
					data.Write( fX );
				}
				return S_OK;
			}

			if (c_nSimpleGraphicType == m_lCurrentCommandType)
			{
				m_oCurrentPage.MoveTo(fX, fY);
			}
			else
			{
				m_pSimpleGraphicsConverter->PathCommandMoveTo(fX, fY);
			}
			return S_OK;
		}
		AVSINLINE HRESULT PathCommandLineTo(double fX, double fY)
		{
			if( m_bRecordingCommands )
			{
				BYTE* pBuffer = m_oStorageCommands.AddSection( sizeof ECommands + sizeof(double) * 2 );
				if( pBuffer )
				{
					DataWriter data( pBuffer );
					data.Write( CommandPathCommandLineTo );
					data.Write( fY );
					data.Write( fX );
				}
				return S_OK;
			}

			if (c_nSimpleGraphicType == m_lCurrentCommandType)
			{
				m_oCurrentPage.LineTo(fX, fY);
			}
			else
			{
				m_pSimpleGraphicsConverter->PathCommandLineTo(fX, fY);
			}
			return S_OK;
		}
		AVSINLINE HRESULT PathCommandLinesTo(SAFEARRAY* pPoints)
		{
			if( m_bRecordingCommands )
			{
				WriteCommand( CommandPathCommandLinesTo, pPoints );
				return S_OK;
			}

			m_pSimpleGraphicsConverter->PathCommandLinesTo(pPoints);
			return S_OK;
		}
		AVSINLINE HRESULT PathCommandCurveTo(double fX1, double fY1, double fX2, double fY2, double fX3, double fY3)
		{
			if( m_bRecordingCommands )
			{
				BYTE* pBuffer = m_oStorageCommands.AddSection( sizeof ECommands + sizeof(double) * 6 );
				if( pBuffer )
				{
					DataWriter data( pBuffer );
					data.Write( CommandPathCommandCurveTo );
					data.Write( fY3 );
					data.Write( fX3 );
					data.Write( fY2 );
					data.Write( fX2 );
					data.Write( fY1 );
					data.Write( fX1 );
				}
				return S_OK;
			}

			if (c_nSimpleGraphicType == m_lCurrentCommandType)
			{
				m_oCurrentPage.CurveTo(fX1, fY1, fX2, fY2, fX3, fY3);
			}
			else
			{
				m_pSimpleGraphicsConverter->PathCommandCurveTo(fX1, fY1, fX2, fY2, fX3, fY3);
			}
			return S_OK;
		}
		AVSINLINE HRESULT PathCommandCurvesTo(SAFEARRAY* pPoints)
		{
			if( m_bRecordingCommands )
			{
				WriteCommand( CommandPathCommandCurvesTo, pPoints );
				return S_OK;
			}

			m_pSimpleGraphicsConverter->PathCommandCurvesTo(pPoints);
			return S_OK;
		}
		AVSINLINE HRESULT PathCommandArcTo(double fX, double fY, double fWidth, double fHeight, double fStartAngle, double fSweepAngle)
		{
			if( m_bRecordingCommands )
			{
				BYTE* pBuffer = m_oStorageCommands.AddSection( sizeof ECommands + sizeof(double) * 6 );
				if( pBuffer )
				{
					DataWriter data( pBuffer );
					data.Write( CommandPathCommandArcTo );
					data.Write( fSweepAngle );
					data.Write( fStartAngle );
					data.Write( fHeight );
					data.Write( fWidth );
					data.Write( fY );
					data.Write( fX );
				}
				return S_OK;
			}

			m_pSimpleGraphicsConverter->PathCommandArcTo(fX, fY, fWidth, fHeight, fStartAngle, fSweepAngle);
			return S_OK;
		}
		AVSINLINE HRESULT PathCommandClose()
		{
			if( m_bRecordingCommands )
			{
				WriteCommand( CommandPathCommandClose );
				return S_OK;
			}

			if (c_nSimpleGraphicType == m_lCurrentCommandType)
			{
				m_oCurrentPage.Close();
			}
			else
			{
				m_pSimpleGraphicsConverter->PathCommandClose();
			}
			return S_OK;
		}
		AVSINLINE HRESULT PathCommandEnd()
		{
			if( m_bRecordingCommands )
			{
				WriteCommand( CommandPathCommandEnd );
				return S_OK;
			}

			if (c_nSimpleGraphicType == m_lCurrentCommandType)
			{
				m_oCurrentPage.End();
			}
			else
			{
				m_pSimpleGraphicsConverter->PathCommandEnd();
			}
			return S_OK;
		}
		AVSINLINE HRESULT DrawPath(long nType)
		{
			if( m_bRecordingCommands )
			{
				WriteCommand( CommandDrawPath, nType );
				return S_OK;
			}

			LONG lTxId = -1;
			if ((nType > 0xFF) && (c_BrushTypeTexture == m_oBrush.Type))
			{
				double x = 0;
				double y = 0;
				double w = 0;
				double h = 0;
				CImageInfo oInfo = m_oManager.WriteImage(m_oBrush.TexturePath, x, y, w, h);
				lTxId = oInfo.m_lID;
			}

			m_oCurrentPage.DrawPath(nType, lTxId);
			return S_OK;
		}
		AVSINLINE HRESULT PathCommandStart()
		{
			if( m_bRecordingCommands )
			{
				WriteCommand( CommandPathCommandStart );
				return S_OK;
			}

			if (c_nSimpleGraphicType == m_lCurrentCommandType)
			{
				m_oCurrentPage.Start();
			}
			else
			{
				m_pSimpleGraphicsConverter->PathCommandStart();
			}
			return S_OK;
		}
		AVSINLINE HRESULT PathCommandGetCurrentPoint(double* fX, double* fY)
		{
			m_pSimpleGraphicsConverter->PathCommandGetCurrentPoint(fX, fY);	
			return S_OK;
		}

		AVSINLINE HRESULT PathCommandText(BSTR bsText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset)
		{
			if( m_bRecordingCommands )
			{
				bsText = m_oStorageDynamicData.AddBstr( bsText );

				BYTE* pBuffer = m_oStorageCommands.AddSection( sizeof ECommands + sizeof(BSTR) + sizeof(double) * 5 );
				if( pBuffer )
				{
					DataWriter data( pBuffer );
					data.Write( CommandPathCommandText );
					data.Write( fBaseLineOffset );
					data.Write( fHeight );
					data.Write( fWidth );
					data.Write( fY );
					data.Write( fX );
					data.Write( bsText );
				}
				return S_OK;
			}

			_SetFont();
			m_pSimpleGraphicsConverter->PathCommandText(bsText, m_pFontManager, fX, fY, fWidth, fHeight, fBaseLineOffset);
			return S_OK;
		}
		AVSINLINE HRESULT PathCommandTextEx(BSTR bsUnicodeText, BSTR bsGidText, BSTR bsSourceCodeText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset, DWORD lFlags)
		{
			if( m_bRecordingCommands )
			{
				bsUnicodeText = m_oStorageDynamicData.AddBstr( bsUnicodeText );
				bsGidText = m_oStorageDynamicData.AddBstr( bsGidText );
				bsSourceCodeText = m_oStorageDynamicData.AddBstr( bsSourceCodeText );

				BYTE* pBuffer = m_oStorageCommands.AddSection( sizeof ECommands + sizeof(BSTR) * 3 + sizeof(double) * 5 + sizeof DWORD );
				if( pBuffer )
				{
					DataWriter data( pBuffer );
					data.Write( CommandPathCommandTextEx );
					data.Write( lFlags );
					data.Write( fBaseLineOffset );
					data.Write( fHeight );
					data.Write( fWidth );
					data.Write( fY );
					data.Write( fX );
					data.Write( bsSourceCodeText );
					data.Write( bsGidText );
					data.Write( bsUnicodeText );
				}
				return S_OK;
			}

			if (NULL != bsGidText)
			{
				m_oFont.StringGID = TRUE;
				return PathCommandText(bsGidText, fX, fY, fWidth, fHeight, fBaseLineOffset);
			}

			m_oFont.StringGID = FALSE;
			return PathCommandText(bsUnicodeText, fX, fY, fWidth, fHeight, fBaseLineOffset);
		}

		AVSINLINE HRESULT GetCommandParams(double* dAngle, double* dLeft, double* dTop, double* dWidth, double* dHeight, DWORD* lFlags)
		{
			return S_OK;
		}
		AVSINLINE HRESULT SetCommandParams(double dAngle, double dLeft, double dTop, double dWidth, double dHeight, DWORD lFlags)
		{
			if( m_bRecordingCommands )
			{
				BYTE* pBuffer = m_oStorageCommands.AddSection( sizeof ECommands + sizeof(double) * 5 + sizeof DWORD );
				if( pBuffer )
				{
					DataWriter data( pBuffer );
					data.Write( CommandSetCommandParams );
					data.Write( lFlags );
					data.Write( dHeight );
					data.Write( dWidth );
					data.Write( dTop );
					data.Write( dLeft );
					data.Write( dAngle );
				}
				return S_OK;
			}

			ApplyTransform2(dAngle, dLeft, dTop, dWidth, dHeight, lFlags);
			return S_OK;
		}
		//-------- Функции для вывода изображений --------------------------------------------------
		AVSINLINE HRESULT DrawImage(IUnknown* pInterface, double fX, double fY, double fWidth, double fHeight)
		{
			if( m_bRecordingCommands )
			{
				pInterface = m_oStorageDynamicData.AddInterface( pInterface );

				BYTE* pBuffer = m_oStorageCommands.AddSection( sizeof ECommands + sizeof(IUnknown*) + sizeof(double) * 4 );
				if( pBuffer )
				{
					DataWriter data( pBuffer );
					data.Write( CommandDrawImage );
					data.Write( fHeight );
					data.Write( fWidth );
					data.Write( fY );
					data.Write( fX );
					data.Write( pInterface );
				}
				return S_OK;
			}

			CImageInfo oInfo = m_oManager.WriteImage(pInterface, fX, fY, fWidth, fHeight);
			m_oCurrentPage.WriteImage(oInfo, fX, fY, fWidth, fHeight);
			return S_OK;
		}
		AVSINLINE HRESULT DrawImageFromFile(BSTR bstrVal, double fX, double fY, double fWidth, double fHeight)
		{
			if( m_bRecordingCommands )
			{
				bstrVal = m_oStorageDynamicData.AddBstr( bstrVal );

				BYTE* pBuffer = m_oStorageCommands.AddSection( sizeof ECommands + sizeof(BSTR) + sizeof(double) * 4 );
				if( pBuffer )
				{
					DataWriter data( pBuffer );
					data.Write( CommandDrawImageFromFile );
					data.Write( fHeight );
					data.Write( fWidth );
					data.Write( fY );
					data.Write( fX );
					data.Write( bstrVal );
				}
				return S_OK;
			}

			CImageInfo oInfo = m_oManager.WriteImage( CString(bstrVal), fX, fY, fWidth, fHeight);
			m_oCurrentPage.WriteImage(oInfo, fX, fY, fWidth, fHeight);
			return S_OK;
		}
		//------------------------------------------------------------------------------------------
		AVSINLINE HRESULT SetAdditionalParam(BSTR ParamName, VARIANT ParamValue)
		{
			return S_OK;
		}
		AVSINLINE HRESULT GetAdditionalParam(BSTR ParamName, VARIANT* ParamValue)
		{
			return S_OK;
		}

		AVSINLINE HRESULT SetTransform(double dA, double dB, double dC, double dD, double dE, double dF)
		{
			if( m_bRecordingCommands )
			{
				BYTE* pBuffer = m_oStorageCommands.AddSection( sizeof ECommands + sizeof(double) * 6 );
				if( pBuffer )
				{
					DataWriter data( pBuffer );
					data.Write( CommandSetTransform );
					data.Write( dF );
					data.Write( dE );
					data.Write( dD );
					data.Write( dC );
					data.Write( dB );
					data.Write( dA );
				}
				return S_OK;
			}

			ApplyTransform(dA, dB, dC, dD, dE, dF);
			return S_OK;
		}
		AVSINLINE HRESULT GetTransform(double *pdA, double *pdB, double *pdC, double *pdD, double *pdE, double *pdF)
		{
			return S_OK;
		}
		AVSINLINE HRESULT ResetTransform(void)
		{
			if( m_bRecordingCommands )
			{
				WriteCommand( CommandResetTransform );
				return S_OK;
			}

			m_oTransform.Reset();
			return S_OK;
		}
		AVSINLINE HRESULT get_ClipMode(LONG* plMode)
		{
			*plMode = m_lClipMode;
			return S_OK;
		}
		AVSINLINE HRESULT put_ClipMode(LONG lMode)
		{
			if( m_bRecordingCommands )
			{
				WriteCommand( CommandClipMode, lMode );
			}

			m_lClipMode = lMode;
			return S_OK;
		}

	protected:
		AVSINLINE void ApplyTransform(double d1, double d2, double d3, double d4, double d5, double d6)
		{
			m_oTransform.SetElements(d1, d2, d3, d4, d5, d6);
		}

		void ApplyTransform2(double dAngle, double dLeft, double dTop, double dWidth, double dHeight, DWORD lFlags)
		{
			if ((dWidth <= 1) || (dHeight <= 1))
				lFlags = 0;

			BOOL bFlipX = (0 != (c_nParamFlipX & lFlags));
			BOOL bFlipY = (0 != (c_nParamFlipY & lFlags));

			double m11 = bFlipX ? -1.0 : 1.0;
			double m22 = bFlipY ? -1.0 : 1.0;

			NSDocxRenderer::CMatrix oMatrix(1, 0, 0, 1, 0, 0);

			if ((0 != dAngle) || (0 != lFlags))
			{
				double dCentreX = (dLeft + dWidth / 2.0);
				double dCentreY = (dTop + dHeight / 2.0);

				oMatrix.Translate(-dCentreX, -dCentreY	, Aggplus::MatrixOrderAppend);

				oMatrix.Rotate(dAngle			, Aggplus::MatrixOrderAppend);
				oMatrix.Scale(m11, m22					, Aggplus::MatrixOrderAppend);

				oMatrix.Translate(dCentreX, dCentreY	, Aggplus::MatrixOrderAppend);
			}

			m_oTransform = oMatrix;
		}

		void _SetFont()
		{
			if (NULL == m_pFontManager)
			{
				CoCreateInstance(__uuidof(AVSGraphics::CAVSFontManager), NULL, CLSCTX_ALL, __uuidof(AVSGraphics::IAVSFontManager), (void**)&m_pFontManager);
				m_pFontManager->Initialize(L"");
			}

			double dPix = m_oFont.CharSpace * m_dDpiX / 25.4;

			if (m_oInstalledFont.IsEqual(&m_oFont))
			{
				if (1 < m_dWidth)
				{
					m_pFontManager->SetCharSpacing(dPix);
				}
				return;
			}

			m_pFontManager->SetStringGID(m_oFont.StringGID);
			if (1 < m_dWidth)
			{
				m_pFontManager->SetCharSpacing(dPix);
			}

			if (_T("") == m_oFont.Path)
			{
				BSTR bsName = m_oFont.Name.AllocSysString();
				m_pFontManager->LoadFontByName(bsName, (float)m_oFont.Size, m_oFont.GetStyle(), m_dDpiX, m_dDpiY);
				SysFreeString(bsName);
			}
			else
			{
				BSTR bsName = m_oFont.Path.AllocSysString();
				m_pFontManager->LoadFontFromFile(bsName, (float)m_oFont.Size, m_dDpiX, m_dDpiY, 0);
				SysFreeString(bsName);
			}

			m_oInstalledFont = m_oFont;
		}

	public:

		bool CreateDocument(IUnknown* pRenderer, CString strTempDirectory)
		{
			ProcessCommand();

			HINSTANCE hInst = _AtlBaseModule.GetModuleInstance();
			m_strTempDirectory = strTempDirectory;

			// rels
			CString strRels = strTempDirectory + _T("\\_rels");
			CDirectory::CreateDirectory(strRels);

			LoadResourceFile(hInst, MAKEINTRESOURCE(IDB_DEFAULT_DOC_RELS), _T("DOCXREND"), strRels + _T("\\.rels"));

			// docProps
			CString strDocProps = strTempDirectory + _T("\\docProps");
			CDirectory::CreateDirectory(strDocProps);

			LoadResourceFile(hInst, MAKEINTRESOURCE(IDB_DEFAULT_DOC_APP),  _T("DOCXREND"), strDocProps + _T("\\app.xml"));
			LoadResourceFile(hInst, MAKEINTRESOURCE(IDB_DEFAULT_DOC_CORE), _T("DOCXREND"), strDocProps + _T("\\core.xml"));

			// contentTypes
			LoadResourceFile(hInst, MAKEINTRESOURCE(IDB_DEFAULT_DOC_CONTENT_TYPES), _T("DOCXREND"), strTempDirectory + _T("\\[Content_Types].xml"));

			// word
			CString strWord = strTempDirectory + _T("\\word");
			CDirectory::CreateDirectory(strWord);

			LoadResourceFile(hInst, MAKEINTRESOURCE(IDB_DEFAULT_DOC_SETTINGS),		_T("DOCXREND"), strWord + _T("\\settings.xml"));
			LoadResourceFile(hInst, MAKEINTRESOURCE(IDB_DEFAULT_DOC_STYLES),		_T("DOCXREND"), strWord + _T("\\styles.xml"));
			LoadResourceFile(hInst, MAKEINTRESOURCE(IDB_DEFAULT_DOC_WEBSETTINGS),	_T("DOCXREND"), strWord + _T("\\webSettings.xml"));

			// theme
			CString strTheme = strWord + _T("\\theme");
			CDirectory::CreateDirectory(strTheme);
			LoadResourceFile(hInst, MAKEINTRESOURCE(IDB_DEFAULT_DOC_THEME),	_T("DOCXREND"), strTheme + _T("\\theme.xml"));

			// documentRels
			CDirectory::CreateDirectory(strWord + _T("\\_rels"));

			// Init
			Clear();

			CoCreateInstance(__uuidof(AVSGraphics::CAVSGraphicSimpleComverter), NULL, CLSCTX_ALL, 
				__uuidof(AVSGraphics::IAVSGraphicSimpleComverter), (void**)&m_pSimpleGraphicsConverter);

			m_punkRenderer = pRenderer;
			ADDREFINTERFACE(m_punkRenderer);
			m_pSimpleGraphicsConverter->put_Renderer(m_punkRenderer);
			m_lCurrentCommandType = 0;

			m_oCurrentPage.Init(&m_oFont, &m_oPen, &m_oBrush, &m_oShadow, &m_oEdge, &m_oTransform, m_pSimpleGraphicsConverter);

			m_oManager.NewDocument();
			// media
			m_oManager.m_strDstMedia = strWord + _T("\\media");
			CDirectory::CreateDirectory(m_oManager.m_strDstMedia);

			m_oCurrentPage.m_oManager.m_oFontTable.m_mapTable.RemoveAll();

			m_oDocumentStream.CloseFile();
			m_oDocumentStream.CreateFile(strWord + _T("\\document.xml"));
			m_oDocumentStream.WriteStringUTF8(g_string_DocumentStart);

			m_lPagesCount = 0;
			m_oWriter.Clear();
			m_oWriter.AddSize(10000);

			return true;
		}

		void Close()
		{
			ProcessCommand();

			// сохраним rels (images & docs)
			CFile oFile;
			oFile.CreateFile(m_strTempDirectory + _T("\\word\\_rels\\document.xml.rels"));

			NSDocxRenderer::CStringWriter oWriter;

			CString strStart = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?>\
								  <Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\">\
								  <Relationship Id=\"rId1\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/styles\" Target=\"styles.xml\"/>\
								  <Relationship Id=\"rId2\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/settings\" Target=\"settings.xml\"/>\
								  <Relationship Id=\"rId3\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/webSettings\" Target=\"webSettings.xml\"/>\
								  <Relationship Id=\"rId4\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/fontTable\" Target=\"fontTable.xml\"/>\
								  <Relationship Id=\"rId5\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/theme\" Target=\"theme/theme.xml\"/>");

			oWriter.WriteString(strStart);

			POSITION pos = m_oManager.m_mapImageData.GetStartPosition();
			while (NULL != pos)
			{
				CAtlMap<DWORD, CImageInfo>::CPair* pPair = m_oManager.m_mapImageData.GetNext(pos);

				LONG lId = pPair->m_value.m_lID;
				if (pPair->m_value.m_eType == itPNG)
				{
					CString strImage = _T("");
					strImage.Format(g_string_imageRID_png, 10 + lId, lId);
					oWriter.WriteString(strImage);
				}
				else
				{
					CString strImage = _T("");
					strImage.Format(g_string_imageRID_jpg, 10 + lId, lId);
					oWriter.WriteString(strImage);
				}
			}

			pos = m_oManager.m_mapImagesFile.GetStartPosition();
			while (NULL != pos)
			{
				CAtlMap<CString, CImageInfo>::CPair* pPair = m_oManager.m_mapImagesFile.GetNext(pos);

				LONG lId = pPair->m_value.m_lID;
				if (pPair->m_value.m_eType == itPNG)
				{
					CString strImage = _T("");
					strImage.Format(g_string_imageRID_png, 10 + lId, lId);
					oWriter.WriteString(strImage);
				}
				else
				{
					CString strImage = _T("");
					strImage.Format(g_string_imageRID_jpg, 10 + lId, lId);
					oWriter.WriteString(strImage);
				}
			}

			CString strEnd = _T("</Relationships>"); 
			oWriter.WriteString(strEnd);

			oFile.WriteStringUTF8(oWriter.GetData());
			oFile.CloseFile();

			// сохраним fontTable
			CFile oFileFontTable;
			oFileFontTable.CreateFile(m_strTempDirectory + _T("\\word\\fontTable.xml"));

			oWriter.ClearNoAttack();

			strStart = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?>\
						  <w:fonts xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\">");

			oWriter.WriteString(strStart);

			CFontTable* pFontTable = &m_oCurrentPage.m_oManager.m_oFontTable;

			pos = pFontTable->m_mapTable.GetStartPosition();
			while (NULL != pos)
			{
				CAtlMap<CString, CFontTableEntry>::CPair* pPair = pFontTable->m_mapTable.GetNext(pos);

				oWriter.WriteString(scXmlNodeFontNodeBegin);
				oWriter.WriteString(CString(pPair->m_value.m_strFamilyName));
				oWriter.WriteString(scXmlNodeEnd);

				oWriter.WriteString(scXmlNodeFontPanoseBegin);
				oWriter.WriteString(pPair->m_value.m_strPANOSE);
				oWriter.WriteString(scXmlShortNodeEnd);

				oWriter.WriteString(scXmlNodeFontCharsetBegin);
				oWriter.WriteString(CString(_T("00")));
				oWriter.WriteString(scXmlShortNodeEnd);

				//oWriter.WriteString(scXmlNodeFontFamilyBegin);
				//oWriter.WriteString(CString(_T("roman")));
				//oWriter.WriteString(scXmlShortNodeEnd);

				if (pPair->m_value.m_bIsFixedWidth)
					oWriter.WriteString(scXmlNodeFontPitchTrue);
				else
					oWriter.WriteString(scXmlNodeFontPitchFalse);

				CString strSig = _T("");
				strSig.Format(g_string_SIG, pPair->m_value.m_arSignature[0], pPair->m_value.m_arSignature[1], pPair->m_value.m_arSignature[2], 
					pPair->m_value.m_arSignature[3], pPair->m_value.m_arSignature[4], pPair->m_value.m_arSignature[5]);

				oWriter.WriteString(strSig);

				oWriter.WriteString(scXmlNodeFontEnd);		
			}

			strEnd = _T("</w:fonts>");
			oWriter.WriteString(strEnd);

			oFileFontTable.WriteStringUTF8(oWriter.GetData());
			oFileFontTable.CloseFile();

			// document
			m_oCurrentPage.WriteSectionToFile(true, m_oWriter);
			m_oWriter.WriteString(g_string_DocumentEnd);
			m_oDocumentStream.WriteStringUTF8(m_oWriter.GetData());
			m_oWriter.ClearNoAttack();

			m_oDocumentStream.CloseFile();
		}

	protected:

		void LoadResourceFile(HINSTANCE hInst, LPCTSTR sResName, LPCTSTR sResType, const CString& strDstFile)
		{
			HRSRC hrRes = FindResource(hInst, sResName, sResType);
			if (!hrRes)
				return;

			HGLOBAL hGlobal = LoadResource(hInst, hrRes);
			DWORD sz = SizeofResource(hInst, hrRes);
			void* ptrRes = LockResource(hGlobal);

			CFile oFile;
			oFile.CreateFile(strDstFile);
			oFile.WriteFile(ptrRes, sz);

			UnlockResource(hGlobal);
			FreeResource(hGlobal);
		}			
		
	protected:

		NSStructures::CPen				m_oPen;
		NSStructures::CBrush			m_oBrush;
		NSStructures::CFont				m_oFont;
		NSStructures::CShadow			m_oShadow;
		NSStructures::CEdgeText			m_oEdge;

		NSStructures::CFont				m_oInstalledFont;

		AVSGraphics::IAVSGraphicSimpleComverter*	m_pSimpleGraphicsConverter;
		AVSGraphics::IAVSFontManager*				m_pFontManager;

		NSDocxRenderer::CMatrix						m_oTransform;

		LONG							m_lCurrentCommandType;

		LONG							m_lClipMode;
		CPage							m_oCurrentPage;

		IUnknown*						m_punkRenderer;

		CImageManager					m_oManager;

		double							m_dWidth;
		double							m_dHeight;

		double							m_dDpiX;
		double							m_dDpiY;

		CString							m_strTempDirectory;

		CFile							m_oDocumentStream;
		LONG							m_lPagesCount;

		NSDocxRenderer::CStringWriter	m_oWriter;

		StorageDynamicData m_oStorageDynamicData;
		DataStorage m_oStorageCommands;
		bool m_bRecordingCommands;

		CTableDetector m_oTableDetector;

		Analyzer::CTableCollector	m_oCollector;

	public:	
		bool m_bIsNeedPDFTextAnalyzer;
	};
}