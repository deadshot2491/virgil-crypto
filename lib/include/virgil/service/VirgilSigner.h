/**
 * Copyright (C) 2014 Virgil Security Inc.
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
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
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

#ifndef VIRGIL_SERVICE_VIRGIL_SIGNER_H
#define VIRGIL_SERVICE_VIRGIL_SIGNER_H

#include <virgil/VirgilByteArray.h>
using virgil::VirgilByteArray;

#include <virgil/service/data/VirgilSign.h>
using virgil::service::data::VirgilSign;

#include <virgil/service/data/VirgilCertificate.h>
using virgil::service::data::VirgilCertificate;

#include <virgil/service/data/VirgilTicket.h>
using virgil::service::data::VirgilTicket;

#include <virgil/service/stream/VirgilDataSource.h>
using virgil::service::stream::VirgilDataSource;

#include <virgil/service/stream/VirgilDataSink.h>
using virgil::service::stream::VirgilDataSink;

namespace virgil { namespace service {

/**
 * @brief This class provides high-level interface to sign and verify data using Virgil Security keys.
 * This module can sign / verify as raw data and Virgil Security tickets.
 */
class VirgilSigner {
public:
    /**
     * @brief Initilaize internal resources.
     */
    VirgilSigner();
    /**
     * @brief Dispose used resources.
     */
    virtual ~VirgilSigner() throw();
    /**
     * @brief Sign data provided by the source with given private key.
     * @return Virgil Security sign.
     */
    VirgilSign sign(VirgilDataSource& source, const VirgilByteArray& signerCertificateId,
            const VirgilByteArray& privateKey, const VirgilByteArray& privateKeyPassword = VirgilByteArray());
    /**
     * @brief Verify sign and data provided by the source to be conformed to the given certificate.
     * @return true if sign is valid and data was not malformed.
     */
    bool verify(VirgilDataSource& source, const VirgilSign& sign, const VirgilByteArray& publicKey);
    /**
     * @brief Sign given ticket with the private key.
     * @return Virgil Security sign.
     */
    VirgilSign sign(VirgilTicket& ticket, const VirgilByteArray& signerCertificateId,
            const VirgilByteArray& privateKey, const VirgilByteArray& privateKeyPassword = VirgilByteArray());
    /**
     * @brief Verify sign and ticket to be conformed to the given certificate.
     * @return true if sign is valid and data was not malformed.
     */
    bool verify(VirgilTicket& ticket, const VirgilSign& sign, const VirgilByteArray& publicKey);
private:
    /**
     * @brief Deny copy constructor.
     */
    VirgilSigner(const VirgilSigner& other);
    /**
     * @brief Deny asignment operator.
     */
    VirgilSigner& operator=(const VirgilSigner& right);
};

}}

#endif /* VIRGIL_SERVICE_VIRGIL_SIGNER_H */