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
