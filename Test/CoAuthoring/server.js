/*
 * (c) Copyright Ascensio System SIA 2010-2024
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */

var settings = require('./settings');
var express = require('express');

var app = express.createServer();

function documentSocket(oSocket)
{
	this.m_oUserName = "";
	this.m_oSocket = oSocket;
};
function document()
{
	this.m_arrSockets = [];
	this.m_arrUsers = [];
	this.m_arrChanges = [];
};

//let the server listen
app.listen(settings.port, function () {
  var addr = app.address();
  console.log('App listening on http://' + addr.address + ':' + addr.port);
});

// express route to ping server.
app.get('/ping', function(req, res) {
    res.send('pong');
});

var io = require('socket.io').listen(app), g_arrDocuments = [], g_oDocumentsId = new Object;

io.configure(function(){
	io.set('log level', 0);
});

// open the socket connection
io.sockets.on('connection', function (socket) {
	console.log('connect');
	socket.on('setDocumentInfo', function (documentInfo) {
		console.log('setDocumentId ' + documentInfo.documentId);
		var nIndexDocument = g_oDocumentsId[documentInfo.documentId];
		if (undefined == nIndexDocument)
		{
			nIndexDocument = g_arrDocuments.length;
			g_oDocumentsId[documentInfo.documentId] = nIndexDocument;
			g_arrDocuments[nIndexDocument] = new document();
		}
			
		socket.nickname = documentInfo.documentId;
		
		// broadcast data recieved from the sender
		// to others who are connected, but not 
		// from the original sender.
		sendEvents ('addUserName', documentInfo.user);
		
		console.log('setDocumentId length ' + g_arrDocuments[nIndexDocument].m_arrSockets.length);
		g_arrDocuments[nIndexDocument].m_arrSockets[g_arrDocuments[nIndexDocument].m_arrSockets.length] = socket;
		
		g_arrDocuments[nIndexDocument].m_arrUsers[g_arrDocuments[nIndexDocument].m_arrUsers.length] = documentInfo.user;
			
		// this kind of emit will send to all! :D
		//io.sockets.emit('nicknames', g_arrDocuments[nIndexDocument].m_arrUsers);
		sendEvents ('nicknames', g_arrDocuments[nIndexDocument].m_arrUsers);
		
		socket.emit ('setChanges', g_arrDocuments[nIndexDocument].m_arrChanges);
	});
	
	socket.on('addChange', function (documentChange) {
		var nIndexDocument = g_oDocumentsId[socket.nickname];
		console.log('addChange ' + nIndexDocument);
		if (undefined == g_arrDocuments[nIndexDocument])
			return;
		
		g_arrDocuments[nIndexDocument].m_arrChanges[g_arrDocuments[nIndexDocument].m_arrChanges.length] = documentChange;
		
		sendEventsNoCurent ('addChange', documentChange);
	});
	
	socket.on('updateUser', function (user) {
		var nIndexDocument = g_oDocumentsId[socket.nickname];
		if (undefined == g_arrDocuments[nIndexDocument])
			return;
		
		var userArrayId = 0;
		for (;userArrayId < g_arrDocuments[nIndexDocument].m_arrUsers.length; ++userArrayId)
		{
			if (user.id == g_arrDocuments[nIndexDocument].m_arrUsers[userArrayId].id)
			{
				g_arrDocuments[nIndexDocument].m_arrUsers[userArrayId] = user;
			}
		}
		
		sendEventsNoCurent ('nicknames', g_arrDocuments[nIndexDocument].m_arrUsers);
	});
	
	socket.on('disconnect', function () {
		console.log('disconnect');
		if (undefined == socket.nickname)
			return;
		
		var nIndexDocument = g_oDocumentsId[socket.nickname];
		var socketArrayId = 0;
		for (;socketArrayId < g_arrDocuments[nIndexDocument].m_arrSockets.length; ++socketArrayId)
		{
			if (socket.id == g_arrDocuments[nIndexDocument].m_arrSockets[socketArrayId].id)
				break;
		}
		
		g_arrDocuments[nIndexDocument].m_arrSockets.splice(socketArrayId, 1);
		
		var deleteUser = g_arrDocuments[nIndexDocument].m_arrUsers[socketArrayId];
		sendEvents ('delUserName', deleteUser);
		
		g_arrDocuments[nIndexDocument].m_arrUsers.splice(socketArrayId, 1);
		
		console.log('index ' + nIndexDocument);
		if (0 == g_arrDocuments[nIndexDocument].m_arrSockets.length)
		{
			g_arrDocuments.splice(nIndexDocument, 1);
			delete g_oDocumentsId[socket.nickname];
			
			for (;nIndexDocument < g_arrDocuments.length; ++nIndexDocument)
				g_oDocumentsId[g_arrDocuments[nIndexDocument].m_arrSockets[0].nickname] -= 1;
			
			console.log('empty array');
		}
		else
			sendEvents ('nicknames', g_arrDocuments[nIndexDocument].m_arrUsers);
	});
	
	function sendEvents(nameEvent, dataEvent){
		var nIndexDocument = g_oDocumentsId[socket.nickname];
		for (var i = 0; i < g_arrDocuments[nIndexDocument].m_arrSockets.length; ++i)
			g_arrDocuments[nIndexDocument].m_arrSockets[i].emit(nameEvent, dataEvent);
	};
	function sendEventsNoCurent(nameEvent, dataEvent){
		var nIndexDocument = g_oDocumentsId[socket.nickname];
		for (var i = 0; i < g_arrDocuments[nIndexDocument].m_arrSockets.length; ++i)
			{
				if (g_arrDocuments[nIndexDocument].m_arrSockets[i].id == socket.id)
					continue;
				
				g_arrDocuments[nIndexDocument].m_arrSockets[i].emit(nameEvent, dataEvent);
			}
	};
});