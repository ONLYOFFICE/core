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


#import "socketRocket_objc.h"
#import "../../../../../DesktopEditor/common/Mac/NSString+StringUtils.h"

@implementation SocketRocketObjC

- (id)init
{
    self = [super init];
    if (self) {
        self.socket = nil;
        m_listener = nil;
        m_url = nil;
    }
    return self;
}

- (void)open
{   
    if (m_url && m_url.length)
    {
        NSURLRequest *request = [NSURLRequest requestWithURL:[NSURL URLWithString:m_url]];
        if (request)
        {
            self.socket = [[SRWebSocket alloc] initWithURLRequest:request
                                                        protocols:nil
                                   allowsUntrustedSSLCertificates:YES];
            self.socket.delegate = self;
            [self.socket open];
        }
    }
}

- (void)send:(NSString*)message
{
#if _LOGGER_SOCKETS
        
    NSLog(@"------------------- SEND TO SOCKET -------------------");
    NSLog(@"%@", message);
        
#endif

    if (self.socket)
    {
        [self.socket send:message];
        
    }
}

- (void)close
{
    if (self.socket)
    {
        [self.socket close];
    }
}

- (void)setListener: (NSNetwork::NSWebSocket::IListener*)listener
{
    m_listener = listener;
}

- (void)setUrl: (NSString *)url
{
    m_url = url;
}

#pragma mark - SRWebSocketDelegate

- (void)webSocket:(SRWebSocket *)webSocket didReceiveMessage:(id)message
{
#if _LOGGER_SOCKETS
        
    NSLog(@"------------------- SOCKET RECEIVE MESSAGE -------------------");
    NSLog(@"%@", message);
        
#endif

    NSString *s = (NSString *)message;
    if (s)
    {
       m_listener->onMessage(s.stdstring);
    }  
}

- (void)webSocketDidOpen:(SRWebSocket *)webSocket
{
    m_listener->onOpen();

#if _LOGGER_SOCKETS
        
    NSLog(@"------------------- SOCKET OPEN -------------------");
    NSLog(@"URL : %@", webSocket.url);
    NSLog(@"description : %@", webSocket.description);
    NSLog(@"hash : %lu", (unsigned long)webSocket.hash);
    NSLog(@"---------------------------------------------------");
        
#endif    
}

- (void)webSocket:(SRWebSocket *)webSocket didFailWithError:(NSError *)error
{
    m_listener->onError(error.localizedDescription.stdstring);

#if _LOGGER_SOCKETS
    
    NSLog(@"---------------------------------------------------------");
    NSLog(@"------------------- SOCKET ERROR : %@ ------------", error);
    NSLog(@"---------------------------------------------------------");
    
#endif
}

- (void)webSocket:(SRWebSocket *)webSocket didCloseWithCode:(NSInteger)code reason:(NSString *)reason wasClean:(BOOL)wasClean
{
    m_listener->onClose(code, reason.stdstring); 

#if _LOGGER_SOCKETS
        
    NSLog(@"---------------------------------------------------------");
    NSLog(@"------------------- SOCKET CLOSE : %@ -----------", reason);
    NSLog(@"---------------------------------------------------------");
        
#endif
}     

- (void)webSocket:(SRWebSocket *)webSocket didReceivePong:(NSData *)pongPayload
{           
#if _LOGGER_SOCKETS
        
    NSString *str = [[NSString alloc] initWithData:pongPayload encoding:NSUTF8StringEncoding];
        
    NSLog(@"---------------------------------------------------");
    NSLog(@"------------------- SOCKET PONG : %@ ---------", str);
    NSLog(@"---------------------------------------------------");
        
#endif
}

@end
