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

#ifndef BATTELLISTPLUGIN_H
#define BATTELLISTPLUGIN_H

#include <QHash>
#include <QPair>
#include <QObject>
#include <QTimerEvent>
#include <QVariantMap>

#include <HookInterface.h>
#include <PluginInterface.h>
#include <Exception.h>

#include "BattleList.h"

class StringException: public Exception {
public:
    StringException(const QString& message): message_(message) {}

    const QString& message() const { return message_; }

private:
    QString message_;
};

class BattleListPlugin: public QObject, public PluginInterface {
	Q_OBJECT
    Q_INTERFACES(PluginInterface)

public:
	static const QString PLUGIN_NAME;
    static const int PLUGIN_VERSION;

	QString name() const;
	int version() const;

	void install(HookInterface*) throw(Exception);
	void uninstall();

    const BattleListEntry* findById(const quint32 id);
    const BattleListEntry* findByName(const QString& name);

private:
    BattleList* list_;
};

#endif
