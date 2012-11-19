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

#ifndef CIRCLE_PACKINGS_APPLICATION_HPP
#define CIRCLE_PACKINGS_APPLICATION_HPP

#include <QApplication>

#include "basic_types.hpp"
#include "configuration.hpp"


class Window;

typedef int user_choice;
enum{RESTORE, RESTART, EXIT, IGNORE};

class Circle_Packings_Application : public QApplication
{
public:
    Circle_Packings_Application(int& argc, char ** argv);
    virtual ~Circle_Packings_Application();

    virtual bool notify(QObject * receiver, QEvent * event);

private:
    Window *window_;
    bool error_caught_;

    user_choice show_dialog_box(QString error_message);
    void exit_choice();
    void restart_choice();
    void restore_choice();
    void ham_choice();
    void ignore_choice();
    void load_file(char* file_name);
};

#endif // CIRCLE_PACKINGS_APPLICATION_HPP
