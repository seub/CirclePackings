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

#ifndef INPUT_MENU_HPP
#define INPUT_MENU_HPP

#include "basic_types.hpp"
#include "QGroupBox"

class QGridLayout;
class QComboBox;
class QPushButton;
class QLabel;
class QCheckBox;
class QSpinBox;
class Window;
class QRadioButton;

class Input_Menu : public QGroupBox
{
    Q_OBJECT

    friend class Window;
    friend class Top_Menu;
    friend class Configuration;

public:
    int max_left_col_width() const;
    int max_right_col_width() const;
    int max_width() const;
    int max_height() const;

private:
    Window* window_;
    QGridLayout* layout_;
    QComboBox* choose_mode_;
    QPushButton* reset_;


    QComboBox* choose_graph_combobox_;
    QRadioButton* draw_vertices_;
    QRadioButton* draw_edges_;
    QPushButton* complete_;
    QSpinBox* graph_parameter_spinbox_;
    QLabel* graph_parameter_label_;
    QSpinBox* graph_parameter_spinbox_2_;
    QLabel* graph_parameter_label_2_;

    QComboBox* choose_curve_;
    QLabel* smooth_curve_label_;
    QCheckBox* smooth_curve_;
    QLabel* nb_points_label_;
    QSpinBox* nb_points_spinbox_;
    QLabel* tiling_size_label_;
    QSpinBox* tiling_size_spinbox_;

    QComboBox* choose_curve_2_;
    QLabel* smooth_curve_label_2_;
    QCheckBox* smooth_curve_2_;
    QLabel* nb_points_label_2_;
    QSpinBox* nb_points_spinbox_2_;    

    QLabel *choose_coloring_label_;
    QComboBox *choose_coloring_box_;
    QComboBox *choose_coloring_box_2_;

    int vert_space_;
    int button_height_;

    Input_Menu(Window *window);

    void resizeEvent(QResizeEvent *);
    void enterEvent(QEvent *);
    void change_mode_menu(const mode_type &old_mode_flag, const mode_type &new_mode_flag, bool create_tiling = true);

    void set_row_heights();
    void create_choose_mode_combobox();
    void create_reset_button();
    void create_choose_graph_combobox();
    void create_draw_vertices_and_edges_radioboxes();
    void create_complete_button();
    void create_graph_paramaters_items();
    void create_graph_paramaters_items_2();
    void create_choose_curve_combobox();
    void create_choose_curve_2_combobox();
    void create_tiling_items();
    void create_smooth_items();
    void create_smooth_2_items();
    void create_choose_nb_points_items();
    void create_choose_nb_points_2_items();
    void create_choose_coloring_label();
    void create_choose_coloring_combobox();
    void create_choose_coloring_2_combobox();

    void empty_draw_graph_menu();
    void empty_draw_graph_submenu();
    void empty_graph_parameters_menu();
    void empty_draw_curve_menu();
    void empty_draw_two_curves_menu();
    void empty_curve_parameters_menu();
    void empty_curve_parameters_menu_2();
    void set_draw_vertices_with_edges_forbidden();
    void set_draw_vertices_with_edges_allowed();
    void set_draw_edges();
    void set_draw_neither_vertices_nor_edges();

    void reset_input_menu();
    void reset_draw_graph_menu();
    void reset_draw_curve_menu();
    void reset_draw_two_curves_menu();

    void create_draw_graph_menu();
    void create_draw_graph_submenu();
    void create_graph_parameters_menu();
    void create_draw_curve_menu();
    void create_draw_two_curves_menu();
    void create_curve_parameters_menu();
    void create_curve_parameters_menu_2();

    void completing();

    void set_nb_points_without_signal(int nb_points);
    void set_nb_points_2_without_signal(int nb_points_2);
    void set_graph_parameter_1_without_signal(int parameter);
    void set_graph_parameter_2_without_signal(int parameter);
};

#endif // INPUT_MENU_HPP
