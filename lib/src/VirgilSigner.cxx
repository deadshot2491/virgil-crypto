/**
 * Copyright (C) 2015 Virgil Security Inc.
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

#include <virgil/crypto/VirgilSigner.h>

#include <virgil/crypto/VirgilCryptoException.h>

#include <virgil/crypto/foundation/PolarsslException.h>
#include <virgil/crypto/foundation/VirgilHash.h>
#include <virgil/crypto/foundation/VirgilAsymmetricCipher.h>
#include <virgil/crypto/foundation/asn1/VirgilAsn1Reader.h>
#include <virgil/crypto/foundation/asn1/VirgilAsn1Writer.h>

using virgil::crypto::VirgilSigner;
using virgil::crypto::VirgilByteArray;
using virgil::crypto::VirgilCryptoException;

using virgil::crypto::foundation::PolarsslException;
using virgil::crypto::foundation::VirgilHash;
using virgil::crypto::foundation::VirgilAsymmetricCipher;
using virgil::crypto::foundation::asn1::VirgilAsn1Reader;
using virgil::crypto::foundation::asn1::VirgilAsn1Writer;

VirgilSigner::VirgilSigner(const VirgilHash& hash) : hash_(hash) {
}

VirgilByteArray VirgilSigner::sign(const VirgilByteArray& data, const VirgilByteArray& privateKey,
        const VirgilByteArray& privateKeyPassword) {
    // Calculate data digest
    VirgilByteArray digest = hash_.hash(data);
    // Prepare cipher
    VirgilAsymmetricCipher cipher;
    cipher.setPrivateKey(privateKey, privateKeyPassword);
    // Sign digest
    VirgilByteArray digestSign = cipher.sign(digest, hash_.type());
    // Create sign
    VirgilAsn1Writer asn1Writer;
    size_t asn1Len = 0;
    asn1Len += asn1Writer.writeOctetString(digestSign);
    asn1Len += hash_.asn1Write(asn1Writer);
    asn1Len += asn1Writer.writeSequence(asn1Len);
    // Return sign as binary data
    return asn1Writer.finish();
}

bool VirgilSigner::verify(const VirgilByteArray& data, const VirgilByteArray& sign, const VirgilByteArray& publicKey) {
    // Read sign
    try {
        VirgilAsn1Reader asn1Reader(sign);
        asn1Reader.readSequence();
        VirgilHash hash;
        hash.asn1Read(asn1Reader);
        VirgilByteArray digestSign = asn1Reader.readOctetString();
        // Calculate data digest
        VirgilByteArray digest = hash.hash(data);
        // Prepare cipher
        VirgilAsymmetricCipher cipher;
        cipher.setPublicKey(publicKey);
        // Verify
        return cipher.verify(digest, digestSign, hash_.type());
    } catch (const PolarsslException& exception) {
        throw VirgilCryptoException("VirgilSigner: malformed sign.");
    }
}
