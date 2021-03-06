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
// Created by sancar koyunlu on 20/02/14.
//


#ifndef HAZELCAST_ILogger
#define HAZELCAST_ILogger

#include "hazelcast/util/HazelcastDll.h"
#include "Mutex.h"
#include <string>

#if  defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
#pragma warning(push)
#pragma warning(disable: 4251) //for dll export	
#endif

namespace hazelcast {
    namespace client {
        enum LogLevel {
            SEVERE = 100, WARNING = 90, INFO = 50, FINEST = 20
        };
    }

    namespace util {
        class HAZELCAST_API ILogger {
        public:
            static ILogger& getLogger();

            void setLogLevel(int logLevel);

            void severe(const std::string& message);

            void warning(const std::string& message);

            void info(const std::string& message);

            void finest(const std::string& message);

            void setPrefix(const std::string& prefix);

            bool isEnabled(int logLevel) const;

            bool isFinestEnabled() const;
        private:
            int HazelcastLogLevel;
            std::string prefix;

            ILogger();

            ~ILogger();

            const char *getTime(char * buffer, size_t length) const;

            ILogger(const ILogger&);

            ILogger& operator=(const ILogger&);

            util::Mutex lockMutex;
        };
    }
}

#if  defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
#pragma warning(pop)
#endif


#endif //HAZELCAST_ILogger
