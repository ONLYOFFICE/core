# TXT BOM regression tests (C++17)

The suite calls the real `TxtFile::readUtf8Lines` implementation and checks
exact UTF-8 bytes, line boundaries and `getLinesCount()`. No Python, mocks,
external test framework or complete ONLYOFFICE application build is needed.

## Build and run

Requirements: a C++17 compiler, CMake >= 3.16 and ICU development libraries
(`uc`, `i18n`, `data`). Run from the `core` repository root:

```sh
cmake -S Test/Applications/TxtBom -B build/txt-bom-tests \
  -DCMAKE_BUILD_TYPE=Release -DICU_ROOT=/opt/homebrew/opt/icu4c
cmake --build build/txt-bom-tests -j4
ctest --test-dir build/txt-bom-tests --output-on-failure
```

`ICU_ROOT` is the local Homebrew path on Apple Silicon; change it for another
ICU installation, or omit it when CMake can find ICU automatically. The build
has been verified on macOS ARM64 with ICU 78.3. Other platforms have not been
verified. CTest reports one suite; the executable prints one result per case:

```sh
./build/txt-bom-tests/txt_bom_tests
```

Expected summary: `24/24 tests passed`. Exit codes: 0 means success, 1 means
at least one assertion failed, 2 means a setup or filesystem error. Checks
are ordinary comparisons, so Release builds with `NDEBUG` still run them.

## What the tests check

- Four committed fixtures: UTF-16LE/BE, each with and without BOM. All contain
  the same complete three-paragraph text. The sentence mentioning "UTF-16 LE"
  is deliberately preserved in BE fixtures to keep their text identical.
- Eighteen generated cases: UTF-16LE, UTF-16BE and UTF-8, with/without BOM,
  each containing either mixed Unicode text, empty content or a single `A`.
- Two generated cases: UTF-16LE/BE BOM overrides a caller's UTF-8 selection.

The mixed sample covers accents, CJK, emoji (a UTF-16 surrogate pair), CRLF,
LF, CR, an empty line and a last line without a terminator. The committed
fixtures also cover a final CRLF. Empty/BOM-only input must yield zero lines.
A one-character input catches accidentally consuming text when skipping BOM.

Expected lines are explicit UTF-8 literals, independent of ICU decoding.
Generated UTF-16 inputs serialize `char16_t` code units in the requested byte
order; they never reinterpret host-endian memory or use the converter under
test. Byte comparisons and hex diagnostics preserve embedded NULs, which
were central to this regression. Generated files use an isolated temporary
directory and are removed on exit; committed fixtures are read-only.

`readUtf8Lines` takes an internal encoding-table index: 46 = UTF-8,
48 = UTF-16LE, 49 = UTF-16BE. These are NOT the Windows code pages 65001,
1200 and 1201. For inputs without BOM, the test explicitly supplies the
correct selection, as the encoding dialog would. It does not claim automatic
encoding detection for files without BOM.

## Verify that the test catches the original bug

Compile the same C++ suite against an unmodified, pre-fix `TxtFile.cpp` using
`TXT_READER_SOURCE`. The following command uses the pre-fix revision against
which this regression was verified:

```sh
mkdir -p build
git show 3250a848ee:TxtFile/Source/TxtFormat/TxtFile.cpp > build/txt-bom-before.cpp
cmake -S Test/Applications/TxtBom -B build/txt-bom-before \
  -DCMAKE_BUILD_TYPE=Release -DICU_ROOT=/opt/homebrew/opt/icu4c \
  -DTXT_READER_SOURCE="$PWD/build/txt-bom-before.cpp"
cmake --build build/txt-bom-before -j4
ctest --test-dir build/txt-bom-before --output-on-failure
```

The test runner never edits production code or reconstructs its own fake
reader. Before the fix, the real numeric-converter branches fail eight cases
on macOS/ICU 78.3: `16/24 tests passed` and a nonzero exit status. With the
named UTF-16 converters, all 24 pass.

## Additional application validation

The existing local macOS ONLYOFFICE 9.4.0 build was separately verified:
recompiled x2t converted all four fixtures into DOCX with exact paragraph
content, and the desktop app displayed the UTF-16LE BOM sample including
accents without an encoding prompt. Those application checks are not run by
this standalone suite. DocumentServer's API-driven opening flow has not been
verified.
