//
// Created by sancar koyunlu on 6/18/13.
// Copyright (c) 2013 hazelcast. All rights reserved.


#include "hazelcast/client/impl/MapEntrySet.h"
#include "hazelcast/client/map/DataSerializableHook.h"
#include "hazelcast/client/serialization/ObjectDataOutput.h"
#include "hazelcast/client/serialization/ObjectDataInput.h"

namespace hazelcast {
    namespace client {
        namespace map {
            MapEntrySet::MapEntrySet() {

            }


            MapEntrySet::MapEntrySet(const std::vector<std::pair<serialization::pimpl::Data, serialization::pimpl::Data> >& entrySet)
            :entrySet(entrySet) {

            }

            int MapEntrySet::getFactoryId() const {
                return DataSerializableHook::F_ID;
            }

            int MapEntrySet::getClassId() const {
                return DataSerializableHook::ENTRY_SET;
            }

            std::vector< std::pair< serialization::pimpl::Data, serialization::pimpl::Data> >  & MapEntrySet::getEntrySet() {
                return entrySet;
            }

            void MapEntrySet::writeData(serialization::ObjectDataOutput& writer) const {
                int size = entrySet.size();
		        writer.writeInt(size);
                for (int i = 0; i < size; ++i) {
                    entrySet[i].first.writeData(writer);
                    entrySet[i].second.writeData(writer);
                }
            }

            void MapEntrySet::readData(serialization::ObjectDataInput& reader) {
                int size = reader.readInt();
                entrySet.resize(size);
                for (int i = 0; i < size; i++) {
                    serialization::pimpl::Data data;
                    data.readData(reader);
                    entrySet[i].first = data;
                    serialization::pimpl::Data data2;
                    data2.readData(reader);
                    entrySet[i].second = data2;
                }
            }
        }
    }
}

