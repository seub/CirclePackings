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

#ifndef MATHS_CONTENTS_HPP
#define MATHS_CONTENTS_HPP


#include "basic_types.hpp"
#include "point.hpp"
#include "graph.hpp"
#include "graph_completer.hpp"
#include "curve.hpp"
#include "top_algo.hpp"
#include "triangulation.hpp"
#include "triangulation_search_tree.hpp"
#include "riemann_positions.hpp"
#include "top_menu.hpp"
#include "graph_sampler.hpp"

#include <QImage>

class Maths_Contents
{
    friend class Window;
    friend class Top_Algo;
    friend class Configuration;

public:
    Maths_Contents();

private:
    void reset_all_but_tiling();
    void synchronise_colors(mode_type mode_flag);
    void synchronise_triangulation_colors();
    void complete_input_planar_graph();
    void initialize_curve_1(curve_choice_type curve_choice_flag, unsigned int nb_sides);
    void initialize_curve_2(curve_choice_type curve_choice_flag_2, unsigned int nb_sides_2);
    bool create_circle_packing_inside_curve();
    void initialize_star_1(const unsigned int& nb_points);
    void initialize_star_2(const unsigned int& nb_points_2);
    void synchronise_input_colors(mode_type);
    void create_default_tangent_factors_1();
    void create_default_tangent_factors_2();

    Graph<Point> input_planar_graph_;
    Graph_Completer graph_completer_;
    Graph_Sampler graph_sampler_;

    Curve curve_1_;
    std::vector<Point> nodes_1_;
    std::vector<complex_number> tangent_factors_1_;
    Graph<Circle> input_circle_packing_1_;

    Curve curve_2_;
    std::vector<Point> nodes_2_;
    std::vector<complex_number> tangent_factors_2_;
    Graph<Circle> input_circle_packing_2_;

    Graph<Circle> tiling_;

    Graph<Empty> input_algo_graph_1_;
    complex_number input_algo_angle_1_;
    Graph<Empty> input_algo_graph_2_;
    complex_number input_algo_angle_2_;


    Top_Algo top_algo_;

    Graph<Circle> show_live_cp_left_;
    Graph<Circle> show_live_cp_right_;

    Graph<Circle> output_circle_packing_save_1_;
    Graph<Circle> output_circle_packing_save_2_;

    Graph<Circle> output_circle_packing_1_;
    Graph<Circle> output_circle_packing_2_;

    Triangulation triangulation_1_;
    Triangulation triangulation_2_;
    Triangulation circle_triangulation_1_;
    Triangulation circle_triangulation_2_;

    Triangulation_Search_Tree input_triangulation_search_tree_1_;
    Triangulation_Search_Tree input_triangulation_search_tree_2_;
    Triangulation_Search_Tree circle_triangulation_search_tree_1_;
    Triangulation_Search_Tree circle_triangulation_search_tree_2_;
    Triangulation_Search_Tree output_triangulation_search_tree_1_;
    Triangulation_Search_Tree output_triangulation_search_tree_2_;

    Triangulation output_triangulation_1_;
    Triangulation output_triangulation_2_;

    Triangulation triangulation_1_temp_;
    Triangulation triangulation_2_temp_;

    Riemann_Positions riemann_positions_1_;
    Riemann_Positions riemann_positions_2_;

    QImage image_;
    double left_image_scale_;
    int left_image_origin_x_in_pixels_;
    int left_image_origin_y_in_pixels_;
    double right_image_scale_;
    int right_image_origin_x_in_pixels_;
    int right_image_origin_y_in_pixels_;
};

#endif // MATHS_CONTENTS_HPP
