/**
 * Copyright (C) 2016 Virgil Security Inc.
 *
 * Lead Maintainer: Virgil Security Inc. <support@virgilsecurity.com>
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     (1) Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *
 *     (2) Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in
 *     the documentation and/or other materials provided with the
 *     distribution.
 *
 *     (3) Neither the name of the copyright holder nor the names of its
 *     contributors may be used to endorse or promote products derived from
 *     this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ''AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef VIRGIL_CRYPTO_FOUNDATION_ERROR_H
#define VIRGIL_CRYPTO_FOUNDATION_ERROR_H

#include <system_error>

#include <virgil/crypto/VirgilCryptoError.h>

namespace virgil { namespace crypto { namespace foundation {

class VirgilSystemCryptoErrorCategory : public std::error_category {
public:
    const char* name() const noexcept override;

    std::string message(int ev) const noexcept override;
};

const VirgilSystemCryptoErrorCategory& system_crypto_category() noexcept;

inline int system_crypto_handler_get_result(int error) {
    if (error >= 0) { return error; }
    throw VirgilCryptoException(error, system_crypto_category());
}

inline void system_crypto_handler(int error) {
    (void)system_crypto_handler_get_result(error);
}

template<typename CatchHandler>
inline int system_crypto_handler_get_result(int error, CatchHandler catch_handler) {
    if (error >= 0) { return error; }
    try {
        throw VirgilCryptoException(error, system_crypto_category());
    } catch (...) {
        catch_handler(error);
        return 0;
    }
}

template<typename CatchHandler>
inline void system_crypto_handler(int error, CatchHandler catch_handler) {
    (void)system_crypto_handler_get_result<CatchHandler>(error, catch_handler);
}

}}}

#endif //VIRGIL_CRYPTO_FOUNDATION_ERROR_H
