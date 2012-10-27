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

#ifndef POSITIONS_MIXER_HPP
#define POSITIONS_MIXER_HPP

#include <QThread>

#include "basic_types.hpp"
#include "graph.hpp"

class Positions_Mixer : public QThread
{
    Q_OBJECT

public:
    Positions_Mixer() {}
    void reset(Graph<Empty> *abstract_graph, Graph<Circle> *circle_packing_up,
               Graph<Circle> *circle_packing_down, Graph<Circle> *output_circle_packing,
               double beta = 0.6);

public slots:
    void run();

private:
    void compute_up_weights();
    void compute_down_weights();
    void mix();

    Graph<Empty> *abstract_graph_;
    Graph<Circle> *circle_packing_up_;
    Graph<Circle> *circle_packing_down_;
    Graph<Circle> *output_circle_packing_;

    std::vector<double> up_weights_;
    std::vector<double> down_weights_;
    double beta_;
};

#endif // POSITIONS_MIXER_HPP
