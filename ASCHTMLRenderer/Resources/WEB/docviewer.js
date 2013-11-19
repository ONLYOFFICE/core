var ZoomPercent = 100;var nPageNumber = 1;var nPageCount = 1;var nFirstSendZoom = 0;
var arrZoom = [8,12,25,33,50,66,75,100,125,150,200,300,400,500];
var arrZoomLength = 14;
var curZoomPosition = 7;
$(document).ready(function(){
	curZoomPosition = 7;
	document.getElementById('zoom').value = curZoomPosition ;
	document.getElementById('zoomValue').value = ZoomPercent;
	document.getElementById('pageNum').value = nPageNumber;
	$(".selectableIcon").bind("mouseenter", function(){$(this).addClass("ToolbarIconOutSelect");});
	$(".selectableIcon").bind("mouseleave", function(){$(this).removeClass("ToolbarIconOutSelect");});
	$(".selectableIcon").bind("mousedown", function(){$(this).addClass("iconPressed");});
	$(".selectableIcon").bind("mouseup", function(){$(this).removeClass("iconPressed");});

	$("#zoomMenu").clickMenu({onClick:function(){
			switch(this.id){
				case "8": curZoomPosition = 0; break;
				case "12": curZoomPosition = 1; break;
				case "25": curZoomPosition = 2; break;
				case "33": curZoomPosition = 3; break;
				case "50": curZoomPosition = 4; break;
				case "66": curZoomPosition = 5; break;
				case "75": curZoomPosition = 6; break;
				case "100": curZoomPosition = 7; break;
				case "125": curZoomPosition = 8; break;
				case "150": curZoomPosition = 9; break;
				case "200": curZoomPosition = 10; break;
				case "300": curZoomPosition = 11; break;
				case "400": curZoomPosition = 12; break;
				case "500": curZoomPosition = 13; break;
			}
			setZoomFromCurPos(curZoomPosition);
			$('#zoomMenu').trigger('closemenu');
			$('#zoomValuePercent').text(this.innerHTML);
			$('#zoomValuePercent').val(this.id);
			return false;
		}
	});
	$("#content").height($(window).height()-$("#menu").height()-16)
	$("#menu").click(function(){closeMenu()});
});
function closeMenu(){
	$('#zoomMenu').trigger('closemenu');
}
function setZoom(dZoom) {
	ZoomPercent = dZoom;
	var element = document.getElementById('zoomValue');
	if (element != null){
		element.value = ZoomPercent;
		if (0 != nFirstSendZoom){
			sendZoomToControl(ZoomPercent);
		}
		nFirstSendZoom = 1;
	}
}
function OnPagePrev(){
	if (nPageNumber > 1){
		var pageNum = nPageNumber - 1;
		sendPageNumberToMenu(pageNum);
	}
}
function OnPageNext(){
	if (nPageNumber < nPageCount){
		var pageNum = nPageNumber + 1;
		sendPageNumberToMenu(pageNum);
	}
}
function OnChangePageNum(){
	var element = document.getElementById('pageNum');
	if (element != null){
		var number = parseInt(element.value);
		if (1 <= number && number <= nPageCount){
			nPageNumber = number;
			sendPageNumberToControl(nPageNumber);
		}
	}
}
function SetPagesCount(pageCount){
	nPageCount = pageCount;
	$("#pageCounts").text('/ '+nPageCount);
}
function OnChangeZoomNum(){
	var element = document.getElementById('zoomValue');
	if (element != null){
		var number = parseInt(element.value);
		if (8 <= number && number <= 500){
			ZoomPercent = number;
			sendZoomToControl(ZoomPercent);
		}
	}
}
// send to viewer -------------------------------------------------------------------------------------
function sendZoomToControl(zoomValue){
	if (top != null){
		var _frame = parent.frames['viewer'];
		if (_frame != null)
		{
			var part = zoomValue / 100.0;
			_frame.OnZoom(part);
		}
	}
}
function sendPageNumberToControl(pageNumber){
	if (top != null){
		var _frame = top.frames['viewer'];
		if (_frame != null){
			_frame.OnChangePage(pageNumber);
		}
	}
}
//---------------------------------------------------------------------------------------------------------
// send to menu (according to viewer) -----------------------------------------------------------
function sendZoomToMenu(zoomValue){
	if ((zoomValue != ZoomPercent) && (8 <= zoomValue) && (zoomValue <= 500)){
		ZoomPercent = zoomValue;
		document.getElementById('zoomValue').value = ZoomPercent;
		sendZoomToControl(ZoomPercent);
	}
}
function sendPageNumberToMenu(pageNumber){
	if ((nPageNumber != pageNumber) && (1 <= pageNumber) && (pageNumber <= nPageCount)){
		nPageNumber = pageNumber;
		document.getElementById('pageNum').value = nPageNumber;
		sendPageNumberToControl(nPageNumber);
	}
}
function funZoomIn(){
	if(curZoomPosition < arrZoomLength - 1){
		curZoomPosition++;
		setZoomFromCurPos(curZoomPosition);
		setZoomMenuValue(curZoomPosition)
	}
};
function funZoomOut(){
	if(curZoomPosition > 0 ){
		curZoomPosition--;
		setZoomFromCurPos(curZoomPosition);
		setZoomMenuValue(curZoomPosition)
	}
};
function setZoomFromCurPos(curPos){
		nFirstSendZoom = 1;
		setZoom(arrZoom[curPos]);
		document.getElementById('zoomValue').value = arrZoom[curPos];
}
function setZoomMenuValue(curPos){
	$('#zoomValuePercent').text(arrZoom[curPos]+'%');
	$('#zoomValuePercent').val(arrZoom[curPos]);
	// document.getElementById('zoom').value = curPos;//for classic html select element.
}
function changeSelPosition(){
	// curZoomPosition = document.getElementById('zoom').value; //for classic html select element
	setZoomFromCurPos(curZoomPosition);
}
