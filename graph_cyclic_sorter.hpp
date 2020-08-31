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

#ifndef GRAPH_CYCLIC_SORTER_HPP
#define GRAPH_CYCLIC_SORTER_HPP

#include "basic_types.hpp"
#include "graph.hpp"

template <typename T> class Graph_Cyclic_Sorter
{
   friend class Graph_Completer;
   friend class Graph_Tiler;
public:
    Graph_Cyclic_Sorter(Graph<T>* G);
    bool total_cyclic_sort();
private:
    Graph<Empty> neighbors_subgraph(const unsigned int &index) const;
    bool neighbors_cyclic_sort(const unsigned int &index);
    bool cyclic_sort(Graph<Empty>& G);
    void reinsert_neighbor(const vertex_label &label,const vertex_label &left_label,
                           const vertex_label &right_label, Graph<Empty>& G); // Think of "Kansas".

    Graph<T>* G_;
};






#endif // GRAPH_CYCLIC_SORTER_HPP
