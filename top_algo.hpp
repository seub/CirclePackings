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

#ifndef TOP_ALGO_HPP
#define TOP_ALGO_HPP

#include "basic_types.hpp"
#include "graph.hpp"
#include "positions.hpp"
#include "algo.hpp"
#include "triangulation.hpp"
#include "riemann_positions.hpp"
#include "triangulation_search_tree.hpp"
#include "positions_mixer.hpp"

#include <QThread>
#include <QStatusBar>
#include <QTimer>
#include <QTime>

class QComboBox;
class QString;
class Canvas;
class Window;

class Top_Algo : public QThread
{
    Q_OBJECT

public:
    Top_Algo();
    ~Top_Algo();

    void reset_algo(Window* window);

    void show_total_time();
    void show_live_error();
    void finish();

    //const Graph<Circle> & get_output_circle_packing() const;
    //const Graph<Circle> & get_output_circle_packing_2() const;
    void launch();

public slots:
    void stop();
    void update_message_draw_graph();
    void update_message_draw_curve();
    void update_message_draw_2_curves();
    void mix_finished();

private slots:
    void A_reg_finished();
    void test_double_RP_finished();
    void test_double_mix_finished();
    void test_up_down_1_finished();
    void test_up_down_2_finished();

signals:
    void end();

private:
    void mix_up_down();
    void mix_up_down_2();

    mode_type mode_flag_;
    QStatusBar* status_bar_;

    Graph<Empty> input_graph_;
    complex_number input_angle_;
    Graph<Empty> input_graph_2_;
    complex_number input_angle_2_;

    double tol_radii_reg_;
    double tol_radii_updown_;
    double tol_positions_reg_;
    double tol_positions_updown_;


    Graph<Circle> output_circle_packing_;
    Graph<Circle> output_circle_packing_2_;

    Curve * curve_1_;
    Curve * curve_2_;
    Triangulation * initial_triangulation_1_;
    Triangulation * initial_triangulation_2_;

    Algo A_reg_;
    Algo A_up_;
    Algo A_down_;
    Algo A_up_2_;
    Algo A_down_2_;

    Positions_Mixer positions_mixer_1_;
    Positions_Mixer positions_mixer_2_;

    Riemann_Positions *RP_1_;
    Riemann_Positions *RP_2_;

    Graph<Circle> output_up_;
    Graph<Circle> output_down_;
    Graph<Circle> output_up_2_;
    Graph<Circle> output_down_2_;

    bool show_live_cp_;

    QTimer* timer_;
    QTime* global_timer_;
    double inter_time_;
    Window* window_;

    bool double_mix_flag_;
    bool up_down_flag_1_;
    bool up_down_flag_2_;
    bool double_RP_flag_;
};



#endif // TOP_ALGO_HPP
