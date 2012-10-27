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

#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <QColor>

#include "basic_types.hpp"
#include "point.hpp"
#include "vertex.hpp"
#include "mobius.hpp"
#include "curve.hpp"



template <typename T>
class Graph
{
    template <typename T2> friend std::ostream& operator<<(std::ostream& out, const Graph<T2> &G);

public:
    Graph();

    void reset_graph();

    unsigned int label_to_index(const vertex_label &label) const;
    const vertex_label & get_label_by_index(const unsigned int &index) const;
    const T & get_content_by_index(const unsigned int &index) const;
    const T & get_content_by_label(const vertex_label &label) const;
    const integer_coordinates & get_tiling_coordinates_by_index(const unsigned int &index) const;

    const std::vector<vertex_label> & get_neighbors_by_index(const unsigned int &index) const;
    const std::vector<vertex_label> & get_neighbors_by_label(const vertex_label &label) const;

    bool is_label_in(const vertex_label &label) const;
    unsigned int nb_vertices() const;
    unsigned int nb_edges() const;
    bool are_neighbor_indices(const unsigned int &index_1, const unsigned int &index_2) const;
    bool are_neighbor_labels(const vertex_label &label_1, const vertex_label &label_2) const;
    bool common_neighbor(vertex_label &output_label, const vertex_label &label_1, const vertex_label &label_2) const;
    bool add_isolated_vertex(const vertex_label &label, const T &content,  bool  label_test);
    bool add_isolated_vertex(const vertex_label &label, const T &content, const QColor &color = QColor("black"), bool  label_test = true);
    bool add_isolated_vertex(const vertex_label &label, const T &content, const integer_coordinates &tiling_coordinates, bool  label_test = true);
    bool add_edge_by_indices(const unsigned int &index_1, const unsigned int &index_2);
    bool add_edge_by_labels(const vertex_label &label_1, const vertex_label &label_2);
    bool remove_edge_by_indices(const unsigned int &index_1, const unsigned int &index_2);
    bool remove_edge_by_labels(const vertex_label &label_1, const vertex_label &label_2);
    bool remove_vertex_by_index(const unsigned int &index);
    bool remove_vertex_by_label(const vertex_label &label);
    bool insert_isolated_vertex(const unsigned int &index, const vertex_label &label, const T &content);

    void set_content_by_index(const unsigned int &index, const T &content);
    const QColor & get_color_by_index(const unsigned int &index) const;
    void set_color_by_index(const unsigned int &index, const QColor &color);
    bool get_draw_borders() const;
    void set_draw_borders(bool b);

    void rename_all_labels();
    void extract_abstract_graph(Graph<Empty> &output_graph);
    bool sort_neighbors_cyclically(); // !Graph must be planarizable *maximal* (completely triangulated)!

    // *** Specialization to Graph<Point> or Graph<Circle> ***
    complex_number get_affix_by_index(const unsigned int &index) const;
    complex_number get_affix_by_label(const vertex_label &label) const;
    void correct_all_neighbor_orientations(); // !Graph must have neighbors cyclically ordered!
    vertex_label nearest_vertex_index(const Point &point) const;
    vertex_label nearest_vertex_label(const Point &point) const;
    Point center_of_mass() const;
    complex_number direction_from_center_of_mass(const unsigned int& index) const;

    // *** Specialization to Graph<Point> ***
    bool index_point_containing(unsigned int &output_index, const Point &point, const double &points_radius);
    bool label_point_containing(vertex_label &output_label, const Point &point, const double &points_radius);

    // *** Specialization to Graph<Circle> ***
    bool index_circle_containing(unsigned int &output_index, const Point &point);
    bool label_circle_containing(vertex_label &output_label, const Point &point);
    void apply_mobius(const Mobius &mobius);
    Mobius optimal_mobius() const; // Minimizes the L1 norm of the inverses of the radii
    void color_circle_packing(const coloring_type &coloring);
    void get_min_max_radii(double &output_min_radius, double &output_max_radius) const;

    // Pseudo private (Graph_Opser or Algo only)
    const vertex_label & get_neighbor_label_by_indices(const unsigned int &vertex_index, const unsigned int &neighbor_index) const;
    bool get_neighbor_index_by_label(unsigned int &output_index, const unsigned int &vertex_index, const vertex_label &label) const;
    const vertex_label & get_last_neighbor(const unsigned int &vertex_index) const;
    void set_neighbor_label_by_indices(const unsigned int &vertex_index, const unsigned int &neighbor_index, const vertex_label &label);
    void reverse_neighbors_by_index(const unsigned int &index);
    void rotate_neighbors_by_index(const unsigned int &index, const unsigned int &shift);
    Graph<Circle> concretize_graph(const std::vector<Circle> &contents);
    void reset_vertices(const std::vector< Vertex<T> > &new_vertices);
    void reset_neighbors(const unsigned int &index, const std::vector<vertex_label> &new_neighbors);

private:
    std::vector< Vertex<T> > vertices_;
};


template <typename T> std::ostream& operator<<(std::ostream& out, const Graph<T>& G)
{
    if(G.vertices_.size() > 0)
    {
        out << "The graph has " << G.vertices_.size() << " vertices:";
        unsigned int i;
        for(i=0; i<G.vertices_.size(); i++)
        {
            out << std::endl << "   " << G.vertices_[i];
        }
    }
    else
    {
        out << "The graph is empty.";
    }
    return out;
}




#endif // GRAPH_HPP
