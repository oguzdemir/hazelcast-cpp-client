//
// Created by sancar koyunlu on 9/18/13.
// Copyright (c) 2013 hazelcast. All rights reserved.


#include "ClientTxnQueueTest.h"
#include "HazelcastInstanceFactory.h"
#include "HazelcastClient.h"


namespace hazelcast {
    namespace client {

        class HazelcastClient;

        namespace test {
            using namespace iTest;

            ClientTxnQueueTest::ClientTxnQueueTest(HazelcastInstanceFactory& hazelcastInstanceFactory)
            :hazelcastInstanceFactory(hazelcastInstanceFactory)
            , instance(hazelcastInstanceFactory)
            , client(new HazelcastClient(clientConfig.addAddress(Address("localhost", 5701)))) {
            };


            ClientTxnQueueTest::~ClientTxnQueueTest() {
            }

            void ClientTxnQueueTest::addTests() {
                addTest(&ClientTxnQueueTest::testTransactionalOfferPoll1, "testTransactionalOfferPoll1");
                addTest(&ClientTxnQueueTest::testTransactionalOfferPoll2, "testTransactionalOfferPoll2");
            };

            void ClientTxnQueueTest::beforeClass() {
            };

            void ClientTxnQueueTest::afterClass() {
                client.reset();
                instance.shutdown();
            };

            void ClientTxnQueueTest::beforeTest() {
            };

            void ClientTxnQueueTest::afterTest() {
            };

            void ClientTxnQueueTest::testTransactionalOfferPoll1() {
                std::string name = "defQueue";

                TransactionContext context = client->newTransactionContext();
                context.beginTransaction();
                TransactionalQueue<std::string> q = context.getQueue<std::string>(name);
                assertTrue(q.offer("ali"));
                std::string s = q.poll();
                assertEqual("ali", s);
                context.commitTransaction();
                assertEqual(0, client->getQueue<std::string>(name).size());
            }

            void testTransactionalOfferPoll2Thread(util::CountDownLatch *latch, HazelcastClient *client) {
                try {
                    latch->await(5 * 1000);
                    boost::this_thread::sleep(boost::posix_time::seconds(3));
                    client->getQueue<std::string>("defQueue0").offer("item0");
                } catch (...) {
                }
            }

            void ClientTxnQueueTest::testTransactionalOfferPoll2() {

                util::CountDownLatch latch(1);
                boost::thread t(testTransactionalOfferPoll2Thread, &latch, client.get());
                TransactionContext context = client->newTransactionContext();
                context.beginTransaction();
                TransactionalQueue<std::string> q0 = context.getQueue<std::string>("defQueue0");
                TransactionalQueue<std::string> q1 = context.getQueue<std::string>("defQueue1");
                std::string s = "";
                latch.countDown();
                try {
                    s = q0.poll(10 * 1000);
                } catch (exception::InterruptedException& e) {
                    assertTrue(false, e.what());
                }
                assertEqual("item0", s);
                q1.offer(s);
                context.commitTransaction();

                assertEqual(0, client->getQueue<std::string>("defQueue0").size());
                assertEqual("item0", client->getQueue<std::string>("defQueue1").poll());
            }


        }
    }
}