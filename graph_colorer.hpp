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

#ifndef GRAPH_COLORER_HPP
#define GRAPH_COLORER_HPP

#include "basic_types.hpp"
#include "graph.hpp"

class Graph_Colorer
{
public:
    Graph_Colorer(Graph<Circle> *graph, const coloring_type &coloring);
    void color();
    void extrema_tiling(int &x_min, int &x_max, int &y_min, int &y_max);
    void compute_erdos();

private:
    void color_plain();
    void color_stripes1();
    void color_stripes2();
    void color_stripes3();
    void color_flowers();
    void color_tiling();
    void color_shading();
    void color_erdos();

    Graph<Circle> *graph_;
    coloring_type coloring_;
    std::vector<int> nb_erdos_;
};

#endif // GRAPH_COLORER_HPP
