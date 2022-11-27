// g++ -std=c++11
#include<cmath> // to use round etc
#include <cstdlib> // to use rand
#include <chrono>
#include <sys/time.h>

#include "rmath/dtype/bigint/bigint-internal.h"
#include "rmath/dtype/bigint/util.h"
// #include "rmath/dtype/bigint/fromstring.cc"

static const char kConversionChars[] = "0123456789abcdefghijklmnopqrstuvwxyz";

class RNG {
 public:
  RNG() = default;

  void Initialize(int64_t seed) {
    state0_ = MurmurHash3(static_cast<uint64_t>(seed));
    state1_ = MurmurHash3(~state0_);
    CHECK(state0_ != 0 || state1_ != 0);
  }

  uint64_t NextUint64() {
    XorShift128(&state0_, &state1_);
    return static_cast<uint64_t>(state0_ + state1_);
  }

  static inline void XorShift128(uint64_t* state0, uint64_t* state1) {
    uint64_t s1 = *state0;
    uint64_t s0 = *state1;
    *state0 = s0;
    s1 ^= s1 << 23;
    s1 ^= s1 >> 17;
    s1 ^= s0;
    s1 ^= s0 >> 26;
    *state1 = s1;
  }

  static uint64_t MurmurHash3(uint64_t h) {
    h ^= h >> 33;
    h *= uint64_t{0xFF51AFD7ED558CCD};
    h ^= h >> 33;
    h *= uint64_t{0xC4CEB9FE1A85EC53};
    h ^= h >> 33;
    return h;
  }

 private:
  uint64_t state0_;
  uint64_t state1_;
};

RNG rng_;


// v8::bigint::ProcessorImpl* processor() {
// return 
// }

void GenerateRandomString(char* str, int len, int radix) {
    DCHECK(2 <= radix && radix <= 36);
    if (len == 0) return;
    uint64_t random;
    int available_bits = 0;
    const int char_bits = v8::bigint::BitLength(radix - 1);
    const uint64_t char_mask = (1u << char_bits) - 1u;
    for (int i = 0; i < len; i++) {
      while (true) {
        if (available_bits < char_bits) {
          random = rng_.NextUint64();
          available_bits = 64;
        }
        int next_char = static_cast<int>(random & char_mask);
        random = random >> char_bits;
        available_bits -= char_bits;
        if (next_char >= radix) continue;
        *str = kConversionChars[next_char];
        str++;
        break;
      };
    }
  }

int main(int argc, const char * argv[]) {
    
    // std::chrono::milliseconds ms = std::chrono::duration_cast< std::chrono::milliseconds >(
    //     std::chrono::system_clock::now().time_since_epoch()
    // );
    
    struct timeval tp;
    gettimeofday(&tp, NULL);
    long int ms = tp.tv_sec * 1000 + tp.tv_usec / 1000;
    std::cout << "tp: " << ms << std::endl; 

    rng_.Initialize(ms);

    std::unique_ptr<v8::bigint::Processor, v8::bigint::Processor::Destroyer> processor_;
    processor_.reset(v8::bigint::Processor::New(new v8::bigint::Platform()));
    
    v8::bigint::ProcessorImpl* processor = static_cast<v8::bigint::ProcessorImpl*>(processor_.get());

    std::cout << "Hello, World!\n";
    int kDigitBits = 10;
    int size = 10;

    constexpr uint8_t radixes[] = {3,  5,  6,  7,  9,  10, 11, 12, 13, 14, 15,
                                     17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27,
                                     28, 29, 30, 31, 33, 34, 35, 36, 10, 10};
    int radix_index = (rng_.NextUint64() & 31);
    int radix = 10 ;// radixes[radix_index];
    int num_chars = 2;// std::round(size * kDigitBits / std::log2(radix));
    std::unique_ptr<char[]> chars(new char[num_chars]);
    GenerateRandomString(chars.get(), num_chars, radix);
    std::cout << "radix : " << radix << ", chars : " << chars << std::endl;

    const char* start = chars.get();
    const char* end = chars.get() + num_chars;

    v8::bigint::FromStringAccumulator accumulator(2);
    v8::bigint::FromStringAccumulator ref_accumulator(2);


    accumulator.Parse(start, end, radix);
    ref_accumulator.Parse(start, end, radix);
    v8::bigint::ScratchDigits result(accumulator.ResultLength());
    v8::bigint::ScratchDigits reference(ref_accumulator.ResultLength());
    processor->FromStringLarge(result, &accumulator);
    processor->FromStringClassic(reference, &ref_accumulator);

    std::cout << "result : " << result.digits() << std::endl;
    std::cout << "referece : " << reference.digits() << std::endl;

    return 0;
}
