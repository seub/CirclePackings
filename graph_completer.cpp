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

#include "graph_completer.hpp"
#include "graph_cyclic_sorter.hpp"
#include "graph_orienter.hpp"


Graph_Completer::Graph_Completer()
{

}

void Graph_Completer::set_graph(Graph<Point> *G)
{
    G_ = G;
    graph_save_ = *G;
}

void Graph_Completer::set_stopped()
{
    stopped_ = true;
}

bool Graph_Completer::get_stopped() const
{
    return stopped_;
}

Graph<Point> Graph_Completer::get_graph_save()
{
    return graph_save_;
}

void Graph_Completer::set_output(Graph<Empty> *output_abstract_graph, complex_number *output_angle)
{
    output_abstract_graph_ = output_abstract_graph;
    output_angle_ = output_angle;
    return;
}

void Graph_Completer::set_zero_neighbors(std::vector<unsigned int> zero_neighbors)
{
    zero_neighbors_ = zero_neighbors;
    return;
}

void Graph_Completer::run()
{
    stopped_ = false;

    //clock_t t0 = clock();
    compute_convex_hull();
    /*clock_t t1 = clock();
    std::cout << "Convex hull points: (computed in " << (t1 - t0)*1.0/CLOCKS_PER_SEC << "s)" << std::endl;
    unsigned int i;
    for (i=0; i<convex_hull_.size(); i++)
    {
        std::cout << convex_hull_[i] + 1 << " ";
    }
    std::cout << std::endl;*/

    //clock_t t2 = clock();
    complete_triangulation();
    /*clock_t t3 = clock();
    std::cout << "Triangulated in " << (t3 - t2)*1.0/CLOCKS_PER_SEC << "s, ";*/

    if (!stopped_)
    {
        add_zero();
        sort_everything();
    }

    /*clock_t t4 = clock();
    std::cout << "added zero in " << (t4 - t3)*1.0/CLOCKS_PER_SEC << "s" << std::endl;*/

    return;
}

void Graph_Completer::complete_triangulation()
{
    create_lengthed_indices_pairs();
    sort_lengths();
    add_edges();

    return;
}

bool Graph_Completer::crosses_an_edge(const segment &S)
{
    bool crossing = false;
    segment segment_1 = S, segment_2;

    vertex_label label;
    Point content;
    std::vector<vertex_label> N;

    unsigned int j, k;
    for(j=0; j<G_->nb_vertices(); j++)
    {
        label = G_->get_label_by_index(j);
        content = G_->get_content_by_index(j);
        N = G_->get_neighbors_by_index(j);

        for(k=0; k<N.size(); k++)
        {
            if(N[k] < label)
            {
                continue;
            }
            else
            {
                segment_2 = segment(content, G_->get_content_by_label(N[k]));
                if (segment_1.first.get_affix() == segment_2.first.get_affix() &&
                        segment_1.second.get_affix() == segment_2.second.get_affix())
                {
                    crossing = true;
                    break;
                }
                if(segment_1.first.get_affix() == segment_2.first.get_affix() ||
                        segment_1.first.get_affix() == segment_2.second.get_affix() ||
                        segment_1.second.get_affix() == segment_2.first.get_affix() ||
                        segment_1.second.get_affix() == segment_2.second.get_affix())
                {
                    continue;
                }
                if(are_intersecting(segment_1,segment_2))
                {
                    crossing = true;
                    break;
                }
            }
        }

        if(crossing)
        {
            break;
        }
    }

    return crossing;
}

void Graph_Completer::create_lengthed_indices_pairs()
{
    lengthed_indices_pairs_.clear();
    unsigned int N=G_->nb_vertices();
    unsigned int i,j;
    for(i=0; i<N; i++)
    {
        for(j=i+1; j<N; j++)
        {
            lengthed_indices_pairs_.push_back(
                        lengthed_indices_pair(indices_pair(i,j),
                                              norm(G_->get_affix_by_index(j) - G_->get_affix_by_index(i))));
        }
    }

    return;
}

void Graph_Completer::sort_lengths()
{
    sort(lengthed_indices_pairs_.begin(), lengthed_indices_pairs_.end(), compare_lengths);
    return;
}

void Graph_Completer::add_edges()
{
    segment segment_1;
    unsigned int nb_edges = G_->nb_edges();
    unsigned int nb_edges_total = 3*(G_->nb_vertices()-1) - zero_neighbors_.size();

    unsigned int j, index_1, index_2;
    for (j=0; j+1<zero_neighbors_.size(); j++)
    {
        index_1 = zero_neighbors_[j];
        index_2 = zero_neighbors_[j+1];
        if (!G_->are_neighbor_indices(index_1, index_2))
        {
            G_->add_edge_by_indices(index_1, index_2);
            nb_edges++;
        }
    }
    if (!G_->are_neighbor_indices(zero_neighbors_.back(), zero_neighbors_.front()))
    {
        G_->add_edge_by_indices(zero_neighbors_.back(), zero_neighbors_.front());
        nb_edges++;
    }

    unsigned int i=0;
    while(i<lengthed_indices_pairs_.size() && nb_edges<nb_edges_total && !stopped_)
    {
        index_1 = lengthed_indices_pairs_[i].first.first;
        index_2 = lengthed_indices_pairs_[i].first.second;
        segment_1 = segment(G_->get_content_by_index(index_1),
                            G_->get_content_by_index(index_2));
        if(!crosses_an_edge(segment_1))
        {
            G_->add_edge_by_indices(index_1, index_2);
            nb_edges++;
        }
        i++;
    }

    /*std::cout << "Stopped adding edges at i = " << i << " out of " << lengthed_indices_pairs_.size() << ". ";*/
    if (i == lengthed_indices_pairs_.size())
    {
        std::cout << "I had to run through all the potential edges. Number of edges predicted was "
                  << nb_edges_total << ", number of actual edges is " << nb_edges;
    }

    complex_number z_1 = G_->get_affix_by_index(index_1);
    complex_number z_2 = G_->get_affix_by_index(index_2);
    z_1 = complex_number(int(100*real(z_1))/100.0, int(100*imag(z_1))/100.0);
    z_2 = complex_number(int(100*real(z_2))/100.0, int(100*imag(z_2))/100.0);
    //std::cout << "Last edge added: " << index_1+1 << " <-> " << index_2+1 << " i.e. "
    //          << z_1 << " <-> " << z_2 << std::endl;

    return;
}

void Graph_Completer::compute_convex_hull()
{
    unsigned int n = G_->nb_vertices();

    unsigned int lowest = 0;
    complex_number z, z_min = G_->get_affix_by_index(0);
    unsigned int i;
    for (i=1; i<n; i++)
    {
        z = G_->get_affix_by_index(i);
        if (imag(z) < imag(z_min) || (imag(z) == imag(z_min) && real(z) < real(z_min)))
        {
            z_min = z;
            lowest = i;
        }
    }

    unsigned int pivot = lowest, first_pivot = lowest;
    complex_number z_pivot = z_min;
    int sign = 1, sign2 = 1;

    zero_neighbors_.clear();
    zero_neighbors_.push_back(first_pivot);

    unsigned int counter = 0;
    while ((pivot != first_pivot || counter==0) && counter<=n)
    {
        counter++;
        lowest = (pivot == 0) ? 1 : 0;
        z_min = (pivot == 0) ? G_->get_affix_by_index(1) - z_pivot : G_->get_affix_by_index(0) - z_pivot;
        for (i=0; i<n; i++)
        {
            if (i == pivot)
            {
                continue;
            }
            z = G_->get_affix_by_index(i) - z_pivot;
            if (norm(z_min)==0)
            {
                std::cout << "ERROR in Graph_Completer::compute_convex_hull : not supposed to test pivot" << std::endl;
                throw(QString("ERROR in Graph_Completer::compute_convex_hull : not supposed to test pivot"));
            }
            else if (imag(conj(z)*z_min) > 0)
            {
                z_min = z;
                lowest = i;
            }
            else if (imag(conj(z)*z_min) == 0)
            {
                if (real(z)==0 && real(z_min)==0)
                {
                    if (imag(z)*imag(z_min)>0)
                    {
                        if (norm(z)<norm(z_min))
                        {
                            z_min = z;
                            lowest = i;
                        }
                    }
                    else
                    {
                        if (imag(z)*sign2>0)
                        {
                            z_min = z;
                            lowest = i;
                        }
                    }
                }
                else if (real(z)*real(z_min)>0)
                {
                    if (norm(z)<norm(z_min))
                    {
                        z_min = z;
                        lowest = i;
                    }
                }
                else if (real(z)*sign>0 && real(z_min)*sign<0)
                {
                    z_min = z;
                    lowest = i;
                }
            }
        }
        sign = real(z_min)>0 ? 1 : -1;
        if (real(z_min) == 0)
        {
            sign = 0;
            sign2 = imag(z_min)>0 ? 1 : -1;
        }
        if (lowest == pivot)
        {
            std::cout << "ERROR in Graph_Completer::compute_convex_hull: point " << lowest << " added twice" << std::endl;
            throw(QString("ERROR in Graph_Completer::compute_convex_hull: point added twice"));
        }
        pivot = lowest;
        z_pivot = z_min + z_pivot;
        zero_neighbors_.push_back(pivot);
    }

    reverse(zero_neighbors_.begin(), zero_neighbors_.end());
    zero_neighbors_.pop_back();

    return;
}

void Graph_Completer::add_zero()
{
    G_->insert_isolated_vertex(0, 0, Point());
    unsigned int i;
    for (i=0; i<zero_neighbors_.size(); i++)
    {
        G_->add_edge_by_indices(0, zero_neighbors_[i]+1);
    }

    return;
}

bool Graph_Completer::sort_everything()
{
    Graph_Cyclic_Sorter<Point> GCS(G_);
    unsigned int i=1, N = G_->nb_vertices();
    while (i<N && GCS.neighbors_cyclic_sort(i))
    {
        i++;
    }

    G_->extract_abstract_graph(*output_abstract_graph_);
    G_->remove_vertex_by_index(0);

    Graph_Orienter<Point> GO(G_);
    for (i=0; i<G_->nb_vertices(); i++)
    {
        if (!GO.are_neighbors_correctly_oriented(i))
        {
            G_->reverse_neighbors_by_index(i);
            output_abstract_graph_->reverse_neighbors_by_index(i+1);
        }
    }

    *output_angle_ = G_->direction_from_center_of_mass(0);

    return (i==N);
}

void Graph_Completer::complete_maximal_graph()
{
    add_zero();
    sort_everything();
    return;
}


bool compare_lengths(const lengthed_indices_pair &E_1, const lengthed_indices_pair &E_2)
{
    return E_1.second < E_2.second;
}
