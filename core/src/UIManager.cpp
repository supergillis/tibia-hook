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

#include "UIManager.h"

UIManager::UIManager(QWidget* parent):
    QMainWindow(parent),
    tabs_(new QTabWidget(this)) {
    setCentralWidget(tabs_);
    setVisible(true);
    setWindowTitle("Tibia Hook");

    resize(1000, 800);
}

void UIManager::addTab(QWidget* tab, const QString& label) {
    tabs_->addTab(tab, label);
}

void UIManager::removeTab(QWidget* tab) {
    int index = tabs_->indexOf(tab);
    if (index != -1) {
        tabs_->removeTab(index);
    }
}
