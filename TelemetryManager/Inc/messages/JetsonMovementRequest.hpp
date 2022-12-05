/** THIS IS AN AUTOMATICALLY GENERATED FILE.  DO NOT MODIFY
 * BY HAND!!
 *
 * Generated by lcm-gen
 **/

#ifndef __messages_JetsonMovementRequest_hpp__
#define __messages_JetsonMovementRequest_hpp__

#include <lcm/lcm_coretypes.h>

#include "messages/Header.hpp"

namespace messages
{

class JetsonMovementRequest
{
    public:
        messages::Header header;

        int8_t     req;

        uint8_t    crc[4];

    public:
        /**
         * Encode a message into binary form.
         *
         * @param buf The output buffer.
         * @param offset Encoding starts at thie byte offset into @p buf.
         * @param maxlen Maximum number of bytes to write.  This should generally be
         *  equal to getEncodedSize().
         * @return The number of bytes encoded, or <0 on error.
         */
        inline int encode(void *buf, int offset, int maxlen) const;

        /**
         * Check how many bytes are required to encode this message.
         */
        inline int getEncodedSize() const;

        /**
         * Decode a message from binary form into this instance.
         *
         * @param buf The buffer containing the encoded message.
         * @param offset The byte offset into @p buf where the encoded message starts.
         * @param maxlen The maximum number of bytes to read while decoding.
         * @return The number of bytes decoded, or <0 if an error occured.
         */
        inline int decode(const void *buf, int offset, int maxlen);

        /**
         * Retrieve the 64-bit fingerprint identifying the structure of the message.
         * Note that the fingerprint is the same for all instances of the same
         * message type, and is a fingerprint on the message type definition, not on
         * the message contents.
         */
        inline static int64_t getHash();

        /**
         * Returns "JetsonMovementRequest"
         */
        inline static const char* getTypeName();

        // LCM support functions. Users should not call these
        inline int _encodeNoHash(void *buf, int offset, int maxlen) const;
        inline int _getEncodedSizeNoHash() const;
        inline int _decodeNoHash(const void *buf, int offset, int maxlen);
        inline static uint64_t _computeHash(const __lcm_hash_ptr *p);
};

int JetsonMovementRequest::encode(void *buf, int offset, int maxlen) const
{
    int pos = 0, tlen;
    int64_t hash = getHash();

    tlen = __int64_t_encode_array(buf, offset + pos, maxlen - pos, &hash, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = this->_encodeNoHash(buf, offset + pos, maxlen - pos);
    if (tlen < 0) return tlen; else pos += tlen;

    return pos;
}

int JetsonMovementRequest::decode(const void *buf, int offset, int maxlen)
{
    int pos = 0, thislen;

    int64_t msg_hash;
    thislen = __int64_t_decode_array(buf, offset + pos, maxlen - pos, &msg_hash, 1);
    if (thislen < 0) return thislen; else pos += thislen;
    if (msg_hash != getHash()) return -1;

    thislen = this->_decodeNoHash(buf, offset + pos, maxlen - pos);
    if (thislen < 0) return thislen; else pos += thislen;

    return pos;
}

int JetsonMovementRequest::getEncodedSize() const
{
    return 8 + _getEncodedSizeNoHash();
}

int64_t JetsonMovementRequest::getHash()
{
    static int64_t hash = static_cast<int64_t>(_computeHash(NULL));
    return hash;
}

const char* JetsonMovementRequest::getTypeName()
{
    return "JetsonMovementRequest";
}

int JetsonMovementRequest::_encodeNoHash(void *buf, int offset, int maxlen) const
{
    int pos = 0, tlen;

    tlen = this->header._encodeNoHash(buf, offset + pos, maxlen - pos);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __boolean_encode_array(buf, offset + pos, maxlen - pos, &this->req, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __byte_encode_array(buf, offset + pos, maxlen - pos, &this->crc[0], 4);
    if(tlen < 0) return tlen; else pos += tlen;

    return pos;
}

int JetsonMovementRequest::_decodeNoHash(const void *buf, int offset, int maxlen)
{
    int pos = 0, tlen;

    tlen = this->header._decodeNoHash(buf, offset + pos, maxlen - pos);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __boolean_decode_array(buf, offset + pos, maxlen - pos, &this->req, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __byte_decode_array(buf, offset + pos, maxlen - pos, &this->crc[0], 4);
    if(tlen < 0) return tlen; else pos += tlen;

    return pos;
}

int JetsonMovementRequest::_getEncodedSizeNoHash() const
{
    int enc_size = 0;
    enc_size += this->header._getEncodedSizeNoHash();
    enc_size += __boolean_encoded_array_size(NULL, 1);
    enc_size += __byte_encoded_array_size(NULL, 4);
    return enc_size;
}

uint64_t JetsonMovementRequest::_computeHash(const __lcm_hash_ptr *p)
{
    const __lcm_hash_ptr *fp;
    for(fp = p; fp != NULL; fp = fp->parent)
        if(fp->v == JetsonMovementRequest::getHash)
            return 0;
    const __lcm_hash_ptr cp = { p, JetsonMovementRequest::getHash };

    uint64_t hash = 0x2136108187a868ebLL +
         messages::Header::_computeHash(&cp);

    return (hash<<1) + ((hash>>63)&1);
}

}

#endif