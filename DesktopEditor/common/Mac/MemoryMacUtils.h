//
//  MemoryMacUtils.h
//  UTILS
//
//  Created by Alexey Musinov on 03.11.15.
//  Copyright © 2015 Ascensio System SIA. All rights reserved.
//

#ifndef _MEMORY_MAC_UTILS_H_
#define _MEMORY_MAC_UTILS_H_

#ifdef __APPLE__

#include <mach/mach.h>
#include <mach/task.h>
#include <mach/message.h>

void LogMemoryUsage(NSString* sBeforeText)
{
    struct mach_task_basic_info info;
    mach_msg_type_number_t size = MACH_TASK_BASIC_INFO_COUNT;
    kern_return_t kerr = task_info(mach_task_self(), MACH_TASK_BASIC_INFO, (task_info_t)&info, &size);
    
    if (kerr == KERN_SUCCESS) {
        NSLog(@"%@ memory in use (in bytes): %d", sBeforeText, (int)((double)info.resident_size / 1048576.0));
    } else {
        NSLog(@"%@ error with task_info(): %s", sBeforeText, mach_error_string(kerr));
    }
}

#endif

#endif /* _MEMORY_MAC_UTILS_H_ */
