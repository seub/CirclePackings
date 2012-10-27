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

#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <QWidget>

#include "basic_types.hpp"
#include "maths_contents.hpp"
#include "graph_tiler.hpp"
#include "graph_triangulater.hpp"


class QStatusBar;
class QGridLayout;
class QComboBox;
class QString;
class QMenuBar;
class QMenu;
class QLineEdit;
class QSpinBox;
class QPushButton;
class QLabel;

class Algo;
class Canvas;
class Input_Menu;
class Output_Menu;
class Top_Menu;
class Configuration;

class Window : public QWidget
{
    Q_OBJECT

    friend class Top_Algo;
    friend class Configuration;
    friend class Circle_Packings_Application;

public:
    Window();
    ~Window();
    friend class Input_Menu;
    friend class Output_Menu;
    friend class Top_Menu;

    void resizeEvent(QResizeEvent *event);
    void resize_canvases();

    void update_configuration();
    void restore_configuration();

    QString message_received_;
    void signal_received(int signal, int message_timeout = 0);


    void print_flags();
    void print_dimensions();

private:
    void create_window();
    void create_menu();
    void reset_flags();
    void reset_status_bar_message();
    void set_canvas_delegates(bool reset_contents = true, bool only_right = false);
    void set_canvas_delegates_show_live();
    void enable_canvases_updates(bool b);
    void update_shower_bools();

    void launch_tests();
    void draw_2_curves_ready_off();
    void update_draw_borders();
    int menu_width() const;
    void optimal_size(unsigned int &, unsigned int&) const;



    // Flags
    mode_type mode_flag_;
    graph_choice_type graph_choice_flag_;
    draw_graph_mode_type draw_graph_flag_;
    draw_curve_mode_type draw_curve_flag_;
    draw_curve_mode_type draw_curve_flag_2_;
    draw_2_curves_mode_type draw_2_curves_flag_;
    curve_choice_type curve_choice_flag_;
    curve_choice_type curve_choice_flag_2_;
    bool curve_1_valid_;
    bool curve_2_valid_;
    bool image_loaded_;
    QString image_loaded_filename_;
    Triangulation_coloring triangulation_coloring_;

    // Widgets
    QGridLayout *layout_;
    Canvas *left_canvas_;
    Canvas *right_canvas_;
    Input_Menu *input_menu_;
    Output_Menu *output_menu_;
    QStatusBar *status_bar_;
    QLabel *status_bar_label_;

    //Menu
    Top_Menu *top_menu_;

    //Other
    Maths_Contents maths_contents_;
    Graph_Tiler graph_tiler_;
    Graph_Triangulater graph_triangulater_1_;
    Graph_Triangulater graph_triangulater_2_;
    Configuration *configuration_;

private slots:
    void change_mode(int mode_flag, bool create_tiling= true);
    void reset_window(bool reset_menus = true); // Only executes action of pressing 'Reset' button (in given mode)
    void reset_selected_stuff();

    void choose_graph(int graph_choice_flag);
    void set_graph_parameter(int);
    void set_graph_parameter_2(int);
    void draw_vertices();
    void draw_edges();
    void complete_graph();
    void stop_completing();
    void end_of_completion();

    void complete_curve();
    void complete_curve_1();
    void complete_curve_2();
    void finish_complete_curves();

    void choose_curve(int curve_choice_flag);
    void smooth_curve();
    void set_nb_points();
    void set_tiling_size();

    void choose_curve_2(int curve_choice_flag_2);
    void smooth_curve_2();
    void set_nb_points_2();

    void compute_circle_packing();
    void stop_computation();
    void end_of_computation();
    void show_final_cp();
    void show_inverse();

    void choose_coloring();
    void choose_coloring_2();
    bool load_image();
};


#endif // WINDOW_HPP
