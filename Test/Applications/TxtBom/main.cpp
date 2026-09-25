// SPDX-License-Identifier: AGPL-3.0-only
#include "TxtFile.h"
#include "DesktopEditor/common/File.h"

#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace fs = std::filesystem;
using Lines = std::vector<std::string>;

namespace
{
// readUtf8Lines takes an INDEX into NSUnicodeConverter::Encodings, not a
// Windows code page: 48 means UTF-16LE here, whereas Windows calls it 1200.
constexpr int Utf8Index = 46;
constexpr int Utf16LEIndex = 48;
constexpr int Utf16BEIndex = 49;

// Isolate generated inputs per run; never overwrite the four checked-in files.
// Cleanup also runs if compilation-independent setup/IO checks throw.
struct TemporaryDirectory
{
    fs::path path;
    TemporaryDirectory()
    {
        std::random_device random;
        for (int attempt = 0; attempt < 100; ++attempt)
        {
            auto candidate = fs::temp_directory_path() /
                ("onlyoffice-txt-bom-" + std::to_string(random()));
            if (fs::create_directory(candidate))
            {
                path = candidate;
                return;
            }
        }
        throw std::runtime_error("Cannot create a unique temporary directory");
    }
    ~TemporaryDirectory()
    {
        std::error_code ignored;
        fs::remove_all(path, ignored);
    }
};

// Serialize UTF-16 code units explicitly. A char16_t literal already contains
// surrogate pairs for emoji; do not use wchar_t (32 bits on macOS/Linux), host
// byte order, or the converter under test to generate these input bytes.
std::string utf16Bytes(const std::u16string& text, bool bigEndian, bool bom)
{
    std::string bytes;
    if (bom)
        bytes = bigEndian ? "\xFE\xFF" : "\xFF\xFE";
    for (char16_t unit : text)
    {
        const char low = static_cast<char>(unit & 0xFF);
        const char high = static_cast<char>(unit >> 8);
        bytes += bigEndian ? high : low;
        bytes += bigEndian ? low : high;
    }
    return bytes;
}

void writeBytes(const fs::path& path, const std::string& bytes)
{
    std::ofstream file(path, std::ios::binary);
    file.write(bytes.data(), static_cast<std::streamsize>(bytes.size()));
    file.close();
    if (!file)
        throw std::runtime_error("Cannot write fixture: " + path.u8string());
}

// Hex diagnostics expose NULs and malformed UTF-8 instead of hiding them in
// terminal output. std::string equality below also compares embedded NULs.
std::string hex(const std::string& bytes)
{
    std::ostringstream result;
    for (unsigned char byte : bytes)
        result << std::hex << std::setw(2) << std::setfill('0') << unsigned(byte) << ' ';
    return result.str();
}

struct Tests
{
    int total = 0;
    int failed = 0;

    void check(const std::string& name, const fs::path& path, int encoding,
               const Lines& expected)
    {
        // Missing files must be setup errors, not successful "empty file" tests.
        if (!fs::is_regular_file(path))
            throw std::runtime_error("Missing fixture: " + path.u8string());
        std::string utf8Path = path.u8string();
        TxtFile reader(NSFile::CUtf8Converter::GetUnicodeStringFromUTF8(
            reinterpret_cast<BYTE*>(utf8Path.data()), utf8Path.size()));
        const auto actual = reader.readUtf8Lines(encoding);
        ++total;
        // Compare exact UTF-8 lines AND the reader's reported count. These are
        // real production calls; no decoder, filesystem or reader is mocked.
        if (actual == expected && reader.getLinesCount() == static_cast<int>(expected.size()))
        {
            std::cout << "PASS " << name << '\n';
            return;
        }
        ++failed;
        std::cerr << "FAIL " << name << ": expected " << expected.size()
                  << " lines, got " << actual.size() << ", reported "
                  << reader.getLinesCount() << '\n';
        for (std::size_t i = 0; i < expected.size() || i < actual.size(); ++i)
        {
            if (i < expected.size() && i < actual.size() && expected[i] == actual[i])
                continue;
            std::cerr << "  line " << i + 1 << " expected: "
                      << (i < expected.size() ? hex(expected[i]) : "<absent>")
                      << "\n          actual: "
                      << (i < actual.size() ? hex(actual[i]) : "<absent>") << '\n';
            break; // The first differing line is enough to diagnose this case.
        }
    }
};
} // namespace

int main()
{
    try
    {
        TemporaryDirectory temporary;
        Tests tests;
        // Expected UTF-8 is explicitly specified, NOT decoded from the fixture
        // by ICU. A broken decoder cannot generate its own passing expectation.
        const Lines original = {
            "This is a plain text file saved in UTF-16 LE encoding.",
            "If this text is not visible in the ONLYOFFICE editor, the bug has been reproduced.",
            u8"Line 3: special characters for extra testing — café, naïve, résumé."};
        for (bool bigEndian : {false, true})
        {
            for (bool bom : {false, true})
            {
                const std::string name = std::string("sample-utf16") +
                    (bigEndian ? "be" : "le") + (bom ? "-with-bom.txt" : "-no-bom.txt");
                // Without BOM this explicitly simulates the user's selection.
                // It is not a test of automatic encoding detection without BOM.
                tests.check(name, fs::path(TXT_BOM_FIXTURE_DIR) / name,
                            bigEndian ? Utf16BEIndex : Utf16LEIndex, original);
            }
        }

        struct Sample
        {
            std::u16string utf16;
            std::string utf8;
            Lines expected;
            const char* name;
        };
        const Sample samples[] = {
            // CRLF counts as ONE separator; the consecutive CRLF gives an empty
            // line. LF and CR also split lines; the last line has no terminator.
            {u"First café naïve résumé — 漢字 😀\r\n\r\nSecond\nThird\rLast",
             u8"First café naïve résumé — 漢字 😀\r\n\r\nSecond\nThird\rLast",
             {u8"First café naïve résumé — 漢字 😀", "", "Second", "Third", "Last"}, "mixed"},
            // Empty input and BOM-only input both produce zero lines.
            {u"", "", {}, "empty"},
            // Catches accidentally skipping the first character along with BOM.
            {u"A", "A", {"A"}, "single-character"}};

        const fs::path input = temporary.path / "input.txt";
        for (int encoding : {Utf16LEIndex, Utf16BEIndex, Utf8Index})
        {
            const std::string label = encoding == Utf8Index ? "UTF-8" :
                (encoding == Utf16BEIndex ? "UTF-16BE" : "UTF-16LE");
            for (const auto& sample : samples)
            {
                for (bool bom : {false, true})
                {
                    const auto bytes = encoding == Utf8Index
                        ? (bom ? std::string("\xEF\xBB\xBF") : std::string()) + sample.utf8
                        : utf16Bytes(sample.utf16, encoding == Utf16BEIndex, bom);
                    writeBytes(input, bytes);
                    tests.check(label + (bom ? " BOM " : " no BOM ") + sample.name,
                                input, encoding, sample.expected);
                }
            }
            if (encoding != Utf8Index)
            {
                // Even if the caller defaults to UTF-8, the UTF-16 BOM must win.
                // This exercises the same branch as automatic opening in x2t.
                writeBytes(input, utf16Bytes(samples[0].utf16, encoding == Utf16BEIndex, true));
                tests.check(label + " BOM overrides UTF-8 selection", input,
                            Utf8Index, samples[0].expected);
            }
        }
        std::cout << tests.total - tests.failed << '/' << tests.total << " tests passed\n";
        // Do not use assert(): release builds with NDEBUG must still fail tests.
        return tests.failed == 0 ? 0 : 1;
    }
    catch (const std::exception& error)
    {
        std::cerr << "Test setup failed: " << error.what() << '\n';
        return 2;
    }
}
