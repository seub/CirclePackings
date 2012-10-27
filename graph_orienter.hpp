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

#ifndef GRAPH_ORIENTER_HPP
#define GRAPH_ORIENTER_HPP

#include "basic_types.hpp"
#include "graph.hpp"


template <typename T> class Graph_Infinity_Adder;

template <typename T>
class Graph_Orienter
{
    friend class Graph_Completer;
    friend class Graph_Infinity_Adder<Point>;
    friend class Graph_Infinity_Adder<Circle>;

public:
    Graph_Orienter(Graph<T>* G);
    void correct_all_orientations();

private:
    bool are_neighbors_correctly_oriented(const unsigned int &index) const;
    Graph<T>* G_;
};






bool are_correctly_oriented(const complex_number &vector_1, const complex_number &vector_2, const complex_number &vector_3);

#endif // GRAPH_ORIENTER_HPP
