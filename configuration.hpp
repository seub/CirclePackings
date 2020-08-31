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

#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include "basic_types.hpp"
#include "graph.hpp"

class Window;

class Configuration// : public QObject
{
    friend class Circle_Packings_Application;

public:
    Configuration(Window *window);
    void reset_all_parameters();
    void update_parameters();
    void restore_configuration();
    void write_in_file(std::ofstream &save_file);
    int read_from_file(std::ifstream &load_file);

private:
    Window *window_;

    //General settings
    mode_type mode_flag_;


    //Graph settings
    draw_graph_mode_type draw_graph_flag_;
    graph_choice_type graph_choice_flag_;
    int graph_parameter_1_;
    int graph_parameter_2_;
    Graph<Point> input_planar_graph_;

    //Curve settings
    int tiling_size_;
    int coloring_type_;

    //Curve 1 settings
    draw_curve_mode_type draw_curve_flag_1_;
    curve_choice_type curve_choice_flag_1_;
    bool smooth_curve_1_;
    unsigned int nb_sides_1_;
    std::vector<Point> nodes_1_;
    std::vector<complex_number> tangent_factors_1_;

    // 2 Curves settings
    draw_2_curves_mode_type draw_2_curves_flag_;
    bool inverse_;

    //Curve 2 settings
    draw_curve_mode_type draw_curve_flag_2_;
    curve_choice_type curve_choice_flag_2_;
    bool smooth_curve_2_;
    unsigned int nb_sides_2_;
    std::vector<Point> nodes_2_;
    std::vector<complex_number> tangent_factors_2_;

    void reset_window_pointer(Window *window);

    void update_graph();
    void update_curve_1();
    void update_curve_2();

    void restore_graph_configuration();
    void restore_curve_configuration();
    void restore_2_curves_configuration();

    void restore_curve_menu_1();
    void restore_curve_menu_2();
    void restore_sample_graph_menu();
    void restore_custom_graph_menu();
    void restore_left_curve_canvas();
    void restore_right_curve_canvas();



    void save_custom_graph(std::ofstream &save_file);
    void save_sample_graph(std::ofstream &save_file);

    void save_curve_1(std::ofstream &save_file);
    void save_curve_2(std::ofstream &save_file);

    void load_custom_graph(std::ifstream &load_file);
    void load_sample_graph(std::ifstream &load_file);

    void load_curve_1(std::ifstream &load_file);
    void load_curve_2(std::ifstream &load_file);

    int compute_key();
};

#endif // CONFIGURATION_H
