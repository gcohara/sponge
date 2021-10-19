#include "byte_stream.hh"

// Dummy implementation of a flow-controlled in-memory byte stream.

// For Lab 0, please replace with a real implementation that passes the
// automated checks run by `make check_lab0`.

// You will need to add private members to the class declaration in `byte_stream.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

ByteStream::ByteStream(const size_t cap) :
    buffer{},
    capacity{cap},
    space_left{cap},
    read_index{0},
    write_index{0},
    total_written_bytes{},
    total_read_bytes{}
{
    buffer.reserve(capacity);
}

size_t ByteStream::write(const string &data) { 
    size_t bytes_written_count {0};
    while(space_left && bytes_written_count < data.size()) {
        buffer[write_index++] = data[bytes_written_count++];
        write_index = write_index == capacity ? 0 : write_index;
        space_left--;
        total_written_bytes++;
    }
    return bytes_written_count;
}

//! \param[in] len bytes will be copied from the output side of the buffer
string ByteStream::peek_output(const size_t len) const {
    string output {};
    auto bytes_available {capacity - space_left};
    auto bytes_wanted {len};
    auto read_index_copy {read_index};
    while(bytes_wanted && bytes_available) {
        output.push_back(buffer[read_index_copy++]);
        read_index_copy = read_index_copy == capacity ? 0 : read_index_copy;
        bytes_wanted--;
        bytes_available--;
    }
    return output;
}

//! \param[in] len bytes will be removed from the output side of the buffer
void ByteStream::pop_output(const size_t len) {
    // what if asking to pop more bytes than are available??
    total_read_bytes += len;
    read_index = (read_index + len) % capacity;
    space_left += len;
    space_left = (space_left > capacity) ? capacity : space_left;
}

//! Read (i.e., copy and then pop) the next "len" bytes of the stream
//! \param[in] len bytes will be popped and returned
//! \returns a string
std::string ByteStream::read(const size_t len) {
    string output {};
    auto bytes_available {capacity - space_left};
    auto bytes_wanted {len};
    while(bytes_wanted && bytes_available) {
        output.push_back(buffer[read_index++]);
        read_index = (read_index == capacity) ? 0 : read_index;
        bytes_wanted--;
        bytes_available--;
        total_read_bytes++;
    }
    return output;
}

void ByteStream::end_input() {_input_done = true;}

bool ByteStream::input_ended() const { return _input_done; }

size_t ByteStream::buffer_size() const { return capacity - space_left; }

bool ByteStream::buffer_empty() const { return capacity == space_left; }

bool ByteStream::eof() const { return _input_done && buffer_empty(); }

size_t ByteStream::bytes_written() const { return total_written_bytes; }

size_t ByteStream::bytes_read() const { return total_read_bytes; }

size_t ByteStream::remaining_capacity() const { return space_left; }
