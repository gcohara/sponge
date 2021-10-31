#include "wrapping_integers.hh"

#include <bits/stdint-uintn.h>
#include <cstdint>
#include <iostream>
#include <limits>

// Dummy implementation of a 32-bit wrapping integer

// For Lab 2, please replace with a real implementation that passes the
// automated checks run by `make check_lab2`.

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

//! Transform an "absolute" 64-bit sequence number (zero-indexed) into a WrappingInt32
//! \param n The input absolute 64-bit sequence number
//! \param isn The initial sequence number
WrappingInt32 wrap(uint64_t n, WrappingInt32 isn) {
    uint64_t seq_64{n + isn.raw_value()};
    WrappingInt32 out(seq_64 & UINT32_MAX);
    return out;
}

//! Transform a WrappingInt32 into an "absolute" 64-bit sequence number (zero-indexed)
//! \param n The relative sequence number
//! \param isn The initial sequence number
//! \param checkpoint A recent absolute 64-bit sequence number
//! \returns the 64-bit sequence number that wraps to `n` and is closest to `checkpoint`
//!
//! \note Each of the two streams of the TCP connection has its own ISN. One stream
//! runs from the local TCPSender to the remote TCPReceiver and has one ISN,
//! and the other stream runs from the remote TCPSender to the local TCPReceiver and
//! has a different ISN.
uint64_t unwrap(WrappingInt32 n, WrappingInt32 isn, uint64_t checkpoint) {
    // The lower 32 bits of the output are simple to get.
    auto out_lower_32{n.raw_value() - isn.raw_value()};
    auto out_candidate_higher_32{checkpoint & (UINT64_MAX - UINT32_MAX)};
    auto out_candidate{out_candidate_higher_32 + out_lower_32};
    if (out_candidate > checkpoint && ((out_candidate - checkpoint) > (UINT32_MAX / 2)) &&
        (out_candidate_higher_32 != 0)) {
        out_candidate -= (1ul << 32);
    } else if (out_candidate < checkpoint && ((checkpoint - out_candidate) > (UINT32_MAX / 2)) &&
               (out_candidate_higher_32 != (UINT64_MAX - UINT32_MAX))) {
        out_candidate += (1ul << 32);
    }
    return out_candidate;
}
