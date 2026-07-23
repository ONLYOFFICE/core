/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
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