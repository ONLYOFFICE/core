/**
 *
 * (c) Copyright Ascensio System SIA 2020
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
(function(window, undefined)
{

	(function()
	{
		let url = "./engine/engine";
		let useWasm = false;
		let webAsmObj = window["WebAssembly"];
		if (typeof webAsmObj === "object" && typeof webAsmObj["Memory"] === "function")
		{
			if ((typeof webAsmObj["instantiateStreaming"] === "function") || (typeof webAsmObj["instantiate"] === "function"))
				useWasm = true;
		}

		url += (useWasm ? ".js" : "_ie.js");

		let script = document.createElement('script');
		script.type = 'text/javascript';
		script.src = url;
		document.head.appendChild(script);
	})();	

})(window, undefined);
