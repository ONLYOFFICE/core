window.onload = function()
{
	var userAgent = navigator.userAgent.toLowerCase();
	// ie detect
	var isIE = (userAgent.indexOf("msie") > -1 || userAgent.indexOf("trident") > -1 || userAgent.indexOf("edge") > -1);
	// chrome detect
	var isChrome = !isIE && (userAgent.indexOf("chrome") > -1);
	// mobile detect
	var isMobile = /android|avantgo|blackberry|blazer|compal|elaine|fennec|hiptop|iemobile|ip(hone|od|ad)|iris|kindle|lge |maemo|midp|mmp|opera m(ob|in)i|palm( os)?|phone|p(ixi|re)\/|plucker|pocket|psp|symbian|treo|up\.(browser|link)|vodafone|wap|windows (ce|phone)|xda|xiino/i.test(navigator.userAgent || navigator.vendor || window.opera);
	var isOperaOld = (!!window.opera);

	var holder = document.body;
	holder.ondragover = function(e) 
	{
		var isFile = false;
		if (e.dataTransfer.types)
		{
			for (var i = 0, length = e.dataTransfer.types.length; i < length; ++i)
			{
				var type = e.dataTransfer.types[i].toLowerCase();
				if (type == "files" && e.dataTransfer.items && e.dataTransfer.items.length == 1)
				{
					var item = e.dataTransfer.items[0];
					if (item.kind && "file" == item.kind.toLowerCase())
					{
						isFile = true;
						break;
					}
				}
			}
		}
		e.dataTransfer.dropEffect = isFile ? "copy" : "none";
		e.preventDefault();
		return false; 
	};
	holder.ondrop = function(e) 
	{ 
		var file = e.dataTransfer.files ? e.dataTransfer.files[0] : null;
		if (!file)
		{
			e.preventDefault();
			return false;
		}
		
		var reader = new FileReader();
		reader.onload = function(e) {
			window.Viewer.open(e.target.result);
		};
		reader.readAsArrayBuffer(file);
	
		return false; 
	};

	function CCacheManager()
	{
		this.images = [];

		this.lock = function(w, h)
		{
			for (let i = 0; i < this.images.length; i++)
			{
				if (this.images[i].locked)
					continue;

				let canvas = this.images[i].canvas;
				let testW = canvas.width;
				let testH = canvas.height;
				if (w > testW || h > testH || ((4 * w * h) < (testW * testH)))
				{
					this.images.splice(i, 1);
					continue;	
				}

				this.images[i].locked = true;
				return canvas;
			}

			let newImage = { canvas : document.createElement("canvas"), locked : true };
			newImage.canvas.width = w + 100;
			newImage.canvas.height = h + 100;
			this.images.push(newImage);
			return newImage.canvas;
		};

		this.unlock = function(canvas)
		{
			for (let i = 0, len = this.images.length; i < len; i++)
			{
				if (this.images[i].canvas === canvas)
				{
					this.images[i].locked = false;
					return;
				}
			}
		};

		this.clear = function()
		{
			this.images = [];
		};
	};

	function CHtmlPage()
	{
		this.width 	= 0;
		this.height = 0;
		this.retinaPixelRatio = 1;
		this.zoom 	= 1;
		this.drawingPages = [];
		this.isRepaint = false;
		this.canvas   = document.getElementById("main");
		this.scroller = document.getElementById("pos");
		this.documentWidth  = 0;
		this.documentHeight = 0;
		this.Selection = { IsSelection : false, Image : null, page : -1 };

		this.file = new AscViewer.DjVuFile();

		/*
			[TIMER START]
		 */
		this.UseRequestAnimationFrame = isChrome;
		this.RequestAnimationFrame    = (function()
		{
			return window.requestAnimationFrame ||
				window.webkitRequestAnimationFrame ||
				window.mozRequestAnimationFrame ||
				window.oRequestAnimationFrame ||
				window.msRequestAnimationFrame || null;
		})();
		this.CancelAnimationFrame     = (function()
		{
			return window.cancelRequestAnimationFrame ||
				window.webkitCancelAnimationFrame ||
				window.webkitCancelRequestAnimationFrame ||
				window.mozCancelRequestAnimationFrame ||
				window.oCancelRequestAnimationFrame ||
				window.msCancelRequestAnimationFrame || null;
		})();
		if (this.UseRequestAnimationFrame)
		{
			if (null == this.RequestAnimationFrame)
				this.UseRequestAnimationFrame = false;
		}
		this.RequestAnimationOldTime = -1;

		this.startTimer = function()
		{
			if (this.UseRequestAnimationFrame)
				this.timerAnimation();
			else
				this.timer();
		};
		/*
			[TIMER END]
		*/

		this.log = function(message)
		{
			//console.log(message);
		};

		window.onscroll    = function(e) { if (window.Viewer) window.Viewer.scroll(e); };
		window.onmousedown = function(e) { if (window.Viewer) window.Viewer.OnMouse(e, true); };
		window.onmousemove = function(e) { if (window.Viewer) window.Viewer.OnMouse(e, false); };
		window.onmouseup   = function(e) { if (window.Viewer) window.Viewer.OnMouseUp(e); };

		this.timerAnimation = function()
		{
			var now = Date.now();
			if (-1 == window.Viewer.RequestAnimationOldTime || (now >= (window.Viewer.RequestAnimationOldTime + 40)) || (now < window.Viewer.RequestAnimationOldTime))
			{
				window.Viewer.RequestAnimationOldTime = now;
				window.Viewer.timer();
			}
			window.Viewer.RequestAnimationFrame.call(window, window.Viewer.timerAnimation);
		};

		this.timer = function()
		{
			if (window.Viewer.isRepaint)
			{
				window.Viewer._paint();
				window.Viewer.isRepaint = false;
			}

			if (!window.Viewer.UseRequestAnimationFrame)
			{
				setTimeout(window.Viewer.timer, 40);
			}
		};

		this.resize = function()
		{
			this.zoom = 1;
			this.retinaPixelRatio = 1;
		
			/*
			if (isChrome && !isOperaOld && !isMobile && document && document.firstElementChild && document.body)
			{
				// делаем простую проверку
				// считаем: 0 < window.devicePixelRatio < 2 => _devicePixelRatio = 1; zoom = window.devicePixelRatio / _devicePixelRatio;
				// считаем: window.devicePixelRatio >= 2 => _devicePixelRatio = 2; zoom = window.devicePixelRatio / _devicePixelRatio;
				if (window.devicePixelRatio > 0.1)
				{
					if (window.devicePixelRatio < 1.99)
					{
						var _devicePixelRatio = 1;
						this.zoom = window.devicePixelRatio / _devicePixelRatio;
					}
					else
					{
						var _devicePixelRatio = 2;
						this.zoom = window.devicePixelRatio / _devicePixelRatio;
						this.retinaPixelRatio = 2;
					}
					// chrome 54.x: zoom = "reset" - clear retina zoom (windows)
					//document.firstElementChild.style.zoom = "reset";
					document.firstElementChild.style.zoom = 1.0 / this.zoom;
				}
				else
					document.firstElementChild.style.zoom = "normal";
			}
			else
			{
				var isRetina = (Math.abs(2 - (window.devicePixelRatio / this.zoom)) < 0.01);
				if (isRetina)
					this.retinaPixelRatio = 2;
		
				if (isMobile)
				{
					this.retinaPixelRatio = window.devicePixelRatio;
				}
			}
			*/

			this.width = this.canvas.clientWidth;
			this.height = this.canvas.clientHeight;
			this.canvas.width = (this.width * this.retinaPixelRatio) >> 0;
			this.canvas.height = (this.height * this.retinaPixelRatio) >> 0;

			this.recalculatePlaces();

			this.log("retinaScale: " + this.retinaPixelRatio + ", zoom: " + this.zoom);
		};

		this.open = function(data)
		{
			if (this.file)
				this.file.close();
			this.file = window["AscViewer"].createFile(data);
			this.Selection.Image = this.cacheManager ? this.cacheManager.lock(0, 0) : document.createElement("canvas");

			document.scrollingElement.scrollLeft = 0;
			document.scrollingElement.scrollTop = 0;
			this.scroller.style.width = "0px";
			this.scroller.style.height = "0px";

			this.resize();
		};

		this.recalculatePlaces = function()
		{
			this.drawingPages = [];
			for (let i = 0, len = this.file.pages.length; i < len; i++)
			{
				this.drawingPages.push({ 
					X : 0,
					Y : 0,
					W : (this.file.pages[i].W * 96 * this.zoom / this.file.pages[i].Dpi) >> 0,
					H : (this.file.pages[i].H * 96 * this.zoom / this.file.pages[i].Dpi) >> 0,
				});
			}

			this.documentWidth = 0;
			for (let i = 0, len = this.drawingPages.length; i < len; i++)
			{
				if (this.drawingPages[i].W > this.documentWidth)
					this.documentWidth = this.drawingPages[i].W;
			}

			var curTop = 20;
			for (let i = 0, len = this.drawingPages.length; i < len; i++)
			{
				this.drawingPages[i].X = (this.documentWidth - this.drawingPages[i].W) >> 1;
				this.drawingPages[i].Y = curTop;

				curTop += this.drawingPages[i].H;
				curTop += 20;
			}

			this.documentHeight = curTop;

			var oldX = document.scrollingElement.scrollLeft;
			var oldY = document.scrollingElement.scrollTop;

			var oldW = parseInt(this.scroller.style.width);
			var oldH = parseInt(this.scroller.style.height);

			this.scroller.style.width = this.documentWidth + "px";
			this.scroller.style.height = this.documentHeight + "px";

			var newX = (oldX * oldW / Math.max(1, this.documentWidth)) >> 0;
			var newY = (oldY * oldH / Math.max(1, this.documentHeight)) >> 0;
			if (newX < 0) newX = 0;
			if (newY < 0) newY = 0;
			var newXMax = Math.max(0, this.documentWidth - this.width);
			var newYMax = Math.max(0, this.documentHeight - this.height);
			if (newX > newXMax)
				newX = newXMax;
			if (newY > newYMax)
				newY = newYMax;

			document.scrollingElement.scrollLeft = newX;
			document.scrollingElement.scrollTop = newY;

			this.paint();
		};

		this.scroll = function(e)
		{
			this.log("scroll: [" + document.scrollingElement.scrollLeft + ", " + document.scrollingElement.scrollTop + "]");
			this.paint();
		};
		
		this.OnMouse = function(e, down)
		{
			if (!this.file.isValid())
				return;

			let lCurrentPage = -1;
			for (let i = 0; i < this.drawingPages.length; i++)
			{
				let page = this.drawingPages[i];
				let pageT = page.Y;
				let pageB = page.Y + page.H;

				if (e.pageY > pageT && e.pageY < pageB)
				{
					lCurrentPage = i;
					break;
				}
			}

			if (lCurrentPage >= 0)
			{
				let yPos = (document.scrollingElement.scrollTop * this.zoom) >> 0;
				let page = this.drawingPages[lCurrentPage];
				let w = (page.W * this.retinaPixelRatio) >> 0;
				let h = (page.H * this.retinaPixelRatio) >> 0;
				let x = e.pageX - (Math.max(0, (this.width - this.documentWidth) >> 1) * this.retinaPixelRatio) >> 0;
				let y = e.pageY - (page.Y * this.retinaPixelRatio) >> 0;
				this.Selection.page = lCurrentPage;
				if (down)
					this.file.OnMouseDown(lCurrentPage, this.Selection, x, y, w, h);
				else
					this.file.OnMouseMove(lCurrentPage, this.Selection, x, y, w, h);
				if (this.Selection.IsSelection)
					this.paint();
			}
		};
		
		this.OnMouseUp = function(e)
		{
			this.file.OnMouseUp();
			this.paint();
		};

		this.paint = function()
		{
			this.isRepaint = true;
		};

		this._paint = function()
		{
			if (!this.isRepaint)
				return;

			if (!this.file.isValid())
				return;

			let ctx = this.canvas.getContext("2d");
			ctx.strokeStyle = "#000000";
			let lineW = this.retinaPixelRatio >> 0;
			ctx.lineWidth = lineW;

			let yPos = (document.scrollingElement.scrollTop * this.zoom) >> 0;
			let yMax = yPos + this.height;
			let xCenter = this.width >> 1;
			if (this.documentWidth > this.width)
			{
				xCenter = (this.documentWidth >> 1) - (document.scrollingElement.scrollLeft * this.zoom) >> 0;
			}

			let lStartPage = -1;
			let lEndPage = -1; 
			
			let lPagesCount = this.drawingPages.length;
			for (let i = 0; i < lPagesCount; i++)
			{
				let page = this.drawingPages[i];
				let pageT = page.Y;
				let pageB = page.Y + page.H;
				
				if (yPos < pageB && yMax > pageT)
				{
					// страница на экране

					if (-1 == lStartPage)
						lStartPage = i;
					lEndPage = i;
				}
				else
				{
					// страница не видна - выкидываем из кэша
					if (page.Image)
					{
						if (this.file.cacheManager)
							this.file.cacheManager.unlock(page.Image);
						
						delete page.Image;						
					}
				}
			}

			for (let i = lStartPage; i <= lEndPage; i++)
			{
				// отрисовываем страницу
				let page = this.drawingPages[i];

				let w = (page.W * this.retinaPixelRatio) >> 0;
				let h = (page.H * this.retinaPixelRatio) >> 0;

				if (!this.file.cacheManager)
				{
					if (page.Image && ((page.Image.width != w) || (page.Image.height != h)))
						delete page.Image;
				}
				else
				{
					if (page.Image && ((page.Image.width < w) || (page.Image.height < h)))
					{
						if (this.file.cacheManager)
							this.file.cacheManager.unlock(page.Image);
						
						delete page.Image;	
					}
				}
				
				if (!page.Image)
					page.Image = this.file.getPage(i, w, h);

				let x = ((xCenter * this.retinaPixelRatio) >> 0) - (w >> 1);
				let y = ((page.Y - yPos) * this.retinaPixelRatio) >> 0;

				ctx.drawImage(page.Image, 0, 0, w, h, x, y, w, h);
				if (this.Selection.page == i && this.Selection.IsSelection)
					ctx.drawImage(this.Selection.Image, 0, 0, w, h, x, y, w, h);

				ctx.strokeRect(x + lineW / 2, y + lineW / 2, w - lineW, h - lineW);
			}
		};
	}
	
	window.Viewer = new CHtmlPage();
	window.Viewer.resize();
	window.Viewer.startTimer();
	window.CCacheManager = CCacheManager;
};

window.onresize = function(e)
{
	if (window.Viewer)
		window.Viewer.resize();
};