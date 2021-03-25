/*
 * (c) Copyright Ascensio System SIA 2010-2021
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
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
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


//#import "socketRocket_objc.h"

#import <SocketRocket/SRWebSocket.h>
#include "listener.h"

@interface SocketRocketObjC  <SRWebSocketDelegate>

@property (strong, nonatomic) SRWebSocket *socket;
@property IListener* listener;
@property (strong) NSString* url;

- (void) open;
- (void) send : (NSString *)name;
- (void) close;
- (void) setListener: (IListener*)listener;
- (void) setUrl: (NSString *)url;

@end

@implementation SocketRocketObjC

- (id)init
{
    self = [super init];
    if (self) {
        self.socket = nil;
    }
    return self;
}

- (void *)open
{   
    if (url && url.length)
    {
        NSURLRequest *request = [NSURLRequest requestWithURL:[NSURL URLWithString:url]];
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
            NSLog(@"%@", [message toString]);
        
#endif

    if (self.socket)
    {
        [self.socket send:[message toString]];
        
    }
}

- (void *)close
{
    if (self.socket)
    {
        [self.socket close];
    }
}

- (void) setListener: (IListener*)listener
{
	self.listener = listener;
}

- (void) setUrl: (NSString *)url
{
	self.url = url;
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
       listener->onMessage(s.stdstring);
    }
    
}

- (void)webSocketDidOpen:(SRWebSocket *)webSocket
{
  
    listener->onOpen();
#if _LOGGER_SOCKETS
        
        NSLog(@"------------------- SOCKET OPEN -------------------");
        //    NSLog(@"URL : %@", webSocket.url);
        //    NSLog(@"description : %@", webSocket.description);
        //    NSLog(@"hash : %lu", (unsigned long)webSocket.hash);
        //    NSLog(@"---------------------------------------------------");
        
#endif
       
}

- (void)webSocket:(SRWebSocket *)webSocket didFailWithError:(NSError *)error
{
	listener->onError(error.stdstring);
#if _LOGGER_SOCKETS
    
    NSLog(@"---------------------------------------------------------");
    NSLog(@"------------------- SOCKET ERROR : %@ ------------", error);
    NSLog(@"---------------------------------------------------------");
    
#endif
}

- (void)webSocket:(SRWebSocket *)webSocket didCloseWithCode:(NSInteger)code reason:(NSString *)reason wasClean:(BOOL)wasClean
{
    
	listener->onClose(code, error.stdstring);   
#if _LOGGER_SOCKETS
        
        NSLog(@"---------------------------------------------------------");
        NSLog(@"------------------- SOCKET CLOSE : %@ -----------", reason);
        NSLog(@"---------------------------------------------------------");
        
#endif
        

- (void)webSocket:(SRWebSocket *)webSocket didReceivePong:(NSData *)pongPayload
{        
        
#if _LOGGER_SOCKETS
        
        NSString *str = [[NSString alloc] initWithData:pongPayload encoding:NSUTF8StringEncoding];
        
        NSLog(@"---------------------------------------------------");
        NSLog(@"------------------- SOCKET PONG : %@ ---------", str);
        NSLog(@"---------------------------------------------------");
        
#endif
  
}
