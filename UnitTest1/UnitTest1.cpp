
#include "pch.h"
#include "CppUnitTest.h"
#include "../lr3_5/main.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{
    TEST_CLASS(UnitTest1)
    {
    public:

        // 1. Перевірка створення FileInfo
        TEST_METHOD(TestFileInfoConstructor)
        {
            FileInfo file("document", "txt", "2025-01-01",
                "12:30", false, false, false, 1500);

            Assert::AreEqual(string("document"), file.GetName());
            Assert::AreEqual(string("txt"), file.GetExtension());
            Assert::AreEqual(1500LL, file.GetSize());
        }

        // 2. Перевірка створення FileEntry
        TEST_METHOD(TestFileEntryConstructor)
        {
            FileInfo info("photo", "jpg", "2025-02-02",
                "10:00", false, true, false, 2000);

            FileEntry entry(info, "2025-02-03", true, false);

            Assert::AreEqual(string("2025-02-03"),
                entry.GetModificationDate());

            Assert::IsTrue(entry.GetSelected());
            Assert::IsFalse(entry.GetDeleted());
        }

        // 3. Перевірка додавання файлу до каталогу
        TEST_METHOD(TestAddFile)
        {
            Directory dir("Documents");

            FileInfo info("file", "txt", "2025-01-01",
                "12:00", false, false, false, 100);

            FileEntry entry(info, "2025-01-02", false, false);

            dir.AddFile(entry);

            Assert::AreEqual(1, dir.GetCount());
        }

        // 4. Перевірка видалення файлу з каталогу
        TEST_METHOD(TestDeleteFile)
        {
            Directory dir("Documents");

            FileInfo info("file", "txt", "2025-01-01",
                "12:00", false, false, false, 100);

            FileEntry entry(info, "2025-01-02", false, false);

            dir.AddFile(entry);
            dir.DeleteFile(0);

            Assert::AreEqual(0, dir.GetCount());
        }

        // 5. Перевірка пошуку за назвою
        TEST_METHOD(TestSearchByName)
        {
            Directory dir("Documents");

            FileInfo info("report", "docx", "2025-01-01",
                "12:00", false, false, false, 500);

            FileEntry entry(info, "2025-01-02", false, false);

            dir.AddFile(entry);

            Assert::AreEqual(0, dir.SearchByName("report"));
            Assert::AreEqual(-1, dir.SearchByName("unknown"));
        }

        // 6. Перевірка пошуку за розширенням
        TEST_METHOD(TestSearchByExtension)
        {
            Directory dir("Documents");

            FileInfo info("photo", "jpg", "2025-01-01",
                "12:00", false, false, false, 500);

            FileEntry entry(info, "2025-01-02", false, false);

            dir.AddFile(entry);

            Assert::AreEqual(0, dir.SearchByExtension("jpg"));
            Assert::AreEqual(-1, dir.SearchByExtension("pdf"));
        }

        // 7. Перевірка загального об'єму каталогу
        TEST_METHOD(TestTotalSize)
        {
            Directory dir("Documents");

            FileInfo info1("file1", "txt", "2025-01-01",
                "12:00", false, false, false, 100);

            FileInfo info2("file2", "txt", "2025-01-01",
                "12:00", false, false, false, 200);

            dir.AddFile(FileEntry(info1, "2025-01-02", false, false));
            dir.AddFile(FileEntry(info2, "2025-01-02", false, false));

            Assert::AreEqual(300LL, dir.TotalSize());
        }

        // 8. Перевірка об'єднання каталогів
        TEST_METHOD(TestDirectoryUnion)
        {
            Directory dir1("First");
            Directory dir2("Second");

            FileInfo info1("file1", "txt", "2025-01-01",
                "12:00", false, false, false, 100);

            FileInfo info2("file2", "txt", "2025-01-01",
                "12:00", false, false, false, 200);

            FileEntry file1(info1, "2025-01-02", false, false);
            FileEntry file2(info2, "2025-01-02", false, false);

            dir1.AddFile(file1);
            dir2.AddFile(file2);

            Directory result = dir1 + dir2;

            Assert::AreEqual(2, result.GetCount());
            Assert::AreEqual(1, dir1.GetCount());
            Assert::AreEqual(1, dir2.GetCount());
        }

        // 9. Перевірка перетину каталогів
        TEST_METHOD(TestDirectoryIntersection)
        {
            Directory dir1("First");
            Directory dir2("Second");

            FileInfo info("file", "txt", "2025-01-01",
                "12:00", false, false, false, 100);

            FileEntry entry(info, "2025-01-02", false, false);

            dir1.AddFile(entry);
            dir2.AddFile(entry);

            Directory result = dir1 * dir2;

            Assert::AreEqual(1, result.GetCount());
        }

        // 10. Перевірка різниці каталогів
        TEST_METHOD(TestDirectoryDifference)
        {
            Directory dir1("First");
            Directory dir2("Second");

            FileInfo info1("file1", "txt", "2025-01-01",
                "12:00", false, false, false, 100);

            FileInfo info2("file2", "txt", "2025-01-01",
                "12:00", false, false, false, 200);

            FileEntry file1(info1, "2025-01-02", false, false);
            FileEntry file2(info2, "2025-01-02", false, false);

            dir1.AddFile(file1);
            dir2.AddFile(file2);

            Directory result = dir1 - dir2;

            Assert::AreEqual(1, result.GetCount());
            Assert::AreEqual(0, result.SearchByName("file1"));
            Assert::AreEqual(-1, result.SearchByName("file2"));
        }

        // 11. Перевірка групи видалених файлів
        TEST_METHOD(TestGroupByDeleted)
        {
            Directory dir("Documents");

            FileInfo info1("file1", "txt", "2025-01-01",
                "12:00", false, false, false, 100);

            FileInfo info2("file2", "txt", "2025-01-01",
                "12:00", false, false, false, 200);

            dir.AddFile(FileEntry(info1, "2025-01-02", false, true));
            dir.AddFile(FileEntry(info2, "2025-01-02", false, false));

            Group result = Group::ByDeleted(dir, true);

            Assert::AreEqual(1, result.GetCount());
        }

        // 12. Перевірка групи за атрибутом
        TEST_METHOD(TestGroupByAttribute)
        {
            Directory dir("Documents");

            FileInfo info("hidden", "txt", "2025-01-01",
                "12:00", false, true, false, 100);

            dir.AddFile(FileEntry(info, "2025-01-02", false, false));

            Group result = Group::ByAttribute(dir, 2);

            Assert::AreEqual(1, result.GetCount());
        }

        // 13. Перевірка групи за датою створення
        TEST_METHOD(TestGroupByDate)
        {
            Directory dir("Documents");

            FileInfo info("old", "txt", "2024-01-01",
                "12:00", false, false, false, 100);

            dir.AddFile(FileEntry(info, "2024-01-02", false, false));

            Group result = Group::ByDate(dir, "2025-01-01", true);

            Assert::AreEqual(1, result.GetCount());
        }

        // 14. Перевірка групи за розміром
        TEST_METHOD(TestGroupBySize)
        {
            Directory dir("Documents");

            FileInfo info("small", "txt", "2025-01-01",
                "12:00", false, false, false, 100);

            dir.AddFile(FileEntry(info, "2025-01-02", false, false));

            Group result = Group::BySize(dir, 500, true);

            Assert::AreEqual(1, result.GetCount());
        }
    };
}
