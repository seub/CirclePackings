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

#include "graph_orienter.hpp"

template <typename T>
Graph_Orienter<T>::Graph_Orienter(Graph<T> *G)
{
    G_ = G;
}

bool are_correctly_oriented(const complex_number &vector_1, const complex_number &vector_2, const complex_number &vector_3)
{
    int test=0;
    if(imag(conj(vector_1)*vector_2)>0)
    {
        test++;
    }
    if(imag(conj(vector_2)*vector_3)>0)
    {
        test++;
    }
    if(imag(conj(vector_3)*vector_1)>0)
    {
        test++;
    }
    return test>1;
}

template <typename T> bool Graph_Orienter<T>::are_neighbors_correctly_oriented(const unsigned int &index) const
{
    std::vector<vertex_label> neighbor_labels = G_->get_neighbors_by_index(index);
    if (neighbor_labels.size() < 2)
    {
        return true;
    }
    else if(neighbor_labels.size() == 2)
    {
        complex_number z = G_->get_affix_by_index(index);
        complex_number z_1 = G_->get_affix_by_label(neighbor_labels[0]);
        complex_number z_2 = G_->get_affix_by_label(neighbor_labels[1]);
        return imag(conj(z_1 - z) * (z_2 - z))>0;
    }
    else
    {
        complex_number z = G_->get_affix_by_index(index);
        complex_number z_1 = G_->get_affix_by_label(neighbor_labels[0]);
        complex_number z_2 = G_->get_affix_by_label(neighbor_labels[1]);
        complex_number z_3 = G_->get_affix_by_label(neighbor_labels[2]);
        return are_correctly_oriented(z_1 - z, z_2 - z, z_3 - z);
    }
}

template <typename T> void Graph_Orienter<T>::correct_all_orientations()
{
    unsigned int i;
    for (i=0; i<G_->nb_vertices(); i++)
    {
        if (!are_neighbors_correctly_oriented(i))
        {
            G_->reverse_neighbors_by_index(i);
        }
    }

    return;
}


template class Graph_Orienter<Point>;
template class Graph_Orienter<Circle>;
