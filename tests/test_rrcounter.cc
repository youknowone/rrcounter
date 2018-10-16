
#include <array>
#include <deque>
#include <unordered_map>
#include <vector>

#include <rrcounter/rrcounter.hh>

#include <gtest/gtest.h>

// notable examples are commented with **Example**

namespace {

TEST(rrcounter, circular_basic) {
  rrcounter::RoundRobinCounter<4> counter;
  
  counter.put(0, 1);
  counter.put(0, 2);
  counter.put(0, 3);
  ASSERT_EQ(counter.summate(0), 6);
  ASSERT_EQ(counter.summate(1), 6);
  ASSERT_EQ(counter.summate(2), 6);
  ASSERT_EQ(counter.summate(4), 0);
}
  
TEST(rrcounter, circular_boundary) {
  rrcounter::RoundRobinCounter<4> counter;
  
  counter.put(0, 11);
  counter.put(1, 7);
  counter.put(2, 13);
  counter.put(3, 19);
  
  ASSERT_EQ(counter.summate(6), 19);
}

TEST(rrcounter, time_based) {
  rrcounter::TimeBasedCounter<4, std::chrono::seconds> counter;
  
  counter.put(std::chrono::seconds(0), 11);
  counter.put(std::chrono::seconds(1), 7);
  counter.put(std::chrono::seconds(2), 13);
  counter.put(std::chrono::seconds(3), 19);
  
  ASSERT_EQ(counter.summate(std::chrono::seconds(6)), 19);
}
  

}  // namespace
