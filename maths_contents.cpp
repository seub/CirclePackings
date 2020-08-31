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

#include "maths_contents.hpp"
#include "graph_tiler.hpp"
#include "graph_triangulater.hpp"

Maths_Contents::Maths_Contents()
{
    riemann_positions_1_.reset_riemann_positions(&triangulation_1_, &output_triangulation_2_, &output_circle_packing_1_,
                                                 &output_circle_packing_2_, &curve_2_, &input_algo_graph_1_, &input_algo_graph_2_,
                                                 &triangulation_2_, &circle_triangulation_2_,
                                                 &input_triangulation_search_tree_1_,
                                                 &circle_triangulation_search_tree_2_,
                                                 &output_triangulation_search_tree_2_, true, true);
    riemann_positions_2_.reset_riemann_positions(&triangulation_2_, &output_triangulation_1_, &output_circle_packing_2_,
                                                 &output_circle_packing_1_, &curve_1_, &input_algo_graph_2_, &input_algo_graph_1_,
                                                 &triangulation_1_, &circle_triangulation_1_,
                                                 &input_triangulation_search_tree_2_,
                                                 &circle_triangulation_search_tree_1_,
                                                 &output_triangulation_search_tree_1_, true, true);
    graph_sampler_.reset(&input_planar_graph_, &input_algo_graph_1_, &input_algo_angle_1_);

    image_.load("CP.JPG", "JPG");

}

void Maths_Contents::reset_all_but_tiling()
{
    input_planar_graph_.reset_graph();

    curve_1_.reset_curve();
    nodes_1_.clear();
    tangent_factors_1_.clear();
    tangent_factors_1_.clear();
    input_circle_packing_1_.reset_graph();

    curve_2_.reset_curve();
    nodes_2_.clear();
    tangent_factors_2_.clear();
    tangent_factors_2_.clear();
    input_circle_packing_2_.reset_graph();

    input_algo_graph_1_.reset_graph();
    input_algo_graph_2_.reset_graph();

    show_live_cp_left_.reset_graph();
    show_live_cp_right_.reset_graph();

    output_circle_packing_save_1_.reset_graph();
    output_circle_packing_save_2_.reset_graph();
    output_circle_packing_1_.reset_graph();
    output_circle_packing_2_.reset_graph();

    triangulation_1_.reset_triangulation();
    triangulation_2_.reset_triangulation();
    circle_triangulation_1_.reset_triangulation();
    circle_triangulation_2_.reset_triangulation();

    output_triangulation_1_.reset_triangulation();
    output_triangulation_2_.reset_triangulation();

    triangulation_1_temp_.reset_triangulation();
    triangulation_2_temp_.reset_triangulation();

    Triangulation_Search_Tree empty_tree;
    input_triangulation_search_tree_1_ = empty_tree;
    input_triangulation_search_tree_2_ = empty_tree;
    output_triangulation_search_tree_1_ = empty_tree;
    output_triangulation_search_tree_2_ = empty_tree;
    circle_triangulation_search_tree_1_ = empty_tree;
    circle_triangulation_search_tree_2_ = empty_tree;

    return;
}

void Maths_Contents::synchronise_colors(mode_type mode_flag)
{
    unsigned int N;
    switch (mode_flag)
    {
    case START:
        std::cout << "ERROR in Maths_Contents::synchronise_colors: not supposed to reach here" << std::endl;
        throw(QString("ERROR in Maths_Contents::synchronise_colors: not supposed to reach here"));
        break;
    case DRAW_GRAPH:
        std::cout << "ERROR in Maths_Contents::synchronise_colors: not supposed to reach here" << std::endl;
        throw(QString("ERROR in Maths_Contents::synchronise_colors: not supposed to reach here"));
        break;
    case DRAW_CURVE:
        N = input_circle_packing_1_.nb_vertices();
        if (N!=output_circle_packing_1_.nb_vertices())
        {
            std::cout << "ERROR in Maths_Contents::synchronise_colors: ";
            std::cout << "input and output graphs do not have the same number of vertices" << std::endl;
            throw(QString("ERROR in Maths_Contents::synchronise_colors: wrong number of vertices"));
        }
        else
        {
            unsigned int i;
            for (i=0; i<N; i++)
            {
                output_circle_packing_1_.set_color_by_index(i, input_circle_packing_1_.get_color_by_index(i));
                output_circle_packing_save_1_.set_color_by_index(i, input_circle_packing_1_.get_color_by_index(i));
            }
        }
        break;
    default:
        std::cout << "ERROR in Maths_Contents::synchronise_colors: flag problem" << std::endl;
        throw(QString("ERROR in Maths_Contents::synchronise_colors: flag problem"));
    }

    return;
}

void Maths_Contents::synchronise_triangulation_colors()
{
    std::vector<QColor> colors = triangulation_1_.get_colors();
    output_triangulation_2_.set_colors(colors);

    colors = triangulation_2_.get_colors();
    output_triangulation_1_.set_colors(colors);
    return;
}

void Maths_Contents::synchronise_input_colors(mode_type mode_flag)
{
    unsigned int N;
    switch (mode_flag)
    {
    case START:
        std::cout << "ERROR in Maths_Contents::synchronise_input_colors: not supposed to reach here" << std::endl;
        throw(QString("ERROR in Maths_Contents::synchronise_input_colors: not supposed to reach here"));
        break;
    case DRAW_GRAPH:
        std::cout << "ERROR in Maths_Contents::synchronise_input_colors: not supposed to reach here" << std::endl;
        throw(QString("ERROR in Maths_Contents::synchronise_input_colors: not supposed to reach here"));
        break;
    case DRAW_CURVE:
        N = input_circle_packing_1_.nb_vertices();
        if ((N+1)!=input_algo_graph_1_.nb_vertices())
        {
            std::cout << "ERROR in Maths_Contents::synchronise_input_colors: ";
            std::cout << "wrong number of vertices" << std::endl;
            throw(QString("ERROR in Maths_Contents::synchronise_input_colors: wrong number of vertices"));
        }
        else
        {
            unsigned int i;
            for (i=0; i<N; i++)
            {
                input_algo_graph_1_.set_color_by_index(i+1, input_circle_packing_1_.get_color_by_index(i));
            }
        }
        break;
    default:
        std::cout << "ERROR in Maths_Contents::synchronise_colors: flag problem" << std::endl;
        throw(QString("ERROR in Maths_Contents::synchronise_colors: flag problem"));
    }

    return;
}

void Maths_Contents::complete_input_planar_graph()
{
    graph_completer_.set_graph(&input_planar_graph_);
    graph_completer_.set_output(&input_algo_graph_1_, &input_algo_angle_1_);
    graph_completer_.start();
    return;
}

void Maths_Contents::initialize_curve_1(curve_choice_type curve_choice_flag, unsigned int nb_sides)
{
    nodes_1_.clear();

    switch(curve_choice_flag)
    {
    case CUSTOM:
        break;
    case RECTANGLE:
        nodes_1_.push_back(Point(complex_number(1.0, 1.0)));
        break;
    case POLYGON:
        nodes_1_.push_back(Point(complex_number(1.0, 0.0)));
        break;
    case STAR:
        initialize_star_1(nb_sides);
        break;
    case ELLIPSE:
        nodes_1_.push_back(Point(complex_number(1.0, 1.0)*(sqrt(2.0)/2.0)));
        break;
    case EPICYCLOID:
        nodes_1_.push_back(Point(complex_number((nb_sides*1.3)/(nb_sides + 2), 0.0)));
        break;
    case KOCH:
        nodes_1_.push_back(Point(complex_number(1.0, 1.0/sqrt(3.0))));
        break;
    default:
        std::cout << "ERROR in Maths_Contents::initialize_curve_1: flag problem" << std::endl;
        throw(QString("ERROR in Maths_Contents::initialize_curve_1: flag problem"));
    }

    if (curve_choice_flag!=STAR)
    {
        curve_1_.create_curve(nodes_1_, tangent_factors_1_, curve_choice_flag, false, nb_sides, 0);
    }
    return;
}

void Maths_Contents::initialize_curve_2(curve_choice_type curve_choice_flag_2, unsigned int nb_sides_2)
{
    nodes_2_.clear();

    switch(curve_choice_flag_2)
    {
    case CUSTOM:
        break;
    case RECTANGLE:
        nodes_2_.push_back(Point(complex_number(1.0, 1.0)));
        break;
    case POLYGON:
        nodes_2_.push_back(Point(complex_number(1.0, 0.0)));
        break;
    case STAR:
        initialize_star_2(nb_sides_2);
        break;
    case ELLIPSE:
        nodes_2_.push_back(Point(complex_number(1.0, 1.0)*(sqrt(2.0)/2.0)));
        break;
    case EPICYCLOID:
        nodes_2_.push_back(Point(complex_number((nb_sides_2*1.3)/(nb_sides_2 + 2), 0.0)));
        break;
    case KOCH:
        nodes_2_.push_back(Point(complex_number(1.0, 1.0/sqrt(3.0))));
        break;
    default:
        std::cout << "ERROR in Maths_Contents::initialize_curve_2: flag problem" << std::endl;
        throw(QString("ERROR in Maths_Contents::initialize_curve_2: flag problem"));
    }

    if (curve_choice_flag_2!=STAR)
    {
        curve_2_.create_curve(nodes_2_, tangent_factors_2_, curve_choice_flag_2, false, nb_sides_2, 0);
    }
    return;
}

bool Maths_Contents::create_circle_packing_inside_curve()
{
    Graph_Tiler GT;
    GT.reset_for_circle_packing(&tiling_, &input_circle_packing_1_, &input_algo_graph_1_, &input_algo_angle_1_, &curve_1_);
    return GT.create_circle_packing();
}

void Maths_Contents::initialize_star_1(const unsigned int &nb_points)
{
    nodes_1_.clear();
    complex_number Z(1.0, 0.0);
    complex_number z;
    double theta;

    if (nb_points==3)
    {
        z = 0.10*complex_number(1.0+sqrt(3.0));
    }
    else if(nb_points==4)
    {
        z = 0.25*complex_number(1.0,1.0);
    }
    else
    {
        theta = M_PI/nb_points;
        z  = complex_number(cos(2*theta), cos(2*theta)*tan(theta));
    }

    nodes_1_.push_back(Point(Z));
    nodes_1_.push_back(Point(z));
    curve_1_.create_star(nodes_1_, 0, nb_points);
}

void Maths_Contents::initialize_star_2(const unsigned int &nb_points_2)
{
    nodes_2_.clear();
    complex_number Z(1.0, 0.0);
    complex_number z;
    double theta;

    if (nb_points_2==3)
    {
        z = 0.10*complex_number(1.0+sqrt(3.0));
    }
    else if(nb_points_2==4)
    {
        z = 0.25*complex_number(1.0,1.0);
    }
    else
    {
        theta = M_PI/nb_points_2;
        z  = complex_number(cos(2*theta), cos(2*theta)*tan(theta));
    }

    nodes_2_.push_back(Point(Z));
    nodes_2_.push_back(Point(z));
    curve_2_.create_star(nodes_2_, 0, nb_points_2);
}

void Maths_Contents::create_default_tangent_factors_1()
{
    unsigned int n = nodes_1_.size();
    tangent_factors_1_.resize(n);
    std::fill(tangent_factors_1_.begin(), tangent_factors_1_.end(), complex_number(0.25, 0.0));
    return;
}

void Maths_Contents::create_default_tangent_factors_2()
{
    unsigned int n = nodes_2_.size();
    tangent_factors_2_.resize(n);
    std::fill(tangent_factors_2_.begin(), tangent_factors_2_.end(), complex_number(0.25, 0.0));
    return;
}


