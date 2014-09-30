#ifndef HAZELCAST_MULTI_MAP
#define HAZELCAST_MULTI_MAP

#include "hazelcast/client/pimpl/MultiMapImpl.h"
#include "hazelcast/client/multimap/PutRequest.h"
#include "hazelcast/client/multimap/RemoveRequest.h"
#include "hazelcast/client/multimap/RemoveAllRequest.h"
#include "hazelcast/client/multimap/KeySetRequest.h"
#include "hazelcast/client/multimap/ValuesRequest.h"
#include "hazelcast/client/multimap/EntrySetRequest.h"
#include "hazelcast/client/multimap/SizeRequest.h"
#include "hazelcast/client/multimap/ClearRequest.h"
#include "hazelcast/client/multimap/CountRequest.h"
#include "hazelcast/client/multimap/AddEntryListenerRequest.h"
#include "hazelcast/client/multimap/RemoveEntryListenerRequest.h"
#include "hazelcast/client/multimap/KeyBasedContainsRequest.h"
#include "hazelcast/client/multimap/ContainsRequest.h"
#include "hazelcast/client/multimap/GetAllRequest.h"
#include "hazelcast/client/multimap/MultiMapLockRequest.h"
#include "hazelcast/client/multimap/MultiMapUnlockRequest.h"
#include "hazelcast/client/impl/PortableCollection.h"
#include "hazelcast/client/multimap/MultiMapIsLockedRequest.h"
#include "hazelcast/client/multimap/PortableEntrySetResponse.h"
#include "hazelcast/client/spi/ClientContext.h"
#include "hazelcast/client/impl/EntryEventHandler.h"
#include "hazelcast/client/spi/ServerListenerService.h"

#include "hazelcast/client/multimap/PortableEntrySetResponse.h"
#include "hazelcast/client/spi/ClientContext.h"
#include "hazelcast/client/impl/EntryEventHandler.h"
#include "hazelcast/client/spi/ServerListenerService.h"
#include "hazelcast/client/DistributedObject.h"
#include "hazelcast/client/pimpl/MultiMapImpl.h"
#include <string>
#include <map>
#include <set>
#include <vector>
#include <stdexcept>

namespace hazelcast {
    namespace client {

        /**
        * A specialized distributed map client whose keys can be associated with multiple values.
        *
        * @see IMap
        */
        template<typename K, typename V>
        class HAZELCAST_API MultiMap : public DistributedObject {
            friend class HazelcastClient;

        public:
            /**
            * Stores a key-value pair in the multimap.
            *
            *
            * @param key   the key to be stored
            * @param value the value to be stored
            * @return true if size of the multimap is increased, false if the multimap
            *         already contains the key-value pair.
            */
            bool put(const K& key, const V& value) {
                return impl->put(toData(key), toData(value));
            }

            /**
            * Returns the multimap of values associated with the key.
            *
            * @param key the key whose associated values are to be returned
            * @return the multimap of the values associated with the key.
            */
            std::vector<V> get(const K& key) {
                return toObjectCollection(impl->get(toData(key)));
            }

            /**
            * Removes the given key value pair from the multimap.
            *
            * @param key
            * @param value
            * @return true if the size of the multimap changed after the remove operation, false otherwise.
            */
            bool remove(const K& key, const V& value) {
                return impl->remove(toData(key), toData(value));                
            }

            /**
            * Removes all the entries with the given key.
            *
            * @param key
            * @return the multimap of removed values associated with the given key. Returned multimap
            *         might be modifiable but it has no effect on the multimap
            */
            std::vector<V> remove(const K& key) {
                return toObjectCollection(impl->remove(toData(key)));
            }

            /**
            * Returns the set of keys in the multimap.
            *
            * @return the set of keys in the multimap. Returned set might be modifiable
            *         but it has no effect on the multimap
            */
            std::vector<K> keySet() {
                return toObjectCollection(impl->keySet());
            }

            /**
            * Returns the multimap of values in the multimap.
            *
            * @return the multimap of values in the multimap. Returned multimap might be modifiable
            *         but it has no effect on the multimap
            */
            std::vector<V> values() {
                return toObjectCollection(impl->values());
            }

            /**
            * Returns the set of key-value pairs in the multimap.
            *
            * @return the set of key-value pairs in the multimap. Returned set might be modifiable
            *         but it has no effect on the multimap
            */
            std::vector<std::pair<K, V> > entrySet() {
                return toObjectEntrySet(impl->entrySet());
            }            

            /**
            * Returns whether the multimap contains an entry with the key.
            *
            * @param key the key whose existence is checked.
            * @return true if the multimap contains an entry with the key, false otherwise.
            */
            bool containsKey(const K& key) {
                return impl->containsKey(toData(key));
            }

            /**
            * Returns whether the multimap contains an entry with the value.
            *
            * @param value the value whose existence is checked.
            * @return true if the multimap contains an entry with the value, false otherwise.
            */
            bool containsValue(const V& value) {
                return impl->containsValue(toData(value));
            }

            /**
            * Returns whether the multimap contains the given key-value pair.
            *
            * @param key   the key whose existence is checked.
            * @param value the value whose existence is checked.
            * @return true if the multimap contains the key-value pair, false otherwise.
            */
            bool containsEntry(const K& key, const V& value) {
                return impl->containsEntry(toData(key), toData(value));
            }

            /**
            * Returns the number of key-value pairs in the multimap.
            *
            * @return the number of key-value pairs in the multimap.
            */
            int size() {
                return  impl->size();
            }

            /**
            * Clears the multimap. Removes all key-value pairs.
            */
            void clear() {
                impl->clear();
            }

            /**
            * Returns number of values matching to given key in the multimap.
            *
            *
            * @param key the key whose values count are to be returned
            * @return number of values matching to given key in the multimap.
            */
            int valueCount(const K& key) {
                return impl->valueCount(toData(key));
            }

            /**
            * Adds an entry listener for this multimap. Listener will get notified
            * for all multimap add/remove/update/evict events.
            *
            * Warning 1: If listener should do a time consuming operation, off-load the operation to another thread.
            * otherwise it will slow down the system.
            *
            * Warning 2: Do not make a call to hazelcast. It can cause deadlock.
            *
            * @param listener     entry listener
            * @param includeValue <tt>true</tt> if <tt>EntryEvent</tt> should
            *                     contain the value.
            * @return returns registration id.
            */
            std::string addEntryListener(EntryListener<K, V>& listener, bool includeValue) {
                spi::ClusterService& clusterService = getContext().getClusterService();
                serialization::pimpl::SerializationService& ss = getContext().getSerializationService();
                impl::EntryEventHandler<K, V> *entryEventHandler = new impl::EntryEventHandler<K, V>(getName(), clusterService, ss, listener, includeValue);
                return impl->addEntryListener(entryEventHandler, includeValue);
            }

            /**
            * Adds the specified entry listener for the specified key.
            * The listener will get notified for all
            * add/remove/update/evict events of the specified key only.
            *
            * Warning 1: If listener should do a time consuming operation, off-load the operation to another thread.
            * otherwise it will slow down the system.
            *
            * Warning 2: Do not make a call to hazelcast. It can cause deadlock.
            *
            * @param listener     entry listener
            * @param key          the key to listen
            * @param includeValue <tt>true</tt> if <tt>EntryEvent</tt> should
            *                     contain the value.
            * @return returns registration id.
            */
            std::string addEntryListener(EntryListener<K, V>& listener, const K& key, bool includeValue) {
                impl::EntryEventHandler<K, V> *entryEventHandler = new impl::EntryEventHandler<K, V>(getName(), getContext().getClusterService(), getContext().getSerializationService(), listener, includeValue);
                return impl->addEntryListener(entryEventHandler, toData(key), includeValue);
            }

            /**
            * Removes the specified entry listener
            * Returns silently if there is no such listener added before.
            *
            * @param registrationId Id of listener registration
            *
            * @return true if registration is removed, false otherwise
            */
            bool removeEntryListener(const std::string& registrationId) {
                return impl->removeEntryListener(registrationId);
            }

            /**
            * Acquires the lock for the specified key.
            * <p>If the lock is not available then
            * the current thread becomes disabled for thread scheduling
            * purposes and lies dormant until the lock has been acquired.
            *
            * Scope of the lock is this multimap only.
            * Acquired lock is only for the key in this multimap.
            *
            * Locks are re-entrant so if the key is locked N times then
            * it should be unlocked N times before another thread can acquire it.
            *
            *
            * @param key key to lock.
            */
            void lock(const K& key) {
                impl->lock(toData(key));
            }

            /**
            * Acquires the lock for the specified key for the specified lease time.
            * <p>After lease time, lock will be released..
            *
            * <p>If the lock is not available then
            * the current thread becomes disabled for thread scheduling
            * purposes and lies dormant until the lock has been acquired.
            *
            * Scope of the lock is this map only.
            * Acquired lock is only for the key in this map.
            *
            * Locks are re-entrant so if the key is locked N times then
            * it should be unlocked N times before another thread can acquire it.
            * @param key key to lock.
            * @param leaseTimeInMillis time in milliseconds to wait before releasing the lock.
            */
            void lock(const K& key, long leaseTimeInMillis) {
                impl->lock(toData(key), leaseTimeInMillis);
            }

            /**
            * Checks the lock for the specified key.
            * <p>If the lock is acquired then returns true, else false.
            *
            * @param key key to lock to be checked.
            * @return <tt>true</tt> if lock is acquired, <tt>false</tt> otherwise.
            */
            bool isLocked(const K& key) {
                return impl->isLocked(toData(key));
            }

            /**
            * Tries to acquire the lock for the specified key.
            * <p>If the lock is not available then the current thread
            * doesn't wait and returns false immediately.
            *
            *
            * @param key key to lock.
            * @return <tt>true</tt> if lock is acquired, <tt>false</tt> otherwise.
            */
            bool tryLock(const K& key) {
                return impl->tryLock(toData(key));
            }

            /**
            * Tries to acquire the lock for the specified key.
            * <p>If the lock is not available then
            * the current thread becomes disabled for thread scheduling
            * purposes and lies dormant until one of two things happens:
            * <ul>
            * <li>The lock is acquired by the current thread; or
            * <li>The specified waiting time elapses
            * </ul>
            *
            *
            * @param key to be locked.
            * @param timeoutInMillis     the maximum time to wait for the lock
            * @return <tt>true</tt> if the lock was acquired and <tt>false</tt>
            *         if the waiting time elapsed before the lock was acquired.
            */
            bool tryLock(const K& key, long timeoutInMillis) {
                return impl->tryLock(toData(key), timeoutInMillis);
            }

            /**
            * Releases the lock for the specified key. It never blocks and
            * returns immediately.
            *
            *
            * @param key key to lock.
            */
            void unlock(const K& key) {
                impl->unlock(toData(key));
            }

            /**
            * Releases the lock for the specified key regardless of the lock owner.
            * It always successfully unlocks the key, never blocks
            * and returns immediately.
            * @param key key to lock.
            */
            void forceUnlock(const K& key) {
                impl->forceUnlock(toData(key));
            }

            /**
            * Destructor
            */
            ~MultiMap(){
                delete impl;
            }
        private:
            void onDestroy() {
            }

            std::vector<V> toObjectCollection(std::vector<serialization::pimpl::Data> const& dataCollection) {
                int size = dataCollection.size();
                std::vector<V> multimap(size);
                for (int i = 0; i < size; i++) {
                    boost::shared_ptr<V> v = toObject<V>(dataCollection[i]);
                    multimap[i] = *v;
                }
                return multimap;
            }

            std::vector<std::pair<K, V> > toObjectEntrySet(std::vector<std::pair<serialization::pimpl::Data, serialization::pimpl::Data> > const& dataEntrySet) {
                int size = dataEntrySet.size();
                std::vector<std::pair<K, V> > entrySet(size);
                for (int i = 0; i < size; i++) {
                    boost::shared_ptr<K> key = toObject<K>(dataEntrySet[i].first);
                    entrySet[i].first = *key;
                    boost::shared_ptr<V> value = toObject<V>(dataEntrySet[i].second);
                    entrySet[i].second = *value;
                }
                return entrySet;
            }

            template<typename T>
            serialization::pimpl::Data toData(const T& object) {
                return getContext().getSerializationService().template toData<T>(&object);
            }

            template<typename T>
            boost::shared_ptr<T> toObject(const serialization::pimpl::Data& data) {
                return getContext().getSerializationService().template toObject<T>(data);
            }

            MultiMap(const std::string& instanceName, spi::ClientContext *context)
            : DistributedObject("hz:impl:multiMapService", instanceName, context)
            , impl(new pimpl::MultiMapImpl(instanceName, context)) {

            }

            pimpl::MultiMapImpl* impl;

        };
    }
}

#endif /* HAZELCAST_MULTI_MAP */

