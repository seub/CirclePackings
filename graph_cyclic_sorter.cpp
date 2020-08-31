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

#include "graph_cyclic_sorter.hpp"

template <typename T> Graph_Cyclic_Sorter<T>::Graph_Cyclic_Sorter(Graph<T> *G)
{
    G_ = G;
}

template <typename T> Graph<Empty> Graph_Cyclic_Sorter<T>::neighbors_subgraph(const unsigned int &index) const
{
    std::vector<vertex_label> N = G_->get_neighbors_by_index(index);
    Graph<Empty> G;
    unsigned int i;
    for(i=0; i<N.size(); i++)
    {
        G.add_isolated_vertex(N[i], Empty(), false);

    }

    unsigned int j;
    for(i=0; i<N.size(); i++)
    {
        for(j=0; j<i; j++)
        {
            if (G_->are_neighbor_labels(N[i],N[j]))
            {
                G.add_edge_by_labels(N[i], N[j]);
            }
        }
    }
    return G;
}

template <typename T> bool Graph_Cyclic_Sorter<T>::cyclic_sort(Graph<Empty>& G)
{
    if (G.nb_vertices() > 3)
    {
        unsigned int i=0;
        while (i<G.nb_vertices() && G.get_neighbors_by_index(i).size() != 2)
        {
            i++;
        }

        if(i==G.nb_vertices())
        {
            return false;
        }

        vertex_label label = G.get_label_by_index(i);
        vertex_label left_label = G.get_neighbors_by_index(i)[0];
        vertex_label right_label = G.get_neighbors_by_index(i)[1];
        G.remove_vertex_by_index(i);
        if (G.are_neighbor_labels(left_label, right_label))
        {
            if(cyclic_sort(G))
            {
                reinsert_neighbor(label, left_label, right_label, G);
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            G.add_edge_by_labels(left_label, right_label);
            if(cyclic_sort(G))
            {
                reinsert_neighbor(label, left_label, right_label,G);
                G.remove_edge_by_labels(left_label, right_label);
                return true;
            }
            else
            {
                return false;
            }
        }
    }
    return true;
}

template <typename T> void Graph_Cyclic_Sorter<T>::reinsert_neighbor(const vertex_label &label,const vertex_label &left_label, const vertex_label &right_label, Graph<Empty> & G)
{
    unsigned int left_index = G.label_to_index(left_label);
    unsigned int right_index = G.label_to_index(right_label);
    unsigned int index_min = (left_index < right_index)? left_index : right_index;
    unsigned int index_max = (left_index < right_index)? right_index : left_index;

    unsigned int index = (index_min == 0 && index_max == G.nb_vertices()-1)? G.nb_vertices() : index_min + 1;
    G.insert_isolated_vertex(index, label, Empty());

    G.add_edge_by_labels(left_label, label);
    G.add_edge_by_labels(label, right_label);
    return;
}

template <typename T> bool Graph_Cyclic_Sorter<T>::neighbors_cyclic_sort(const unsigned int &index)
{
    Graph<Empty> G = neighbors_subgraph(index);
    if (cyclic_sort(G))
    {
        unsigned int j;
        for (j=0; j<G.nb_vertices(); j++)
        {
            G_->set_neighbor_label_by_indices(index, j, G.get_label_by_index(j));
        }
        return true;
    }
    else
    {
        return false;
    }
}

template <typename T> bool Graph_Cyclic_Sorter<T>::total_cyclic_sort()
{
    unsigned int i=0, N = G_->nb_vertices();
    while (i< N && neighbors_cyclic_sort(i))
    {
        i++;
    }
    return (i==N);
}

template class Graph_Cyclic_Sorter<Empty>;
template class Graph_Cyclic_Sorter<Point>;
template class Graph_Cyclic_Sorter<Circle>;
