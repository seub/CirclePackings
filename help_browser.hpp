/*This file is part of Circle Packings.

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

#ifndef HELP_BROWSER_HPP
#define HELP_BROWSER_HPP

#include "basic_types.hpp"
#include <QWidget>
#include <QUrl>


class QTextBrowser;
class QPushButton;


class Help_Browser : public QWidget
{
    Q_OBJECT

public:
    Help_Browser();

    static void show_page();

private slots:
    void update_window_title();
    void anchor_clicked(QUrl url);

private:
    QSize initial_size_;

    QTextBrowser *text_browser_;
    QPushButton *home_button_;
    QPushButton *back_button_;
    QPushButton *close_button_;
};

#endif // HELP_BROWSER_HPP
