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

#ifndef GRAPH_COMPLETER_HPP
#define GRAPH_COMPLETER_HPP

#include "basic_types.hpp"
#include "graph.hpp"
#include <QThread>


typedef std::pair<unsigned int, unsigned int> indices_pair;
typedef std::pair<indices_pair, double> lengthed_indices_pair;

class Graph_Completer : public QThread
{
public:
    Graph_Completer();
    void set_graph(Graph<Point>* G);
    void set_output(Graph<Empty>* output_abstract_graph, complex_number* output_angle);
    void set_zero_neighbors(std::vector<unsigned int> zero_neighbors);

    void complete_triangulation();
    bool crosses_an_edge(const segment& S);

    void set_stopped();
    bool get_stopped() const;
    Graph<Point> get_graph_save();

    void add_zero();
    bool sort_everything();

    void complete_maximal_graph();

public slots:
    void run();

private:
    void create_lengthed_indices_pairs();
    void sort_lengths();
    void add_edges();

    void compute_convex_hull();

    Graph<Point>* G_;
    Graph<Empty>* output_abstract_graph_;
    complex_number* output_angle_;

    std::vector<lengthed_indices_pair> lengthed_indices_pairs_;

    std::vector<unsigned int> zero_neighbors_;

    bool stopped_;
    Graph<Point> graph_save_;
};


bool compare_lengths(const lengthed_indices_pair &E_1, const lengthed_indices_pair &E_2);


#endif // GRAPH_COMPLETER_HPP
