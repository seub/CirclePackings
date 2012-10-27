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

#ifndef GRAPH_SAMPLER_HPP
#define GRAPH_SAMPLER_HPP

#include "basic_types.hpp"
#include "graph.hpp"


class Graph_Sampler
{
public:
    Graph_Sampler() {}
    void reset(Graph<Point> * graph, Graph<Empty> *output_graph, complex_number *angle);
    void create_sample_graph(graph_choice_type graph_choice, unsigned int parameter_1 = 1, unsigned int parameter_2 = 1);
private:
    void create_grid(unsigned int size);
    void create_web(unsigned int size, unsigned int angle);
    void create_output_graph();

    std::vector<unsigned int> zero_neighbors_indices_;

    Graph<Point> * graph_;
    Graph<Empty> * output_graph_;
    complex_number * angle_;

};

#endif // GRAPH_SAMPLER_HPP
