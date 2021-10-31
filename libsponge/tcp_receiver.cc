#include "tcp_receiver.hh"

#include <cstdlib>
#include <iostream>
#include <optional>

// Dummy implementation of a TCP receiver

// For Lab 2, please replace with a real implementation that passes the
// automated checks run by `make check_lab2`.

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

void TCPReceiver::segment_received(const TCPSegment &seg) {
    auto header{seg.header()};
    auto payload{seg.payload()};
    auto seqno{header.seqno};
    if (header.syn && !syn_received) {
        syn_received = true;
        initial_seqno = make_optional(seqno);
        abs_seqno++;
        seqno = seqno + 1;
    }
    if (syn_received) {
        auto bytestream_index{unwrap(seqno, initial_seqno.value(), abs_seqno) - 1};
        reassembler.push_substring(string(payload.str()), bytestream_index, header.fin);
        abs_seqno = reassembler.stream_out().bytes_written() + 1;
        // account for FIN bit
        if (reassembler.stream_out().input_ended()) {
            abs_seqno++;
        }
    }
}

optional<WrappingInt32> TCPReceiver::ackno() const {
    return initial_seqno.has_value() ? make_optional(wrap(abs_seqno, initial_seqno.value())) : nullopt;
}

size_t TCPReceiver::window_size() const { return reassembler.stream_out().remaining_capacity(); }
