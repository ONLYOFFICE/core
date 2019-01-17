/*
 * (c) Copyright Ascensio System SIA 2010-2019
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
//
//  DocumentEditorCtrl.h
//  DocumentEditorCtrl
//
//  Created by NewOleg on 07.07.14.
//  Copyright (c) 2014 Ascensio System. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <GLKit/GLKView.h>
#import <UIKit/UIKit.h>
#import <QuartzCore/QuartzCore.h>

#include "./Editor_Api.h"

@interface DocumentEditorContextMenuHandler : UIScrollView
{
@public
    UIView* m_pParent;
}
@end

@interface DocumentEditorCtrl : GLKView<UIKeyInput, UITextInput, UIScrollViewDelegate, UITextFieldDelegate>

-(void) TM_InvalidateRectNative: (CGRect)rect;
-(void) TM_InvalidateRectNativeOnlyTarget;
-(void) TM_SetCursorTypeNative: (const wchar_t*) strType;
-(void) TM_CaptureMouse;
-(void) TM_UnCaptureMouse;

-(void) TM_SetSettings: (const NSEditorApi::CAscEditorSettings&)settings;
-(void) TM_SetScriptPath: (NSString*)path;
-(void) TM_SetFontsPath: (NSString*)path;
-(void) TM_SetApplicationInfo: (unsigned char*)data : (unsigned int)len;

-(void) TM_LoadDocument: (NSString*)filename;
-(void) TM_Zoom: (double)zoom;
-(void) TM_Destroy;

-(void) TM_Apply : (NSEditorApi::CAscMenuEvent*)pEvent;
-(NSEditorApi::CAscMenuEvent*) TM_ApplySync : (NSEditorApi::CAscMenuEvent*)pEvent;
-(void) TM_SetListener : (NSEditorApi::CAscMenuEventListener*)pListener;

-(void) TM_ShowKeyboard;
-(void) TM_UnShowKeyboard;

-(void) private_DrawSnapshot:(CGContextRef)ctx : (CGRect)rect;
-(void) private_StartDrawLock;
-(void) private_EndDrawLock;

-(void) TM_OnUpdateContentSizes;
-(void) TM_CheckTargetOnScreen;
-(void) TM_Init:(UIViewController*)controller : (DocumentEditorContextMenuHandler*)handler;
-(void) TM_SetOffsetY:(int)offset;
-(int) TM_GetOffsetY;

-(bool) TM_Clipboard_Copy;
-(bool) TM_Clipboard_Cut;
-(bool) TM_Clipboard_Paste;

-(NSString*) TM_GetDocumentBase64;
-(bool) TM_SavePDF:(NSString*)path;
-(UIViewController*) GetController;

@property(nonatomic) UITextAutocorrectionType autocorrectionType;
@property(nonatomic) UIKeyboardType keyboardType;

@end
