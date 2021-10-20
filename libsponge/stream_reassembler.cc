#include "stream_reassembler.hh"

// Dummy implementation of a stream reassembler.

// For Lab 1, please replace with a real implementation that passes the
// automated checks run by `make check_lab1`.

// You will need to add private members to the class declaration in `stream_reassembler.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

StreamReassembler::StreamReassembler(const size_t capacity) : _output(capacity), _capacity(capacity) {}

//! \details This function accepts a substring (aka a segment) of bytes,
//! possibly out-of-order, from the logical stream, and assembles any newly
//! contiguous substrings and writes them into the output stream in order.
void StreamReassembler::push_substring(const string &data, const size_t index, const bool eof) {
<<<<<<< HEAD
    if (index <= output_index) {
=======
<<<<<<< HEAD
    if (index <= output_index && ((index - output_index) < data.size())) {
>>>>>>> master
        // Take a substr to make sure we don't write the same bytes twice
        // We write what we can - any bytes we were unable to write would've taken us past
        // capacity, so we can silently discard them.
        auto bytes_written{output.write(data.substr(output_index - index))};
        output_index += bytes_written;
        for (size_t i{0}; i < bytes_written; i++) {
            buffer[buffer_head_idx++] = nullopt;
            buffer_head_idx = buffer_head_idx == capacity ? 0 : buffer_head_idx;
        }
        // need to check there's still some capacity, before or after we call this
        try_write_more_bytes();
    } else {
        buffer_bytes(data, index);
    }
    if (eof) {
        eof_idx = index + data.size();
    }
    if (output_index == eof_idx) {
        output.end_input();
    }
}
// Write as much as we can to output - this includes retrieving data from buffer.
size_t StreamReassembler::try_write_more_bytes() {
    string out{};
    while (buffer[buffer_head_idx].has_value()) {
        out.push_back(buffer[buffer_head_idx].value());
        buffer[buffer_head_idx] = nullopt;
        buffer_head_idx = (buffer_head_idx + 1) % capacity;
        output_index++;
    }
    output.write(out);
    return out.size();
}

void StreamReassembler::buffer_bytes(const string &data, const size_t index) {
    auto space_in_buffer{capacity - output.buffer_size()};
    auto max_bytes_to_buffer{space_in_buffer - (index - output_index)};
    for (size_t i{0}; i < data.size() && i < max_bytes_to_buffer; i++) {
        buffer[(index + i) % capacity] = data[i];
    }
=======
    DUMMY_CODE(data, index, eof);
>>>>>>> parent of ba43a48... 3 tests remain
}

size_t StreamReassembler::unassembled_bytes() const { return {}; }

bool StreamReassembler::empty() const { return {}; }
