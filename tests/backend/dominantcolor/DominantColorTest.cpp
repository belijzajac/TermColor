#include "DominantColorTest.h"
#include <QtTest/QtTest>

void DominantColorTest::testRemapping() {
    std::string name{"Thom"};
    QVERIFY(!name.empty());
}

QTEST_MAIN(DominantColorTest)
