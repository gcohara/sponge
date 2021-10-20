#include "stream_reassembler.hh"

#include <cstddef>
#include <limits>
#include <optional>

// Dummy implementation of a stream reassembler.

// For Lab 1, please replace with a real implementation that passes the
// automated checks run by `make check_lab1`.

// You will need to add private members to the class declaration in `stream_reassembler.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

size_t attempt_write_to_output(const string &data, const size_t index);

StreamReassembler::StreamReassembler(const size_t cap)
    : buffer(cap, nullopt), buffer_head_idx{0}, output(cap), output_index{0}, eof_idx{numeric_limits<size_t>::max()}, capacity(cap)  {}

//! \details This function accepts a substring (aka a segment) of bytes,
//! possibly out-of-order, from the logical stream, and assembles any newly
//! contiguous substrings and writes them into the output stream in order.
void StreamReassembler::push_substring(const string &data, const size_t index, const bool eof) {
    // Check that a.) we are ready to write out this string rather than buffer it
    // b.) there is actually anything to write - we may have already written the indexes the string contains
    if (index <= output_index && (output_index < (index + data.size())))
    {
        // Take a substr to make sure we don't write the same bytes twice
        // We write what we can - any bytes we were unable to write would've taken us past
        // capacity, so we can silently discard them.
        auto bytes_to_write {data.substr(output_index - index)};
        auto bytes_written {output.write(bytes_to_write)};
        output_index += bytes_written;
        // We may have written some bytes that are also in the buffer, so nullify those bytes
        for (size_t i{0}; i < bytes_written; i++) {
            buffer[buffer_head_idx++] = nullopt;
            buffer_head_idx = (buffer_head_idx == capacity) ? 0 : buffer_head_idx;
        }
        // Only try and write more bytes if the ByteStream has space
        // Note this check is unnecessary (the case is handled implicitly in try_write_more_bytes)
        // but is included here for the sake of explicitness
        if (bytes_written == (bytes_to_write.size())) {
            try_write_more_bytes();
        }
    }
    else if (output_index < (index + data.size()))
    {
        buffer_bytes(data, index);
    }
    if (eof)
    {
        eof_idx = index + data.size();
    }
    if (output_index == eof_idx)
    {
        output.end_input();
    }
}
// Write as much as we can to output - this includes retrieving data from buffer.
size_t StreamReassembler::try_write_more_bytes() {
    string out{};
    // While there are contiguous bytes in buffer, place them in string
    while (buffer[buffer_head_idx].has_value())
    {
        out.push_back(buffer[buffer_head_idx].value());
        buffer[buffer_head_idx] = nullopt;
        buffer_head_idx++;
        buffer_head_idx = buffer_head_idx == capacity ? 0 : buffer_head_idx;
        output_index++;
    }
    output.write(out);
    return out.size();
}

void StreamReassembler::buffer_bytes(const string &data, const size_t index) {
    auto space_in_buffer{capacity - output.buffer_size()};
    auto max_bytes_to_buffer{space_in_buffer - (index - output_index)};
    for (size_t i{0}; i < data.size() && i < max_bytes_to_buffer; i++)
    {
        buffer[(index + i) % capacity] = data[i];
    }
}

size_t StreamReassembler::unassembled_bytes() const {
    size_t output_count{0};
    for (auto x : buffer)
    {
        output_count += x.has_value() ? 1 : 0;
    }
    return output_count;
}

bool StreamReassembler::empty() const { return unassembled_bytes() == 0; }
