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

#include "vertex.hpp"
#include "mobius.hpp"

template<typename T> Vertex<T>::Vertex()
{
    neighbor_labels_.reserve(6);
}

template<typename T> Vertex<T>::Vertex(const vertex_label &label, T content, QColor color)
{
    label_ = label;
    content_ = content;
    neighbor_labels_.reserve(6);
    color_ = color;
}

template<typename T> Vertex<T>::Vertex(const vertex_label &label, T content)
{
    label_ = label;
    content_ = content;
    neighbor_labels_.reserve(6);
}

template<typename T> bool Vertex<T>::is_neighbor_label(const vertex_label &label) const
{
    if (label_ == label)
    {
        std::cout << "ERROR in bool Vertex<T>::is_neighbor_label: " << label << " is the label of the vertex!" << std::endl;
        throw(QString("ERROR in bool Vertex<T>::is_neighbor_label: label of the vertex"));
    }

    int counter = 0;
    unsigned int i = 0;
    while(counter<2 && i<neighbor_labels_.size())
    {
        if (neighbor_labels_[i]==label)
        {
            counter++;
        }
        i++;
    }

    if (counter==0)
    {
        return false;
    }
    else if (counter==1)
    {
        return true;
    }
    else
    {
        std::cout << "ERROR in Vertex<T>::is_neighbor: " << label << " is the label of several neighbors!" << std::endl;
        throw(QString("ERROR in Vertex<T>::is_neighbor: label of several neighbors"));
        return true;
    }
}

template<typename T> bool Vertex<T>::add_neighbor_label(const vertex_label &label)
{
    if (label_ == label)
    {
        std::cout << "ERROR in Vertex<T>::add_neighbor_label: " << label << " is the label of the vertex!" << std::endl;
        throw(QString("ERROR in Vertex<T>::add_neighbor_label: of the vertex"));
        return false;
    }
    else if(is_neighbor_label(label))
    {
        std::cout << "ERROR in Vertex<T>::add_neighbor_label: " << label << " is already a neigbor label!" << std::endl;
        throw(QString("ERROR in Vertex<T>::add_neighbor_label: already a neighbor label"));
        return false;
    }
    else
    {
        neighbor_labels_.push_back(label);
        return true;
    }
}

template<typename T> bool Vertex<T>::remove_neighbor_label(const vertex_label &label)
{
    if (is_neighbor_label(label))
    {
        neighbor_labels_.erase(remove(neighbor_labels_.begin(), neighbor_labels_.end(), label), neighbor_labels_.end());
        return true;
    }
    else
    {
        std::cout << "ERROR in Vertex<T>::remove_neighbor_label: " << label << " is not a neigbor label!" << std::endl;
        throw(QString("ERROR in Vertex<T>::remove_neighbor_label: not a neighbor label"));
        return false;
    }
}

template <typename T> void Vertex<T>::reverse_neighbor_labels()
{
    reverse(neighbor_labels_.begin(), neighbor_labels_.end());
    return;
}

template <typename T> void Vertex<T>::rotate_neighbor_labels(const unsigned int &shift)
{
    rotate(neighbor_labels_.begin(), neighbor_labels_.begin()+shift, neighbor_labels_.end());
    return;
}

template <> void Vertex<Circle>::apply_mobius(const Mobius &mobius)
{
    content_ = mobius*content_;
    return;
}


template class Vertex<Empty>;
template class Vertex<Point>;
template class Vertex<Circle>;
