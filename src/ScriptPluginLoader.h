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

#ifndef SCRIPTPLUGINLOADER_H_
#define SCRIPTPLUGINLOADER_H_

#include <QLibrary>
#include <QPointer>
#include <QString>

#include <ScriptPlugin.h>
#include <ScriptPluginInterface.h>

class ScriptPluginLoader {
public:
	ScriptPluginLoader(const QString&);
	virtual ~ScriptPluginLoader() {}

	ScriptPluginInterface* instance();

private:
	QString path_;
	ScriptPluginInterface* instance_;
};

#endif /* SCRIPTPLUGINLOADER_H_ */
