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

#ifndef RIEMANN_POSITIONS_HPP
#define RIEMANN_POSITIONS_HPP

#include <QThread>

#include "basic_types.hpp"
#include "graph.hpp"
#include "triangulation.hpp"
#include "triangulation_search_tree.hpp"

class Riemann_Positions: public QThread
{
public:
    Riemann_Positions(){}
    void reset_riemann_positions(Triangulation *initial_triangulation, Triangulation *other_side_final_triangulation,
                                 Graph<Circle> *circle_cp, Graph<Circle> *other_side_circle_cp,
                                 Curve *other_side_curve,
                                 Graph<Empty> *abstract_graph, Graph<Empty> *other_side_abstract_graph,
                                 Triangulation *other_side_initial_triangulation,
                                 Triangulation *other_side_circle_triangulation,
                                 Triangulation_Search_Tree *initial_triangulation_tree,
                                 Triangulation_Search_Tree *other_side_circle_triangulation_tree,
                                 Triangulation_Search_Tree *other_side_final_triangulation_tree,
                                 bool compute_other_side_final_triangulation_tree = false,
                                 bool compute_initial_triangulation_tree = false);


    void compute_circle_triangulation();
    void compute_output_circle_packing();
    void compute_final_triangulation(bool create_output_triangulation_search_tree);

    Point compute_image_in_output_triangulation(const Point &point_in_unit_disk) const;
    Point compute_inverse_image_in_unit_disk(const Point &point_in_initial_triangulation_2,
                                             const Triangulation_Search_Tree *initial_triangulation_2_search_tree) const;

public slots:
    void run();

private:
    Triangulation *initial_triangulation_;
    Triangulation *other_side_final_triangulation_;
    Graph<Circle> *circle_cp_;
    Graph<Circle> *other_side_circle_cp_;
    Curve *other_side_curve_;
    Graph<Empty> *abstract_graph_;
    Graph<Empty> *other_side_abstract_graph_;
    Triangulation *other_side_initial_triangulation_;

    Triangulation *other_side_circle_triangulation_;
    Graph<Circle> output_circle_packing_;
    Triangulation final_triangulation_temp_;

    Triangulation_Search_Tree *initial_triangulation_tree_;
    Triangulation_Search_Tree *other_side_circle_triangulation_tree_;
    Triangulation_Search_Tree *other_side_final_triangulation_tree_;
    bool compute_final_triangulation_search_tree_;
    bool compute_initial_triangulation_search_tree_;
};

#endif // RIEMANN_POSITIONS_HPP
