#ifndef __RRCOUNTER_HH__
#define __RRCOUNTER_HH__

#include <array>
#include <cassert>
#include <chrono>

namespace rrcounter {

template <size_t _size>
class RoundRobinCounter {
 public:
  size_t last_index = 0;
  std::array<int, _size> slots = {};
  int summation = 0;

  void invalidate(size_t current_index) {
    auto new_slot_id = current_index % _size;
    if (auto diff = current_index - this->last_index; diff > 0) {
      if (diff >= _size) {
        memset(this->slots.data(), 0, sizeof(this->slots));
      } else {
        const auto last_slot_id = this->last_index % _size;
        const auto invalidated_begin = (last_slot_id + 1) % _size;
        const auto invalidated_end = (new_slot_id + 1) % _size;
        if (invalidated_end > invalidated_begin) {
          memset(
              this->slots.data() + invalidated_begin, 0,
              (invalidated_end - invalidated_begin) * sizeof(int));
        } else {
          if (invalidated_end > 0) {
            memset(this->slots.data(), 0, (invalidated_end) * sizeof(int));
          }
          memset(
              this->slots.data() + invalidated_begin, 0,
              (_size - invalidated_begin) * sizeof(int));
        }
      }
      this->summation = 0;
      for (int i = 0; i < _size; i++) {
        this->summation += this->slots[i];
      }
      this->last_index = current_index;
    }
  }

  void put(size_t current_index, int count) {
    auto new_slot_id = current_index % _size;
    this->invalidate(current_index);
    this->slots[new_slot_id] += count;
    this->summation += count;
  }

  int summate(size_t current_index) {
    this->invalidate(current_index);
    return this->summation;
  }
};

template <size_t _size, typename _duration_type>
class TimeBasedCounter : public RoundRobinCounter<_size> {
 public:
  using duration = _duration_type;
  using period = typename _duration_type::period;

  void put(duration time, int count) {
    size_t x = time.count();
    RoundRobinCounter<_size>::put(x, count);
  }

  void put(int count) {
    const duration casted_time =
        std::chrono::time_point_cast<duration>(std::chrono::steady_clock::now())
            .time_since_epoch();
    this->put(casted_time, count);
  }

  int summate(duration time) {
    return RoundRobinCounter<_size>::summate(time.count());
  }

  int summate() {
    const duration casted_time =
        std::chrono::time_point_cast<duration>(std::chrono::steady_clock::now())
            .time_since_epoch();
    return this->summate(casted_time);
  }
};

}  // namespace rrcounter

#endif
