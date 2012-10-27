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

#include <QColor>

#include "graph_colorer.hpp"

Graph_Colorer::Graph_Colorer(Graph<Circle> *graph, const coloring_type &coloring)
{
    graph_ = graph;
    coloring_ = coloring;
}

void Graph_Colorer::color()
{
    switch(coloring_)
    {
    case PLAIN:
        color_plain();
        break;
    case STRIPES1:
        color_stripes1();
        break;
    case STRIPES2:
        color_stripes2();
        break;
    case STRIPES3:
        color_stripes3();
        break;
    case FLOWERS:
        color_flowers();
        break;
    case TILING:
        color_tiling();
        break;
    case SHADING:
        color_shading();
        break;
    case ERDOS:
        color_erdos();
        break;
    default:
        std::cout << "ERROR in Graph_Colorer::color: flag problem" << std::endl;
        throw(QString("ERROR in Graph_Colorer::color: flag problem"));
    }
    return;
}

void Graph_Colorer::color_plain()
{

    QColor color(100, 215, 255);

    unsigned int i;
    for(i=0; i<graph_->nb_vertices(); i++)
    {
        graph_->set_color_by_index(i, color);
    }
    return;
}

void Graph_Colorer::color_stripes1()
{

    QColor color1(138, 43, 226);
    QColor color2(51, 204, 255);
    QColor color;

    int y;

    unsigned int i;
    for(i=0; i<graph_->nb_vertices(); i++)
    {
        y = graph_->get_tiling_coordinates_by_index(i).second;
        if (y%2==0)
        {
            color = color1;
        }
        else
        {
            color = color2;
        }
        graph_->set_color_by_index(i, color);
    }
    return;
}

void Graph_Colorer::color_stripes2()
{

    int x,y;
    QColor color1(255, 130, 0);
    QColor color2(255, 210, 10);
    QColor color;

    unsigned int i;
    for(i=0; i<graph_->nb_vertices(); i++)
    {
        x = graph_->get_tiling_coordinates_by_index(i).first;
        y = graph_->get_tiling_coordinates_by_index(i).second;
        if ((x+(y+1)/2)%2==0)
        {
            color = color1;
        }
        else
        {
            color = color2;
        }
        graph_->set_color_by_index(i, color);
    }
    return;
}

void Graph_Colorer::color_stripes3()
{

    int x,y;
    QColor color1(55, 215, 55);
    QColor color2(30, 130, 30);
    QColor color;

    unsigned int i;
    for(i=0; i<graph_->nb_vertices(); i++)
    {
        x = graph_->get_tiling_coordinates_by_index(i).first;
        y = graph_->get_tiling_coordinates_by_index(i).second;
        if ((x+y/2)%2==0)
        {
            color = color1;
        }
        else
        {
            color = color2;
        }
        graph_->set_color_by_index(i, color);
    }
    return;
}

void Graph_Colorer::color_flowers()
{

    int x,y;
    QColor color_2 = QColor(255, 160, 20);
    QColor color_1 = QColor(160, 0, 220);
    QColor color;
    unsigned int i;
    for(i=0;i<graph_->nb_vertices();i++)
    {
        x = graph_->get_tiling_coordinates_by_index(i).first;
        y = graph_->get_tiling_coordinates_by_index(i).second;

        if (y%2==1)
        {
            color = color_1;
        }
        else
        {
            color = (x+y/2)%2==0 ? color_1 : color_2;
        }
        graph_->set_color_by_index(i, color);
    }
    return;
}

void Graph_Colorer::color_tiling()
{

    QColor color1(255,204,0);
    QColor color2(51,204,0);
    QColor color3(51,102,255);
    QColor color;
    unsigned int x,y,t;

    unsigned int i;
    for(i=0; i<graph_->nb_vertices(); i++)
    {
        x = graph_->get_tiling_coordinates_by_index(i).first;
        y = graph_->get_tiling_coordinates_by_index(i).second;
        t = (x + y%2)%3;
        if (t==0)
        {
            color = color1;
        }
        else if (t==1)
        {
            color = color2;
        }
        else if (t==2)
        {
            color = color3;
        }
        graph_->set_color_by_index(i, color);
    }
    return;
}

void Graph_Colorer::extrema_tiling(int &x_min, int &x_max, int &y_min, int &y_max)
{
    integer_coordinates xy = graph_->get_tiling_coordinates_by_index(0);
    x_min = xy.first;
    x_max = xy.first;
    y_min = xy.second;
    y_max = xy.second;

    unsigned int i;
    for(i=1; i<graph_->nb_vertices(); i++)
    {
        xy = graph_->get_tiling_coordinates_by_index(i);
        if (xy.first < x_min)
        {
            x_min = xy.first;
        }
        if (xy.first > x_max)
        {
            x_max = xy.first;
        }
        if (xy.second < y_min)
        {
            y_min = xy.second;
        }
        if (xy.second > y_max)
        {
            y_max = xy.second;
        }
    }

    return;
}

void Graph_Colorer::color_shading()
{

    int x_min, x_max, y_min, y_max, x, y;
    extrema_tiling(x_min, x_max, y_min, y_max);

    int color1[3]={255,255,255};
    int color2[3]={255,0,0};
    int color3[3]={255,255,0};
    int color4[3]={0,0,255};

    int rouge, vert, bleu;
    double t1,t2;

    unsigned int i;
    for(i=0; i<graph_->nb_vertices(); i++)
    {
        x = graph_->get_tiling_coordinates_by_index(i).first;
        y = graph_->get_tiling_coordinates_by_index(i).second;
        t1=(x-x_min)*1.0/(x_max-x_min);
        t2=(y-y_min)*1.0/(y_max-y_min);
        rouge=(int) (t1*color1[0]+(1-t1)*color2[0]+t2*color3[0]+(1-t2)*color4[0])/2;
        vert=(int) ((t1*color1[1]+(1-t1)*color2[1]+t2*color3[1]+(1-t2)*color4[1])/2);
        bleu=(int) ((t1*color1[2]+(1-t1)*color2[2]+t2*color3[2]+(1-t2)*color4[2])/2);
        rouge = std::min(std::max(rouge, 0),255);
        vert = std::min(std::max(vert, 0),255);
        bleu = std::min(std::max(bleu, 0),255);
        graph_->set_color_by_index(i, QColor(rouge,vert,bleu));
    }
    return;
}

void Graph_Colorer::compute_erdos()
{
    unsigned int n = graph_->nb_vertices();
    nb_erdos_.resize(n);

    std::vector<bool> computed;
    computed.resize(n);
    fill(computed.begin(),computed.end(),false);

    std::vector<unsigned int> previously_computed;
    std::vector<unsigned int> new_computed;

    unsigned int nb_computed=0;
    int nb_erdos = 0;

    unsigned int k;
    for(k=0; k<graph_->nb_vertices(); k++)
    {
        if (graph_->get_neighbors_by_index(k).size() < 6)
        {
            nb_erdos_[k] = nb_erdos + 1;
            computed[k] = true;
            previously_computed.push_back(k);
            nb_computed++;
        }
    }
    nb_erdos++;

    std::vector<vertex_label> neighbors;
    unsigned int i,j;
    while(nb_computed<n)
    {
        new_computed.clear();
        for(i=0; i<previously_computed.size(); i++)
        {
            neighbors = graph_->get_neighbors_by_index(previously_computed[i]);
            for (j=0; j<neighbors.size(); j++)
            {
                if(!computed[neighbors[j]-1])
                {
                    nb_erdos_[neighbors[j]-1] = nb_erdos+1;
                    computed[neighbors[j]-1] = true;
                    new_computed.push_back(neighbors[j]-1);
                    nb_computed++;
                }
            }
        }
        previously_computed = new_computed;
        nb_erdos++;
    }
    return;
}

void Graph_Colorer::color_erdos()
{
    compute_erdos();

    QColor color_1 = QColor(100, 149, 237);
    QColor color_2 = QColor(25, 25, 112);
    QColor color;


    unsigned int i;

    for(i=0; i<graph_->nb_vertices(); i++)
    {
        if ((nb_erdos_[i]/2)%2 == 0)
        {
            color = color_1;
        }
        else
        {
            color = color_2;
        }
        graph_->set_color_by_index(i, color);
    }
    return;
}



