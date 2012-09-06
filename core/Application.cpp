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

#include "Application.h"
#include "DetourManager.h"
#include "DetourSender.h"
#include "Hook.h"
#include "Settings.h"

#include <stdexcept>

#include <QMessageBox>

class ClientBufferHandler: public BufferHandler {
public:
    ClientBufferHandler(SenderInterface* sender, ReceiverInterface* receiver): sender_(sender), receiver_(receiver) {}

    inline void handle(const QByteArray& data) {
        if(receiver_->receiveOutgoingMessage(data)) {
            sender_->sendToServer(data);
        }
    }

private:
    SenderInterface* sender_;
    ReceiverInterface* receiver_;
};

class ServerBufferHandler: public BufferHandler {
public:
    ServerBufferHandler(ReceiverInterface* receiver): receiver_(receiver) {}

    inline void handle(const QByteArray& data) {
        receiver_->receiveIncomingMessage(data);
    }

private:
    ReceiverInterface* receiver_;
};

int Application::argc_ = 0;

Application::Application():
	QApplication(Application::argc_, NULL) {
    QApplication::setApplicationName("Tibia Hook");
    QApplication::setApplicationVersion("beta");

    QFile configFile("config.js");
    try {
        if(!configFile.open(QFile::ReadOnly))
            throw std::runtime_error("Could not load config.js!");

        SettingsInterface* settings = new Settings(configFile.readAll());
        SenderInterface* sender = new DetourSender(DetourManager::instance());
        ReceiverInterface* receiver = new Hook(settings, sender, this);

        // Connect the DetourManager with the sender and receiver
        DetourManager::instance()->setClientBufferHandler(new ClientBufferHandler(sender, receiver));
        DetourManager::instance()->setServerBufferHandler(new ServerBufferHandler(receiver));
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
