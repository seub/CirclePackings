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

#include "graph_tiler.hpp"
#include "graph_cyclic_sorter.hpp"

void Graph_Tiler::reset_for_tiling(Graph<Circle>* tiling, double tiling_step)
{
    tiling_ = tiling;
    tiling_step_ = tiling_step;
    return;
}

void Graph_Tiler::reset_for_circle_packing(Graph<Circle> *tiling, Graph<Circle> *circle_packing,
                                           Graph<Empty> *abstract_graph, complex_number *angle, Curve *curve)
{
    tiling_ = tiling;
    circle_packing_ = circle_packing;
    abstract_graph_ = abstract_graph;
    angle_ = angle;
    curve_ = curve;
    return;
}

void Graph_Tiler::create_tiling()
{
    tiling_->reset_graph();
    double origin = 1.5;
    double horizontal_step = tiling_step_;
    double radius = tiling_step_/2.0;
    double vertical_step = tiling_step_*sqrt(3.0)/2.0; //int(tiling_size_in_pixels_*sqrt(3)/2 + 0.5);
    unsigned int nb_tiling_points_vertical = int_round(3.0/vertical_step) + 1;
    unsigned int nb_tiling_points_horizontal = int_round(3.0/horizontal_step) + 1;

    vertex_label label=1;
    Circle content;
    unsigned int j,i;
    for(j=1; j<nb_tiling_points_vertical; j++)
    {
        if (j%2==1)
        {
            for(i=1; i<nb_tiling_points_horizontal; i++)
            {
                content = Circle(Point(complex_number(i*horizontal_step - origin, j*vertical_step - origin)), radius);
                integer_coordinates tiling_coordinates(i,j);
                tiling_->add_isolated_vertex(label, content, tiling_coordinates,false);

                if (j!=1)
                {
                    tiling_->add_edge_by_indices(label-1, label-nb_tiling_points_horizontal);
                    tiling_->add_edge_by_indices(label-1, label-nb_tiling_points_horizontal-1);
                }
                if (i!=1)
                {
                    tiling_->add_edge_by_indices(label-1, label-2);
                }
                label++;
            }
        }
        else
        {
            for(i=0; i<nb_tiling_points_horizontal; i++)
            {
                content = Circle(Point(complex_number((i+0.5)*horizontal_step- origin, j*vertical_step - origin)), radius);
                integer_coordinates tiling_coordinates(i,j);
                tiling_->add_isolated_vertex(label, content, tiling_coordinates,false);

                if (i!=0)
                {
                    tiling_->add_edge_by_indices(label-1, label-nb_tiling_points_horizontal-1);
                    tiling_->add_edge_by_indices(label-1, label-2);
                }
                if (i!=nb_tiling_points_horizontal-1)
                {
                    tiling_->add_edge_by_indices(label-1, label-nb_tiling_points_horizontal);
                }
                label++;
            }
        }
    }
    return;
}

bool Graph_Tiler::create_circle_packing()
{
    if (curve_->nb_points() < 3)
    {
        std::cout << "ERROR in Graph_Tiler::create_circle_packing: not supposed to be called "
                  << "(curve has less than 3 points)" << std::endl;
        return false;
    }
    circle_packing_->reset_graph();
    abstract_graph_->reset_graph();

    unsigned int nb_vertices;
    create_to_be_added(nb_vertices, *tiling_, *curve_);

    if (nb_vertices == 0)
    {
        return false;
    }

    create_graphs(nb_vertices, *abstract_graph_, *tiling_);

    if (circle_packing_->nb_vertices()!=nb_vertices || abstract_graph_->nb_vertices()!=nb_vertices+1)
    {
        std::cout << "ERROR in Graph_Tiler::create_circle_packing: wrong number of vertices" << std::endl;
        throw(QString("ERROR in Graph_Tiler::create_circle_packing: wrong number of vertices"));
    }

    bool res = zero_neighbors_sort(*abstract_graph_);

    *angle_ = circle_packing_->direction_from_center_of_mass(0);

    return res;
}

bool Graph_Tiler::zero_neighbors_sort(Graph<Empty> &G)
{
    unsigned int one = 1;
    /*integer_coordinates xy, xy_min = G.get_tiling_coordinates_by_index(1);
    for (i=2; i<G_->nb_vertices(); i++)
    {
        xy = G.get_tiling_coordinates_by_index(i);
        if (xy.second < xy_min.second || (xy.second==xy_min.second && xy.first < xy_min.first))
        {
            xy_min = xy;
            one = i;
        }
    }*/

    unsigned int current_vertex = one;
    unsigned int zero_neighbors_size = G.get_neighbors_by_index(0).size();
    std::vector<vertex_label> new_zero_neighbors;
    std::vector<vertex_label> current_vertex_neighbors;
    new_zero_neighbors.push_back(one);

    unsigned int j;
    while (true)//(current_vertex!=one || new_zero_neighbors.size()==1))
    {
        if(new_zero_neighbors.size() > zero_neighbors_size)
        {
            G.reset_graph();
            return false;
        }
        current_vertex_neighbors = G.get_neighbors_by_index(current_vertex);
        if (current_vertex_neighbors[0] == 0)
        {
            current_vertex = current_vertex_neighbors.back();
            if(current_vertex==one)
            {
                break;
            }
            new_zero_neighbors.push_back(current_vertex);
        }
        else
        {
            j=0;
            while (current_vertex_neighbors[j+1]!=0 && j+1<current_vertex_neighbors.size())
            {
                j++;
            }
            if (j>= current_vertex_neighbors.size()-1)
            {
                std::cout << "Problem in Graph_Tiler::zero_neighbors_sort..." << std::endl;
                throw(QString("Problem in Graph_Tiler::zero_neighbors_sort..."));
            }
            else
            {
                current_vertex = current_vertex_neighbors[j];
                if(current_vertex==one)
                {
                    break;
                }
                new_zero_neighbors.push_back(current_vertex);
            }
        }
    }

    if (new_zero_neighbors.size()==zero_neighbors_size)
    {
        G.reset_neighbors(0, new_zero_neighbors);
        return true;
    }
    else
    {
        G.reset_graph();
        return false;
    }
}

void Graph_Tiler::create_to_be_added(unsigned int &output_nb_vertices, const Graph<Circle> &tiling_graph, const Curve &curve)
{
    std::vector<vertex_label> N;

    std::vector<bool> are_inside;
    are_inside.reserve(tiling_graph.nb_vertices());

    to_be_added_.clear();
    to_be_added_.reserve(tiling_graph.nb_vertices());

    unsigned int nb_neighbor_labels_in;

    unsigned int tiling_nb_vertices = tiling_graph.nb_vertices();


    unsigned int i,j;
    for (i=0; i<tiling_nb_vertices; i++)
    {
        are_inside.push_back(curve.is_inside(tiling_graph.get_content_by_index(i).get_centre()));
    }

    for(i=0; i<tiling_nb_vertices; i++)
    {
        if(are_inside[i])
        {
            N = tiling_graph.get_neighbors_by_index(i);
            nb_neighbor_labels_in = 0;
            for (j=0; j<N.size(); j++)
            {
                if (are_inside[N[j]-1])
                {
                    nb_neighbor_labels_in++;
                }
            }
            if (nb_neighbor_labels_in > 1)
            {
                to_be_added_.push_back(true);
            }
            else
            {
                to_be_added_.push_back(false);
            }
        }
        else
        {
            to_be_added_.push_back(false);
        }
    }

    i=0;

    output_nb_vertices = 0;
    while (i < tiling_nb_vertices)
    {
        while(i < tiling_nb_vertices)
        {
            if (to_be_added_[i])
            {
                break;
            }
            i++;
        }
        if (i==tiling_nb_vertices)
        {
            break;
        }
        nb_neighbor_labels_in = 0;
        N = tiling_graph.get_neighbors_by_index(i);
        for (j=0; j<N.size(); j++)
        {
            if (to_be_added_[N[j]-1])
            {
                nb_neighbor_labels_in++;
            }
        }
        if (nb_neighbor_labels_in < 2)
        {
            to_be_added_[i] = false;
            i = 0;
            output_nb_vertices = 0;
        }
        else
        {
            output_nb_vertices++;
            i++;
        }
    }
    return;
}

void Graph_Tiler::create_graphs(const unsigned int &nb_vertices, Graph<Empty> &output_abstract_graph,
                                const Graph<Circle> &tiling_graph)
{
    integer_coordinates xy_i, xy_j;
    std::vector< Vertex<Empty> > vertices_e;
    vertices_e.reserve(nb_vertices + 1);
    std::vector< Vertex<Circle> > vertices_c;
    vertices_c.reserve(nb_vertices);
    Vertex<Empty> V_0;
    V_0.label_ = 0;
    V_0.neighbor_labels_.clear();
    vertices_e.push_back(V_0);
    Vertex<Empty> V_e;
    Vertex<Circle> V_c;
    std::vector<vertex_label> labels;
    unsigned int tiling_nb_vertices = tiling_graph.nb_vertices();
    labels.resize(tiling_nb_vertices+1);
    vertex_label label = 0;

    std::vector<vertex_label> N;
    bool zero_added, missing_neighbor;
    unsigned int i, j;
    for (i=0; i<tiling_nb_vertices; i++)
    {
        if (to_be_added_[i])
        {
            label++;
            labels[i+1] = label;
            V_e.label_ = label;
            V_c.label_ = label;
            V_e.content_ = Empty();
            V_c.content_ = tiling_graph.get_content_by_index(i);
            xy_i = tiling_graph.get_tiling_coordinates_by_index(i);
            V_e.tiling_coordinates_ = xy_i;
            V_c.tiling_coordinates_ = xy_i;

            V_e.neighbor_labels_.clear();
            V_c.neighbor_labels_.clear();
            zero_added = false;

            N = tiling_graph.get_neighbors_by_index(i);
            missing_neighbor = true;
            for (j=0; j<N.size(); j++)
            {
                xy_j = tiling_graph.get_tiling_coordinates_by_index(N[j]-1);
                if (to_be_added_[N[j]-1] && xy_j.first==xy_i.first+1 && xy_j.second==xy_i.second)
                {
                    V_e.neighbor_labels_.push_back(N[j]);
                    V_c.neighbor_labels_.push_back(N[j]);
                    missing_neighbor = false;
                }
            }
            if (missing_neighbor && (!zero_added))
            {
                V_e.neighbor_labels_.push_back(0);
                zero_added = true;
            }

            missing_neighbor = true;
            for (j=0; j<N.size(); j++)
            {
                xy_j = tiling_graph.get_tiling_coordinates_by_index(N[j]-1);
                if (to_be_added_[N[j]-1] && xy_j.first==xy_i.first+(xy_i.second%2==0) && xy_j.second==xy_i.second+1)
                {
                    V_e.neighbor_labels_.push_back(N[j]);
                    V_c.neighbor_labels_.push_back(N[j]);
                    missing_neighbor = false;
                }
            }
            if (missing_neighbor && (!zero_added))
            {
                V_e.neighbor_labels_.push_back(0);
                zero_added = true;
            }

            missing_neighbor = true;
            for (j=0; j<N.size(); j++)
            {
                xy_j = tiling_graph.get_tiling_coordinates_by_index(N[j]-1);
                if (to_be_added_[N[j]-1] && xy_j.first==xy_i.first+(xy_i.second%2==0)-1 && xy_j.second==xy_i.second+1)
                {
                    V_e.neighbor_labels_.push_back(N[j]);
                    V_c.neighbor_labels_.push_back(N[j]);
                    missing_neighbor = false;
                }
            }
            if (missing_neighbor && (!zero_added))
            {
                V_e.neighbor_labels_.push_back(0);
                zero_added = true;
            }

            missing_neighbor = true;
            for (j=0; j<N.size(); j++)
            {
                xy_j = tiling_graph.get_tiling_coordinates_by_index(N[j]-1);
                if (to_be_added_[N[j]-1] && xy_j.first==xy_i.first-1 && xy_j.second==xy_i.second)
                {
                    V_e.neighbor_labels_.push_back(N[j]);
                    V_c.neighbor_labels_.push_back(N[j]);
                    missing_neighbor = false;
                }
            }
            if (missing_neighbor && (!zero_added))
            {
                V_e.neighbor_labels_.push_back(0);
                zero_added = true;
            }

            missing_neighbor = true;
            for (j=0; j<N.size(); j++)
            {
                xy_j = tiling_graph.get_tiling_coordinates_by_index(N[j]-1);
                if (to_be_added_[N[j]-1] && xy_j.first==xy_i.first-(xy_i.second%2==1) && xy_j.second==xy_i.second-1)
                {
                    V_e.neighbor_labels_.push_back(N[j]);
                    V_c.neighbor_labels_.push_back(N[j]);
                    missing_neighbor = false;
                }
            }
            if (missing_neighbor && (!zero_added))
            {
                V_e.neighbor_labels_.push_back(0);
                zero_added = true;
            }

            missing_neighbor = true;
            for (j=0; j<N.size(); j++)
            {
                xy_j = tiling_graph.get_tiling_coordinates_by_index(N[j]-1);
                if (to_be_added_[N[j]-1] && xy_j.first==xy_i.first-(xy_i.second%2==1)+1 && xy_j.second==xy_i.second-1)
                {
                    V_e.neighbor_labels_.push_back(N[j]);
                    V_c.neighbor_labels_.push_back(N[j]);
                    missing_neighbor = false;
                }
            }
            if (missing_neighbor && (!zero_added))
            {
                V_e.neighbor_labels_.push_back(0);
                zero_added = true;
            }

            if (zero_added)
            {
                vertices_e[0].neighbor_labels_.push_back(i+1);
            }

            vertices_e.push_back(V_e);
            vertices_c.push_back(V_c);
        }
    }


    for (i=0; i<vertices_e.size(); i++)
    {
        for (j=0; j<vertices_e[i].neighbor_labels_.size(); j++)
        {
            vertices_e[i].neighbor_labels_[j] = labels[vertices_e[i].neighbor_labels_[j]];
        }
    }

    for (i=0; i<vertices_c.size(); i++)
    {
        for (j=0; j<vertices_c[i].neighbor_labels_.size(); j++)
        {
            vertices_c[i].neighbor_labels_[j] = labels[vertices_c[i].neighbor_labels_[j]];
        }
    }

    circle_packing_->reset_vertices(vertices_c);
    output_abstract_graph.reset_vertices(vertices_e);

    return;
}
