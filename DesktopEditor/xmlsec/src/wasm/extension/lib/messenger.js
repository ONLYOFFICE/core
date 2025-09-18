(function (window) {
	const onlyofficeClientChannel = "onlyoffice-sign-client-channel";
	const onlyofficeExtensionChannel = "onlyoffice-sign-extension-channel";
	function PluginMessenger() {
		this.id = 0;
		this.resolvers = {};
		this.isInitEngine = null;
	}
	PluginMessenger.prototype.isInitExtension = async function () {
		if (typeof this.isInitEngine !== "boolean") {
			this.isInitEngine = await new Promise((resolve) => {
				const handler = () => {
					this.isInitEngine = true;
					resolve(true);
					window.removeEventListener(onlyofficeClientChannel, handler);
				}
				window.addEventListener(onlyofficeClientChannel, handler);
				setTimeout(() => {
					this.isInitEngine = false;
					window.removeEventListener(onlyofficeClientChannel, handler);
					resolve(false);
				}, 1000);
			});
		}
		return this.isInitEngine;
	};
	PluginMessenger.prototype.init = function () {
		window.addEventListener(onlyofficeClientChannel, (event) => {
			if (event.detail && typeof event.detail.id === 'number' && this.resolvers[event.detail.id]) {
				const resolve = this.resolvers[event.detail.id];
				delete this.resolvers[event.detail.id];
				resolve(event.detail.data);
			}
		});
	};
	PluginMessenger.prototype.postMessage = async function (data, timeout) {
		try {
			const id = this.id++;
			return new Promise((resolve, reject) => {
				window.dispatchEvent(new CustomEvent(onlyofficeExtensionChannel, { detail: {data: data, id: id} }));
				this.resolvers[id] = resolve;
				if (typeof timeout === "number") {
					setTimeout(() => {
						delete this.resolvers[id];
						reject(`Long waiting time: ${id}`);
					}, timeout);
				}
			});
		} catch (e) {
			
		}
	};

	const pluginMessenger = new PluginMessenger();
	pluginMessenger.init();
	window.Asc = window.Asc || {};
	window.Asc.pluginMessenger = pluginMessenger;
})(window);