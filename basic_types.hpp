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

#ifndef BASIC_TYPES_HPP
#define BASIC_TYPES_HPP

#include <algorithm>
#include <complex>
#include <utility>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <QDebug>
#include <QString>
#include <time.h>

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif


typedef std::complex<double> complex_number;

typedef int vertex_label;

typedef std::pair<vertex_label, vertex_label> edge;

typedef std::pair<int,int> integer_coordinates;
typedef std::pair<double,double> real_coordinates;

typedef int graph_choice_type;
enum{CUSTOM_GRAPH, GRID, WEB};

typedef int curve_choice_type;
enum{CUSTOM, RECTANGLE, POLYGON, STAR, ELLIPSE, EPICYCLOID, KOCH};

typedef int coloring_type;
enum{PLAIN, STRIPES1, STRIPES2, STRIPES3, FLOWERS, TILING, SHADING, ERDOS};

typedef int Triangulation_coloring;
enum{T_PLAIN, T_STRIPES_1, T_STRIPES_2, T_STRIPES_3, T_TRIANGLES, T_SQUARES, T_SQUARES_2, T_SHADING, T_IMAGE};

typedef int radii_algo_type;
enum{STANDARD, SUPER};

typedef int positions_algo_type;
enum{REGULAR, DOWNWARDS, UPWARDS};

typedef int mode_type;
enum {START, DRAW_GRAPH, DRAW_CURVE, DRAW_2_CURVES};

typedef int draw_graph_mode_type;
enum {DRAW_GRAPH_VERTICES, DRAW_GRAPH_EDGES, DRAW_GRAPH_READY, DRAW_GRAPH_DONE};

typedef int draw_curve_mode_type;
enum {DRAW_CURVE_NODES, DRAW_CURVE_INVALID, DRAW_CURVE_READY, DRAW_CURVE_DONE};

typedef int draw_2_curves_mode_type;
enum {DRAWING_CURVES, DRAW_2_CURVES_DONE};

typedef int signal_type;
enum{GHOST, VERTEX_HIGHLIGHTED_LEFT, VERTEX_HIGHLIGHTED_RIGHT, DRAW_EDGES_ON, DRAW_EDGES_OFF,
     GRAPH_COMPLETABLE_ON, GRAPH_COMPLETABLE_OFF, CURVE_CREATED,
     CURVE_READY_ON, CURVE_READY_OFF, MESSAGE, CURVE_CREATED_2, CURVE_READY_ON_2, CURVE_READY_OFF_2};

int int_round(double x);

#endif // BASIC_TYPES_HPP
