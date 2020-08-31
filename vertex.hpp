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

#ifndef VERTEX_HPP
#define VERTEX_HPP

#include <QColor>

#include "basic_types.hpp"
#include "point.hpp"
#include "circle.hpp"
#include "empty.hpp"

class Mobius;
template <typename T> class Graph;

template <typename T> class Vertex
{
    template <typename T2> friend std::ostream& operator<<(std::ostream& out, const Vertex<T2> &V);
    friend class Graph<Empty>;
    friend class Graph<Point>;
    friend class Graph<Circle>;
    friend class Graph_Tiler;


private:
    Vertex();
    Vertex(const vertex_label &label, T content, QColor color);
    Vertex(const vertex_label &label, T content);
    bool is_neighbor_label(const vertex_label &label) const;

    bool add_neighbor_label(const vertex_label &label);
    bool remove_neighbor_label(const vertex_label &label);
    void reverse_neighbor_labels();
    void rotate_neighbor_labels(const unsigned int& shift);

    // Specialization to Vertex<Circle>
    void apply_mobius(const Mobius &mobius);

    vertex_label label_;
    T content_;
    std::vector<vertex_label> neighbor_labels_;
    integer_coordinates tiling_coordinates_;
    QColor color_;
};










template <typename T>
std::ostream& operator<<(std::ostream& out, const Vertex<T>& V)
{
    out << "Vertex of label " << V.label_ << ".";
    out << " The vertex's content is: " << V.content_ << ".";

    if(V.neighbor_labels_.size() > 0)
    {
        out << " The vertex has " << V.neighbor_labels_.size() << " neighbors:";
        unsigned int i;
        for(i=0; i<V.neighbor_labels_.size(); i++)
        {
            out << " " << V.neighbor_labels_[i];
        }
        out << ".";
    }
    else
    {
        out << " The vertex does not have any neighbors.";
    }
    return out;
}

#endif // VERTEX_HPP
