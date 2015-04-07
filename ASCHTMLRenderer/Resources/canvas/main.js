//----------------------------------------------------------------------------------------------------------------------------------------------------------
var koef_mm_to_pix = 96 / 25.4;
var scrollPositionX = 0;
var scrollPositionY = 0;
var documentWidth	= 0;
var documentHeight	= 0;
var arrayPages;
var pagesCount = 0;

var bIsMobile =  /android|avantgo|blackberry|blazer|compal|elaine|fennec|hiptop|iemobile|ip(hone|od|ad)|iris|kindle|lge |maemo|midp|mmp|opera m(ob|in)i|palm( os)?|phone|p(ixi|re)\/|plucker|pocket|psp|symbian|treo|up\.(browser|link)|vodafone|wap|windows (ce|phone)|xda|xiino/i.test(navigator.userAgent || navigator.vendor || window.opera);

//----------------------------------------------------------------------------------------------------------------------------------------------------------
function getBodyWidth()
{
var result = 0;
if (this.innerWidth)  
	result = this.innerWidth;  
else if (document.documentElement && document.documentElement.clientWidth)  
	result = document.documentElement.clientWidth;  
else if (document.body)  
	result = document.body.clientWidth; 
return result;
}
function getBodyHeight()
{
var result = 0;
if (this.innerHeight)  
	result = this.innerHeight;  
else if (document.documentElement && document.documentElement.clientHeight)  
	result = document.documentElement.clientHeight;  
else if (document.body)  
	result = document.body.clientHeight; 
return result;
}
function getScrollTop()
{
var result = 0;
var bIsIE = /*@cc_on ! @*/ false;
if (bIsIE)
	return document.documentElement.scrollTop;
return this.scrollY;
}
function getScrollLeft()
{
var result = 0;
var bIsIE = /*@cc_on ! @*/ false;
if (bIsIE)
	return document.documentElement.scrollLeft;
return this.scrollX;
}
function getScrollMaxY(_size)
{
var result = 0;
var bIsIE = /*@cc_on ! @*/ false;
if (_size >= documentHeight)
	return 1;
return documentHeight - _size;
}
function getScrollMaxX(_size)
{
var result = 0;
var bIsIE = /*@cc_on ! @*/ false;
if (_size >= documentWidth)
	return 1;
return documentWidth - _size;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------
function StartViewer()
{
InitDocument();
OnResize();
    
window.onscroll = OnScroll;
window.onresize = OnResize;
OnPaint();
}

function OnScroll()
{
scrollPositionY = getScrollTop();
scrollPositionX = getScrollLeft();

if (bIsMobile)
{
var canvas = document.getElementById("wind");
if (null == canvas)
    return;
	
canvas.style.top = scrollPositionY + "px";
}

OnPaint();
}

function OnTimerRepaint()
{
if (documentRenderer.checkRepaint() == 1)
	OnPaint();
}

function OnResize()
{
var main_div = document.getElementById("main_div");
main_div.style.height = documentHeight + "px";
if (documentWidth > document.body.clientWidth)
{
main_div.style.width = documentWidth + "px";
}

var canvas = document.getElementById("wind");
if (null == canvas)
    return;

canvas.width = document.body.clientWidth;
canvas.height = getBodyHeight();
var _width  = canvas.width;
var _height = canvas.height;

//OnPaint();
}

function InitDocument()
{
pagesCount = documentRenderer.pagesCount;
arrayPages = new Array(pagesCount);
InitDocumentPages(arrayPages);
CalculateDocumentSize();
setInterval(OnTimerRepaint, 5);
}

function CalculateDocumentSize()
{
documentWidth = 0;
documentHeight = 0;

for (var i = 0; i < pagesCount; ++i)
{
if (arrayPages[i].width_pix > documentWidth)
	documentWidth = arrayPages[i].width_pix;

documentHeight += 20;
documentHeight += arrayPages[i].height_pix;
}

documentHeight += 20;

scrollPositionX = 0;
scrollPositionY = 0;
}

function OnPaint()
{
var canvas = document.getElementById("wind");
if (null == canvas)
    return;
	
var context = canvas.getContext("2d");
var _width = canvas.width;
var _height = canvas.height;

context.fillStyle = "#eef0f2";
context.fillRect(0, 0, _width, _height);

var hor_pos_median = parseInt(_width / 2);
if (0 != scrollPositionX)
{
	var part = scrollPositionX / getScrollMaxX(_width);
	hor_pos_median = parseInt(documentWidth / 2 + part * (_width - documentWidth));
}

var documentMaxY = documentHeight - _height;
if (documentMaxY < 0)
	documentMaxY = 0;

var lCurrentTopInDoc = parseInt(scrollPositionY * documentMaxY / getScrollMaxY(_height));
var lCurrentBottomInDoc = lCurrentTopInDoc + _height;

var lStart = 0;
for (var i = 0; i < pagesCount; ++i)
{
	if (lStart > lCurrentBottomInDoc)
    {   
        arrayPages[i].stopRenderingAttack();
		continue;
		//break;
    }

	var _pageWidth = arrayPages[i].width_pix;
	var _pageHeight = arrayPages[i].height_pix;
	
	if (lStart + 20 + _pageHeight < lCurrentTopInDoc)
	{
		lStart += (20 + _pageHeight);
		arrayPages[i].stopRenderingAttack();
		continue;
	}
		
	var xDst = hor_pos_median - parseInt(_pageWidth / 2);
	var wDst = _pageWidth;
	var yDst = lStart + 20 - lCurrentTopInDoc;
	var hDst = _pageHeight;
	
	arrayPages[i].Draw(context, xDst, yDst, wDst, hDst);
		
	lStart += (20 + _pageHeight);
}
}


