#include "help_browser.hpp"

#include <QTextBrowser>
#include <QPushButton>
#include <QHBoxLayout>
#include <QDir>
#include <QApplication>
#include <QStyle>

Help_Browser::Help_Browser()
{/*This file is part of Circle Packings.

    Circle Packings is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Circle Packings is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Circle Packings.  If not, see <http://www.gnu.org/licenses/>.*/


    setAttribute(Qt::WA_DeleteOnClose);
    setWindowModality(Qt::WindowModal);

    text_browser_ = new QTextBrowser;

    home_button_ = new QPushButton(tr("&Home"));
    home_button_->setIcon(QApplication::style()->standardIcon(QStyle::SP_ArrowUp));
    back_button_ = new QPushButton(tr("&Back"));
    back_button_->setIcon(QApplication::style()->standardIcon(QStyle::SP_ArrowBack));
    close_button_ = new QPushButton(tr("Close"));
    close_button_->setIcon(QApplication::style()->standardIcon(QStyle::SP_TitleBarCloseButton));
    close_button_->setShortcut(tr("Esc"));

    QHBoxLayout *button_layout = new QHBoxLayout;
    button_layout->addWidget(home_button_);
    button_layout->addWidget(back_button_);
    button_layout->addStretch();
    button_layout->addWidget(close_button_);

    QVBoxLayout *main_layout = new QVBoxLayout;
    main_layout->addLayout(button_layout);
    main_layout->addWidget(text_browser_);
    setLayout(main_layout);

    connect(home_button_, SIGNAL(clicked()), text_browser_, SLOT(home()));
    connect(back_button_, SIGNAL(clicked()), text_browser_, SLOT(backward()));
    connect(close_button_, SIGNAL(clicked()), this, SLOT(close()));
    connect(text_browser_, SIGNAL(sourceChanged(const QUrl &)), this, SLOT(update_window_title()));
    connect(text_browser_, SIGNAL(anchorClicked(QUrl)), this, SLOT(anchor_clicked(QUrl)));

    text_browser_->setSource(QUrl("qrc:/doc/index.html"));
    text_browser_->setOpenExternalLinks(true);

}

void Help_Browser::update_window_title()
{
    setWindowTitle(tr("Help"));//: %1").arg(text_browser_->documentTitle()));
    return;
}

void Help_Browser::show_page()
{
    Help_Browser *browser = new Help_Browser();
    browser->resize(940, 640);
    browser->show();
    return;
}

void Help_Browser::anchor_clicked(QUrl url)
{
    if (url == QUrl( "qrc:/doc/index.html#about_qt" ))
    {
        QApplication::aboutQt();
    }
    return;
}
