#ifndef SPONGE_LIBSPONGE_STREAM_REASSEMBLER_HH
#define SPONGE_LIBSPONGE_STREAM_REASSEMBLER_HH

#include "byte_stream.hh"

#include <bits/stdint-uintn.h>
#include <cstdint>
#include <string>
#include <vector>
#include <optional>

//! \brief A class that assembles a series of excerpts from a byte stream (possibly out of order,
//! possibly overlapping) into an in-order byte stream.
class StreamReassembler {
  private:
    // Your code here -- add private members as necessary.
    //!< Buffer to store unassembled bytes. Pair is of byte, index
    std::vector<std::optional<uint8_t>> buffer;
    size_t buffer_head_idx; //!< Head of the buffer. The output index of this element is output_index.
    ByteStream output;  //!< The reassembled in-order byte stream
    //!< The current index for the output bytestream. Any byte with index before this has been written.
    size_t output_index;
    size_t eof_idx;
    size_t capacity;    //!< The maximum number of bytes
    //!< If we were able to write to the output ByteStream, try and write some bytes from the buffer. Called within push_substring
    size_t try_write_more_bytes();
    //!< If we were unable to write to the output ByteStream, store what we have room for in the buffer. Called within push_substring
    void buffer_bytes(const std::string& data, const size_t index); 

  public:
    //! \brief Construct a `StreamReassembler` that will store up to `capacity` bytes.
    //! \note This capacity limits both the bytes that have been reassembled,
    //! and those that have not yet been reassembled.
    StreamReassembler(const size_t capacity);

    //! \brief Receive a substring and write any newly contiguous bytes into the stream.
    //!
    //! The StreamReassembler will stay within the memory limits of the `capacity`.
    //! Bytes that would exceed the capacity are silently discarded.
    //!
    //! \param data the substring
    //! \param index indicates the index (place in sequence) of the first byte in `data`
    //! \param eof the last byte of `data` will be the last byte in the entire stream
    void push_substring(const std::string &data, const uint64_t index, const bool eof);

    //! \name Access the reassembled byte stream
    //!@{
    const ByteStream &stream_out() const { return output; }
    ByteStream &stream_out() { return output; }
    //!@}

    //! The number of bytes in the substrings stored but not yet reassembled
    //!
    //! \note If the byte at a particular index has been pushed more than once, it
    //! should only be counted once for the purpose of this function.
    size_t unassembled_bytes() const;

    //! \brief Is the internal state empty (other than the output stream)?
    //! \returns `true` if no substrings are waiting to be assembled
    bool empty() const;
};

#endif  // SPONGE_LIBSPONGE_STREAM_REASSEMBLER_HH
