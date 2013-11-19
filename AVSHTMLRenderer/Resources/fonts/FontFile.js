function CFontFile(fileName)
{
    this.m_arrdFontMatrix = [];
    this.m_arrdTextMatrix = [];
    this.m_bAntiAliasing = true;
    this.m_bUseKerning = true;

    this.m_fSize = 1.0;       // Размер шрифта
    this.m_unHorDpi = 0;      // Горизонтальное разрешение
    this.m_unVerDpi = 0;      // Вертикальное разрешение

    this.m_bNeedDoItalic = false;
    this.m_bNeedDoBold = false;

    this.m_fCharSpacing = 0.0;

    this.m_nMinX = 0;        //
    this.m_nMinY = 0;        // Glyph BBox
    this.m_nMaxX = 0;        //
    this.m_nMaxY = 0;        //
    
	this.m_sFileName = fileName;
	this.m_nError = 0;
	this.m_pFace = null;
	
	this.m_dUnitsKoef = 1.0;
	this.m_nDefaultChar = -1;
	this.m_nSymbolic = -1;
	this.m_dTextScale = 0;

    this.m_bStringGID = false;
	
	this.m_oFontMatrix = null;
	this.m_oTextMatrix = null;
	
	this.m_nNum_charmaps = 0;
	this.m_pGlyph = null;

    this.m_arrCacheSizes = new Array();

    this.m_bUseDefaultFont = false;
    this.m_pDefaultFont = null;
	
	this.m_bIsNeedUpdateMatrix12 = true;

    this.SetDefaultFont = function(pDefFont)
    {
        this.m_pDefaultFont = pDefFont;
    }
	
	this.LoadFont = function(pLibrary)
	{
		var pfontPointer = Pointer_make(intArrayFromString(this.m_sFileName),null);
		
		this.m_pFace = STACKTOP;
		STACKTOP+=4;
		Runtime.memset(this.m_pFace,0,4);
		
		this.m_oFontMatrix = STACKTOP;
		STACKTOP+=16;
		this.m_oTextMatrix = STACKTOP;
		STACKTOP+=16;
		Runtime.memset(this.m_oFontMatrix,0,32);
		
		var oOpenArgs = STACKTOP;
		STACKTOP+=32;
		Runtime.memset(oOpenArgs,0,32);
		
		/*
			typedef struct  FT_Open_Args_
			{
				FT_UInt         flags;
				const FT_Byte*  memory_base;
				FT_Long         memory_size;
				FT_String*      pathname;
				FT_Stream       stream;
				FT_Module       driver;
				FT_Int          num_params;
				FT_Parameter*   params;

			} FT_Open_Args;
		*/
		// flags    = FT_OPEN_PATHNAME;
		HEAP[oOpenArgs]=4;
		// pathname = FilePath;
		HEAP[oOpenArgs+12]=pfontPointer;
		// stream = 0;
		HEAP[oOpenArgs+16]=0;
		// _FT_Open_Face (FT_Library library, FT_Open_Args* args, FT_Long face_index, FT_Face *aface)
		this.m_nError = _FT_Open_Face(pLibrary,oOpenArgs,0,this.m_pFace);
		
		STACKTOP = oOpenArgs;
		
		if (this.IsSuccess())
		{
			var nCharmap = HEAP[HEAP[this.m_pFace]+92];
			this.m_nNum_charmaps = HEAP[HEAP[this.m_pFace] + 36];
			if ( null == nCharmap && 0 != this.m_nNum_charmaps )
				alert('loadCharmap');
			
			this.LoadDefaultCharAndSymbolicCmapIndex();
			
			this.m_nError = _FT_Set_Char_Size(HEAP[this.m_pFace], 0, this.m_fSize * 64, 0, 0);
			if (!this.IsSuccess())
				return;
			
			this.ResetTextMatrix();
			this.ResetFontMatrix();

            if (true == this.m_bUseKerning)
            {
                var face_flags = HEAP[HEAP[this.m_pFace]+8];
                this.m_bUseKerning = ((face_flags & FT_FACE_FLAG_KERNING) > 0 ? true : false);
            }
			
			this.m_pGlyph = HEAP[HEAP[this.m_pFace]+84];
		}
	}
	
	this.LoadDefaultCharAndSymbolicCmapIndex = function()
	{
        this.m_nDefaultChar = -1;
        this.m_nSymbolic = -1;
        
		var pTable = _FT_Get_Sfnt_Table (HEAP[this.m_pFace], 2);
		if (null == pTable)
			return;
		else
		{
			// pTable->usDefaultChar
			this.m_nDefaultChar = HEAP[pTable + 92];
			
			// version
			if (0xFFFF == HEAP[pTable])
				return;
			
			var ulCodePageRange1 = HEAP[pTable + 80];
			var ulCodePageRange2 = HEAP[pTable + 84];
			if ( !(ulCodePageRange1 & 0x80000000) && !(ulCodePageRange1 == 0 && ulCodePageRange2 == 0))
				return;

            var charMapArray = HEAP[HEAP[this.m_pFace] + 40];
            for (var nIndex = 0; nIndex < this.m_nNum_charmaps; ++nIndex)
            {
                var pCharMap = charMapArray + nIndex*4;

                var nPlatformId = HEAP[HEAP[pCharMap] + 8];
                var nEncodingId = HEAP[HEAP[pCharMap] + 10];
                // Symbol
                if (0 == nEncodingId && 3 == nPlatformId)
                {
                    this.m_nSymbolic = nIndex;
                    return;
                }
            }
		}
	}
	
	this.ResetFontMatrix = function()
	{
        if (this.m_pDefaultFont)
            this.m_pDefaultFont.ResetFontMatrix();

		if ( this.m_bNeedDoItalic )
		{
			this.m_arrdFontMatrix[0] = 1;
			this.m_arrdFontMatrix[1] = 0;
			this.m_arrdFontMatrix[2] = FONT_ITALIC_ANGLE;
			this.m_arrdFontMatrix[3] = 1;
			this.m_arrdFontMatrix[4] = 0;
			this.m_arrdFontMatrix[5] = 0;
		}
		else
		{
			this.m_arrdFontMatrix[0] = 1;
			this.m_arrdFontMatrix[1] = 0;
			this.m_arrdFontMatrix[2] = 0;
			this.m_arrdFontMatrix[3] = 1;
			this.m_arrdFontMatrix[4] = 0;
			this.m_arrdFontMatrix[5] = 0;
		}

		this.UpdateMatrix0();
	}
	
	this.ResetTextMatrix = function()
	{
		this.m_arrdTextMatrix[0] =  1;
		this.m_arrdTextMatrix[1] = -0;
		this.m_arrdTextMatrix[2] = -0; // 
		this.m_arrdTextMatrix[3] =  1; // 
		this.m_arrdTextMatrix[4] =  0;
		this.m_arrdTextMatrix[5] =  0;
		
		this.CheckTextMatrix();
	}
	
	this.UpdateMatrix0 = function()
	{
		var dSize = this.m_fSize;

		this.m_dTextScale = Math.sqrt(this.m_arrdTextMatrix[2] * this.m_arrdTextMatrix[2] + this.m_arrdTextMatrix[3] * this.m_arrdTextMatrix[3]);
		
		var bbox = HEAP[this.m_pFace] + 52;
		var xMin = HEAP[bbox];
		var yMin = HEAP[bbox+4];
		var xMax = HEAP[bbox+8];
		var yMax = HEAP[bbox+12];
		
		var units_per_EM = HEAP[HEAP[this.m_pFace] + 68];
		var dDiv = xMax > 20000 ? 65536 : 1;

		// ��������� BBox
		if (units_per_EM == 0 )
			units_per_EM = HEAP[HEAP[this.m_pFace] + 68] = 2048;

		var nX = parseInt(((this.m_arrdFontMatrix[0] * xMin + this.m_arrdFontMatrix[2] * yMin) * dSize / (dDiv * units_per_EM)));
		this.m_nMinX = this.m_nMaxX = nX;

		var nY = parseInt(((this.m_arrdFontMatrix[1] * xMin + this.m_arrdFontMatrix[3] * yMin) * dSize / (dDiv * units_per_EM)));
		this.m_nMinY = this.m_nMaxY = nY;

		nX = parseInt(((this.m_arrdFontMatrix[0] * xMin + this.m_arrdFontMatrix[2] * yMax) * dSize / (dDiv * units_per_EM)));

		if (nX < this.m_nMinX)
			this.m_nMinX = nX;
		else if (nX > this.m_nMaxX)
			this.m_nMaxX = nX;

		nY = parseInt(((this.m_arrdFontMatrix[1] * xMin + this.m_arrdFontMatrix[3] * yMax) * dSize / (dDiv * units_per_EM)));

		if (nY < this.m_nMinY)
			this.m_nMinY = nY;
		else if (nY > this.m_nMaxY)
			this.m_nMaxY = nY;

		nX = parseInt(((this.m_arrdFontMatrix[0] * xMax + this.m_arrdFontMatrix[2] * yMin) * dSize / (dDiv * units_per_EM)));
		if (nX < this.m_nMinX)
			this.m_nMinX = nX;
		else if (nX > this.m_nMaxX)
			this.m_nMaxX = nX;

		nY = parseInt(((this.m_arrdFontMatrix[1] * xMax + this.m_arrdFontMatrix[3] * yMin) * dSize / (dDiv * units_per_EM)));
		if (nY < this.m_nMinY)
			this.m_nMinY = nY;
		else if (nY > this.m_nMaxY)
			this.m_nMaxY = nY;

		nX = parseInt(((this.m_arrdFontMatrix[0] * xMax + this.m_arrdFontMatrix[2] * yMax) * dSize / (dDiv * units_per_EM)));
		if (nX < this.m_nMinX)
			this.m_nMinX = nX;
		else if (nX > this.m_nMaxX)
			this.m_nMaxX = nX;

		nY = parseInt(((this.m_arrdFontMatrix[1] * xMax + this.m_arrdFontMatrix[3] * yMax) * dSize / (dDiv * units_per_EM)));
		if (nY < this.m_nMinY)
			this.m_nMinY = nY;
		else if (nY > this.m_nMaxY)
			this.m_nMaxY = nY;

		// This is a kludge: some buggy PDF generators embed fonts with  zero bounding boxes.
		if (this.m_nMaxX == this.m_nMinX)
		{
			this.m_nMinX = 0;
			this.m_nMaxX = parseInt(dSize);
		}

		if (this.m_nMaxY == this.m_nMinY)
		{
			this.m_nMinY = 0;
			this.m_nMaxY = parseInt((1.2 * dSize));
		}

		// �������� ������� �������������� (FontMatrix)
		HEAP[this.m_oFontMatrix] = Math.floor((this.m_arrdFontMatrix[0] * 65536));
		HEAP[this.m_oFontMatrix+8] = Math.floor((this.m_arrdFontMatrix[1] * 65536));
		HEAP[this.m_oFontMatrix+4] = Math.floor((this.m_arrdFontMatrix[2] * 65536));
		HEAP[this.m_oFontMatrix+12] = Math.floor((this.m_arrdFontMatrix[3] * 65536));

		HEAP[this.m_oTextMatrix] = Math.floor(((this.m_arrdTextMatrix[0] / this.m_dTextScale) * 65536));
		HEAP[this.m_oTextMatrix+8] = Math.floor(((this.m_arrdTextMatrix[1] / this.m_dTextScale) * 65536));
		HEAP[this.m_oTextMatrix+4] = Math.floor(((this.m_arrdTextMatrix[2] / this.m_dTextScale) * 65536));
		HEAP[this.m_oTextMatrix+12] = Math.floor(((this.m_arrdTextMatrix[3] / this.m_dTextScale) * 65536));

		_FT_Set_Transform(HEAP[this.m_pFace], this.m_oFontMatrix, 0);
	}

    this.UpdateMatrix1 = function()
    {
        HEAP[this.m_oFontMatrix] = (this.m_arrdFontMatrix[0] * 65536);
        HEAP[this.m_oFontMatrix + 8] = (this.m_arrdFontMatrix[1] * 65536);
        HEAP[this.m_oFontMatrix + 4] = (this.m_arrdFontMatrix[2] * 65536);
        HEAP[this.m_oFontMatrix + 12] = (this.m_arrdFontMatrix[3] * 65536);

        _FT_Set_Transform(HEAP[this.m_pFace], this.m_oFontMatrix, 0);
    }

    this.UpdateMatrix2 = function()
    {
        HEAP[this.m_oFontMatrix] = ((this.m_arrdFontMatrix[0] * this.m_arrdTextMatrix[0] + this.m_arrdFontMatrix[1] * this.m_arrdTextMatrix[2] ) * 65536);
        HEAP[this.m_oFontMatrix + 8] = ((this.m_arrdFontMatrix[0] * this.m_arrdTextMatrix[1] + this.m_arrdFontMatrix[1] * this.m_arrdTextMatrix[3] ) * 65536);
        HEAP[this.m_oFontMatrix + 4] = ((this.m_arrdFontMatrix[2] * this.m_arrdTextMatrix[0] + this.m_arrdFontMatrix[3] * this.m_arrdTextMatrix[2] ) * 65536);
        HEAP[this.m_oFontMatrix + 12] = ((this.m_arrdFontMatrix[2] * this.m_arrdTextMatrix[1] + this.m_arrdFontMatrix[3] * this.m_arrdTextMatrix[3] ) * 65536);

        _FT_Set_Transform(HEAP[this.m_pFace], this.m_oFontMatrix, 0);
    }
	
	this.SetSizeAndDpi = function(fSize, unHorDpi, unVerDpi)
	{
        if (this.m_pDefaultFont)
            this.m_pDefaultFont.SetSizeAndDpi (fSize, unHorDpi, unVerDpi);

		var fOldSize = this.m_fSize;
		var fNewSize = fSize;
		var fKoef    = fNewSize / fOldSize;

		if (fKoef > 1.001 || fKoef < 0.999 || unHorDpi != this.m_unHorDpi || unVerDpi != this.m_unVerDpi)
		{
			this.m_unHorDpi = unHorDpi;
			this.m_unVerDpi = unVerDpi;

			if (fKoef > 1.001 || fKoef < 0.999)
			{
				this.m_fSize    = fNewSize;
				this.UpdateMatrix0();
			}

			this.m_dUnitsKoef = this.m_unHorDpi / 72.0 * this.m_fSize;

			// ���������� ������ ������ (dSize) � DPI 
			this.m_nError = _FT_Set_Char_Size(HEAP[this.m_pFace], 0, parseInt(fNewSize * 64), unHorDpi, unVerDpi )
			
			this.ClearCache();
		}
	}
	
	this.ClearCache = function()
	{
        this.m_arrCacheSizes = [];
	}
	
	this.SetTextMatrix = function(fA, fB, fC, fD, fE, fF)
	{
        if (this.m_arrdTextMatrix[0] ==  fA && this.m_arrdTextMatrix[1] == -fB &&
            this.m_arrdTextMatrix[2] == -fC && this.m_arrdTextMatrix[3] ==  fD &&
            this.m_arrdTextMatrix[4] ==  fE && this.m_arrdTextMatrix[5] ==  fF)
            return false;

        if (this.m_pDefaultFont)
            this.m_pDefaultFont.SetTextMatrix (fA, fB, fC, fD, fE, fF);

		this.m_arrdTextMatrix[0] =  fA;
		this.m_arrdTextMatrix[1] = -fB;
		this.m_arrdTextMatrix[2] = -fC;
		this.m_arrdTextMatrix[3] =  fD;
		this.m_arrdTextMatrix[4] =  fE;
		this.m_arrdTextMatrix[5] =  fF;

        this.ClearCache();		
		this.CheckTextMatrix();
		return true;
	}

    this.SetFontMatrix = function(fA, fB, fC, fD, fE, fF)
    {
        if (this.m_pDefaultFont)
            this.m_pDefaultFont.SetFontMatrix (fA, fB, fC, fD, fE, fF);

        if (this.m_bNeedDoItalic)
        {
            this.m_arrdFontMatrix[0] = fA;
            this.m_arrdFontMatrix[1] = fB;
            this.m_arrdFontMatrix[2] = fC + fA * FONT_ITALIC_ANGLE;
            this.m_arrdFontMatrix[3] = fD + fB * FONT_ITALIC_ANGLE;
            this.m_arrdFontMatrix[4] = fE;
            this.m_arrdFontMatrix[5] = fF;
        }
        else
        {
            this.m_arrdFontMatrix[0] = fA;
            this.m_arrdFontMatrix[1] = fB;
            this.m_arrdFontMatrix[2] = fC;
            this.m_arrdFontMatrix[3] = fD;
            this.m_arrdFontMatrix[4] = fE;
            this.m_arrdFontMatrix[5] = fF;
        }

        this.ClearCache();
    }
	
	this.CheckTextMatrix = function()
	{
		this.m_bIsNeedUpdateMatrix12 = true;
		if ((this.m_arrdTextMatrix[0] == 1) && (this.m_arrdTextMatrix[1] == 0) && (this.m_arrdTextMatrix[2] == 0) && (this.m_arrdTextMatrix[3] == 1))
		{
			this.m_bIsNeedUpdateMatrix12 = false;
			
			if (this.m_pDefaultFont)
				this.m_pDefaultFont.UpdateMatrix1();
			this.UpdateMatrix1();						
		}
	}
	
	this.GetString = function(pString)
	{
		if (pString.GetLength() <= 0)
			return true;

		var unPrevGID = 0;
		var fPenX = 0, fPenY = 0;

        // Сначала мы все рассчитываем исходя только из матрицы шрифта FontMatrix
		if (this.m_bIsNeedUpdateMatrix12)
		{
			if (this.m_pDefaultFont)
				this.m_pDefaultFont.UpdateMatrix1();
			this.UpdateMatrix1();
		}
		
		for (var nIndex = 0; nIndex < pString.GetLength(); ++nIndex)
		{
            var pFace = this.m_pFace;
            var pCurentGliph = this.m_pGlyph;

			var pCurGlyph = pString.GetAt(nIndex);
			var ushUnicode = pCurGlyph.lUnicode;
			
			var unGID = 0;
			var charSymbolObj = this.m_arrCacheSizes[ushUnicode];
			if (undefined == charSymbolObj)
			{
				var nCMapIndex = 0;
				unGID = this.SetCMapForCharCode(ushUnicode, nCMapIndex);
				
				var oSizes = new TFontCacheSizes();
				oSizes.ushUnicode = ushUnicode;

                if (!((unGID > 0) || (-1 != this.m_nSymbolic && (ushUnicode < 0xF000)  && 0 < (unGID = this.SetCMapForCharCode (ushUnicode + 0xF000, nCMapIndex)))))
				{
                    // Пробуем загрузить через стандартный шрифт
                    if (false == this.m_bUseDefaultFont || null == this.m_pDefaultFont || 0 >= (unGID = this.m_pDefaultFont.SetCMapForCharCode (ushUnicode, nCMapIndex)))
                    {
                        if (this.m_nDefaultChar < 0)
                        {
                            oSizes.ushGID    = -1;
                            oSizes.eState    = EGlyphState.glyphstateMiss;
                            var max_advance = HEAP[HEAP[HEAP[this.m_pFace]+88]+12+24];
                            oSizes.fAdvanceX = (max_advance >> 6) / 2.0;

                            // В стандартном шрифте данный символ тоже не рисуется
                            pString.SetStartPoint (nIndex, fPenX, fPenY);
                            pString.SetBBox (nIndex, 0, 0, 0, 0);
                            pString.SetState (nIndex, EGlyphState.glyphstateMiss);

                            fPenX += oSizes.fAdvanceX + this.m_fCharSpacing;
                            unPrevGID = 0;

                            continue;
                        }
                        else
                        {
                            unGID = this.m_nDefaultChar;
                            oSizes.eState = EGlyphState.glyphstateNormal;
                            pString.SetState (nIndex, EGlyphState.glyphstateNormal);

                            pFace = this.m_pDefaultFont.m_pFace;
                            pCurentGliph = this.m_pDefaultFont.m_pGlyph;
                        }
                    }
                    else
                    {
                        oSizes.eState = EGlyphState.glyphstateDeafault;

                        pString.SetState (nIndex, EGlyphState.glyphstateDeafault);

                        pFace = this.m_pDefaultFont.m_pFace;
                        pCurentGliph = this.m_pDefaultFont.m_pGlyph;
                    }
				}
				else
				{
					oSizes.eState = EGlyphState.glyphstateNormal;
					pString.SetState (nIndex, EGlyphState.glyphstateNormal);
				}
				
				oSizes.ushGID     = unGID;
				oSizes.nCMapIndex = nCMapIndex;
				
				if (false)
				{
				}
				
				var fX = pString.m_fX + fPenX;
				var fY = pString.m_fY + fPenY;
				
				// ��������� ����� ������������ ����� ������ �� ���������� ������� 
				var fXX = (pString.m_arrCTM[4] + fX * pString.m_arrCTM[0] + fY * pString.m_arrCTM[2] - pString.m_fX);
				var fYY = (pString.m_arrCTM[5] + fX * pString.m_arrCTM[1] + fY * pString.m_arrCTM[3] - pString.m_fY);
				
				pString.SetStartPoint (nIndex, fXX, fYY);
				
				if (_FT_Load_Glyph (HEAP[pFace], unGID, LOAD_MODE))
				{
					pString.SetStartPoint (nIndex, -0xFFFF, -0xFFFF);
					pString.SetState (nIndex, EGlyphState.glyphstateMiss);
					continue;
				}
				
				var pGlyph = STACKTOP;
				STACKTOP+=4;
				Runtime.memset(pGlyph,0,4);
				
				if (_FT_Get_Glyph (pCurentGliph, pGlyph))
				{
                    pString.SetStartPoint (nIndex, -0xFFFF, -0xFFFF);
                    pString.SetState (nIndex, EGlyphState.glyphstateMiss);
					continue;
				}
				
				var oBBox = STACKTOP;
				STACKTOP+=16;
				Runtime.memset(oBBox,0,16);
				
				_FT_Glyph_Get_CBox (HEAP[pGlyph], ft_glyph_bbox_gridfit, oBBox);
				var xMin = HEAP[oBBox];
				var yMin = HEAP[oBBox+4];
				var xMax = HEAP[oBBox+8];
				var yMax = HEAP[oBBox+12];
				_FT_Done_Glyph (HEAP[pGlyph]);
				STACKTOP = pGlyph;
				
				var linearHoriAdvance = HEAP[pCurentGliph+56];
				var units_per_EM = HEAP[HEAP[pFace] + 68];
				
				oSizes.fAdvanceX = (linearHoriAdvance * this.m_dUnitsKoef / units_per_EM);
				oSizes.oBBox.fMinX = (xMin >> 6);
				oSizes.oBBox.fMaxX = (xMax >> 6);
				oSizes.oBBox.fMinY = (yMin >> 6);
				oSizes.oBBox.fMaxY = (yMax >> 6);

				oSizes.oMetrics.fWidth        = (HEAP[pCurentGliph+24] >> 6);
				oSizes.oMetrics.fHeight       = (HEAP[pCurentGliph+28] >> 6);
				oSizes.oMetrics.fHoriBearingX = (HEAP[pCurentGliph+32] >> 6);
				oSizes.oMetrics.fHoriBearingY = (HEAP[pCurentGliph+36] >> 6);
				oSizes.oMetrics.fHoriAdvance  = (HEAP[pCurentGliph+40] >> 6);
				oSizes.oMetrics.fVertBearingX = (HEAP[pCurentGliph+44] >> 6);
				oSizes.oMetrics.fVertBearingY = (HEAP[pCurentGliph+48] >> 6);
				oSizes.oMetrics.fVertAdvance  = (HEAP[pCurentGliph+52] >> 6);

				oSizes.bBitmap = false;
				oSizes.oBitmap.nX        = 0;
				oSizes.oBitmap.nY        = 0;
				oSizes.oBitmap.nHeight   = 0;
				oSizes.oBitmap.nWidth    = 0;
				oSizes.oBitmap.bFreeData = false;
				oSizes.oBitmap.pData     = null;
				oSizes.oBitmap.bAA       = false;
				
				pString.SetMetrics (nIndex, oSizes.oMetrics.fWidth, oSizes.oMetrics.fHeight, oSizes.oMetrics.fHoriAdvance, oSizes.oMetrics.fHoriBearingX, oSizes.oMetrics.fHoriBearingY, oSizes.oMetrics.fVertAdvance, oSizes.oMetrics.fVertBearingX, oSizes.oMetrics.fVertBearingY);
				pString.SetBBox (nIndex, oSizes.oBBox.fMinX, oSizes.oBBox.fMaxY, oSizes.oBBox.fMaxX, oSizes.oBBox.fMinY);

				fPenX += oSizes.fAdvanceX + this.m_fCharSpacing;
				if (this.m_bNeedDoBold)
				{
                    // Когда текст делаем жирным сами, то мы увеличиваем расстояние на 1 пиксель в ширину (независимо от DPI и размера текста всегда 1 пиксель)
					fPenX += 1;
				}
                this.m_arrCacheSizes[oSizes.ushUnicode] = oSizes;
			}
			else
			{
                var nCMapIndex     = charSymbolObj.nCMapIndex;
                unGID              = charSymbolObj.ushGID;
                var eState         = charSymbolObj.eState;

                if (EGlyphState.glyphstateMiss == eState)
                {			
					pCurGlyph.fX = fPenX;
					pCurGlyph.fY = fPenY;
				
                    pCurGlyph.fLeft   = 0;
					pCurGlyph.fTop    = 0;
					pCurGlyph.fRight  = 0;
					pCurGlyph.fBottom = 0;
                    pCurGlyph.eState = EGlyphState.glyphstateMiss;

                    fPenX += charSymbolObj.fAdvanceX + this.m_fCharSpacing;
                    unPrevGID = 0;

                    continue;
                }
                else if (EGlyphState.glyphstateDeafault == eState)
                {
                    pCurGlyph.eState = EGlyphState.glyphstateDeafault;
                    //pFace = pDefFace;
                }
                else
                {
                     pCurGlyph.eState = EGlyphState.glyphstateNormal;
                    //pFace = pSrcFace;
                }				

                if (0 != this.m_nNum_charmaps)
                {
                    var nCharmap = HEAP[HEAP[pFace]+92];
                    var nCurCMapIndex = _FT_Get_Charmap_Index (nCharmap);
                    if (nCurCMapIndex != nCMapIndex)
                    {
                        nCMapIndex = Math.max (0, nCMapIndex);
                        var pCharMapArray = HEAP[HEAP[pFace] + 40]+ nCMapIndex*4;
                        _FT_Set_Charmap (HEAP[pFace], HEAP[pCharMapArray]);
                    }
                }

                if (this.m_bUseKerning && unPrevGID && (nIndex >= 0 && pString.GetAt (nIndex).eState == pString.GetAt (nIndex - 1).eState))
                {
                    var pDelta = STACKTOP;
                    Runtime.memset(pDelta,0,4);
                    _FT_Get_Kerning (HEAP[pFace], unPrevGID, unGID, FT_Kerning_Mode.FT_KERNING_DEFAULT, pDelta);
                    fPenX += (HEAP[pDelta] >> 6);
                    STACKTOP = pDelta;
                }

                var fX = pString.m_fX + fPenX;
                var fY = pString.m_fY + fPenY;

                // ��������� ����� ������������ ����� ������ �� ���������� �������
               	pCurGlyph.fX = (pString.m_arrCTM[4] + fX * pString.m_arrCTM[0] + fY * pString.m_arrCTM[2] - pString.m_fX );
                pCurGlyph.fY = (pString.m_arrCTM[5] + fX * pString.m_arrCTM[1] + fY * pString.m_arrCTM[3] - pString.m_fY );

				pCurGlyph.oMetrix = charSymbolObj.oMetrics;
				
				pCurGlyph.fLeft   = charSymbolObj.oBBox.fMinX;
				pCurGlyph.fTop    = charSymbolObj.oBBox.fMaxY;
				pCurGlyph.fRight  = charSymbolObj.oBBox.fMaxX;
				pCurGlyph.fBottom = charSymbolObj.oBBox.fMinY;
				
                fPenX += charSymbolObj.fAdvanceX + this.m_fCharSpacing;

                if (this.m_bNeedDoBold)
                {
                 // Когда текст делаем жирным сами, то мы увеличиваем расстояние на 1 пиксель в ширину (независимо от DPI и размера текста всегда 1 пиксель)
                    fPenX += 1;
                }

                /*charSymbolObj.bBitmap = false;
                charSymbolObj.oBitmap.nX        = 0;
                charSymbolObj.oBitmap.nY        = 0;
                charSymbolObj.oBitmap.nHeight   = 0;
                charSymbolObj.oBitmap.nWidth    = 0;
                charSymbolObj.oBitmap.bFreeData = false;
                charSymbolObj.oBitmap.pData     = null;
                charSymbolObj.oBitmap.bAA       = false;*/
			}
            unPrevGID = unGID;
		}
		
		pString.m_fEndX = fPenX + pString.m_fX;
		pString.m_fEndY = fPenY + pString.m_fY;

		if (this.m_bIsNeedUpdateMatrix12)
		{
			if (this.m_pDefaultFont)
				this.m_pDefaultFont.UpdateMatrix2();
			this.UpdateMatrix2();
		}
	}
	
	this.GetString2 = function(pString)
	{
		if (pString.GetLength() <= 0)
			return true;
		
		var unPrevGID = 0;
		var fPenX = 0, fPenY = 0;
		
		for (var nIndex = 0; nIndex < pString.GetLength(); ++nIndex)
		{
            // Сначала мы все рассчитываем исходя только из матрицы шрифта FontMatrix
			if (this.m_bIsNeedUpdateMatrix12)
			{
				if (this.m_pDefaultFont)
					this.m_pDefaultFont.UpdateMatrix1();
				this.UpdateMatrix1();
			}

            var pFace = this.m_pFace;
            var pCurentGliph = this.m_pGlyph;
            
			var pCurGlyph = pString.GetAt(nIndex);
			var ushUnicode = pCurGlyph.lUnicode;
			
			var unGID = 0;
            var charSymbolObj = this.m_arrCacheSizes[ushUnicode];
			if (undefined == charSymbolObj || null == charSymbolObj.oBitmap.pData)
			{
				var nCMapIndex = 0;
				unGID = this.SetCMapForCharCode(ushUnicode, nCMapIndex);

				var oSizes = new TFontCacheSizes();
				oSizes.ushUnicode = ushUnicode;

                if (!((unGID > 0) || (-1 != this.m_nSymbolic && (ushUnicode < 0xF000)  && 0 < (unGID = this.SetCMapForCharCode (ushUnicode + 0xF000, nCMapIndex)))))
				{
                    // Пробуем загрузить через стандартный шрифт
                    if (false == this.m_bUseDefaultFont || null == this.m_pDefaultFont || 0 >= (unGID = this.m_pDefaultFont.SetCMapForCharCode (ushUnicode, nCMapIndex)))
                    {
                        if (this.m_nDefaultChar < 0)
                        {
                            oSizes.ushGID    = -1;
                            oSizes.eState    = EGlyphState.glyphstateMiss;
                            var max_advance = HEAP[HEAP[HEAP[this.m_pFace]+88]+12+24];
                            oSizes.fAdvanceX = (max_advance >> 6) / 2.0;

                            // В стандартном шрифте данный символ тоже не рисуется
                            pString.SetStartPoint (nIndex, fPenX, fPenY);
                            pString.SetBBox (nIndex, 0, 0, 0, 0);
                            pString.SetState (nIndex, EGlyphState.glyphstateMiss);

                            fPenX += oSizes.fAdvanceX + this.m_fCharSpacing;
                            unPrevGID = 0;

                            continue;
                        }
                        else if (undefined != this.m_pDefaultFont)
                        {
                            unGID = this.m_nDefaultChar;
                            oSizes.eState = EGlyphState.glyphstateNormal;
                            pString.SetState (nIndex, EGlyphState.glyphstateNormal);
                            pFace = this.m_pDefaultFont.m_pFace;
                            pCurentGliph = this.m_pDefaultFont.m_pGlyph;
                        }
                    }
                    else
                    {
                        oSizes.eState = EGlyphState.glyphstateDeafault;

                        pString.SetState (nIndex, EGlyphState.glyphstateDeafault);

                        pFace = this.m_pDefaultFont.m_pFace;
                        pCurentGliph = this.m_pDefaultFont.m_pGlyph;
                    }
				}
				else
				{
					oSizes.eState = EGlyphState.glyphstateNormal;
					pString.SetState (nIndex, EGlyphState.glyphstateNormal);
				}

				oSizes.ushGID     = unGID;
				oSizes.nCMapIndex = nCMapIndex;

                if (this.m_bUseKerning && unPrevGID && (nIndex >= 0 && pString.GetAt (nIndex).eState == pString.GetAt (nIndex - 1).eState))
                {
                    var pDelta = STACKTOP;
                    Runtime.memset(pDelta,0,4);
                    _FT_Get_Kerning (HEAP[pFace], unPrevGID, unGID, FT_Kerning_Mode.FT_KERNING_DEFAULT, pDelta);
                    fPenX += (HEAP[pDelta] >> 6);
                    STACKTOP = pDelta;
                }
				
				var fX = pString.m_fX + fPenX;
				var fY = pString.m_fY + fPenY;
				
				// ��������� ����� ������������ ����� ������ �� ���������� ������� 
				var fXX = (pString.m_arrCTM[4] + fX * pString.m_arrCTM[0] + fY * pString.m_arrCTM[2] - pString.m_fX);
				var fYY = (pString.m_arrCTM[5] + fX * pString.m_arrCTM[1] + fY * pString.m_arrCTM[3] - pString.m_fY);
				
				pString.SetStartPoint (nIndex, fXX, fYY);

				if (this.m_bIsNeedUpdateMatrix12)
				{
					if (this.m_pDefaultFont)
						this.m_pDefaultFont.UpdateMatrix2();
					this.UpdateMatrix2();
				}
				
				if (_FT_Load_Glyph (HEAP[pFace], unGID, LOAD_MODE))
				{
                    pString.SetStartPoint (nIndex, -0xFFFF, -0xFFFF);
                    pString.SetState (nIndex, EGlyphState.glyphstateMiss);
					continue;
				}
				
				var pGlyph = STACKTOP;
				STACKTOP+=4;
				Runtime.memset(pGlyph,0,4);
				
				if (_FT_Get_Glyph (pCurentGliph, pGlyph))
				{
					pString.SetStartPoint (nIndex, -0xFFFF, -0xFFFF);
					pString.SetState (nIndex, EGlyphState.glyphstateMiss);
					continue;
				}
				
				var oBBox = STACKTOP;
				STACKTOP+=16;
				Runtime.memset(oBBox,0,16);
				
				_FT_Glyph_Get_CBox (HEAP[pGlyph], ft_glyph_bbox_gridfit, oBBox);
				var xMin = HEAP[oBBox];
				var yMin = HEAP[oBBox+4];
				var xMax = HEAP[oBBox+8];
				var yMax = HEAP[oBBox+12];
				_FT_Done_Glyph (HEAP[pGlyph]);
				STACKTOP = pGlyph;
				
				var linearHoriAdvance = HEAP[pCurentGliph+56];
				var units_per_EM = HEAP[HEAP[pFace] + 68];

				oSizes.fAdvanceX = (linearHoriAdvance * this.m_dUnitsKoef / units_per_EM);
				oSizes.oBBox.fMinX = (xMin >> 6);
				oSizes.oBBox.fMaxX = (xMax >> 6);
				oSizes.oBBox.fMinY = (yMin >> 6);
				oSizes.oBBox.fMaxY = (yMax >> 6);

				oSizes.oMetrics.fWidth        = (HEAP[pCurentGliph+24] >> 6);
				oSizes.oMetrics.fHeight       = (HEAP[pCurentGliph+28] >> 6);
				oSizes.oMetrics.fHoriBearingX = (HEAP[pCurentGliph+32] >> 6);
				oSizes.oMetrics.fHoriBearingY = (HEAP[pCurentGliph+36] >> 6);
				oSizes.oMetrics.fHoriAdvance  = (HEAP[pCurentGliph+40] >> 6);
				oSizes.oMetrics.fVertBearingX = (HEAP[pCurentGliph+44] >> 6);
				oSizes.oMetrics.fVertBearingY = (HEAP[pCurentGliph+48] >> 6);
				oSizes.oMetrics.fVertAdvance  = (HEAP[pCurentGliph+52] >> 6);
				
				//pString.SetMetrics (nIndex, oSizes.oMetrics.fWidth, oSizes.oMetrics.fHeight, oSizes.oMetrics.fHoriAdvance, oSizes.oMetrics.fHoriBearingX, oSizes.oMetrics.fHoriBearingY, oSizes.oMetrics.fVertAdvance, oSizes.oMetrics.fVertBearingX, oSizes.oMetrics.fVertBearingY);
				pString.m_pGlyphsBuffer[nIndex].oMetrics = oSizes.oMetrics;
				pString.SetBBox (nIndex, oSizes.oBBox.fMinX, oSizes.oBBox.fMaxY, oSizes.oBBox.fMaxX, oSizes.oBBox.fMinY);
				//pString->SetBBox( nIndex, (float)(oBBox.xMin >> 6), (float)(oBBox.yMax >> 6), (float)(oBBox.xMax >> 6), (float)(oBBox.yMin >> 6) );

				fPenX += oSizes.fAdvanceX + this.m_fCharSpacing;
				if (this.m_bNeedDoBold)
				{
                    // Когда текст делаем жирным сами, то мы увеличиваем расстояние на 1 пиксель в ширину (независимо от DPI и размера текста всегда 1 пиксель)
					fPenX += 1;
				}
				
				oSizes.bBitmap  = true;
				if (_FT_Render_Glyph(pCurentGliph, REND_MODE))
					return;

				var pBitmap = pCurentGliph+76;
				
				oSizes.oBitmap = new TGlyphBitmap();
				oSizes.oBitmap.nX      = HEAP[pCurentGliph+100];
				oSizes.oBitmap.nY      = HEAP[pCurentGliph+104];
				oSizes.oBitmap.nWidth  = HEAP[pBitmap+4];
				oSizes.oBitmap.nHeight = HEAP[pBitmap];
				oSizes.oBitmap.bAA     = this.m_bAntiAliasing;

				var nRowSize = 0;
				if (this.m_bAntiAliasing)
				{
					if (this.m_bNeedDoBold)
                        oSizes.oBitmap.nWidth++;

					nRowSize = oSizes.oBitmap.nWidth;
				} 
				else 
				{
					nRowSize = (oSizes.oBitmap.nWidth + 7) >> 3;
				}

				var nSizeBuffer = nRowSize * oSizes.oBitmap.nHeight;
				oSizes.oBitmap.pData = new Array(nSizeBuffer);
				oSizes.oBitmap.bFreeData = false; // Все удаляется в кэше (во время очистки или замены)

				var pCharBuffer = HEAP[pBitmap+12];
				if (!this.m_bNeedDoBold || !this.m_bAntiAliasing)
				{
					var pMemoryData = oSizes.oBitmap.pData;
					for (var nIndex2 = 0; nIndex2 < nSizeBuffer; ++nIndex2)
						pMemoryData[nIndex2] = HEAP[pCharBuffer+nIndex2];
				}
				else
				{
					var nY, nX;
                    var pDstBuffer;
                    var pSrcBuffer;
                    var nPitch = HEAP[pBitmap+8];
					var pMemoryData = oSizes.oBitmap.pData;
					for (nY = 0, pDstBuffer = 0, pSrcBuffer = pCharBuffer; nY < oSizes.oBitmap.nHeight; ++nY, pDstBuffer += nRowSize, pSrcBuffer += nPitch)
					{
						for (nX = oSizes.oBitmap.nWidth - 1; nX >= 0; --nX)
						{
							if (0 != nX)
							{
								var nFirstByte, nSecondByte;

								if (oSizes.oBitmap.nWidth - 1 == nX)
									nFirstByte = 0;
								else
									nFirstByte = HEAP[pSrcBuffer + nX];

                                nSecondByte = HEAP[pSrcBuffer + nX - 1];

                                pMemoryData[pDstBuffer + nX] = Math.min(255, nFirstByte + nSecondByte);
							}
							else
							{
                                pMemoryData[pDstBuffer + nX] = HEAP[pSrcBuffer + nX];
							}
						}
					}
				}
	
				pCurGlyph.bBitmap  = oSizes.bBitmap;
				pCurGlyph.oBitmap  = oSizes.oBitmap;

                // new scheme!!! --------------------------
				oSizes.oBitmap.fromAlphaMask();
                // ----------------------------------------

                this.m_arrCacheSizes[oSizes.ushUnicode] = oSizes;
			}
			else
			{
                var nCMapIndex     = charSymbolObj.nCMapIndex;
                unGID              = charSymbolObj.ushGID;
                var eState = charSymbolObj.eState;

                if (EGlyphState.glyphstateMiss == eState)
                {			
					pCurGlyph.fX = fPenX;
					pCurGlyph.fY = fPenY;
				
                    pCurGlyph.fLeft   = 0;
					pCurGlyph.fTop    = 0;
					pCurGlyph.fRight  = 0;
					pCurGlyph.fBottom = 0;
                    pCurGlyph.eState = EGlyphState.glyphstateMiss;

                    fPenX += charSymbolObj.fAdvanceX + this.m_fCharSpacing;
                    unPrevGID = 0;

                    continue;
                }
                else if (EGlyphState.glyphstateDeafault == eState)
                {
                    pCurGlyph.eState = EGlyphState.glyphstateDeafault;
                    //pFace = pDefFace;
                }
                else
                {
                     pCurGlyph.eState = EGlyphState.glyphstateNormal;
                    //pFace = pSrcFace;
                }

                if (0 != this.m_nNum_charmaps)
                {
                    var nCharmap = HEAP[HEAP[pFace]+92];
                    var nCurCMapIndex = _FT_Get_Charmap_Index (nCharmap);
                    if (nCurCMapIndex != nCMapIndex)
                    {
                        nCMapIndex = Math.max (0, nCMapIndex);
                        var pCharMapArray = HEAP[HEAP[pFace] + 40]+ nCMapIndex*4;
                        _FT_Set_Charmap (HEAP[pFace], HEAP[pCharMapArray]);
                    }
                }

                if (this.m_bUseKerning && unPrevGID && (nIndex >= 0 && pCurGlyph.eState == pString.GetAt (nIndex - 1).eState))
                {
                    var pDelta = STACKTOP;
                    Runtime.memset(pDelta,0,4);
                    _FT_Get_Kerning (HEAP[pFace], unPrevGID, unGID, FT_Kerning_Mode.FT_KERNING_DEFAULT, pDelta);
                    fPenX += (HEAP[pDelta] >> 6);
                    STACKTOP = pDelta;
                }

                var fX = pString.m_fX + fPenX;
                var fY = pString.m_fY + fPenY;

                // ��������� ����� ������������ ����� ������ �� ���������� �������
                pCurGlyph.fX = (pString.m_arrCTM[4] + fX * pString.m_arrCTM[0] + fY * pString.m_arrCTM[2] - pString.m_fX );
                pCurGlyph.fY = (pString.m_arrCTM[5] + fX * pString.m_arrCTM[1] + fY * pString.m_arrCTM[3] - pString.m_fY );

				pCurGlyph.oMetrix = charSymbolObj.oMetrics;
				
				pCurGlyph.fLeft   = charSymbolObj.oBBox.fMinX;
				pCurGlyph.fTop    = charSymbolObj.oBBox.fMaxY;
				pCurGlyph.fRight  = charSymbolObj.oBBox.fMaxX;
				pCurGlyph.fBottom = charSymbolObj.oBBox.fMinY;
		
                fPenX += charSymbolObj.fAdvanceX + this.m_fCharSpacing;

                if (this.m_bNeedDoBold)
                {
                    // Когда текст делаем жирным сами, то мы увеличиваем расстояние на 1 пиксель в ширину (независимо от DPI и размера текста всегда 1 пиксель)
                    fPenX += 1;
                }

                pCurGlyph.bBitmap = charSymbolObj.bBitmap;
                pCurGlyph.oBitmap = charSymbolObj.oBitmap;
			}
            unPrevGID = unGID;
		}
		
		pString.m_fEndX = fPenX + pString.m_fX;
		pString.m_fEndY = fPenY + pString.m_fY;

		if (this.m_bIsNeedUpdateMatrix12)
		{
			if (this.m_pDefaultFont)
				this.m_pDefaultFont.UpdateMatrix2();
			this.UpdateMatrix2();
		}
	}
	
	this.SetCMapForCharCode = function(lUnicode, pnCMapIndex)
	{
		if (this.m_bStringGID ||  !this.m_pFace || 0 == this.m_nNum_charmaps )
			return lUnicode;

		var nCharIndex = 0;

		var charMapArray = HEAP[HEAP[this.m_pFace] + 40];
		for (var nIndex = 0; nIndex < this.m_nNum_charmaps; ++nIndex)
		{
			var pCharMap = charMapArray + nIndex*4;

			if (_FT_Set_Charmap(HEAP[this.m_pFace], HEAP[pCharMap]))
				continue;

			var pEncoding = HEAP[HEAP[pCharMap] + 4];

			if (FT_ENCODING_UNICODE == pEncoding)
			{
				if (nCharIndex = _FT_Get_Char_Index(HEAP[this.m_pFace], lUnicode))
				{
					//*pnCMapIndex = nIndex;
					return nCharIndex;
				}
			}
			else if (FT_ENCODING_NONE == pEncoding || FT_ENCODING_MS_SYMBOL == pEncoding || FT_ENCODING_APPLE_ROMAN == pEncoding)
			{
				var charcode;
                var gindex = STACKTOP;
                STACKTOP+=4;
                Runtime.memset(gindex,0,4);

				charcode = _FT_Get_First_Char (HEAP[this.m_pFace], gindex);
				while (HEAP[gindex] != 0)
				{                                                               
					charcode = _FT_Get_Next_Char (HEAP[this.m_pFace], charcode, gindex);
					if (charcode == lUnicode)
					{
						nCharIndex = HEAP[gindex];
						//*pnCMapIndex = nIndex;
						break;
					}
				}                                                               

				if (nCharIndex = _FT_Get_Char_Index (HEAP[this.m_pFace], lUnicode ))
				{
					//*pnCMapIndex = nIndex;
				}

                STACKTOP = gindex;
			}
		}

		return nCharIndex;
	}

    this.GetChar = function(lUnicode)
    {
        var pFace = this.m_pFace;
        var pCurentGliph = this.m_pGlyph;
        
        var Result;
        var ushUnicode = lUnicode;

        // Сначала мы все рассчитываем исходя только из матрицы шрифта FontMatrix
        if (this.m_bIsNeedUpdateMatrix12)
		{
			if (this.m_pDefaultFont)
				this.m_pDefaultFont.UpdateMatrix1();
			this.UpdateMatrix1();
		}

        var unGID = 0;
        var charSymbolObj = this.m_arrCacheSizes[ushUnicode];
        if (undefined == charSymbolObj)
        {
            var nCMapIndex = 0;
            unGID = this.SetCMapForCharCode(ushUnicode, nCMapIndex);

            var oSizes = new TFontCacheSizes();
            oSizes.ushUnicode = ushUnicode;

            if (!((unGID > 0) || (-1 != this.m_nSymbolic && (ushUnicode < 0xF000)  && 0 < (unGID = this.SetCMapForCharCode (ushUnicode + 0xF000, nCMapIndex)))))
            {
                // Пробуем загрузить через стандартный шрифт
                if (false == this.m_bUseDefaultFont || null == this.m_pDefaultFont || 0 >= (unGID = this.m_pDefaultFont.SetCMapForCharCode (ushUnicode, nCMapIndex)))
                {
                    if (this.m_nDefaultChar < 0)
                    {
                        oSizes.ushGID    = -1;
                        oSizes.eState    = EGlyphState.glyphstateMiss;
                        var max_advance = HEAP[HEAP[HEAP[this.m_pFace]+88]+12+24];
                        oSizes.fAdvanceX = (max_advance >> 6) / 2.0;

                        return;
                    }
                    else
                    {
                        unGID = this.m_nDefaultChar;
                        oSizes.eState = EGlyphState.glyphstateNormal;

                        pFace = this.m_pDefaultFont.m_pFace;
                        pCurentGliph = this.m_pDefaultFont.m_pGlyph;
                    }
                }
                else
                {
                    oSizes.eState = EGlyphState.glyphstateDeafault;

                    pFace = this.m_pDefaultFont.m_pFace;
                    pCurentGliph = this.m_pDefaultFont.m_pGlyph;
                }
            }
            else
            {
                oSizes.eState = EGlyphState.glyphstateNormal;
            }

            oSizes.ushGID     = unGID;
            oSizes.nCMapIndex = nCMapIndex;

            if (_FT_Load_Glyph (HEAP[pFace], unGID, LOAD_MODE))
            {
                return;
            }

            var pGlyph = STACKTOP;
            STACKTOP+=4;
            Runtime.memset(pGlyph,0,4);

            if (_FT_Get_Glyph (pCurentGliph, pGlyph))
            {
                return;
            }

            var oBBox = STACKTOP;
            STACKTOP+=16;
            Runtime.memset(oBBox,0,16);

            _FT_Glyph_Get_CBox (HEAP[pGlyph], ft_glyph_bbox_gridfit, oBBox);
            var xMin = HEAP[oBBox];
            var yMin = HEAP[oBBox+4];
            var xMax = HEAP[oBBox+8];
            var yMax = HEAP[oBBox+12];
            _FT_Done_Glyph (HEAP[pGlyph]);
            STACKTOP = pGlyph;

            var linearHoriAdvance = HEAP[pCurentGliph+56];
            var units_per_EM = HEAP[HEAP[pFace] + 68];

            oSizes.fAdvanceX = (linearHoriAdvance * this.m_dUnitsKoef / units_per_EM);
            oSizes.oBBox.fMinX = (xMin >> 6);
            oSizes.oBBox.fMaxX = (xMax >> 6);
            oSizes.oBBox.fMinY = (yMin >> 6);
            oSizes.oBBox.fMaxY = (yMax >> 6);

            oSizes.oMetrics.fWidth        = (HEAP[pCurentGliph+24] >> 6);
            oSizes.oMetrics.fHeight       = (HEAP[pCurentGliph+28] >> 6);
            oSizes.oMetrics.fHoriBearingX = (HEAP[pCurentGliph+32] >> 6);
            oSizes.oMetrics.fHoriBearingY = (HEAP[pCurentGliph+36] >> 6);
            oSizes.oMetrics.fHoriAdvance  = (HEAP[pCurentGliph+40] >> 6);
            oSizes.oMetrics.fVertBearingX = (HEAP[pCurentGliph+44] >> 6);
            oSizes.oMetrics.fVertBearingY = (HEAP[pCurentGliph+48] >> 6);
            oSizes.oMetrics.fVertAdvance  = (HEAP[pCurentGliph+52] >> 6);

            oSizes.bBitmap = false;
            
			/*
			oSizes.oBitmap.nX        = 0;
            oSizes.oBitmap.nY        = 0;
            oSizes.oBitmap.nHeight   = 0;
            oSizes.oBitmap.nWidth    = 0;
            oSizes.oBitmap.bFreeData = false;
            oSizes.oBitmap.pData     = null;
            oSizes.oBitmap.bAA       = false;
			*/

            this.m_arrCacheSizes[oSizes.ushUnicode] = oSizes;
            Result = oSizes;
        }
        else
        {
            if (EGlyphState.glyphstateMiss == charSymbolObj.eState)
                return;
				
				var nCMapIndex     = charSymbolObj.nCMapIndex;

            if (0 != this.m_nNum_charmaps)
            {
				var nCMapIndex     = charSymbolObj.nCMapIndex;
                var nCharmap = HEAP[HEAP[pFace]+92];
                var nCurCMapIndex = _FT_Get_Charmap_Index (nCharmap);
                if (nCurCMapIndex != nCMapIndex)
                {
                    nCMapIndex = Math.max (0, nCMapIndex);
                    var pCharMapArray = HEAP[HEAP[pFace] + 40]+ nCMapIndex*4;
                    _FT_Set_Charmap (HEAP[pFace], HEAP[pCharMapArray]);
                }
            }

            /*charSymbolObj.bBitmap           = false;
            charSymbolObj.oBitmap.nX        = 0;
            charSymbolObj.oBitmap.nY        = 0;
            charSymbolObj.oBitmap.nHeight   = 0;
            charSymbolObj.oBitmap.nWidth    = 0;
            charSymbolObj.oBitmap.bFreeData = false;
            charSymbolObj.oBitmap.pData     = null;
            charSymbolObj.oBitmap.bAA       = false;*/

            Result = charSymbolObj;
        }

        if (this.m_bIsNeedUpdateMatrix12)
		{
			if (this.m_pDefaultFont)
				this.m_pDefaultFont.UpdateMatrix2();
			this.UpdateMatrix2();
		}
        return Result;
    }

    this.GetChar2 = function(lUnicode)
    {
        var pFace = this.m_pFace;
        var pCurentGliph = this.m_pGlyph;
        
        var Result;

        var ushUnicode = lUnicode;

        // Сначала мы все рассчитываем исходя только из матрицы шрифта FontMatrix
        if (this.m_bIsNeedUpdateMatrix12)
		{
			if (this.m_pDefaultFont)
				this.m_pDefaultFont.UpdateMatrix1();
			this.UpdateMatrix1();
		}

        var unGID = 0;
        var charSymbolObj = this.m_arrCacheSizes[ushUnicode];
        if (undefined == charSymbolObj || null == charSymbolObj.oBitmap.pData)
        {
            var nCMapIndex = 0;
            unGID = this.SetCMapForCharCode(ushUnicode, nCMapIndex);

            var oSizes = new TFontCacheSizes();
            oSizes.ushUnicode = ushUnicode;

            if (!((unGID > 0) || (-1 != this.m_nSymbolic && (ushUnicode < 0xF000)  && 0 < (unGID = this.SetCMapForCharCode (ushUnicode + 0xF000, nCMapIndex)))))
            {
                // Пробуем загрузить через стандартный шрифт
                if (false == this.m_bUseDefaultFont || null == this.m_pDefaultFont || 0 >= (unGID = this.m_pDefaultFont.SetCMapForCharCode (ushUnicode, nCMapIndex)))
                {
                    if (this.m_nDefaultChar < 0)
                    {
                        oSizes.ushGID    = -1;
                        oSizes.eState    = EGlyphState.glyphstateMiss;
                        var max_advance = HEAP[HEAP[HEAP[this.m_pFace]+88]+12+24];
                        oSizes.fAdvanceX = (max_advance >> 6) / 2.0;

                        return;
                    }
                    else
                    {
                        unGID = this.m_nDefaultChar;
                        oSizes.eState = EGlyphState.glyphstateNormal;

                        pFace = this.m_pDefaultFont.m_pFace;
                        pCurentGliph = this.m_pDefaultFont.m_pGlyph;
                    }
                }
                else
                {
                    oSizes.eState = EGlyphState.glyphstateDeafault;

                    pFace = this.m_pDefaultFont.m_pFace;
                    pCurentGliph = this.m_pDefaultFont.m_pGlyph;
                }
            }
            else
            {
                oSizes.eState = EGlyphState.glyphstateNormal;
            }

            oSizes.ushGID     = unGID;
            oSizes.nCMapIndex = nCMapIndex;

            if (_FT_Load_Glyph (HEAP[pFace], unGID, LOAD_MODE))
            {
                return;
            }

            var pGlyph = STACKTOP;
            STACKTOP+=4;
            Runtime.memset(pGlyph,0,4);

            if (_FT_Get_Glyph (pCurentGliph, pGlyph))
            {
                return;
            }

            var oBBox = STACKTOP;
            STACKTOP+=16;
            Runtime.memset(oBBox,0,16);

            _FT_Glyph_Get_CBox (HEAP[pGlyph], ft_glyph_bbox_gridfit, oBBox);
            var xMin = HEAP[oBBox];
            var yMin = HEAP[oBBox+4];
            var xMax = HEAP[oBBox+8];
            var yMax = HEAP[oBBox+12];
            _FT_Done_Glyph (HEAP[pGlyph]);
            STACKTOP = pGlyph;

            var linearHoriAdvance = HEAP[pCurentGliph+56];
            var units_per_EM = HEAP[HEAP[pFace] + 68];

            oSizes.fAdvanceX = (linearHoriAdvance * this.m_dUnitsKoef / units_per_EM);
            oSizes.oBBox.fMinX = (xMin >> 6);
            oSizes.oBBox.fMaxX = (xMax >> 6);
            oSizes.oBBox.fMinY = (yMin >> 6);
            oSizes.oBBox.fMaxY = (yMax >> 6);

            oSizes.oMetrics.fWidth        = (HEAP[pCurentGliph+24] >> 6);
            oSizes.oMetrics.fHeight       = (HEAP[pCurentGliph+28] >> 6);
            oSizes.oMetrics.fHoriBearingX = (HEAP[pCurentGliph+32] >> 6);
            oSizes.oMetrics.fHoriBearingY = (HEAP[pCurentGliph+36] >> 6);
            oSizes.oMetrics.fHoriAdvance  = (HEAP[pCurentGliph+40] >> 6);
            oSizes.oMetrics.fVertBearingX = (HEAP[pCurentGliph+44] >> 6);
            oSizes.oMetrics.fVertBearingY = (HEAP[pCurentGliph+48] >> 6);
            oSizes.oMetrics.fVertAdvance  = (HEAP[pCurentGliph+52] >> 6);

            oSizes.oBitmap = new TGlyphBitmap();
            oSizes.bBitmap  = true;
            if (_FT_Render_Glyph(pCurentGliph, REND_MODE))
                return;

            var pBitmap = pCurentGliph+76;
            oSizes.oBitmap.nX      = HEAP[pCurentGliph+100];
            oSizes.oBitmap.nY      = HEAP[pCurentGliph+104];
            oSizes.oBitmap.nWidth  = HEAP[pBitmap+4];
            oSizes.oBitmap.nHeight = HEAP[pBitmap];
            oSizes.oBitmap.bAA     = this.m_bAntiAliasing;

            var nRowSize = 0;
            if (this.m_bAntiAliasing)
            {
                if (this.m_bNeedDoBold)
                    oSizes.oBitmap.nWidth++;

                nRowSize = oSizes.oBitmap.nWidth;
            }
            else
            {
                nRowSize = (oSizes.oBitmap.nWidth + 7) >> 3;
            }

			var nSizeBuffer = nRowSize * oSizes.oBitmap.nHeight;
            oSizes.oBitmap.pData = new Array(nSizeBuffer);
            oSizes.oBitmap.bFreeData = false; // Все удаляется в кэше (во время очистки или замены)

            var pCharBuffer = HEAP[pBitmap+12];
            if (!this.m_bNeedDoBold || !this.m_bAntiAliasing)
            {
                for (var nIndex = 0; nIndex < nSizeBuffer; ++nIndex)
                    oSizes.oBitmap.pData[nIndex] = HEAP[pCharBuffer+nIndex];
            }
            else
            {
                var nY, nX;
                var pDstBuffer;
                var pSrcBuffer;
                var nPitch = HEAP[pBitmap+8];
                for (nY = 0, pDstBuffer = 0, pSrcBuffer = pCharBuffer; nY < oSizes.oBitmap.nHeight; ++nY, pDstBuffer += nRowSize, pSrcBuffer += nPitch)
                {
                    for (nX = pCurGlyph.oBitmap.nWidth - 1; nX >= 0; --nX)
                    {
                        if (0 != nX)
                        {
                            var nFirstByte, nSecondByte;

                            if (pCurGlyph.oBitmap.nWidth - 1 == nX)
                                nFirstByte = 0;
                            else
                                nFirstByte = HEAP[pSrcBuffer + nX];

                            nSecondByte = HEAP[pSrcBuffer + nX - 1];

                            oSizes.oBitmap.pData[pDstBuffer + nX] = Math.min(255, nFirstByte + nSecondByte);
                        }
                        else
                        {
                            oSizes.oBitmap.pData[pDstBuffer + nX] = HEAP[pSrcBuffer + nX];
                        }
                    }
                }
            }

            this.m_arrCacheSizes[oSizes.ushUnicode] = oSizes;
            Result = oSizes;
        }
        else
        {
            var nCMapIndex     = charSymbolObj.nCMapIndex;
            unGID              = charSymbolObj.ushGID;
            var eState = charSymbolObj.eState;

            if (EGlyphState.glyphstateMiss == eState)
            {
                return;
            }
            else if (EGlyphState.glyphstateDeafault == eState)
            {
                //pFace = pDefFace;
            }
            else
            {
                //pFace = pSrcFace;
            }

            if (0 != this.m_nNum_charmaps)
            {
                var nCharmap = HEAP[HEAP[pFace]+92];
                var nCurCMapIndex = _FT_Get_Charmap_Index (nCharmap);
                if (nCurCMapIndex != nCMapIndex)
                {
                    nCMapIndex = Math.max (0, nCMapIndex);
                    var pCharMapArray = HEAP[HEAP[pFace] + 40]+ nCMapIndex*4;
                    _FT_Set_Charmap (HEAP[pFace], HEAP[pCharMapArray]);
                }
            }

            Result = charSymbolObj;
        }

		if (this.m_bIsNeedUpdateMatrix12)
		{
			if (this.m_pDefaultFont)
				this.m_pDefaultFont.UpdateMatrix2();
			this.UpdateMatrix2();
		}
		return Result;
    }

    this.GetKerning = function(unPrevGID, unGID)
    {
        var pDelta = STACKTOP;
        Runtime.memset(pDelta,0,4);
        _FT_Get_Kerning (HEAP[this.m_pFace], unPrevGID, unGID, FT_Kerning_Mode.FT_KERNING_DEFAULT, pDelta);
        var oDelta = (HEAP[pDelta] >> 6);
        STACKTOP = pDelta;
        return oDelta;
    }

    this.SetStringGID = function(bGID)
    {
        if (this.m_bStringGID == bGID)
            return;

        //this.ClearCache();
        this.m_bStringGID = bGID;
    }
    this.GetStringGID = function()
    {
        return this.m_bStringGID;
    }
    this.SetUseDefaultFont = function (bUse)
    {
        this.m_bUseDefaultFont = bUse;
    }
    this.GetUseDefaultFont = function()
    {
        return this.m_bUseDefaultFont;
    }
    this.SetCharSpacing = function (fCharSpacing)
    {
        this.m_fCharSpacing = fCharSpacing;
    }
    this.GetCharSpacing = function()
    {
        return this.m_fCharSpacing;
    }

    this.GetStyleName = function()
    {
        var sResult = "";
        var sName = HEAP[HEAP[this.m_pFace] + 24];
        var nLength = _strlen(sName);
        for (var nIndex = 0; nIndex < nLength; ++nIndex, ++sName)
            sResult += String.fromCharCode (HEAP[sName]);

        return sResult;
    }

    this.UpdateStyles = function(bBold, bItalic)
    {
        var sStyle = this.GetStyleName();

        // Смотрим какой стиль у исходного шрифта
        var bSrcBold   = (-1 != sStyle.indexOf("Bold"));
        var bSrcItalic = (-1 != sStyle.indexOf("Italic"));

        if (!bBold) // Нам нужен не жирный шрифт
        {
            this.m_bNeedDoBold = false;
        }
        else if (bBold) // Нам нужно сделать шрифт жирным
        {
            if (bSrcBold)
            {
                // Исходный шрифт уже жирный, поэтому ничего дополнительного делать не надо
                this.m_bNeedDoBold = false;
            }
            else
            {
                // Иходный шрифт не жирный, поэтому жирность делаем сами
                this.m_bNeedDoBold = true;
            }
        }

        if (!bItalic) // Нам нужен не наклонный шрифт
        {
            this.SetItalic (false);
        }
        else if (bItalic) // Нам нужно сделать наклонный шрифт
        {
            if (bSrcItalic)
            {
                // Исходный шрифт уже наклонный, поэтому ничего дополнительного делать не надо
                this.SetItalic (false);
            }
            else
            {
                // Иходный шрифт не наклонный, поэтому делаем его наклонным сами
                this.SetItalic (true);
            }
        }

    }

    this.SetItalic = function(value)
    {
        this.m_bNeedDoItalic = value;
        this.ResetFontMatrix();
    }

    this.ReleaseFontFace = function()
    {
        if (null != this.m_pFace)
        {
            _FT_Done_Face(HEAP[this.m_pFace]);
            STACKTOP = this.m_pFace;
        }
    }

	this.IsSuccess = function()
	{
		return (0 == this.m_nError);
	}
};