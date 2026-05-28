#include <catch2/catch_test_macros.hpp>

// A trivial test, just to prove the harness runs end-to-end.
// We'll replace this with real SEM tests soon.
TEST_CASE("sanity: arithmetic works", "[sanity]") { REQUIRE(2 + 2 == 4); }
