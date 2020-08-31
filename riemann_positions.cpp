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

#include "riemann_positions.hpp"
#include "graph_triangulater.hpp"

void Riemann_Positions::reset_riemann_positions(Triangulation *initial_triangulation, Triangulation *other_side_final_triangulation,
                                                Graph<Circle> *circle_cp, Graph<Circle> *other_side_circle_cp,
                                                Curve *other_side_curve,
                                                Graph<Empty> *abstract_graph, Graph<Empty> *other_side_abstract_graph,
                                                Triangulation *other_side_initial_triangulation,
                                                Triangulation *other_side_circle_triangulation,
                                                Triangulation_Search_Tree *initial_triangulation_tree,
                                                Triangulation_Search_Tree *other_side_circle_triangulation_tree,
                                                Triangulation_Search_Tree *other_side_final_triangulation_tree,
                                                bool compute_other_side_final_triangulation_tree, bool compute_initial_triangulation_tree)
{
    initial_triangulation_ = initial_triangulation;
    other_side_final_triangulation_ = other_side_final_triangulation;
    circle_cp_ = circle_cp;
    other_side_circle_cp_ = other_side_circle_cp;
    other_side_curve_ = other_side_curve;
    abstract_graph_ = abstract_graph;
    other_side_abstract_graph_ = other_side_abstract_graph;
    other_side_initial_triangulation_ = other_side_initial_triangulation;
    other_side_circle_triangulation_ = other_side_circle_triangulation;

    initial_triangulation_tree_ = initial_triangulation_tree;
    other_side_circle_triangulation_tree_ = other_side_circle_triangulation_tree;
    other_side_final_triangulation_tree_ = other_side_final_triangulation_tree;
    compute_final_triangulation_search_tree_ = compute_other_side_final_triangulation_tree;
    compute_initial_triangulation_search_tree_ = compute_initial_triangulation_tree;
    return;
}

void Riemann_Positions::compute_circle_triangulation()
{
    //std::cout << "Entering Riemann_Positions::compute_circle_triangulation()" << std::endl;

    other_side_circle_triangulation_->reset_triangulation();
    Curve *circle = new Curve;
    std::vector<Point> nodes;
    nodes.push_back(Point(1.0 / sqrt(2.0) * complex_number(1.0, 1.0)));
    circle->create_ellipse(nodes, 100);
    Graph_Triangulater GT;
    GT.reset(other_side_circle_cp_, other_side_circle_triangulation_, other_side_abstract_graph_,
             circle, other_side_circle_triangulation_tree_, true);
    GT.create_triangle_packing(true);
    delete circle;
    return;
}

void Riemann_Positions::compute_output_circle_packing()
{
    output_circle_packing_ = *circle_cp_;
    double alpha, beta, gamma;
    unsigned int i, triangle_index;
    Point point;
    for(i=0;i<circle_cp_->nb_vertices();i++)
    {
        if(other_side_circle_triangulation_tree_->get_containing_triangle_index(alpha, beta, gamma, triangle_index,
                                                         circle_cp_->get_content_by_index(i).get_centre()))
        {
            point = other_side_initial_triangulation_->get_triangle(triangle_index).point_from_barycentric_coordinates(alpha, beta, gamma);
            output_circle_packing_.set_content_by_index(i, Circle(point, 0));
        }
        else
        {
            other_side_circle_triangulation_->nearest_exterior_triangle(triangle_index, alpha, beta, gamma,
                                                             circle_cp_->get_content_by_index(i).get_centre());
            point = other_side_initial_triangulation_->get_triangle(triangle_index).point_from_barycentric_coordinates(alpha, beta, gamma);
            output_circle_packing_.set_content_by_index(i, Circle(point, 0));
        }
    }
    return;
}

void Riemann_Positions::compute_final_triangulation(bool create_output_triangulation_search_tree)
{
    //std::cout << "Entering Riemann_Positions::compute_final_triangulation" << std::endl;

    final_triangulation_temp_.reset_triangulation();
    Graph_Triangulater GT;
    GT.reset(&output_circle_packing_, &final_triangulation_temp_, abstract_graph_,
             other_side_curve_, other_side_final_triangulation_tree_,
             create_output_triangulation_search_tree);
    GT.create_triangle_packing(true);
    std::vector<QColor> colors;
    colors = initial_triangulation_->get_colors();
    final_triangulation_temp_.set_colors(colors);
    *other_side_final_triangulation_ = final_triangulation_temp_;
    return;
}

void Riemann_Positions::run()
{
    if (compute_initial_triangulation_search_tree_)
    {
        initial_triangulation_tree_->reset_search_tree(initial_triangulation_);
    }

    compute_circle_triangulation();

    compute_output_circle_packing();

    compute_final_triangulation(compute_final_triangulation_search_tree_);

    return;
}

Point Riemann_Positions::compute_image_in_output_triangulation(const Point &point_in_unit_disk) const
{
    double alpha, beta, gamma;
    unsigned int triangle_index;
    if(other_side_circle_triangulation_tree_->get_containing_triangle_index(alpha, beta, gamma, triangle_index,
                                                     point_in_unit_disk))
    {
        return other_side_initial_triangulation_->get_triangle(triangle_index).point_from_barycentric_coordinates(alpha, beta, gamma);
    }
    else
    {
        other_side_circle_triangulation_->nearest_exterior_triangle(triangle_index, alpha, beta, gamma,
                                                         point_in_unit_disk);
        return other_side_initial_triangulation_->get_triangle(triangle_index).point_from_barycentric_coordinates(alpha, beta, gamma);
    }
}

Point Riemann_Positions::compute_inverse_image_in_unit_disk(const Point &point_in_initial_triangulation_2,
                                                            const Triangulation_Search_Tree *initial_triangulation_2_search_tree) const
{
    double alpha, beta, gamma;
    unsigned int triangle_index;
    if (initial_triangulation_2_search_tree->get_containing_triangle_index(alpha, beta, gamma, triangle_index,
                                                                           point_in_initial_triangulation_2))
    {
        return other_side_circle_triangulation_->get_triangle(triangle_index).point_from_barycentric_coordinates(alpha, beta, gamma);
    }
    else
    {
        std::cout << "WARNING in Riemann_Positions::compute_inverse_image: input point is not in triangulation 2" << std::endl;
        other_side_initial_triangulation_->nearest_exterior_triangle(triangle_index, alpha, beta, gamma, point_in_initial_triangulation_2);
        return other_side_circle_triangulation_->get_triangle(triangle_index).point_from_barycentric_coordinates(alpha, beta, gamma);
    }
}
