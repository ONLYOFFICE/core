SRC_DIR = $$PWD/harfbuzz/src

DEFINES += HAVE_FREETYPE

INCLUDEPATH += \
  $$SRC_DIR \

HEADERS += \
  $$SRC_DIR/hb-aat-layout-ankr-table.hh \
  $$SRC_DIR/hb-aat-layout-bsln-table.hh \
  $$SRC_DIR/hb-aat-layout-common.hh \
  $$SRC_DIR/hb-aat-layout-feat-table.hh \
  $$SRC_DIR/hb-aat-layout-just-table.hh \
  $$SRC_DIR/hb-aat-layout-kerx-table.hh \
  $$SRC_DIR/hb-aat-layout-morx-table.hh \
  $$SRC_DIR/hb-aat-layout-opbd-table.hh \
  $$SRC_DIR/hb-aat-layout-trak-table.hh \
  $$SRC_DIR/hb-aat-layout.h \
  $$SRC_DIR/hb-aat-layout.hh \
  $$SRC_DIR/hb-aat-ltag-table.hh \
  $$SRC_DIR/hb-aat-map.hh \
  $$SRC_DIR/hb-aat.h \
  $$SRC_DIR/hb-algs.hh \
  $$SRC_DIR/hb-array.hh \
  $$SRC_DIR/hb-atomic.hh \
  $$SRC_DIR/hb-bimap.hh \
  $$SRC_DIR/hb-bit-page.hh \
  $$SRC_DIR/hb-bit-set-invertible.hh \
  $$SRC_DIR/hb-bit-set.hh \
  $$SRC_DIR/hb-blob.h \
  $$SRC_DIR/hb-blob.hh \
  $$SRC_DIR/hb-buffer-deserialize-json.hh \
  $$SRC_DIR/hb-buffer-deserialize-text.hh \
  $$SRC_DIR/hb-buffer.h \
  $$SRC_DIR/hb-buffer.hh \
  $$SRC_DIR/hb-cache.hh \
  $$SRC_DIR/hb-cff-interp-common.hh \
  $$SRC_DIR/hb-cff-interp-cs-common.hh \
  $$SRC_DIR/hb-cff-interp-dict-common.hh \
  $$SRC_DIR/hb-cff1-interp-cs.hh \
  $$SRC_DIR/hb-cff2-interp-cs.hh \
  $$SRC_DIR/hb-common.h \
  $$SRC_DIR/hb-config.hh \
  $$SRC_DIR/hb-coretext.h \
  $$SRC_DIR/hb-debug.hh \
  $$SRC_DIR/hb-deprecated.h \
  $$SRC_DIR/hb-directwrite.h \
  $$SRC_DIR/hb-dispatch.hh \
  $$SRC_DIR/hb-draw.h \
  $$SRC_DIR/hb-draw.hh \
  $$SRC_DIR/hb-face.h \
  $$SRC_DIR/hb-face.hh \
  $$SRC_DIR/hb-font.h \
  $$SRC_DIR/hb-font.hh \
  $$SRC_DIR/hb-ft.h \
  $$SRC_DIR/hb-gdi.h \
  $$SRC_DIR/hb-glib.h \
  $$SRC_DIR/hb-gobject-structs.h \
  $$SRC_DIR/hb-gobject.h \
  $$SRC_DIR/hb-graphite2.h \
  $$SRC_DIR/hb-icu.h \
  $$SRC_DIR/hb-iter.hh \
  $$SRC_DIR/hb-kern.hh \
  $$SRC_DIR/hb-machinery.hh \
  $$SRC_DIR/hb-map.h \
  $$SRC_DIR/hb-map.hh \
  $$SRC_DIR/hb-meta.hh \
  $$SRC_DIR/hb-ms-feature-ranges.hh \
  $$SRC_DIR/hb-mutex.hh \
  $$SRC_DIR/hb-null.hh \
  $$SRC_DIR/hb-number-parser.hh \
  $$SRC_DIR/hb-number.hh \
  $$SRC_DIR/hb-object.hh \
  $$SRC_DIR/hb-open-file.hh \
  $$SRC_DIR/hb-open-type.hh \
  $$SRC_DIR/hb-ot-cff-common.hh \
  $$SRC_DIR/hb-ot-cff1-std-str.hh \
  $$SRC_DIR/hb-ot-cff1-table.hh \
  $$SRC_DIR/hb-ot-cff2-table.hh \
  $$SRC_DIR/hb-ot-cmap-table.hh \
  $$SRC_DIR/hb-ot-color-cbdt-table.hh \
  $$SRC_DIR/hb-ot-color-colr-table.hh \
  $$SRC_DIR/hb-ot-color-colrv1-closure.hh \
  $$SRC_DIR/hb-ot-color-cpal-table.hh \
  $$SRC_DIR/hb-ot-color-sbix-table.hh \
  $$SRC_DIR/hb-ot-color-svg-table.hh \
  $$SRC_DIR/hb-ot-color.h \
  $$SRC_DIR/hb-ot-deprecated.h \
  $$SRC_DIR/hb-ot-face-table-list.hh \
  $$SRC_DIR/hb-ot-face.hh \
  $$SRC_DIR/hb-ot-font.h \
  $$SRC_DIR/hb-ot-gasp-table.hh \
  $$SRC_DIR/hb-ot-glyf-table.hh \
  $$SRC_DIR/hb-ot-hdmx-table.hh \
  $$SRC_DIR/hb-ot-head-table.hh \
  $$SRC_DIR/hb-ot-hhea-table.hh \
  $$SRC_DIR/hb-ot-hmtx-table.hh \
  $$SRC_DIR/hb-ot-kern-table.hh \
  $$SRC_DIR/hb-ot-layout-base-table.hh \
  $$SRC_DIR/hb-ot-layout-common.hh \
  $$SRC_DIR/hb-ot-layout-gdef-table.hh \
  $$SRC_DIR/hb-ot-layout-gpos-table.hh \
  $$SRC_DIR/hb-ot-layout-gsub-table.hh \
  $$SRC_DIR/hb-ot-layout-gsubgpos.hh \
  $$SRC_DIR/hb-ot-layout-jstf-table.hh \
  $$SRC_DIR/hb-ot-layout.h \
  $$SRC_DIR/hb-ot-layout.hh \
  $$SRC_DIR/hb-ot-map.hh \
  $$SRC_DIR/hb-ot-math-table.hh \
  $$SRC_DIR/hb-ot-math.h \
  $$SRC_DIR/hb-ot-maxp-table.hh \
  $$SRC_DIR/hb-ot-meta-table.hh \
  $$SRC_DIR/hb-ot-meta.h \
  $$SRC_DIR/hb-ot-metrics.h \
  $$SRC_DIR/hb-ot-metrics.hh \
  $$SRC_DIR/hb-ot-name-language-static.hh \
  $$SRC_DIR/hb-ot-name-language.hh \
  $$SRC_DIR/hb-ot-name-table.hh \
  $$SRC_DIR/hb-ot-name.h \
  $$SRC_DIR/hb-ot-os2-table.hh \
  $$SRC_DIR/hb-ot-os2-unicode-ranges.hh \
  $$SRC_DIR/hb-ot-post-macroman.hh \
  $$SRC_DIR/hb-ot-post-table-v2subset.hh \
  $$SRC_DIR/hb-ot-post-table.hh \
  $$SRC_DIR/hb-ot-shape-complex-arabic-fallback.hh \
  $$SRC_DIR/hb-ot-shape-complex-arabic-joining-list.hh \
  $$SRC_DIR/hb-ot-shape-complex-arabic-table.hh \
  $$SRC_DIR/hb-ot-shape-complex-arabic-win1256.hh \
  $$SRC_DIR/hb-ot-shape-complex-arabic.hh \
  $$SRC_DIR/hb-ot-shape-complex-indic-machine.hh \
  $$SRC_DIR/hb-ot-shape-complex-indic.hh \
  $$SRC_DIR/hb-ot-shape-complex-khmer-machine.hh \
  $$SRC_DIR/hb-ot-shape-complex-khmer.hh \
  $$SRC_DIR/hb-ot-shape-complex-myanmar-machine.hh \
  $$SRC_DIR/hb-ot-shape-complex-myanmar.hh \
  $$SRC_DIR/hb-ot-shape-complex-syllabic.hh \
  $$SRC_DIR/hb-ot-shape-complex-use-machine.hh \
  $$SRC_DIR/hb-ot-shape-complex-use-table.hh \
  $$SRC_DIR/hb-ot-shape-complex-vowel-constraints.hh \
  $$SRC_DIR/hb-ot-shape-complex.hh \
  $$SRC_DIR/hb-ot-shape-fallback.hh \
  $$SRC_DIR/hb-ot-shape-normalize.hh \
  $$SRC_DIR/hb-ot-shape.h \
  $$SRC_DIR/hb-ot-shape.hh \
  $$SRC_DIR/hb-ot-stat-table.hh \
  $$SRC_DIR/hb-ot-tag-table.hh \
  $$SRC_DIR/hb-ot-var-avar-table.hh \
  $$SRC_DIR/hb-ot-var-common.hh \
  $$SRC_DIR/hb-ot-var-fvar-table.hh \
  $$SRC_DIR/hb-ot-var-gvar-table.hh \
  $$SRC_DIR/hb-ot-var-hvar-table.hh \
  $$SRC_DIR/hb-ot-var-mvar-table.hh \
  $$SRC_DIR/hb-ot-var.h \
  $$SRC_DIR/hb-ot-vorg-table.hh \
  $$SRC_DIR/hb-ot.h \
  $$SRC_DIR/hb-pool.hh \
  $$SRC_DIR/hb-priority-queue.hh \
  $$SRC_DIR/hb-repacker.hh \
  $$SRC_DIR/hb-sanitize.hh \
  $$SRC_DIR/hb-serialize.hh \
  $$SRC_DIR/hb-set-digest.hh \
  $$SRC_DIR/hb-set.h \
  $$SRC_DIR/hb-set.hh \
  $$SRC_DIR/hb-shape-plan.h \
  $$SRC_DIR/hb-shape-plan.hh \
  $$SRC_DIR/hb-shape.h \
  $$SRC_DIR/hb-shaper-impl.hh \
  $$SRC_DIR/hb-shaper-list.hh \
  $$SRC_DIR/hb-shaper.hh \
  $$SRC_DIR/hb-string-array.hh \
  $$SRC_DIR/hb-style.h \
  $$SRC_DIR/hb-subset-cff-common.hh \
  $$SRC_DIR/hb-subset-cff1.hh \
  $$SRC_DIR/hb-subset-cff2.hh \
  $$SRC_DIR/hb-subset-input.hh \
  $$SRC_DIR/hb-subset-plan.hh \
  $$SRC_DIR/hb-subset.h \
  $$SRC_DIR/hb-subset.hh \
  $$SRC_DIR/hb-ucd-table.hh \
  $$SRC_DIR/hb-unicode-emoji-table.hh \
  $$SRC_DIR/hb-unicode.h \
  $$SRC_DIR/hb-unicode.hh \
  $$SRC_DIR/hb-uniscribe.h \
  $$SRC_DIR/hb-utf.hh \
  $$SRC_DIR/hb-vector.hh \
  $$SRC_DIR/hb-version.h \
  $$SRC_DIR/hb.h \
  $$SRC_DIR/hb.hh

SOURCES += \
  $$SRC_DIR/harfbuzz.cc \
  $$SRC_DIR/hb-aat-layout.cc \
  $$SRC_DIR/hb-aat-map.cc \
  $$SRC_DIR/hb-blob.cc \
  $$SRC_DIR/hb-buffer-serialize.cc \
  $$SRC_DIR/hb-buffer-verify.cc \
  $$SRC_DIR/hb-buffer.cc \
  $$SRC_DIR/hb-common.cc \
  $$SRC_DIR/hb-coretext.cc \
  $$SRC_DIR/hb-directwrite.cc \
  $$SRC_DIR/hb-draw.cc \
  $$SRC_DIR/hb-face.cc \
  $$SRC_DIR/hb-fallback-shape.cc \
  $$SRC_DIR/hb-font.cc \
  $$SRC_DIR/hb-ft.cc \
  $$SRC_DIR/hb-gdi.cc \
  $$SRC_DIR/hb-glib.cc \
  $$SRC_DIR/hb-gobject-structs.cc \
  $$SRC_DIR/hb-graphite2.cc \
  $$SRC_DIR/hb-icu.cc \
  $$SRC_DIR/hb-map.cc \
  $$SRC_DIR/hb-number.cc \
  $$SRC_DIR/hb-ot-cff1-table.cc \
  $$SRC_DIR/hb-ot-cff2-table.cc \
  $$SRC_DIR/hb-ot-color.cc \
  $$SRC_DIR/hb-ot-face.cc \
  $$SRC_DIR/hb-ot-font.cc \
  $$SRC_DIR/hb-ot-layout.cc \
  $$SRC_DIR/hb-ot-map.cc \
  $$SRC_DIR/hb-ot-math.cc \
  $$SRC_DIR/hb-ot-meta.cc \
  $$SRC_DIR/hb-ot-metrics.cc \
  $$SRC_DIR/hb-ot-name.cc \
  $$SRC_DIR/hb-ot-shape-complex-arabic.cc \
  $$SRC_DIR/hb-ot-shape-complex-default.cc \
  $$SRC_DIR/hb-ot-shape-complex-hangul.cc \
  $$SRC_DIR/hb-ot-shape-complex-hebrew.cc \
  $$SRC_DIR/hb-ot-shape-complex-indic-table.cc \
  $$SRC_DIR/hb-ot-shape-complex-indic.cc \
  $$SRC_DIR/hb-ot-shape-complex-khmer.cc \
  $$SRC_DIR/hb-ot-shape-complex-myanmar.cc \
  $$SRC_DIR/hb-ot-shape-complex-syllabic.cc \
  $$SRC_DIR/hb-ot-shape-complex-thai.cc \
  $$SRC_DIR/hb-ot-shape-complex-use.cc \
  $$SRC_DIR/hb-ot-shape-complex-vowel-constraints.cc \
  $$SRC_DIR/hb-ot-shape-fallback.cc \
  $$SRC_DIR/hb-ot-shape-normalize.cc \
  $$SRC_DIR/hb-ot-shape.cc \
  $$SRC_DIR/hb-ot-tag.cc \
  $$SRC_DIR/hb-ot-var.cc \
  $$SRC_DIR/hb-set.cc \
  $$SRC_DIR/hb-shape-plan.cc \
  $$SRC_DIR/hb-shape.cc \
  $$SRC_DIR/hb-shaper.cc \
  $$SRC_DIR/hb-static.cc \
  $$SRC_DIR/hb-style.cc \
  $$SRC_DIR/hb-subset-cff-common.cc \
  $$SRC_DIR/hb-subset-cff1.cc \
  $$SRC_DIR/hb-subset-cff2.cc \
  $$SRC_DIR/hb-subset-input.cc \
  $$SRC_DIR/hb-subset-plan.cc \
  $$SRC_DIR/hb-subset.cc \
  $$SRC_DIR/hb-ucd.cc \
  $$SRC_DIR/hb-unicode.cc \
  $$SRC_DIR/hb-uniscribe.cc

#SOURCES += \
#  $$SRC_DIR/main.cc \
#  $$SRC_DIR/failing-alloc.c \
#  $$SRC_DIR/test-algs.cc \
#  $$SRC_DIR/test-array.cc \
#  $$SRC_DIR/test-bimap.cc \
#  $$SRC_DIR/test-buffer-serialize.cc \
#  $$SRC_DIR/test-gpos-size-params.cc \
#  $$SRC_DIR/test-gsub-would-substitute.cc \
#  $$SRC_DIR/test-iter.cc \
#  $$SRC_DIR/test-machinery.cc \
#  $$SRC_DIR/test-map.cc \
#  $$SRC_DIR/test-number.cc \
#  $$SRC_DIR/test-ot-glyphname.cc \
#  $$SRC_DIR/test-ot-meta.cc \
#  $$SRC_DIR/test-ot-name.cc \
#  $$SRC_DIR/test-priority-queue.cc \
#  $$SRC_DIR/test-repacker.cc \
#  $$SRC_DIR/test-serialize.cc \
#  $$SRC_DIR/test-set.cc \
#  $$SRC_DIR/test-unicode-ranges.cc \
#  $$SRC_DIR/test-vector.cc \
#  $$SRC_DIR/test.cc
