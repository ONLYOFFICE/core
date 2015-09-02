#include <Cocoa/Cocoa.h>
#include "mac_common.h"

void MAC_COMMON_set_window_handle_sizes(WindowHandleId handle,
                                               int x, int y, int w, int h)
{
    NSView* view = (NSView*)handle;
    NSRect f = view.frame;
    f.origin.x = x;
    f.origin.y = y;
    f.size.width = w;
    f.size.height = h;
    view.frame = f;
}
