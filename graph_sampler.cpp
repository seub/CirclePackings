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

#include "graph_sampler.hpp"
#include "graph_completer.hpp"

void Graph_Sampler::reset(Graph<Point> *graph, Graph<Empty> * output_graph, complex_number * angle)
{
    graph_ = graph;
    output_graph_ = output_graph;
    angle_ = angle;
    return;
}

void Graph_Sampler::create_sample_graph(graph_choice_type graph_choice, unsigned int parameter_1, unsigned int parameter_2)
{
    switch(graph_choice)
    {
    case CUSTOM_GRAPH:
        std::cout << "ERROR in Graph_Sampler::create_sample_graph: not supposed to be called" << std::endl;
        throw(QString("ERROR in Graph_Sampler::create_sample_graph: not supposed to be called"));

    case GRID:
        create_grid(parameter_1);
        break;

    case WEB:
        create_web(parameter_1, parameter_2);
        break;

    default:
        std::cout << "ERROR in Graph_Sampler::create_sample_graph: flag problem" << std::endl;
        throw(QString("ERROR in Graph_Sampler::create_sample_graph: flag problem"));
    }

    Graph_Completer GP;
    GP.set_graph(graph_);
    GP.set_output(output_graph_, angle_);
    GP.set_zero_neighbors(zero_neighbors_indices_);
    GP.complete_maximal_graph();

    return;
}

void Graph_Sampler::create_grid(unsigned int size)
{
    graph_->reset_graph();
    zero_neighbors_indices_.clear();
    zero_neighbors_indices_.reserve(4*(size-1));
    double a = -1.5, b = 1.5;
    complex_number z;
    unsigned int i, j, n = 1;
    for (i=1; i<=size; i++)
    {
        for (j=1; j<=size; j++)
        {
            z = complex_number(a + i*(b-a)/(size+1), a + j*(b-a)/(size+1));
            graph_->add_isolated_vertex(n, Point(z), QColor(rand()%256,rand()%256,rand()%256),false);
            n++;
        }
    }

    for(j = 1;j<size;j++)
    {
        n = j -1;
        zero_neighbors_indices_.push_back(n);
    }

    for(i = 1;i<size;i++)
    {
        n = i*size -1;
        zero_neighbors_indices_.push_back(n);
    }

    for(j = size;j>1;j--)
    {
        n =(size-1)*size + j -1;
        zero_neighbors_indices_.push_back(n);
    }

    for(i = size ;i>1;i--)
    {
        n = (i-1)*size;
        zero_neighbors_indices_.push_back(n);
    }


    for (i=1; i<size; i++)
    {
        for (j=1; j<size; j++)
        {
            n = (i-1)*size + (j - 1);
            graph_->add_edge_by_indices(n, n +1);
            graph_->add_edge_by_indices(n,n+size);
            graph_->add_edge_by_indices(n,n+size+1);
        }
        n = i*size -1;
        graph_->add_edge_by_indices(n,n+size);
    }
    for (j=1; j<size; j++)
    {
        n = (size-1)*size + (j - 1);
        graph_->add_edge_by_indices(n, n +1);
    }
    return;
}

void Graph_Sampler::create_web(unsigned int size, unsigned int angle)
{
    //std::cout << "Entering Graph_Sampler::create_web with size = " << size << " and angle = " << angle << std::endl;

    if(size < 1)
    {
        size = 1;
    }

    graph_->reset_graph();
    zero_neighbors_indices_.clear();
    zero_neighbors_indices_.reserve(angle);

    double a = -1.5, b = 1.5;
    complex_number z;

    unsigned int i, j, n = 1;
    graph_->add_isolated_vertex(n, Point(complex_number(0,0)), QColor(rand()%256,rand()%256,rand()%256),false);

    n++;
    for (i=1; i<=size; i++)
    {
        for (j=1; j<=angle; j++)
        {
            z = complex_number(i*(b-a)/(2.0*(size+1)) *
                               std::cos(j*2*M_PI*1.0/angle), i*(b-a)/(2.0*(size+1)) * std::sin(j*2*M_PI*1.0/angle));
            graph_->add_isolated_vertex(n, Point(z), QColor(rand()%256,rand()%256,rand()%256), false);
            n++;
        }
    }


    for(j = angle;j>=1;j--)
    {
        n = angle*(size-1)+j;
        zero_neighbors_indices_.push_back(n);
    }

    for(j = 1;j<=angle;j++)
    {
        graph_->add_edge_by_indices(0, j);
    }

    for (i=1; i<size; i++)
    {
        for (j=1; j<angle; j++)
        {
            n = (i-1)*angle + j;
            graph_->add_edge_by_indices(n,n+angle);
            graph_->add_edge_by_indices(n,n+angle+1);
            graph_->add_edge_by_indices(n,n+1);
        }
        graph_->add_edge_by_indices(i*angle,(i+1)*angle);
        graph_->add_edge_by_indices(i*angle, i*angle + 1);
        graph_->add_edge_by_indices(i*angle ,(i-1)*angle + 1);
    }
    for (j=1; j<angle; j++)
    {
        n = (size-1)*angle + j;
        graph_->add_edge_by_indices(n,n+1);
    }
    graph_->add_edge_by_indices(size*angle ,(size-1)*angle + 1);
    return;
}



