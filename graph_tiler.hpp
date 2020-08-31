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

#ifndef GRAPH_TILER_HPP
#define GRAPH_TILER_HPP

#include <QObject>

#include "basic_types.hpp"
#include "graph.hpp"

class Graph_Tiler : public QObject
{
    Q_OBJECT

public:
    Graph_Tiler() {}

    void reset_for_tiling(Graph<Circle>* tiling, double tiling_step);
    void reset_for_circle_packing(Graph<Circle> *tiling, Graph<Circle> *circle_packing,
                                  Graph<Empty> *abstract_graph, complex_number *angle,
                                  Curve *curve);
    void create_tiling();
public slots:

    bool create_circle_packing();

signals:
    void tiling_created();

private:
    bool zero_neighbors_sort(Graph<Empty> &G);

    void create_to_be_added(unsigned int &output_nb_vertices, const Graph<Circle> &tiling_graph, const Curve &curve);
    void create_graphs(const unsigned int &nb_vertices, Graph<Empty> &output_abstract_graph, const Graph<Circle> &tiling_graph);

    Graph<Circle> *tiling_;
    Graph<Circle> *circle_packing_;
    Graph<Empty> *abstract_graph_;
    complex_number *angle_;
    Curve *curve_;
    double tiling_step_;

    std::vector<bool> to_be_added_;
};

#endif // GRAPH_TILER_HPP
