#include "stdafx.h"

using namespace	Microsoft::VisualStudio::TestTools::UnitTesting;
using FileSystem::String;
using FileSystem::Directory;
using FileSystem::StringArray;

namespace FileSystemTest {
    [TestClass]
    public ref class DirectoryTest {
    private:
        TestContext^ testContextInstance;

    public: 
        property Microsoft::VisualStudio::TestTools::UnitTesting::TestContext^ TestContext {
            Microsoft::VisualStudio::TestTools::UnitTesting::TestContext^ get() {
                return testContextInstance;
            }
            System::Void set(Microsoft::VisualStudio::TestTools::UnitTesting::TestContext^ value) {
                testContextInstance = value;
            }
        };

    public:
        [TestMethod]
        void CurrentDirectoryStringIsNotEmpty() {
            String path = Directory::GetCurrentDirectoryS();
            Assert::IsFalse(path.empty());
        };

        [TestMethod]
        void CreateNewDirectoryInCurrent() {
            String currentDir = Directory::GetCurrentDirectoryS();
            String newDir = currentDir + TEXT("\\new_dir_1");
            Directory::CreateDirectory(newDir);
        }

        [TestMethod]
        void FilesListOfNewDirectoryIsEmpty() {
            String currentDir = Directory::GetCurrentDirectoryS();
            String newDir = currentDir + TEXT("\\new_dir_1");
            Directory::CreateDirectory(newDir);
            StringArray files = Directory::GetFilesInDirectory(newDir);
            Assert::IsTrue(files.empty());
        }

        [TestMethod]
        void FilesListOfDirectoryWithFewItemsIsNotEmpty() {
            String path = Directory::GetCurrentDirectoryS();
            StringArray files = Directory::GetFilesInDirectory(path);
            Assert::IsFalse(files.empty());
        }

        [TestMethod]
        void InSubdirectoryFilesAndFoldersMoreThanOnlyFiles() {
            String path = Directory::GetCurrentDirectoryS();
            StringArray files = Directory::GetFilesInDirectory(path);
            int filesAmount = (int) files.size();
            StringArray filesAndFolders = Directory::GetFilesInDirectory(path, true);
            int filesAndFoldersAmount = (int) filesAndFolders.size();
            Assert::IsTrue(filesAndFoldersAmount > filesAmount);
        }

        [TestMethod]
        void CountFilesInDirectory() {
            CString path = (CString) Directory::GetCurrentDirectoryS().c_str();
            Directory::CreateDirectories(path + _T("\\temp_ dir"));
            int count = Directory::GetFilesCount(path);
            Assert::IsTrue(count > 0);
        }
    };
}
