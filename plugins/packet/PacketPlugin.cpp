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

#include "PacketPlugin.h"

#include <QDebug>

EXPORT_PLUGIN(PacketPlugin)

const QString PacketPlugin::PLUGIN_NAME("packet");
const int PacketPlugin::PLUGIN_VERSION(1);

const QString PacketPlugin::VARIABLE_NAME("Packet");

QString PacketPlugin::name() const {
	return PLUGIN_NAME;
}

int PacketPlugin::version() const {
	return PLUGIN_VERSION;
}

void PacketPlugin::install(HookInterface* hook) throw(Exception) {
	hook_ = hook;
	engine_ = hook->engine();
	QScriptValue constructor = engine_->newFunction(PacketPlugin::constructor);
	constructor.setData(engine_->newQObject(this));
	engine_->globalObject().setProperty(VARIABLE_NAME, constructor, QScriptValue::ReadOnly | QScriptValue::Undeletable);
}

void PacketPlugin::uninstall() {
	engine_->globalObject().setProperty(VARIABLE_NAME, QScriptValue::UndefinedValue);
	engine_ = NULL;
	hook_ = NULL;
}

QScriptValue PacketPlugin::constructor(QScriptContext* context, QScriptEngine* engine) {
	if (context->isCalledAsConstructor() && context->argumentCount() == 0) {
		QScriptValue object = context->thisObject();
		PacketPlugin* plugin = dynamic_cast<PacketPlugin*>(context->callee().data().toQObject());
		return engine->newQObject(object, plugin->hook_->createReadWritePacket(), QScriptEngine::ScriptOwnership);
	}
	return context->throwError("invalid call to Packet constructor");
}
