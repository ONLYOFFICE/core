(function (window) {
	const onlyofficeChannel = "onlyoffice-sign-extension-channel";
	function PluginMessenger() {
		this.id = 0;
		this.resolvers = {};
	}
	PluginMessenger.prototype.init = function () {
		window.addEventListener("message", (event) => {
			if (event.data.type === onlyofficeChannel && typeof event.data.id === 'number' && this.resolvers[event.data.id]) {
				const resolve = this.resolvers[event.data.id];
				resolve(event.data.response);
				delete this.resolvers[event.data.id];
			}
		});
	};
	PluginMessenger.prototype.postMessage = function (data, timeout = 3000) {
		const id = this.id++;
		return new Promise((resolve, reject) => {
			postMessage({content: data, type: onlyofficeChannel}, "*");
			this.resolvers[id] = resolve;
			setTimeout(() => {
				delete this.resolvers[id];
				reject(`Long waiting time: ${id}`);
			}, timeout);
		});
	};

	const pluginMessenger = new PluginMessenger();
	pluginMessenger.init();
	window.Asc = window.Asc || {};
	window.Asc.pluginMessenger = pluginMessenger;
})(window);