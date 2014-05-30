// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_MENU_MENU_WIN_H_
#define UI_VIEWS_CONTROLS_MENU_MENU_WIN_H_

#include <windows.h>

#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "ui/views/controls/menu/menu.h"

namespace views {

namespace {
class MenuHostWindow;
}

///////////////////////////////////////////////////////////////////////////////
//
// Menu class
//
//   A wrapper around a Win32 HMENU handle that provides convenient APIs for
//   menu construction, display and subsequent command execution.
//
///////////////////////////////////////////////////////////////////////////////
class MenuWin : public Menu {
 public:
  // Construct a Menu using the specified controller to determine command
  // state.
  // delegate     A Menu::Delegate implementation that provides more
  //              information about the Menu presentation.
  // anchor       An alignment hint for the popup menu.
  // owner        The window that the menu is being brought up relative
  //              to. Not actually used for anything but must not be
  //              NULL.
  MenuWin(Delegate* d, AnchorPoint anchor, HWND owner);
  // Alternatively, a Menu object can be constructed wrapping an existing
  // HMENU. This can be used to use the convenience methods to insert
  // menu items and manage label string ownership. However this kind of
  // Menu object cannot use the delegate.
  explicit MenuWin(HMENU hmenu);
  virtual ~MenuWin();

  // Overridden from Menu:
  virtual void AddMenuItemWithIcon(int index,
                                   int item_id,
                                   const string16& label,
                                   const gfx::ImageSkia& icon) OVERRIDE;
  virtual Menu* AddSubMenuWithIcon(int index,
                                   int item_id,
                                   const string16& label,
                                   const gfx::ImageSkia& icon) OVERRIDE;
  virtual void AddSeparator(int index) OVERRIDE;
  virtual void EnableMenuItemByID(int item_id, bool enabled) OVERRIDE;
  virtual void EnableMenuItemAt(int index, bool enabled) OVERRIDE;
  virtual void SetMenuLabel(int item_id, const string16& label) OVERRIDE;
  virtual bool SetIcon(const gfx::ImageSkia& icon, int item_id) OVERRIDE;
  virtual void RunMenuAt(int x, int y) OVERRIDE;
  virtual void Cancel() OVERRIDE;
  virtual int ItemCount() OVERRIDE;

  virtual HMENU GetMenuHandle() const {
    return menu_;
  }

  // Gets the Win32 TPM alignment flags for the specified AnchorPoint.
  DWORD GetTPMAlignFlags() const;

 protected:
  virtual void AddMenuItemInternal(int index,
                                   int item_id,
                                   const string16& label,
                                   const gfx::ImageSkia& icon,
                                   MenuItemType type) OVERRIDE;

 private:
  friend class MenuHostWindow;

  // The data of menu items needed to display.
  struct ItemData;

  void AddMenuItemInternal(int index,
                           int item_id,
                           const string16& label,
                           const gfx::ImageSkia& icon,
                           HMENU submenu,
                           MenuItemType type);

  explicit MenuWin(MenuWin* parent);

  // Sets menu information before displaying, including sub-menus.
  void SetMenuInfo();

  // Get all the state flags for the |fState| field of MENUITEMINFO for the
  // item with the specified id. |delegate| is consulted if non-NULL about
  // the state of the item in preference to |controller_|.
  UINT GetStateFlagsForItemID(int item_id) const;

  // The Win32 Menu Handle we wrap
  HMENU menu_;

  // The window that would receive WM_COMMAND messages when the user selects
  // an item from the menu.
  HWND owner_;

  // This list is used to store the default labels for the menu items.
  // We may use contextual labels when RunMenu is called, so we must save
  // a copy of default ones here.
  std::vector<string16> labels_;

  // A flag to indicate whether this menu will be drawn by the Menu class.
  // If it's true, all the menu items will be owner drawn. Otherwise,
  // all the drawing will be done by Windows.
  bool owner_draw_;

  // This list is to store the string labels and icons to display. It's used
  // when owner_draw_ is true. We give MENUITEMINFO pointers to these
  // structures to specify what we'd like to draw. If owner_draw_ is false,
  // we only give MENUITEMINFO pointers to the labels_.
  // The label member of the ItemData structure comes from either labels_ or
  // the GetContextualLabel.
  std::vector<ItemData*> item_data_;

  // Our sub-menus, if any.
  std::vector<MenuWin*> submenus_;

  // Whether the menu is visible.
  bool is_menu_visible_;

  DISALLOW_COPY_AND_ASSIGN(MenuWin);
};

}  // namespace views

#endif  // UI_VIEWS_CONTROLS_MENU_MENU_WIN_H_
