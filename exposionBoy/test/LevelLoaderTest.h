#ifndef LEVELLOADERTEST_H
#define LEVELLOADERTEST_H

#include <cppunit/extensions/HelperMacros.h>

#include "macros.h"
#include "brief.h"

#include "LevelLoader.h"

class LevelLoaderTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(LevelLoaderTest);
    CPPUNIT_TEST(testCheckWallsString);
    CPPUNIT_TEST_SUITE_END();

private:
    void testCheckWallsString() {
        std::string json = "{"
            "\"numCellsX\": 8,"
            "\"numCellsY\": 2,"
            "\"cellSizeX\": 1.5,"
            "\"cellSizeY\": 1.5,"
            "\"ceilingHeight\": 2.5,"
            "\"wallThickness\": 0.2,"
            "\"walls\": ["
            "    \"   1       1   \","
            "    \" x1x x x x x1x \","
            "    \" 1           1 \","
            "],"
            "\"spawnLocations\": []"
        "}";
        
//        *this = LevelLoader(json);
    }
};

#endif /* LEVELLOADERTEST_H */

