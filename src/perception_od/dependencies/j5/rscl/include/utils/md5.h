/* MD5
 converted to C++ class by Frank Thilo (thilo@unix-ag.org)
 for bzflag (http://www.bzflag.org)

   based on:

   md5.h and md5.c
   reference implementation of RFC 1321

   Copyright (C) 1991-2, RSA Data Security, Inc. Created 1991. All
rights reserved.

License to copy and use this software is granted provided that it
is identified as the "RSA Data Security, Inc. MD5 Message-Digest
Algorithm" in all material mentioning or referencing this software
or this function.

License is also granted to make and use derivative works provided
that such works are identified as "derived from the RSA Data
Security, Inc. MD5 Message-Digest Algorithm" in all material
mentioning or referencing the derived work.

RSA Data Security, Inc. makes no representations concerning either
the merchantability of this software or the suitability of this
software for any particular purpose. It is provided "as is"
without express or implied warranty of any kind.

These notices must be retained in any copies of any part of this
documentation and/or software.

*/

#ifndef AVP_MODULE_PERCEPTION_INCLUDE_UTILS_MD5_H_
#define AVP_MODULE_PERCEPTION_INCLUDE_UTILS_MD5_H_

#include <stdint.h>
#include <string>
#include <iostream>

// a small class for calculating MD5 hashes of strings or byte arrays
// it is not meant to be fast or secure
//
// usage: 1) feed it blocks of uchars with update()
//      2) finalize()
//      3) get hexdigest() string
//      or
//      MD5(std::string).hexdigest()
//
// assumes that char is 8 bit and int is 32 bit
namespace senseAD {
class MD5 {
 public:
    MD5();
    explicit MD5(const std::string &text);
    void update(const unsigned char *buf, int length);
    void update(const char *buf, int length);
    void update(const std::string &data);
    MD5 &finalize();
    std::string hexdigest() const;
    friend std::ostream &operator<<(std::ostream &, MD5 md5);

 private:
    static const int kBlocksize = 64;

    void transform(const uint8_t *block);
    static void decode(uint32_t *output, const uint8_t *input, int len);
    static void encode(uint8_t *output, const uint32_t *input, int len);

    bool finalized_;
    uint8_t buffer_[kBlocksize] { 0 };  // bytes that didn't fit in last 64 byte chunk
    uint32_t count_[2];                 // 64bit counter for number of bits (lo, hi)
    uint32_t state_[4];                 // digest so far
    uint8_t digest_[16] { 0 };          // the result
};

std::string md5(const std::string str);
}
#endif
