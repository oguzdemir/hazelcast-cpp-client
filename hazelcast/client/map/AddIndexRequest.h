//
// Created by sancar koyunlu on 6/11/13.
// Copyright (c) 2013 hazelcast. All rights reserved.



#ifndef HAZELCAST_ADD_INDEX_REQUEST
#define HAZELCAST_ADD_INDEX_REQUEST

#include "RequestIDs.h"
#include "../serialization/SerializationConstants.h"
#include <string>

namespace hazelcast {
    namespace client {
        namespace map {

            class AddIndexRequest {
                AddIndexRequest(const std::string& name, const std::string& attribute, bool ordered)
                :name(name)
                , attribute(attribute)
                , ordered(ordered) {

                };

                int getTypeSerializerId() const {
                    return serialization::SerializationConstants::CONSTANT_TYPE_PORTABLE;
                };

                int getFactoryId() const {
                    return map::RequestIDs::F_ID;
                }

                int getClassId() const {
                    return map::RequestIDs::ADD_INDEX;
                }

                template<typename HzWriter>
                inline void writePortable(HzWriter& writer) const {
                    writer["n"] << name;
                    writer["a"] << attribute;
                    writer["o"] << ordered;
                };

                template<typename HzReader>
                inline void readPortable(HzReader& reader) {
                    reader["n"] >> name;
                    reader["a"] >> attribute;
                    reader["o"] >> ordered;
                };
            private:
                std::string name;
                std::string attribute;
                bool ordered;
            };

        }
    }
}
#endif //HAZELCAST_ADD_INDEX_REQUEST