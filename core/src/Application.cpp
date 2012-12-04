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

#include <QMessageBox>

#include <stdexcept>

#include <Position.h>

#include "Application.h"
#include "DetourManager.h"

#define SETTINGS_FILE "config.js"

#define SETTINGS_PLUGINS_DIRECTORY "plugins"
#define SETTINGS_ADDRESSES "addresses"
#define SETTINGS_ADDRESSES_IN_FUNCTION "inFunction"
#define SETTINGS_ADDRESSES_IN_NEXT_FUNCTION "inNextFunction"
#define SETTINGS_ADDRESSES_IN_STREAM "inStream"
#define SETTINGS_ADDRESSES_OUT_FUNCTION "outFunction"
#define SETTINGS_ADDRESSES_OUT_BUFFER_LENGTH "outBufferLength"
#define SETTINGS_ADDRESSES_OUT_BUFFER "outBuffer"

int Application::argc_ = 0;

Application::Application():
    QApplication(Application::argc_, NULL),
    settings_(),
    memory_(),
    sender_(),
    plugins_(this),
    proxies_(),
    receiver_(&proxies_),
    clientBufferHandler_(&sender_, &receiver_),
    serverBufferHandler_(&receiver_) {

    QApplication::setApplicationName("Tibia Hook");
    QApplication::setApplicationVersion("beta");
    QApplication::setQuitOnLastWindowClosed(false);

    qRegisterMetaType<Position>("Position");
    qRegisterMetaType<Logger::Entry>("Logger::Entry");
}

void Application::initialize() {
    try {
        // Try to load the configuration file
        QFile configFile(SETTINGS_FILE);
        if (!configFile.open(QFile::ReadOnly)) {
            throw std::runtime_error("Could not open " SETTINGS_FILE "!");
        }

        // Try to parse the configuration file
        if (!settings_.parse(configFile.readAll())) {
            throw std::runtime_error("Could not load " SETTINGS_FILE "!");
        }

        if (!settings_.contains(SETTINGS_ADDRESSES)) {
            throw std::runtime_error("Could not load addresses!");
        }

        QVariantMap addressSettings = settings_.value(SETTINGS_ADDRESSES).toMap();
        DetourManager::Addresses addresses;
        addresses.inFunction = addressSettings.value(SETTINGS_ADDRESSES_IN_FUNCTION).toUInt();
        addresses.inNextFunction = addressSettings.value(SETTINGS_ADDRESSES_IN_NEXT_FUNCTION).toUInt();
        addresses.inStream = addressSettings.value(SETTINGS_ADDRESSES_IN_STREAM).toUInt();
        addresses.outFunction = addressSettings.value(SETTINGS_ADDRESSES_OUT_FUNCTION).toUInt();
        addresses.outBufferLength = addressSettings.value(SETTINGS_ADDRESSES_OUT_BUFFER_LENGTH).toUInt();
        addresses.outBuffer = addressSettings.value(SETTINGS_ADDRESSES_OUT_BUFFER).toUInt();

        // Connect the DetourManager with the sender and receiver and install detours
        DetourManager::setClientBufferHandler(&clientBufferHandler_);
        DetourManager::setServerBufferHandler(&serverBufferHandler_);
        DetourManager::install(addresses);

        // Create user interface
        ui_ = new UIManager();
        uiLogger_ = new UILogger(&logger_);
        ui_->addTab(uiLogger_, "Log");

        // Load the plugins directory
        QVariant pluginsDir = settings_.value(SETTINGS_PLUGINS_DIRECTORY);
        if (pluginsDir.type() != QVariant::String) {
            throw std::runtime_error("Could not load plugins directory!");
        }

        // Load plugins from the given plugins directory
        plugins_.load(pluginsDir.toString());
    }
    catch(std::exception& exception) {
        QMessageBox message;
        message.setWindowTitle(QApplication::applicationName());
        message.setText("Something terrible has happened!");
        message.setDetailedText(exception.what());
        message.setDefaultButton(QMessageBox::Ignore);
        message.exec();
    }
}

Application::~Application() {
    // First unload plugins
    plugins_.unload();

    // Then delete the user interface
    delete uiLogger_;
    delete ui_;

    // And finally unload detours
    DetourManager::uninstall();
    DetourManager::setServerBufferHandler(NULL);
    DetourManager::setClientBufferHandler(NULL);
}
