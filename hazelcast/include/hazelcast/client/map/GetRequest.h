//
// Created by sancar koyunlu on 5/23/13.
// Copyright (c) 2013 hazelcast. All rights reserved.
#ifndef HAZELCAST_MAP_GET_REQUEST
#define HAZELCAST_MAP_GET_REQUEST

#include "hazelcast/client/impl/ClientRequest.h"
#include "hazelcast/client/serialization/pimpl/Data.h"
#include <string>

namespace hazelcast {
    namespace client {

        namespace serialization {
            namespace pimpl{
                class Data;
            }

        }
        namespace map {

            class GetRequest : public impl::ClientRequest {
            public:
                GetRequest(const std::string &name, const serialization::pimpl::Data &key);

                int getFactoryId() const;

                int getClassId() const;

                void write(serialization::PortableWriter &writer) const;

                bool isRetryable() const;

            private:
                std::string name;
                bool async;
                serialization::pimpl::Data key;
            };
        }
    }
}

#endif //HAZELCAST_MAP_GET_REQUEST

