# macOS legacy character-map regression

This manual regression test exercises the macOS Heiti and Apple SD Gothic
system fonts, plus Wingdings. It requires the system fonts and Homebrew
FreeType; it does not replace an end-to-end Desktop Editors test.

From the core repository root on Apple Silicon:

```sh
clang++ -std=c++11 -ffunction-sections -fdata-sections \
  $(pkg-config --cflags freetype2) \
  -I "$(brew --prefix freetype)/include/freetype2/freetype" \
  -I DesktopEditor/freetype-2.10.4/include \
  -I DesktopEditor/freetype-2.10.4/include/freetype \
  DesktopEditor/fontengine/tests/macos_legacy_cmap_regression.cpp \
  DesktopEditor/fontengine/TextShaper.cpp \
  $(pkg-config --libs freetype2) -Wl,-dead_strip \
  -o /tmp/onlyoffice-legacy-cmap-regression
/tmp/onlyoffice-legacy-cmap-regression
```

The first test invokes the actual `NSShaper::FT_SetCMapForCharCode` function.
Run the following separate test to exercise the actual font coverage enumerator:

```sh
clang++ -std=c++11 -w -include sys/stat.h -include unistd.h -include utime.h \
  -ffunction-sections -fdata-sections $(pkg-config --cflags freetype2) \
  -I "$(brew --prefix freetype)/include/freetype2/freetype" \
  -I DesktopEditor/freetype-2.10.4/include \
  -I DesktopEditor/freetype-2.10.4/include/freetype \
  DesktopEditor/fontengine/tests/macos_font_coverage_regression.cpp \
  DesktopEditor/fontengine/ApplicationFonts.cpp \
  DesktopEditor/graphics/pro/pro_base.cpp DesktopEditor/common/File.cpp \
  $(pkg-config --libs freetype2) -Wl,-dead_strip \
  -o /tmp/onlyoffice-font-coverage-regression
/tmp/onlyoffice-font-coverage-regression
```

Neither test exercises font-cache regeneration or the Desktop Editors app.
