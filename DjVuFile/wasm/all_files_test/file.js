(function(window, undefined) {

    function CDocMetaSelection()
    {
        this.Page1 = 0;
        this.Line1 = 0;
        this.Glyph1 = 0;

        this.Page2 = 0;
        this.Line2 = 0;
        this.Glyph2 = 0;

        this.IsSelection = false;
    }

    function CFile()
    {
    	this.nativeFile = 0;
    	this.pages = [];
    	this.zoom = 1;
    	this.isUse3d = false;
    	this.cacheManager = null;
    	this.logging = true;
    	this.Selection = new CDocMetaSelection();
    }

    // interface
    CFile.prototype.loadFromData = function(data) {};
    CFile.prototype.load = function(arrayBuffer)
    {
        return this.loadFromData(new Uint8Array(arrayBuffer));
    };

    CFile.prototype.getInfo = function() {};
    CFile.prototype.getPagePixmap = function(pageIndex, width, height) {};
    CFile.prototype.close = function() {};
    CFile.prototype.memory = function() {};
    CFile.prototype.free = function(pointer) {};

	CFile.prototype.getPage = function(pageIndex, width, height)
	{
		if (pageIndex < 0 || pageIndex >= this.pages.length)
			return null;
		if (!width) width = this.pages[pageIndex].W;
		if (!height) height = this.pages[pageIndex].H;
		var t0 = performance.now();
		var pixels = this.getPagePixmap(pageIndex, width, height);
		if (!pixels)
			return null;
		
		if (!this.logging)
		{
			var image = this._pixelsToCanvas(pixels, width, height);
		}
		else
		{
			var t1 = performance.now();
			var image = this._pixelsToCanvas(pixels, width, height);
			var t2 = performance.now();
			//console.log("time: " + (t1 - t0) + ", " + (t2 - t1));
		}
        /*
        if (this.pages[pageIndex].Lines)
        {
            var ctx = image.getContext("2d");
            for (let i = 0; i < this.pages[pageIndex].Lines.length; i++)
            {
                for (let j = 0; j < this.pages[pageIndex].Lines[i].Glyphs.length; j++)
                {
                    let glyph = this.pages[pageIndex].Lines[i].Glyphs[j];
                    ctx.font = glyph.fontSize + 'px ' + glyph.fontName;
                    ctx.fillText(glyph.UChar, glyph.X, glyph.Y);
                }
            }
        }
        */
        this.free(pixels);
        return image;
	};

    CFile.prototype.GetNearestPos = function(pageIndex, x, y)
    {
        var _line = -1;
        var _glyph = -1;
        var minDist = Number.MAX_SAFE_INTEGER;

        if (!this.pages[pageIndex].Lines)
            return { Line : _line, Glyph : _glyph };

        for (let i = 0; i < this.pages[pageIndex].Lines.length; i++)
        {
            let Y = this.pages[pageIndex].Lines[i].Glyphs[0].Y;
            if (Math.abs(Y - y) < minDist)
            {
                minDist = Math.abs(Y - y);
                _line   = i;
            }
        }
        minDist = Number.MAX_SAFE_INTEGER;
        for (let j = 0; j < this.pages[pageIndex].Lines[_line].Glyphs.length; j++)
        {
            let X = this.pages[pageIndex].Lines[_line].Glyphs[j].X;
            if (Math.abs(X - x) < minDist)
            {
                minDist = Math.abs(X - x);
                _glyph  = j;
            }
        }

        return { Line : _line, Glyph : _glyph };
    }

    CFile.prototype.OnUpdateSelection = function(Selection, width, height)
    {
        if (!this.Selection.IsSelection)
            return;

        var sel = this.Selection;
        var page1  = sel.Page1;
        var page2  = sel.Page2;
        var line1  = sel.Line1;
        var line2  = sel.Line2;
        var glyph1 = sel.Glyph1;
        var glyph2 = sel.Glyph2;
        if (page1 == page2 && line1 == line2 && glyph1 == glyph2)
        {
            Selection.IsSelection = false;
            return;
        }
        else if (page1 == page2 && line1 == line2)
        {
            glyph1 = Math.min(sel.Glyph1, sel.Glyph2);
            glyph2 = Math.max(sel.Glyph1, sel.Glyph2);
        }
        else if (page1 == page2)
        {
            if (line1 > line2)
            {
                line1  = sel.Line2;
                line2  = sel.Line1;
                glyph1 = sel.Glyph2;
                glyph2 = sel.Glyph1;
            }
        }

        if (this.cacheManager)
        {
            this.cacheManager.unlock(Selection.Image);
            Selection.Image = this.cacheManager.lock(width, height);
        }
        else
        {
            delete Selection.Image;
            Selection.Image = document.createElement("canvas");
            Selection.Image.width = width;
            Selection.Image.height = height;
        }
        let ctx = Selection.Image.getContext("2d");
        // TODO: После изменения размера экрана наложение областей выделения
        ctx.clearRect(0, 0, Selection.Image.width, Selection.Image.height);
        ctx.globalAlpha = 0.4;
        ctx.fillStyle = "#7070FF";

        for (let page = page1; page <= page2; page++)
        {
            let start = 0;
            let end   = this.pages[page].Lines.length;
            if (page == page1)
                start = line1;
            if (page == page2)
                end = line2;
            for (let line = start; line <= end; line++)
            {
                let Glyphs = this.pages[page].Lines[line].Glyphs;
                let x = Glyphs[0].X;
                let y = Glyphs[0].Y - Glyphs[0].fontSize;
                // первая строка на первой странице
                if (page == page1 && line == start)
                    x = Glyphs[glyph1].X;
                let w = Glyphs[Glyphs.length - 1].X - x;
                let h = Glyphs[0].fontSize;
                // последняя строка на последней странице
                if (page == page2 && line == end)
                    w = Glyphs[glyph2].X - x;

                ctx.fillRect(x, y, w, h);
            }
        }
        ctx.globalAlpha = 1;
        Selection.IsSelection = true;
    }

	CFile.prototype.OnMouseDown = function(pageIndex, Selection, x, y, w, h)
    {
        var ret = this.GetNearestPos(pageIndex, x, y);

        var sel = this.Selection;
        sel.Page1  = pageIndex;
        sel.Line1  = ret.Line;
        sel.Glyph1 = ret.Glyph;

        sel.Page2  = pageIndex;
        sel.Line2  = ret.Line;
        sel.Glyph2 = ret.Glyph;

        sel.IsSelection = true;
        this.OnUpdateSelection(Selection, w, h);
    }

    CFile.prototype.OnMouseMove = function(pageIndex, Selection, x, y, w, h)
    {
        if (false === this.Selection.IsSelection)
            return;
        var ret = this.GetNearestPos(pageIndex, x, y);

        var sel = this.Selection;
        sel.Page2  = pageIndex;
        sel.Line2  = ret.Line;
        sel.Glyph2 = ret.Glyph;

        this.OnUpdateSelection(Selection, w, h);
    }

    CFile.prototype.OnMouseUp = function()
    {
        this.Selection.IsSelection = false;
    }

    CFile.prototype.getPageBase64 = function(pageIndex, width, height)
	{
		var _canvas = this.getPage(pageIndex, width, height);
		if (!_canvas)
			return "";
		
		try
		{
			return _canvas.toDataURL("image/png");
		}
		catch (err)
		{
		}
		
		return "";
	};

    window["AscViewer"] = window["AscViewer"] || {};
    window["AscViewer"].IFile = CFile;

    window["AscViewer"].createFile = function(buffer)
    {
        var data = new Uint8Array(buffer);
        var file = new window["AscViewer"].CDrawingFile();

        file.loadFromData(data);
        file.cacheManager = new window.CCacheManager();
        return file;
    };

})(window, undefined);
