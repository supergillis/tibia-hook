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

#include "EnvironmentPlugin.h"

#include <ScriptPlugin.h>

EXPORT_PLUGIN(EnvironmentPlugin)

const QString EnvironmentPlugin::PLUGIN_NAME("environment");
const int EnvironmentPlugin::PLUGIN_VERSION(1);

const QString EnvironmentPlugin::VARIABLE_NAME("Environment");

QString EnvironmentPlugin::name() const {
	return PLUGIN_NAME;
}

int EnvironmentPlugin::version() const {
	return PLUGIN_VERSION;
}

void EnvironmentPlugin::install(HookInterface* hook) {
	hook_ = hook;
	engine_ = hook->engine();
	engine_->globalObject().setProperty(VARIABLE_NAME, engine_->newQObject(this), QScriptValue::ReadOnly | QScriptValue::Undeletable);
}

void EnvironmentPlugin::uninstall() {
	engine_->globalObject().setProperty(VARIABLE_NAME, QScriptValue::UndefinedValue);
	engine_ = NULL;
	hook_ = NULL;
}

bool EnvironmentPlugin::reload() {
	return hook_->reload();
}

bool EnvironmentPlugin::require(const QString& path) {
	return hook_->require(path);
}
