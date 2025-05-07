#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "./include/doctest.h"
#include "TimeSharedPtr.h"
#include <thread>

struct Dummy {
    int val;
    Dummy(int v) : val(v) {}
};

TEST_CASE("TimedSharedPtr expires after given duration") {
    TimedSharedPtr<Dummy> p(new Dummy(10), 50);
    CHECK(p.get() != nullptr);
    std::this_thread::sleep_for(std::chrono::milliseconds(60));
    CHECK(p.get() == nullptr);
}

TEST_CASE("TimedSharedPtr correctly tracks use_count") {
    TimedSharedPtr<int> p(new int(100), 500);
    CHECK(p.use_count() == 1);

    TimedSharedPtr<int> q = p;
    CHECK(p.use_count() == 2);
    CHECK(q.use_count() == 2);
}

TEST_CASE("TimedSharedPtr returns the correct pointer before expiration") {
    TimedSharedPtr<int> p(new int(42), 100);
    int* valBefore = p.get();
    CHECK(valBefore != nullptr);
    CHECK(*valBefore == 42);
}
