// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/gfx/font_render_params_linux.h"

#include "base/command_line.h"
#include "base/logging.h"
#include "ui/gfx/switches.h"

#if defined(TOOLKIT_GTK)
#include <gtk/gtk.h>
#else
#include <fontconfig/fontconfig.h>
#endif

namespace gfx {

namespace {

bool SubpixelPositioningRequested(bool renderer) {
  return CommandLine::ForCurrentProcess()->HasSwitch(
      renderer ?
      switches::kEnableWebkitTextSubpixelPositioning :
      switches::kEnableBrowserTextSubpixelPositioning);
}

// Initializes |params| with the system's default settings. |renderer| is true
// when setting WebKit renderer defaults.
void LoadDefaults(FontRenderParams* params, bool renderer) {
#if defined(TOOLKIT_GTK)
  params->antialiasing = true;
  // TODO(wangxianzhu): autohinter is now true to keep original behavior
  // of WebKit, but it might not be the best value.
  params->autohinter = true;
  params->use_bitmaps = true;
  params->hinting = FontRenderParams::HINTING_SLIGHT;
  params->subpixel_rendering = FontRenderParams::SUBPIXEL_RENDERING_NONE;

  GtkSettings* gtk_settings = gtk_settings_get_default();
  CHECK(gtk_settings);
  gint gtk_antialias = 0;
  gint gtk_hinting = 0;
  gchar* gtk_hint_style = NULL;
  gchar* gtk_rgba = NULL;
  g_object_get(gtk_settings,
               "gtk-xft-antialias", &gtk_antialias,
               "gtk-xft-hinting", &gtk_hinting,
               "gtk-xft-hintstyle", &gtk_hint_style,
               "gtk-xft-rgba", &gtk_rgba,
               NULL);

  // g_object_get() doesn't tell us whether the properties were present or not,
  // but if they aren't (because gnome-settings-daemon isn't running), we'll get
  // NULL values for the strings.
  if (gtk_hint_style && gtk_rgba) {
    params->antialiasing = gtk_antialias;

    if (gtk_hinting == 0 || strcmp(gtk_hint_style, "hintnone") == 0)
      params->hinting = FontRenderParams::HINTING_NONE;
    else if (strcmp(gtk_hint_style, "hintslight") == 0)
      params->hinting = FontRenderParams::HINTING_SLIGHT;
    else if (strcmp(gtk_hint_style, "hintmedium") == 0)
      params->hinting = FontRenderParams::HINTING_MEDIUM;
    else if (strcmp(gtk_hint_style, "hintfull") == 0)
      params->hinting = FontRenderParams::HINTING_FULL;

    if (strcmp(gtk_rgba, "none") == 0)
      params->subpixel_rendering = FontRenderParams::SUBPIXEL_RENDERING_NONE;
    else if (strcmp(gtk_rgba, "rgb") == 0)
      params->subpixel_rendering = FontRenderParams::SUBPIXEL_RENDERING_RGB;
    else if (strcmp(gtk_rgba, "bgr") == 0)
      params->subpixel_rendering = FontRenderParams::SUBPIXEL_RENDERING_BGR;
    else if (strcmp(gtk_rgba, "vrgb") == 0)
      params->subpixel_rendering = FontRenderParams::SUBPIXEL_RENDERING_VRGB;
    else if (strcmp(gtk_rgba, "vbgr") == 0)
      params->subpixel_rendering = FontRenderParams::SUBPIXEL_RENDERING_VBGR;
  }

  g_free(gtk_hint_style);
  g_free(gtk_rgba);
#else
  // For non-GTK builds (read: Aura), just use reasonable hardcoded values.
  params->antialiasing = true;
  params->autohinter = true;
  params->use_bitmaps = true;
  params->hinting = FontRenderParams::HINTING_SLIGHT;

  // Fetch default subpixel rendering settings from FontConfig.
  FcPattern* pattern = FcPatternCreate();
  FcConfigSubstitute(NULL, pattern, FcMatchPattern);
  FcDefaultSubstitute(pattern);
  FcResult result;
  FcPattern* match = FcFontMatch(0, pattern, &result);
  DCHECK(match);
  int fc_rgba = FC_RGBA_RGB;
  FcPatternGetInteger(match, FC_RGBA, 0, &fc_rgba);
  FcPatternDestroy(pattern);
  FcPatternDestroy(match);

  switch (fc_rgba) {
    case FC_RGBA_RGB:
      params->subpixel_rendering = FontRenderParams::SUBPIXEL_RENDERING_RGB;
      break;
    case FC_RGBA_BGR:
      params->subpixel_rendering = FontRenderParams::SUBPIXEL_RENDERING_BGR;
      break;
    case FC_RGBA_VRGB:
      params->subpixel_rendering = FontRenderParams::SUBPIXEL_RENDERING_VRGB;
      break;
    case FC_RGBA_VBGR:
      params->subpixel_rendering = FontRenderParams::SUBPIXEL_RENDERING_VBGR;
      break;
    default:
      params->subpixel_rendering = FontRenderParams::SUBPIXEL_RENDERING_NONE;
  }
#endif

  params->subpixel_positioning = SubpixelPositioningRequested(renderer);

  // To enable subpixel positioning, we need to disable hinting.
  if (params->subpixel_positioning)
    params->hinting = FontRenderParams::HINTING_NONE;
}

}  // namespace

const FontRenderParams& GetDefaultFontRenderParams() {
  static bool loaded_defaults = false;
  static FontRenderParams default_params;
  if (!loaded_defaults)
    LoadDefaults(&default_params, /* renderer */ false);
  loaded_defaults = true;
  return default_params;
}

const FontRenderParams& GetDefaultWebKitFontRenderParams() {
  static bool loaded_defaults = false;
  static FontRenderParams default_params;
  if (!loaded_defaults)
    LoadDefaults(&default_params, /* renderer */ true);
  loaded_defaults = true;
  return default_params;
}

bool GetDefaultWebkitSubpixelPositioning() {
  return SubpixelPositioningRequested(true);
}

}  // namespace gfx
