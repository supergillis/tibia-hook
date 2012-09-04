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

#ifndef BATTELLISTUIPLUGIN_H
#define BATTELLISTUIPLUGIN_H

#include <QtPlugin>
#include <QObject>

#include <HookInterface.h>
#include <PluginInterface.h>

#include "BattleListWidget.h"

class BattleListUIPlugin: public QObject, public PluginInterface {
	Q_OBJECT
    Q_INTERFACES(PluginInterface)

public:
    BattleListUIPlugin();

    void install(HookInterface*, SettingsInterface*) throw(std::exception);
    void uninstall();

private:
    UIManagerInterface* ui_;
    BattleListWidget* widget_;
};

#endif
