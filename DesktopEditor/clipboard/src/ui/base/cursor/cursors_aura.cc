// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/base/cursor/cursors_aura.h"

#include "grit/ui_resources.h"
#include "ui/base/cursor/cursor.h"
#include "ui/gfx/point.h"

namespace ui {
namespace {

struct HotPoint {
  int x;
  int y;
};

struct CursorData {
  int id;
  int resource_id;
  HotPoint hot_1x;
  HotPoint hot_2x;
};

const CursorData kImageCursors[] = {
  {ui::kCursorNull, IDR_AURA_CURSOR_PTR, {4, 4}, {8, 9}},
  {ui::kCursorPointer, IDR_AURA_CURSOR_PTR, {4, 4}, {8, 9}},
  {ui::kCursorNoDrop, IDR_AURA_CURSOR_NO_DROP, {9, 9}, {18, 18}},
  {ui::kCursorNotAllowed, IDR_AURA_CURSOR_NO_DROP, {9, 9}, {18, 18}},
  {ui::kCursorCopy, IDR_AURA_CURSOR_COPY, {9, 9}, {18, 18}},
  {ui::kCursorHand, IDR_AURA_CURSOR_HAND, {9, 4}, {19, 8}},
  {ui::kCursorMove, IDR_AURA_CURSOR_MOVE, {11, 11}, {23, 23}},
  {ui::kCursorNorthEastResize, IDR_AURA_CURSOR_NORTH_EAST_RESIZE,
   {12, 11}, {25, 23}},
  {ui::kCursorSouthWestResize, IDR_AURA_CURSOR_SOUTH_WEST_RESIZE,
   {12, 11}, {25, 23}},
  {ui::kCursorSouthEastResize, IDR_AURA_CURSOR_SOUTH_EAST_RESIZE,
   {11, 11}, {24, 23}},
  {ui::kCursorNorthWestResize, IDR_AURA_CURSOR_NORTH_WEST_RESIZE,
   {11, 11}, {24, 23}},
  {ui::kCursorNorthResize, IDR_AURA_CURSOR_NORTH_RESIZE, {11, 12}, {23, 23}},
  {ui::kCursorSouthResize, IDR_AURA_CURSOR_SOUTH_RESIZE, {11, 12}, {23, 23}},
  {ui::kCursorEastResize, IDR_AURA_CURSOR_EAST_RESIZE, {12, 11}, {25, 23}},
  {ui::kCursorWestResize, IDR_AURA_CURSOR_WEST_RESIZE, {12, 11}, {25, 23}},
  {ui::kCursorIBeam, IDR_AURA_CURSOR_IBEAM, {12, 12}, {24, 25}},
  {ui::kCursorAlias, IDR_AURA_CURSOR_ALIAS, {8, 6}, {15, 11}},
  {ui::kCursorCell, IDR_AURA_CURSOR_CELL, {11, 11}, {24, 23}},
  {ui::kCursorContextMenu, IDR_AURA_CURSOR_CONTEXT_MENU, {4, 4}, {8, 9}},
  {ui::kCursorCross, IDR_AURA_CURSOR_CROSSHAIR, {12, 12}, {25, 23}},
  {ui::kCursorHelp, IDR_AURA_CURSOR_HELP, {4, 4}, {8, 9}},
  {ui::kCursorVerticalText, IDR_AURA_CURSOR_XTERM_HORIZ, {12, 11}, {26, 23}},
  {ui::kCursorZoomIn, IDR_AURA_CURSOR_ZOOM_IN, {10, 10}, {20, 20}},
  {ui::kCursorZoomOut, IDR_AURA_CURSOR_ZOOM_OUT, {10, 10}, {20, 20}},
  {ui::kCursorRowResize, IDR_AURA_CURSOR_ROW_RESIZE, {11, 12}, {23, 23}},
  {ui::kCursorColumnResize, IDR_AURA_CURSOR_COL_RESIZE, {12, 11}, {25, 23}},
  {ui::kCursorEastWestResize, IDR_AURA_CURSOR_EAST_WEST_RESIZE,
   {12, 11}, {25, 23}},
  {ui::kCursorNorthSouthResize, IDR_AURA_CURSOR_NORTH_SOUTH_RESIZE,
   {11, 12}, {23, 23}},
  {ui::kCursorNorthEastSouthWestResize,
   IDR_AURA_CURSOR_NORTH_EAST_SOUTH_WEST_RESIZE, {12, 11}, {25, 23}},
  {ui::kCursorNorthWestSouthEastResize,
   IDR_AURA_CURSOR_NORTH_WEST_SOUTH_EAST_RESIZE, {11, 11}, {24, 23}},
  {ui::kCursorGrab, IDR_AURA_CURSOR_GRAB, {8, 5}, {16, 10}},
  {ui::kCursorGrabbing, IDR_AURA_CURSOR_GRABBING, {9, 9}, {18, 18}},
};

const CursorData kAnimatedCursors[] = {
  {ui::kCursorWait, IDR_THROBBER, {7, 7}, {14, 14}},
  {ui::kCursorProgress, IDR_THROBBER, {7, 7}, {14, 14}},
};

bool SearchTable(const CursorData* table,
                 size_t table_length,
                 int id,
                 float scale_factor,
                 int* resource_id,
                 gfx::Point* point) {
  for (size_t i = 0; i < table_length; ++i) {
    if (table[i].id == id) {
      *resource_id = table[i].resource_id;
      *point = scale_factor == 1.0f ?
               gfx::Point(table[i].hot_1x.x, table[i].hot_1x.y) :
               gfx::Point(table[i].hot_2x.x, table[i].hot_2x.y);
      return true;
    }
  }

  return false;
}

}  // namespace

bool GetCursorDataFor(int id,
                      float scale_factor,
                      int* resource_id,
                      gfx::Point* point) {
  return SearchTable(kImageCursors, arraysize(kImageCursors),
                     id, scale_factor, resource_id, point);
}

bool GetAnimatedCursorDataFor(int id,
                              float scale_factor,
                              int* resource_id,
                              gfx::Point* point) {
  return SearchTable(kAnimatedCursors, arraysize(kAnimatedCursors),
                     id, scale_factor, resource_id, point);
}

}  // namespace ui
