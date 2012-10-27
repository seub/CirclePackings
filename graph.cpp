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

#include "graph.hpp"
#include "graph_completer.hpp"
#include "graph_cyclic_sorter.hpp"
#include "graph_orienter.hpp"
#include "graph_tiler.hpp"
#include "graph_colorer.hpp"

template <typename T> Graph<T>::Graph()
{
}

template <typename T> void Graph<T>::reset_graph()
{
    vertices_.clear();
}

template <typename T> unsigned int Graph<T>::label_to_index(const vertex_label &label) const
{
    unsigned int res = 0;

    int counter=0;
    unsigned int i=0;
    while (i<vertices_.size() && counter<2)
    {
        if (vertices_[i].label_ == label)
        {
            res = i;
            counter++;
        }
        i++;
    }

    if (counter==0)
    {
        std::cout << "ERROR in Graph<T>::label_to_index: " << label << " is not a vertex label!" << std::endl;
        throw(QString("ERROR in Graph<T>::label_to_index: not a vertex label"));
    }
    else if (counter==2)
    {
        std::cout << "ERROR in Graph<T>::label_to_index: " << label << " is the label of several vertices!" << std::endl;
        throw(QString("ERROR in Graph<T>::label_to_index: label of several vertices"));
    }

    return res;
}

template <typename T> const vertex_label & Graph<T>::get_label_by_index(const unsigned int &index) const
{
    if (index >= vertices_.size())
    {
        std::cout << "ERROR in Graph<T>::get_label_by_index: bad index" << std::endl;
        throw(QString("ERROR in Graph<T>::get_label_by_index: bad index"));
    }
    return vertices_[index].label_;
}

template <typename T> const T & Graph<T>::get_content_by_index(const unsigned int &index) const
{
    if (index >= vertices_.size())
    {
        std::cout << "ERROR in Graph<T>::get_content_by_index: bad index " << index
                  << " (graph only has " << vertices_.size() << " vertices)" << std::endl;
        throw(QString("ERROR in Graph<T>::get_content_by_index: bad index"));
    }
    return vertices_[index].content_;
}

template <typename T> const T & Graph<T>::get_content_by_label(const vertex_label &label) const
{
    return vertices_[label_to_index(label)].content_;
}

template <typename T> const std::vector<vertex_label> & Graph<T>::get_neighbors_by_index(const unsigned int &index) const
{
    if (index >= vertices_.size())
    {
        std::cout << "ERROR in Graph<T>::get_neighbors_by_index: bad index" << std::endl;
        throw(QString("ERROR in Graph<T>::get_neighbors_by_index: bad index"));
    }
    return vertices_[index].neighbor_labels_;
}

template <typename T> const std::vector<vertex_label> & Graph<T>::get_neighbors_by_label(const vertex_label &label) const
{
    return vertices_[label_to_index(label)].neighbor_labels_;
}

template <typename T> const integer_coordinates & Graph<T>::get_tiling_coordinates_by_index(const unsigned int &index) const
{
    if (index >= vertices_.size())
    {
        std::cout << "ERROR in Graph<T>::get_tiling_coordinates_by_index: bad index " << index
                     << " (graph only has " << vertices_.size() << " vertices)" << std::endl;
        throw(QString("ERROR in Graph<T>::get_tiling_coordinates_by_index: bad index "));
    }

    return vertices_[index].tiling_coordinates_;
}


template <typename T> bool Graph<T>::is_label_in(const vertex_label &label) const
{
    int counter = 0;
    unsigned int i = 0;
    while(counter<2 && i<vertices_.size())
    {
        if (vertices_[i].label_==label)
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
        std::cout << "ERROR in Graph<T>::is_label_in: " << label << " is the label of several vertices!" << std::endl;
        throw(QString("ERROR in Graph<T>::is_label_in: label of several vertices"));
        return true;
    }
}

template <typename T> unsigned int Graph<T>::nb_edges() const
{
    unsigned int i;
    unsigned int res = 0;
    for (i=0; i<vertices_.size(); i++)
    {
        res += vertices_[i].neighbor_labels_.size();
    }
    return (int) (res/2);
}

template <typename T> unsigned int Graph<T>::nb_vertices() const
{
    return vertices_.size();
}

template <typename T> bool Graph<T>::are_neighbor_indices(const unsigned int &index_1, const unsigned int &index_2) const
{
    if (index_1 > vertices_.size() || index_2 > vertices_.size())
    {
        std::cout << "ERROR in Graph<T>::are_neighbor_indices: bad index" << std::endl;
        throw(QString("ERROR in Graph<T>::are_neighbor_indices: bad index"));
    }
    return vertices_[index_1].is_neighbor_label(vertices_[index_2].label_) && vertices_[index_2].is_neighbor_label(vertices_[index_1].label_);
}

template <typename T> bool Graph<T>::are_neighbor_labels(const vertex_label &label_1, const vertex_label &label_2) const
{
    return vertices_[label_to_index(label_1)].is_neighbor_label(label_2) && vertices_[label_to_index(label_2)].is_neighbor_label(label_1);
}

template <typename T> bool Graph<T>::add_isolated_vertex(const vertex_label &label, const T &content, const QColor &color, bool  label_test)
{
    if (label_test && is_label_in(label))
    {
        std::cout << "ERROR in Graph<T>::add_isolated_vertex: " << label << " is already a vertex label!" << std::endl;
        throw(QString("ERROR in Graph<T>::add_isolated_vertex: already a vertex label"));
        return false;
    }
    else
    {
        vertices_.push_back(Vertex<T>(label, content, color));
        return true;
    }
}
template <typename T> bool Graph<T>::add_isolated_vertex(const vertex_label &label, const T &content, bool  label_test)
{
    if (label_test && is_label_in(label))
    {
        std::cout << "ERROR in Graph<T>::add_isolated_vertex: " << label << " is already a vertex label!" << std::endl;
        throw(QString("ERROR in Graph<T>::add_isolated_vertex: already a vertex label"));
        return false;
    }
    else
    {
        vertices_.push_back(Vertex<T>(label, content));
        return true;
    }
}
template <typename T> bool Graph<T>::add_isolated_vertex(const vertex_label &label, const T &content, const integer_coordinates &tiling_coordinates, bool  label_test)
{
    if (label_test && is_label_in(label))
    {
        std::cout << "ERROR in Graph<T>::add_isolated_vertex: " << label << " is already a vertex label!" << std::endl;
        throw(QString("ERROR in Graph<T>::add_isolated_vertex: already a vertex label"));
        return false;
    }
    else
    {
        Vertex<T> V(label, content);
        V.tiling_coordinates_ = tiling_coordinates;
        vertices_.push_back(V);
        return true;
    }
}

template <typename T> bool Graph<T>::add_edge_by_indices(const unsigned int &index_1, const unsigned int &index_2)
{
    if (index_1 > vertices_.size() || index_2 > vertices_.size())
    {
        std::cout << "ERROR in Graph<T>::add_edge_by_indices: bad index" << std::endl;
        throw(QString("ERROR in Graph<T>::add_edge_by_indices: bad index"));
    }
    return vertices_[index_1].add_neighbor_label(vertices_[index_2].label_) && vertices_[index_2].add_neighbor_label(vertices_[index_1].label_);
}

template <typename T> bool Graph<T>::add_edge_by_labels(const vertex_label &label_1, const vertex_label &label_2)
{
    return vertices_[label_to_index(label_1)].add_neighbor_label(label_2) && vertices_[label_to_index(label_2)].add_neighbor_label(label_1);
}

template <typename T> bool Graph<T>::remove_edge_by_indices(const unsigned int &index_1, const unsigned int &index_2)
{
    if (index_1 > vertices_.size() || index_2 > vertices_.size())
    {
        std::cout << "ERROR in Graph<T>::remove_edge_by_indices: bad index" << std::endl;
        throw(QString("ERROR in Graph<T>::remove_edge_by_indices: bad index"));
    }
    return vertices_[index_1].remove_neighbor_label(vertices_[index_2].label_) && vertices_[index_2].remove_neighbor_label(vertices_[index_1].label_);
}


template <typename T> bool Graph<T>::remove_edge_by_labels(const vertex_label &label_1, const vertex_label &label_2)
{
    return vertices_[label_to_index(label_1)].remove_neighbor_label(label_2) && vertices_[label_to_index(label_2)].remove_neighbor_label(label_1);
}

template <typename T> bool Graph<T>::remove_vertex_by_index(const unsigned int &index)
{
    if (index >= vertices_.size())
    {
        std::cout << "ERROR in Graph<T>::get_neighbors_by_index: bad index" << std::endl;
        throw(QString("ERROR in Graph<T>::get_neighbors_by_index: bad index"));
    }

    std::vector<vertex_label> N = vertices_[index].neighbor_labels_;

    bool res = true;
    unsigned int i;
    for (i=0; i<N.size(); i++)
    {
        res = res && remove_edge_by_indices(index, label_to_index(N[i]));
    }

    vertices_.erase(vertices_.begin() + index);
    return res;
}

template <typename T> bool Graph<T>::common_neighbor(vertex_label &output_label,
                                                     const vertex_label &label_1, const vertex_label &label_2) const
{
    std::vector<vertex_label> N=get_neighbors_by_label(label_1);
    unsigned int i;
    for(i=0;i< N.size();i++)
    {
        if (N[i] == label_2)
        {
            continue;
        }
        if (are_neighbor_labels(N[i], label_2))
        {
            output_label = N[i];
            return true;
        }
    }
    return false;
}
template <typename T> bool Graph<T>::remove_vertex_by_label(const vertex_label &label)
{
    unsigned int index = label_to_index(label);
    std::vector<vertex_label> N = vertices_[index].neighbor_labels_;

    bool res = true;
    unsigned int i;
    for (i=0; i<N.size(); i++)
    {
        res = res && remove_edge_by_labels(label, N[i]);
    }

    vertices_.erase(vertices_.begin() + index);
    return res;
}

template <typename T> bool Graph<T>::insert_isolated_vertex(const unsigned int &index, const vertex_label &label, const T &content)
{
    if (index>vertices_.size())
    {
        std::cout << "ERROR in Graph<T>::insert_isolated_vertex: bad index" << std::endl;
        throw(QString("ERROR in Graph<T>::insert_isolated_vertex: bad index"));
        return false;
    }
    else if (is_label_in(label))
    {
        std::cout << "ERROR in Graph<T>::insert_isolated_vertex: label already taken" << std::endl;
        throw(QString("ERROR in Graph<T>::insert_isolated_vertex: label already taken"));
        return false;
    }

    vertices_.insert(vertices_.begin()+ index, Vertex<T>(label, content));
    return true;
}


template <typename T> void Graph<T>::rename_all_labels()
{
    std::vector<int> old_neighbor_labels;
    std::vector<int> new_neighbor_labels;

    unsigned int i, j;
    for (i=0; i<vertices_.size(); i++)
    {
        old_neighbor_labels = vertices_[i].neighbor_labels_;
        new_neighbor_labels.clear();

        for(j=0; j<old_neighbor_labels.size(); j++)
        {
            new_neighbor_labels.push_back(label_to_index(old_neighbor_labels[j]) + 1);
        }

        vertices_[i].neighbor_labels_ = new_neighbor_labels;
    }

    for (i=0; i<vertices_.size(); i++)
        vertices_[i].label_ = i+1;
    return;
}

template <typename T> void Graph<T>::extract_abstract_graph(Graph<Empty> &output_graph)
{
    Vertex<Empty> V_i;
    std::vector< Vertex<Empty> > new_vertices;

    unsigned int i;
    for(i=0; i<vertices_.size(); i++)
    {
        V_i.label_ = vertices_[i].label_;
        V_i.content_ = Empty();
        V_i.neighbor_labels_ = vertices_[i].neighbor_labels_;
        V_i.tiling_coordinates_ = vertices_[i].tiling_coordinates_;
        V_i.color_ = vertices_[i].color_;
        new_vertices.push_back(V_i);
    }

    output_graph.reset_vertices(new_vertices);
    return;
}


template <> complex_number Graph<Point>::get_affix_by_index(const unsigned int &index) const
{
    if (index >= vertices_.size())
    {
        std::cout << "ERROR in Graph<T>::get_affix_by_index: bad index" << std::endl;
        throw(QString("ERROR in Graph<T>::get_affix_by_index: bad index"));
    }

    return vertices_[index].content_.get_affix();
}

template <> complex_number Graph<Point>::get_affix_by_label(const vertex_label &label) const
{
    return vertices_[label_to_index(label)].content_.get_affix();
}

template <> complex_number Graph<Circle>::get_affix_by_index(const unsigned int &index) const
{
    if (index >= vertices_.size())
    {
        std::cout << "ERROR in Graph<T>::get_affix_by_index: bad index " << index
                  << " (graph only has " << nb_vertices() << " vertices" << std::endl;
        throw(QString("ERROR in Graph<T>::get_affix_by_index: bad index"));
    }

    return vertices_[index].content_.get_affix();
}

template <> complex_number Graph<Circle>::get_affix_by_label(const vertex_label &label) const
{
    return vertices_[label_to_index(label)].content_.get_centre().get_affix();
}

template <typename T> const QColor & Graph<T>::get_color_by_index(const unsigned int &index) const
{
    return vertices_[index].color_;
}

template <typename T> void Graph<T>::set_color_by_index(const unsigned int &index, const QColor &color)
{
    if (index >= vertices_.size())
    {
        std::cout << "ERROR in Graph<T>::set_color_by_index: bad index" << std::endl;
        throw(QString("ERROR in Graph<T>::set_color_by_index: bad index"));
    }
    vertices_[index].color_ = color;
}

template <typename T> void Graph<T>::set_content_by_index(const unsigned int &index, const T &content)
{
    if (index >= vertices_.size())
    {
        std::cout << "ERROR in Graph<T>::set_content_by_index: bad index" << std::endl;
        throw(QString("ERROR in Graph<T>::set_content_by_index: bad index"));
    }
    vertices_[index].content_ = content;
}

template <typename T> bool Graph<T>::sort_neighbors_cyclically()
{
    Graph_Cyclic_Sorter<T> GCS(this);
    return GCS.total_cyclic_sort();
}

template <> void Graph<Point>::correct_all_neighbor_orientations()
{
    Graph_Orienter<Point> GO(this);
    GO.correct_all_orientations();
    return;
}

template <> void Graph<Circle>::correct_all_neighbor_orientations()
{
    Graph_Orienter<Circle> GO(this);
    GO.correct_all_orientations();
    return;
}

template <> bool Graph<Point>::index_point_containing(unsigned int &output_index, const Point &point, const double &points_radius)
{
    unsigned int i;
    for (i=0; i<vertices_.size(); i++)
    {
        if (norm(vertices_[i].content_.get_affix() - point.get_affix()) < pow(points_radius,2))
        {
            output_index = i;
            return true;
        }
    }
    return false;
}

template <> bool Graph<Point>::label_point_containing(vertex_label &output_label, const Point &point, const double &points_radius)
{
    unsigned int i;
    for (i=0; i<vertices_.size(); i++)
    {
        if (norm(vertices_[i].content_.get_affix() - point.get_affix()) < pow(points_radius,2))
        {
            output_label = vertices_[i].label_;
            return true;
        }
    }
    return false;
}

template <> bool Graph<Circle>::index_circle_containing(unsigned int &output_index, const Point &point)
{
    bool res = false;
    complex_number affix_centre, affix_point = point.get_affix();
    double radius;

    unsigned int i;
    for (i=0; i<vertices_.size(); i++)
    {
        affix_centre = vertices_[i].content_.get_affix();
        radius = vertices_[i].content_.get_radius();
        if (norm(affix_centre - affix_point) < radius*radius)
        {
            if (radius > 1 && norm(affix_centre) < radius*radius)
            {
                output_index = i;
                res = true;
                continue;
            }
            else
            {
                output_index = i;
                return true;
            }
        }
    }
    return res;
}

template <> bool Graph<Circle>::label_circle_containing(vertex_label &output_label, const Point &point)
{
    unsigned int i;
    for (i=0; i<vertices_.size(); i++)
    {
        if (norm(vertices_[i].content_.get_affix() - point.get_affix()) < pow(vertices_[i].content_.get_radius(),2))
        {
            output_label = vertices_[i].label_;
            return true;
        }
    }
    return false;
}

template <> vertex_label Graph<Point>::nearest_vertex_index(const Point &point) const
{
    complex_number point_affix = point.get_affix();
    double min_norm = norm(vertices_[0].content_.get_affix()-point_affix);
    unsigned int candidate_index = 0;

    unsigned int i;
    for(i=1; i<vertices_.size(); i++)
    {
        if (norm(vertices_[i].content_.get_affix()-point_affix) < min_norm)
        {
            min_norm = norm(vertices_[i].content_.get_affix()-point_affix);
            candidate_index = i;
        }
    }
    return candidate_index;
}

template <> vertex_label Graph<Point>::nearest_vertex_label(const Point &point) const
{
    return vertices_[nearest_vertex_index(point)].label_;
}

template <> vertex_label Graph<Circle>::nearest_vertex_index(const Point &point) const
{
    complex_number point_affix = point.get_affix();
    double min_norm = norm(vertices_[0].content_.get_affix()-point_affix);
    unsigned int candidate_index = 0;

    unsigned int i;
    for(i=1; i<vertices_.size(); i++)
    {
        if (norm(vertices_[i].content_.get_affix()-point_affix) < min_norm)
        {
            min_norm = norm(vertices_[i].content_.get_affix()-point_affix);
            candidate_index = i;
        }
    }
    return candidate_index;
}

template <> void Graph<Circle>::get_min_max_radii(double &output_min_radius, double &output_max_radius) const
{
    output_min_radius = vertices_[0].content_.get_radius();
    output_max_radius = vertices_[0].content_.get_radius();

    double radius;
    unsigned int i;
    for (i=0; i<vertices_.size(); i++)
    {
        radius = vertices_[i].content_.get_radius();
        if (radius < output_min_radius)
        {
            output_min_radius = radius;
        }
        else if (radius > output_max_radius)
        {
            output_max_radius = radius;
        }
    }
    return;
}

template <> vertex_label Graph<Circle>::nearest_vertex_label(const Point &point) const
{
    return vertices_[nearest_vertex_index(point)].label_;
}

template <> Mobius Graph<Circle>::optimal_mobius() const
{
    complex_number z_k;
    double r_k, rho_k, cos_k, sin_k, b_k;
    double A = 0, S = 0, C = 0;

    unsigned int k;
    for (k=0; k<vertices_.size(); k++)
    {
        z_k = vertices_[k].content_.get_affix();
        r_k = vertices_[k].content_.get_radius();
        rho_k = abs(z_k);
        cos_k = real(z_k / rho_k);
        sin_k = imag(z_k / rho_k);
        b_k = rho_k/r_k;
        C += b_k * cos_k;
        S += b_k * sin_k;
        A += (rho_k*rho_k - r_k*r_k + 1.0)/r_k;
    }


    double theta = atan(S/C);
    double cos_theta = cos(theta);
    double sin_theta = sin(theta);

    double u = -(C*cos_theta + S*sin_theta);

    double B = 0.5*A/u;
    double discrim_reduit = B*B - 1;

    if (discrim_reduit < 0)
    {
        //std::cout << "ERROR in Graphe<Circle>::optimal_mobius(): negative discriminant!" << std::endl;
        return Mobius(complex_number(0.0, 0.0));
    }
    else
    {
        double rac = sqrt(discrim_reduit);
        double rho = -B - rac;
        return Mobius(complex_number(rho*cos_theta, rho*sin_theta));
    }
}

template <> void Graph<Circle>::apply_mobius(const Mobius &mobius)
{
    unsigned int i;
    for(i=0; i<vertices_.size(); i++)
    {
        vertices_[i].apply_mobius(mobius);
    }
    return;
}

template <> void Graph<Circle>::color_circle_packing(const coloring_type &coloring)
{
    Graph_Colorer GC(this, coloring);
    GC.color();
    return;
}

template <typename T> const vertex_label & Graph<T>::get_neighbor_label_by_indices(const unsigned int &vertex_index,
                                                                                   const unsigned int &neighbor_index) const
{
    if (vertex_index >= vertices_.size() || neighbor_index >= vertices_[vertex_index].neighbor_labels_.size())
    {
        std::cout << "ERROR in Graph<T>::get_neighbor_label_by_indices: bad index"  << std::endl;
        throw(QString("ERROR in Graph<T>::get_neighbor_label_by_indices: bad index"));
    }
    return vertices_[vertex_index].neighbor_labels_[neighbor_index];
}

template <typename T>
bool Graph<T>::get_neighbor_index_by_label(unsigned int &output_index,
                                           const unsigned int &vertex_index, const vertex_label &label) const
{
    if (vertex_index >= vertices_.size())
    {
        std::cout << "ERROR in Graph<T>::get_neighbor_label_by_indices: bad index"  << std::endl;
        throw(QString("ERROR in Graph<T>::get_neighbor_label_by_indices: bad index"));
    }
    std::vector<vertex_label> N = vertices_[vertex_index].neighbor_labels_;
    unsigned int j;
    for (j=0; j<N.size(); j++)
    {
        if (N[j]==label)
        {
            output_index = j;
            return true;
        }
    }
    return false;
}

template <typename T> const vertex_label & Graph<T>::get_last_neighbor(const unsigned int &vertex_index) const
{
    return vertices_[vertex_index].neighbor_labels_.back();
}

template <typename T> void Graph<T>::set_neighbor_label_by_indices(const unsigned int &vertex_index,
                                                                   const unsigned int &neighbor_index, const vertex_label &label)
{
    if (vertex_index >= vertices_.size() || neighbor_index >= vertices_[vertex_index].neighbor_labels_.size())
    {
        std::cout << "ERROR in Graph<T>::set_neighbor_label_by_indices: bad index" << std::endl;
        throw(QString("ERROR in Graph<T>::set_neighbor_label_by_indices: bad index"));
    }
    vertices_[vertex_index].neighbor_labels_[neighbor_index] = label;
    return;
}

template <typename T> void Graph<T>::reverse_neighbors_by_index(const unsigned int &index)
{
    if (index >= vertices_.size())
    {
        std::cout << "ERROR in Graph<T>::reverse_neighbors_by_index: bad index" << std::endl;
        throw(QString("ERROR in Graph<T>::reverse_neighbors_by_index: bad index"));
    }
    vertices_[index].reverse_neighbor_labels();
}

template <typename T> void Graph<T>::rotate_neighbors_by_index(const unsigned int &index, const unsigned int &shift)
{
    vertices_[index].rotate_neighbor_labels(shift);
    return;
}

template <typename T> void Graph<T>::reset_vertices(const std::vector< Vertex<T> > &new_vertices)
{
    vertices_ = new_vertices;
    return;
}

template <> Graph<Circle> Graph<Empty>::concretize_graph(const std::vector<Circle> &contents)
{
    if (vertices_.size() != contents.size())
    {
        std::cout << "ERROR in Graph<Empty>::concretize_graph: incorrect size" << std::endl;
        throw(QString("ERROR in Graph<Empty>::concretize_graph: incorrect size"));
    }

    Graph<Circle> G;

    Vertex<Circle> V;
    std::vector< Vertex<Circle> > new_vertices;
    unsigned int i;
    for (i=0; i<vertices_.size(); i++)
    {
        V.label_ = vertices_[i].label_;
        V.content_ = contents[i];
        V.neighbor_labels_ = vertices_[i].neighbor_labels_;
        V.tiling_coordinates_ = vertices_[i].tiling_coordinates_;
        V.color_ = vertices_[i].color_;

        new_vertices.push_back(V);
    }

    G.reset_vertices(new_vertices);
    return G;
}

template <> Point Graph<Point>::center_of_mass() const
{
    complex_number z_res(0.0, 0.0);

    unsigned int i;
    for (i=0; i<vertices_.size(); i++)
    {
        z_res += get_affix_by_index(i);
    }

    return Point((1.0/vertices_.size())*z_res);
}

template <> Point Graph<Circle>::center_of_mass() const
{
    complex_number z_res(0.0, 0.0);

    unsigned int i;
    for (i=0; i<vertices_.size(); i++)
    {
        z_res += get_affix_by_index(i);
    }

    return Point((1.0/vertices_.size())*z_res);
}

template <> complex_number Graph<Point>::direction_from_center_of_mass(const unsigned int &index) const
{
    if (index > vertices_.size())
    {
        std::cout << "ERROR in Graph<Point>::direction_from_center_of_mass: bad index" << std::endl;
        throw(QString("ERROR in Graph<Point>::direction_from_center_of_mass: bad index"));
    }

    complex_number u = get_affix_by_index(index) - center_of_mass().get_affix();
    return u/abs(u);
}

template <> complex_number Graph<Circle>::direction_from_center_of_mass(const unsigned int &index) const
{
    if (index > vertices_.size())
    {
        std::cout << "ERROR in Graph<Point>::direction_from_center_of_mass: bad index" << std::endl;
        throw(QString("ERROR in Graph<Point>::direction_from_center_of_mass: bad index"));
    }

    complex_number u = get_affix_by_index(index) - center_of_mass().get_affix();
    return u/abs(u);
}

template <typename T> void Graph<T>::reset_neighbors(const unsigned int &index, const std::vector<vertex_label> &new_neighbors)
{
    vertices_[index].neighbor_labels_ = new_neighbors;
    return;
}



template class Graph<Empty>;
template class Graph<Point>;
template class Graph<Circle>;
