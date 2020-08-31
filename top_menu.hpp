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

#ifndef TOP_MENU_HPP
#define TOP_MENU_HPP
#include <QMenuBar>

#include "basic_types.hpp"

class QLineEdit;
class QComboBox;
class QSpinBox;
class QPushButton;
class Window;


class Top_Menu : public QMenuBar
{

    Q_OBJECT
    friend class Window;

public:
    Top_Menu();
    ~Top_Menu();

    Top_Menu(Window* window);

private:
    Window* window_;
    QMenu *file_menu_;
    QAction *exit_action_;
    QAction *print_action_;
    QAction *save_action_;
    QAction *save_as_action_;
    QAction *load_action_;

    QMenu *setting_menu_;
    QAction *load_image_action_;

    QMenu *help_menu_;
    QAction *help_action_;
    QAction *about_action_;
    QLineEdit *file_name_;
    QComboBox *canvas_choice_;
    QSpinBox *quality_spinbox_;
    QDialog *print_dialog_;
    QPushButton *print_button_;

    QString last_loaded_file_;


    bool show_save_image_warning_;

private slots:
    void browse();
    void print();
    void print_in_file();
    void save();
    void save_as();
    void open();
    void print_file_name_changed(QString filename);
    void help();
    void about();


};

#endif // TOP_MENU_HPP
