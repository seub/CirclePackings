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

#ifndef OUTPUT_MENU_H
#define OUTPUT_MENU_H

#include <QGroupBox>

#include "basic_types.hpp"

class Window;
class QGridLayout;
class QPushButton;
class QLabel;
class QSpinBox;
class QCheckBox;

class Output_Menu : public QGroupBox
{
    Q_OBJECT

    friend class Window;
    friend class Top_Algo;
    friend class Configuration;

public :
    int max_left_col_width() const;
    int max_right_col_width() const;
    int max_width() const;
    int max_height() const;

private:
    Output_Menu(Window* window);
    void resizeEvent(QResizeEvent *);
    void enterEvent(QEvent *);
    void reset_output_menu();
    void disable_all_but_stop();
    void enable_all();

    void change_mode_menu(const mode_type &old_mode_flag, const mode_type &new_mode_flag);
    void empty_draw_graph_menu();
    void empty_draw_curve_menu();
    void empty_draw_two_curves_menu();
    void create_draw_graph_menu();
    void create_draw_curve_menu();
    void create_draw_two_curves_menu();

    Window *window_;
    QGridLayout *layout_;
    QPushButton *compute_button_;
    QLabel *show_live_label_;
    QCheckBox *show_live_checkbox_;
    QLabel *show_inverse_label_;
    QCheckBox *show_inverse_checkbox_;

    int vert_space_;
    int button_height_;
};

#endif // OUTPUT_MENU_H
