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

#include "UILogger.h"

UILogger::UILogger(Logger* logger, QWidget* parent):
    QWidget(parent),
    logger_(logger) {

    setupUi(this);

    logTable_->setColumnCount(3);
    logTable_->setHorizontalHeaderLabels(QStringList() << "Type" << "Tag" << "Message");

    // Listen for log messages
    QObject::connect(logger_, SIGNAL(logged(const Logger::Entry&)), this, SLOT(addLogMessage(const Logger::Entry&)), Qt::QueuedConnection);
}

void UILogger::addLogMessage(const Logger::Entry& entry) {
    int rowIndex = logTable_->rowCount();
    logTable_->insertRow(rowIndex);
    logTable_->setItem(rowIndex, 0, new QTableWidgetItem("Debug"));
    logTable_->setItem(rowIndex, 1, new QTableWidgetItem(entry.tag));
    logTable_->setItem(rowIndex, 2, new QTableWidgetItem(entry.message));
}
