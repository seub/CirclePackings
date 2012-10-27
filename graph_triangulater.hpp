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

#ifndef GRAPH_TRIANGULATER_HPP
#define GRAPH_TRIANGULATER_HPP


#include <QThread>

#include "basic_types.hpp"
#include "graph.hpp"
#include "triangulation.hpp"

class Triangulation_Search_Tree;

class Graph_Triangulater : public QThread
{
    Q_OBJECT

public:
    Graph_Triangulater() {}

    void reset(Graph<Circle>* circle_packing, Triangulation *triangulation, Graph<Empty> *abstract_graph,
               Curve *curve, Triangulation_Search_Tree *triangulation_search_tree = NULL, bool create_search_tree = false,
               bool *valid = NULL, complex_number *angle = NULL,
               Graph<Circle> *tiling_graph = NULL, Triangulation_coloring coloring = T_PLAIN);
    bool create_circle_and_triangle_packing();
    void create_triangle_packing(bool res); // !Does not set colors!

public slots:
    void run();

private:
    void add_interior_points();
    bool add_exterior_points();
    void add_interior_triangles();
    void add_exterior_triangles();

    bool create_directions();
    void create_turns();

    integer_coordinates neighbor_tiling_coordinates(const integer_coordinates &tiling_coordinates, int neighbor_index) const;

    Graph<Circle> *circle_packing_;
    Triangulation *triangulation_;
    Graph<Empty> *abstract_graph_;

    bool *valid_;
    complex_number *angle_;
    Graph<Circle> *tiling_graph_;
    Curve *curve_;
    Triangulation_coloring coloring_;

    std::vector<int> directions_;
    std::vector<int> turns_;

    Triangulation_Search_Tree *triangulations_search_tree_;
    bool create_search_tree_;
};

#endif // GRAPH_TRIANGULATER_HPP
