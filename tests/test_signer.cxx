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

/**
 * @file test_signer.cxx
 * @brief Covers class VirgilSigner
 */

#include "catch.hpp"

#include <virgil/crypto/VirgilByteArray.h>
#include <virgil/crypto/VirgilSigner.h>
#include <virgil/crypto/VirgilKeyPair.h>
#include <virgil/crypto/VirgilCryptoException.h>

using virgil::crypto::str2bytes;
using virgil::crypto::VirgilByteArray;
using virgil::crypto::VirgilSigner;
using virgil::crypto::VirgilKeyPair;
using virgil::crypto::VirgilCryptoException;

TEST_CASE("sign-ec", "[signer]") {
    VirgilByteArray testData = str2bytes("this string will be signed");
    VirgilByteArray malformedData = str2bytes("this string will is malformed");
    VirgilByteArray malformedSign = str2bytes("I am malformed sign");
    VirgilByteArray keyPassword = str2bytes("password");
    VirgilKeyPair keyPair(keyPassword);

    VirgilSigner signer;
    VirgilByteArray sign = signer.sign(testData, keyPair.privateKey(), keyPassword);

    SECTION("and verify with original data and correspond sign") {
        REQUIRE(signer.verify(testData, sign, keyPair.publicKey()));
    }

    SECTION("and verify with malformed data") {
        REQUIRE(!signer.verify(malformedData, sign, keyPair.publicKey()));
    }

    SECTION("and verify with malformed sign") {
        REQUIRE_THROWS_AS(signer.verify(testData, malformedSign, keyPair.publicKey()), VirgilCryptoException);
    }
}

TEST_CASE("sign-rsa", "[signer]") {
    VirgilByteArray testData = str2bytes("this string will be signed");
    VirgilByteArray malformedData = str2bytes("this string will is malformed");
    VirgilByteArray malformedSign = str2bytes("I am malformed sign");
    VirgilByteArray keyPassword = str2bytes("password");
    VirgilKeyPair keyPair = VirgilKeyPair::generate(VirgilKeyPair::Type_RSA_2048, keyPassword);

    VirgilSigner signer;
    VirgilByteArray sign = signer.sign(testData, keyPair.privateKey(), keyPassword);

    SECTION("and verify with original data and correspond sign") {
        REQUIRE(signer.verify(testData, sign, keyPair.publicKey()));
    }

    SECTION("and verify with malformed data") {
        REQUIRE(!signer.verify(malformedData, sign, keyPair.publicKey()));
    }

    SECTION("and verify with malformed sign") {
        REQUIRE_THROWS_AS(signer.verify(testData, malformedSign, keyPair.publicKey()), VirgilCryptoException);
    }
}

TEST_CASE("sign-rsa with small key", "[signer]") {
    VirgilByteArray testData = str2bytes("this string will be signed");
    VirgilByteArray keyPassword = str2bytes("password");
    VirgilKeyPair keyPair = VirgilKeyPair::generate(VirgilKeyPair::Type_RSA_256, keyPassword);

    REQUIRE_THROWS(VirgilSigner().sign(testData, keyPair.privateKey(), keyPassword));
}

TEST_CASE("sign-curve25519", "[signer]") {
    VirgilByteArray testData = str2bytes("this string will be signed");
    VirgilByteArray malformedData = str2bytes("this string will is malformed");
    VirgilByteArray malformedSign = str2bytes("I am malformed sign");
    VirgilByteArray keyPassword = str2bytes("password");
    VirgilKeyPair keyPair = VirgilKeyPair::generate(VirgilKeyPair::Type_EC_M255, keyPassword);

    VirgilSigner signer;
    VirgilByteArray sign = signer.sign(testData, keyPair.privateKey(), keyPassword);

    SECTION("and verify with original data and correspond sign") {
        REQUIRE(signer.verify(testData, sign, keyPair.publicKey()));
    }

    SECTION("and verify with malformed data") {
        REQUIRE(!signer.verify(malformedData, sign, keyPair.publicKey()));
    }

    SECTION("and verify with malformed sign") {
        REQUIRE_THROWS_AS(signer.verify(testData, malformedSign, keyPair.publicKey()), VirgilCryptoException);
    }
}


TEST_CASE("sign with wrong key password", "[signer]") {
    VirgilByteArray testData = str2bytes("this string will be signed");
    VirgilByteArray keyPassword = str2bytes("password");
    VirgilByteArray wrongKeyPassword = str2bytes("wrong password");
    VirgilKeyPair keyPair = VirgilKeyPair::generate(VirgilKeyPair::Type_EC_M255, keyPassword);

    VirgilSigner signer;
    REQUIRE_THROWS_AS(signer.sign(testData, keyPair.privateKey(), wrongKeyPassword), VirgilCryptoException);
}