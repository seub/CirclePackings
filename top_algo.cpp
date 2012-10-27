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

#include "top_algo.hpp"
#include "window.hpp"
#include "canvas.hpp"
#include "output_menu.hpp"
#include "riemann_positions.hpp"

#include <QComboBox>
#include <QCheckBox>
#include <QThread>
#include <QDebug>
#include <QLabel>


Top_Algo::Top_Algo()
{
    tol_radii_reg_ = 0.000000000001;
    tol_radii_updown_ = 0.000000000001;
    tol_positions_reg_ = 0.003;
    timer_ = new QTimer(this);
    global_timer_ = new QTime();
    global_timer_->start();
}

Top_Algo::~Top_Algo()
{
    delete timer_;
    delete global_timer_;
}

void Top_Algo::reset_algo(Window *window)
{
    window_ = window;
    mode_flag_ = window->mode_flag_;


    input_graph_ = window->maths_contents_.input_algo_graph_1_;
    input_angle_ = window->maths_contents_.input_algo_angle_1_;

    input_graph_2_ = window->maths_contents_.input_algo_graph_2_;
    input_angle_2_ = window->maths_contents_.input_algo_angle_2_;

    status_bar_ = window->status_bar_;

    output_up_.reset_graph();
    output_down_.reset_graph();
    output_circle_packing_.reset_graph();

    curve_1_ = &(window->maths_contents_.curve_1_);
    curve_2_ = &(window->maths_contents_.curve_2_);

    RP_1_ = &window_->maths_contents_.riemann_positions_1_;
    RP_2_ = &window_->maths_contents_.riemann_positions_2_;

    output_up_2_.reset_graph();
    output_down_2_.reset_graph();
    output_circle_packing_2_.reset_graph();

    show_live_cp_ = window->output_menu_->show_live_checkbox_->checkState();

    double_mix_flag_ = false;
    up_down_flag_1_ = false;
    up_down_flag_2_ = false;
    double_RP_flag_ = false;

    disconnect(&A_reg_, SIGNAL(finished()), this, SLOT(A_reg_finished()));
    disconnect(&A_up_, SIGNAL(finished()), this, SLOT(test_up_down_1_finished()));
    disconnect(&A_up_2_, SIGNAL(finished()), this, SLOT(test_up_down_2_finished()));
    disconnect(&A_down_, SIGNAL(finished()), this, SLOT(test_up_down_1_finished()));
    disconnect(&A_down_2_, SIGNAL(finished()), this, SLOT(test_up_down_2_finished()));
    disconnect(&positions_mixer_1_, SIGNAL(finished()), this, SLOT(test_double_mix_finished()));
    disconnect(&positions_mixer_2_, SIGNAL(finished()), this, SLOT(test_double_mix_finished()));
    disconnect(RP_1_, SIGNAL(finished()), this, SLOT(test_double_RP_finished()));
    disconnect(RP_2_, SIGNAL(finished()), this, SLOT(test_double_RP_finished()));


    connect(&A_reg_, SIGNAL(finished()), this, SLOT(A_reg_finished()));
    connect(&A_up_, SIGNAL(finished()), this, SLOT(test_up_down_1_finished()));
    connect(&A_up_2_, SIGNAL(finished()), this, SLOT(test_up_down_2_finished()));
    connect(&A_down_, SIGNAL(finished()), this, SLOT(test_up_down_1_finished()));
    connect(&A_down_2_, SIGNAL(finished()), this, SLOT(test_up_down_2_finished()));
    connect(&positions_mixer_1_, SIGNAL(finished()), this, SLOT(test_double_mix_finished()));
    connect(&positions_mixer_2_, SIGNAL(finished()), this, SLOT(test_double_mix_finished()));
    connect(RP_1_, SIGNAL(finished()), this, SLOT(test_double_RP_finished()));
    connect(RP_2_, SIGNAL(finished()), this, SLOT(test_double_RP_finished()));

    if (mode_flag_ == DRAW_CURVE)
    {
        tol_positions_updown_ = 0.02;
    }
    else if (mode_flag_ == DRAW_2_CURVES)
    {
        tol_positions_updown_ = 0.06;
    }

    return;
}

void Top_Algo::test_double_mix_finished()
{
    if(!double_mix_flag_ && mode_flag_ == DRAW_2_CURVES)
    {
        double_mix_flag_ = true;
    }
    else
    {
        double_mix_flag_ = false;
        mix_finished();
    }
}

void Top_Algo::test_up_down_1_finished()
{
    if(!up_down_flag_1_)
    {
        up_down_flag_1_ = true;
    }
    else
    {
        up_down_flag_1_ = false;
        output_up_ = A_up_.get_ouput_circle_packing();
        output_down_ = A_down_.get_ouput_circle_packing();
        mix_up_down();

    }
    return;
}

void Top_Algo::test_up_down_2_finished()
{
    if(!up_down_flag_2_)
    {
        up_down_flag_2_ = true;
    }
    else
    {
        up_down_flag_2_ = false;
        output_up_2_ = A_up_2_.get_ouput_circle_packing();
        output_down_2_ = A_down_2_.get_ouput_circle_packing();
        mix_up_down_2();

    }
    return;
}
/*const Graph<Circle> & Top_Algo::get_output_circle_packing() const
{
    return output_circle_packing_;
}

const Graph<Circle> & Top_Algo::get_output_circle_packing_2() const
{
    return output_circle_packing_2_;
}*/

void Top_Algo::launch()
{
    global_timer_->restart();
    switch(mode_flag_)
    {
    case DRAW_GRAPH:
        A_reg_.reset_algo(input_graph_, input_angle_, SUPER, REGULAR, tol_radii_reg_, tol_positions_reg_,
                          &(window_->maths_contents_.show_live_cp_right_), window_->right_canvas_, show_live_cp_);
        A_reg_.start();
        break;

    case DRAW_CURVE:
        A_up_.reset_algo(input_graph_, input_angle_, SUPER, UPWARDS, tol_radii_updown_, tol_positions_updown_,
                         &(window_->maths_contents_.show_live_cp_left_), window_->left_canvas_, show_live_cp_);
        A_down_.reset_algo(input_graph_, input_angle_, SUPER, DOWNWARDS, tol_radii_updown_, tol_positions_updown_,
                           &(window_->maths_contents_.show_live_cp_right_), window_->right_canvas_, show_live_cp_);
        A_up_.start();
        A_down_.start();
        break;

    case DRAW_2_CURVES:
        A_up_.reset_algo(input_graph_, input_angle_, SUPER, UPWARDS, tol_radii_updown_, tol_positions_updown_);
        A_down_.reset_algo(input_graph_, input_angle_, SUPER, DOWNWARDS, tol_radii_updown_, tol_positions_updown_);
        A_up_.start();
        A_down_.start();
        A_up_2_.reset_algo(input_graph_2_, input_angle_2_, SUPER, UPWARDS, tol_radii_updown_, tol_positions_updown_);
        A_down_2_.reset_algo(input_graph_2_, input_angle_2_, SUPER, DOWNWARDS, tol_radii_updown_, tol_positions_updown_);
        A_up_2_.start();
        A_down_2_.start();
        break;

    default:
        std::cout << "ERROR in Top_Algo::launch : flag problem" << std::endl;
        throw(QString("ERROR in Top_Algo::launch : flag problem"));
    }

    show_live_error();
    return;
}

void Top_Algo::stop()
{
    switch(mode_flag_)
    {
    case DRAW_GRAPH:
        A_reg_.stop();
        break;

    case DRAW_CURVE:
        A_up_.stop();
        A_down_.stop();
        break;

    case DRAW_2_CURVES:
        A_up_.stop();
        A_down_.stop();
        A_up_2_.stop();
        A_down_2_.stop();
        break;

    default:
        std::cout << "ERROR in Top_Algo::stop : flag problem" << std::endl;
        throw(QString("ERROR in Top_Algo::stop : flag problem"));
    }
}

void Top_Algo::show_total_time()
{
    double time = global_timer_->elapsed()/1000.0;

    QString str;

    switch(mode_flag_)
    {
    case DRAW_GRAPH:
        if(time<2)
        {
            str = QString("Circle packing computed in %2 second (%3 iterations)")
                    .arg(time, 0, 'f', 2)
                    .arg(A_reg_.get_nb_iterations());
        }
        else
        {
            str = QString("Circle packing computed in %2 seconds (%3 iterations)")
                    .arg(time, 0, 'f', 2)
                    .arg( A_reg_.get_nb_iterations());
        }
        break;

    case DRAW_CURVE:
        if(time<2)
        {
            str = QString("Circle packing computed in %2 second (%3 iterations up, %4 iterations down)")
                    .arg(time, 0, 'f', 2)
                    .arg(A_up_.get_nb_iterations())
                    .arg(A_down_.get_nb_iterations());
        }
        else
        {
            str = QString("Circle packing computed in %2 seconds (%3 iterations up, %4 iterations down)")
                    .arg(time, 0, 'f', 2)
                    .arg(A_up_.get_nb_iterations())
                    .arg(A_down_.get_nb_iterations());
        }
        break;
    case DRAW_2_CURVES:
        if(time<2)
        {
            /*str = QString("Computed in %2 second (Iterations: %3 / %4 left, %5 / %6 right)")
                    .arg(time)
                    .arg(A_up_.get_nb_iterations())
                    .arg(A_down_.get_nb_iterations())
                    .arg(A_up_2_.get_nb_iterations())
                    .arg(A_down_2_.get_nb_iterations());*/

            str = QString("Computed in %2 second   (circle packings %3s,  triangulations and search trees %4s)")
                    .arg(time, 0, 'f', 2)
                    .arg(inter_time_, 0, 'f', 2)
                    .arg(time - inter_time_, 0, 'f', 2);
        }
        else
        {
            /*str = QString("Computed in %2 seconds (Iterations: %3 / %4 left, %5 / %6 right)")
                    .arg(time)
                    .arg(A_up_.get_nb_iterations())
                    .arg(A_down_.get_nb_iterations())
                    .arg(A_up_2_.get_nb_iterations())
                    .arg(A_down_2_.get_nb_iterations());*/

            str = QString("Computed in %2 seconds   (circle packings %3s,  triangulations and search trees %4s)")
                    .arg(time, 0, 'f', 2)
                    .arg(inter_time_, 0, 'f', 2)
                    .arg(time - inter_time_, 0, 'f', 2);
        }
        break;

    default:
        std::cout << "ERROR in Top_Algo::show_time : flag problem" << std::endl;
        throw(QString("ERROR in Top_Algo::show_time : flag problem"));
    }

    status_bar_->showMessage(str, 7000);
    return;
}

void Top_Algo::A_reg_finished()
{
    output_circle_packing_ = A_reg_.get_ouput_circle_packing();
    window_->maths_contents_.output_circle_packing_1_ = output_circle_packing_;
    finish();
    return;
}

void Top_Algo::mix_finished()
{

    if (mode_flag_ == DRAW_CURVE)
    {
        window_->maths_contents_.output_circle_packing_1_ = output_circle_packing_;
        finish();
    }
    else if (mode_flag_ == DRAW_2_CURVES)
    {
        disconnect(timer_, SIGNAL(timeout()), this, SLOT(update_message_draw_2_curves()));
        timer_->stop();

        window_->maths_contents_.output_circle_packing_1_ = output_circle_packing_;
        window_->maths_contents_.output_circle_packing_2_ = output_circle_packing_2_;

        window_->status_bar_->showMessage("Computing triangulations and search trees...");
        window_->status_bar_->repaint();

        inter_time_ = global_timer_->elapsed()/1000.0;

        RP_1_->start();
        RP_2_->start();

    }
    else
    {
        std::cout << "ERROR in Top_Algo::mix_finished: flag problem" << std::endl;
        throw(QString("ERROR in Top_Algo::mix_finished: flag problem"));
    }
    return;
}

void Top_Algo::test_double_RP_finished()
{
    if(!double_RP_flag_)
    {
        double_RP_flag_ = true;
    }
    else
    {
        double_RP_flag_=false;
        finish();
    }
    return;
}



void Top_Algo::mix_up_down()
{
    positions_mixer_1_.reset(&input_graph_, &output_up_, &output_down_, &output_circle_packing_);
    positions_mixer_1_.start();
    return;
}

void Top_Algo::mix_up_down_2()
{
    positions_mixer_2_.reset(&input_graph_2_, &output_up_2_, &output_down_2_, &output_circle_packing_2_);
    positions_mixer_2_.start();
    return;
}

void Top_Algo::show_live_error()
{

    switch(mode_flag_)
    {
    case DRAW_GRAPH:
        connect(timer_, SIGNAL(timeout()), this, SLOT(update_message_draw_graph()));
        timer_->start(30);
        break;

    case DRAW_CURVE:
        connect(timer_, SIGNAL(timeout()), this, SLOT(update_message_draw_curve()));
        timer_->start(30);
        break;

    case DRAW_2_CURVES:
        connect(timer_, SIGNAL(timeout()), this, SLOT(update_message_draw_2_curves()));
        timer_->start(30);
        break;

    default:
        std::cout << "ERROR in Top_Algo::show_live_error: flag problem" << std::endl;
        throw(QString("ERROR in Top_Algo::show_live_error: flag problem"));
    }
}

void Top_Algo::update_message_draw_graph()
{
    QString str = QString("Computing circle packing...          Error:      Radii :  %2 Positions :  %3 (target: %4)")
            .arg(A_reg_.get_radii_error(), -13, 'e', 2)
            .arg(A_reg_.get_positions_error(), -13, 'e', 2)
            .arg(tol_positions_reg_, 6, 'e', 1);

    status_bar_->showMessage(str);
    timer_->start(30);
}

void Top_Algo::update_message_draw_curve()
{
    QString str = QString(
                "Computing circle packing...          Error (Up / Down):      Radii:  %2 / %3 Positions:  %4 / %5 (target: %6)")
            .arg(A_up_.get_radii_error(), 6, 'e', 2)
            .arg(A_down_.get_radii_error(), -13, 'e', 2)
            .arg(A_up_.get_positions_error(), 6, 'e', 2)
            .arg(A_down_.get_positions_error(), -13, 'e', 2)
            .arg(tol_positions_updown_, 6, 'e', 1);
    status_bar_->showMessage(str);
    timer_->start(50);
    return;
}

void Top_Algo::update_message_draw_2_curves()
{
    QString str = QString(
                "Computing circle packings...          Error on positions (Up 1 / Down 1 / Up 2 / Down 2):      %2 / %3 / %4 / %5 (target: %6)")
            .arg(A_up_.get_positions_error(), 6, 'e', 2)
            .arg(A_down_.get_positions_error(), 6, 'e', 2)
            .arg(A_up_2_.get_positions_error(), 6, 'e', 2)
            .arg(A_down_2_.get_positions_error(), -13, 'e', 2)
            .arg(tol_positions_updown_, 6, 'e', 1);
    status_bar_->showMessage(str);
    timer_->start(50);
    return;
}

void Top_Algo::finish()
{
    switch(mode_flag_)
    {
    case DRAW_GRAPH:
        disconnect(timer_, SIGNAL(timeout()), this, SLOT(update_message_draw_graph()));
        timer_->stop();
        show_total_time();
        break;

    case DRAW_CURVE:
        disconnect(timer_, SIGNAL(timeout()), this, SLOT(update_message_draw_curve()));
        timer_->stop();
        show_total_time();
        break;

    case DRAW_2_CURVES:
        show_total_time();
        break;

    default:
        std::cout << "ERROR in Top_Algo::finish: flag problem" << std::endl;
        throw(QString("ERROR in Top_Algo::finish: flag problem"));
    }


    window_->maths_contents_.output_circle_packing_save_1_ = output_circle_packing_;

    if (mode_flag_ == DRAW_2_CURVES)
    {
        window_->maths_contents_.output_circle_packing_save_2_ = output_circle_packing_2_;
    }
    emit end();
    return;
}



