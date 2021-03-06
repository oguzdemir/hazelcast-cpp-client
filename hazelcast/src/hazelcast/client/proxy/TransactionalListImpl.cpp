/*
 * Copyright (c) 2008-2015, Hazelcast, Inc. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
//
// Created by sancar koyunlu on 01/10/14.
//

#include "hazelcast/util/Util.h"
#include "hazelcast/client/proxy/TransactionalListImpl.h"

// Includes for parameters classes
#include "hazelcast/client/protocol/codec/TransactionalListAddCodec.h"
#include "hazelcast/client/protocol/codec/TransactionalListRemoveCodec.h"
#include "hazelcast/client/protocol/codec/TransactionalListSizeCodec.h"

namespace hazelcast {
    namespace client {
        namespace proxy {
            TransactionalListImpl::TransactionalListImpl(const std::string& objectName, txn::TransactionProxy *context)
            : TransactionalObject("hz:impl:listService", objectName, context) {
            }

            bool TransactionalListImpl::add(const serialization::pimpl::Data& e) {
                std::auto_ptr<protocol::ClientMessage> request =
                        protocol::codec::TransactionalListAddCodec::RequestParameters::encode(
                                getName(), getTransactionId(), util::getThreadId(), e);

                return invokeAndGetResult<bool, protocol::codec::TransactionalListAddCodec::ResponseParameters>(request);
            }

            bool TransactionalListImpl::remove(const serialization::pimpl::Data& e) {
                std::auto_ptr<protocol::ClientMessage> request =
                        protocol::codec::TransactionalListRemoveCodec::RequestParameters::encode(
                                getName(), getTransactionId(), util::getThreadId(), e);

                return invokeAndGetResult<bool, protocol::codec::TransactionalListRemoveCodec::ResponseParameters>(request);
            }

            int TransactionalListImpl::size() {
                std::auto_ptr<protocol::ClientMessage> request =
                        protocol::codec::TransactionalListSizeCodec::RequestParameters::encode(
                                getName(), getTransactionId(), util::getThreadId());

                return invokeAndGetResult<int, protocol::codec::TransactionalListSizeCodec::ResponseParameters>(request);
            }
        }
    }
}

