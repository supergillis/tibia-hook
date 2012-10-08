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

#include <stdexcept>

#include <json.h>

#include "Settings.h"

#ifdef Q_OS_WIN
#define SETTING_PREFIX "win32:"
#else
#define SETTING_PREFIX "linux:"
#endif

Settings::Settings(const QString& json) throw (std::exception) {
	bool parsed = true;
    QVariant result = QtJson::Json::parse(json, parsed);
    if (!parsed) {
        throw std::runtime_error("Could not parse JSON string!");
    }

	values_ = result.toMap();
}

bool Settings::contains(const QString& key) const {
    if (!values_.contains(key)) {
        return values_.contains(SETTING_PREFIX + key);
    }
    return true;
}

QVariant Settings::value(const QString& key) const {
    QVariant result = values_.value(key);
    if (result.isNull()) {
        return values_.value(SETTING_PREFIX + key);
    }
    return result;
}
