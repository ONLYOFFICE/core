//
//  ImageUtils.h
//  UTILS
//
//  Created by alexey.musinov on 30.04.15.
//  Copyright (c) 2015 Ascensio System SIA. All rights reserved.
//

#ifndef _IMAGE_UTILS_H_
#define _IMAGE_UTILS_H_

#import <Foundation/Foundation.h>
#import <GLKit/GLKView.h>
#import <UIKit/UIKit.h>

#include "BgraFrame.h"

namespace ImageUtils_IOS
{
    static UIImage* BgraToUIImage(CBgraFrame* image)
    {
        unsigned char *data     = image->get_Data();
       
        int width               = image->get_Width();
        int height              = image->get_Height();
        int bitsPerComponent    = 8;
        int bitsPerPixel        = 32;
        int bytesPerRow         = 4 * width;
        
        CGDataProviderRef provider = CGDataProviderCreateWithData(NULL,
                                                                  data,
                                                                  width * height * 4,
                                                                  NULL);
        
        CGColorSpaceRef colorSpaceRef = CGColorSpaceCreateDeviceRGB();
        CGBitmapInfo bitmapInfo = kCGBitmapByteOrderDefault;
        CGColorRenderingIntent renderingIntent = kCGRenderingIntentDefault;
        CGImageRef imageRef = CGImageCreate(width,
                                            height,
                                            bitsPerComponent,
                                            bitsPerPixel,
                                            bytesPerRow,
                                            colorSpaceRef,
                                            bitmapInfo,
                                            provider,
                                            NULL,
                                            NO,
                                            renderingIntent);
        
        CGFloat scale = [[UIScreen mainScreen] scale];
        UIImage *ret = [UIImage imageWithCGImage:imageRef scale:scale orientation:UIImageOrientationDownMirrored];
        CGImageRelease(imageRef);
        CGColorSpaceRelease(colorSpaceRef);
        CGDataProviderRelease(provider);
        
        return ret;
    }
}

#endif
