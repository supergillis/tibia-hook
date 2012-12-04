/* Copyright (c) 2012 Gillis Van Ginderachter <supergillis@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef LOGGERINTERFACE_H
#define LOGGERINTERFACE_H

class QString;

class LoggerInterface {
public:
    enum Level {
        Debug,
        Error,
        Info,
        Verbose,
        Warn
    };

    virtual ~LoggerInterface() {}

    virtual void log(Level level, const QString& tag, const QString& message) = 0;

    virtual void d(const QString& tag, const QString& message) = 0;
    virtual void e(const QString& tag, const QString& message) = 0;
    virtual void i(const QString& tag, const QString& message) = 0;
    virtual void v(const QString& tag, const QString& message) = 0;
    virtual void w(const QString& tag, const QString& message) = 0;
};

#endif
