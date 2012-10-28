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



#include <QIcon>
#include <QGridLayout>
#include <QStatusBar>
#include <QKeyEvent>
#include <QSpinBox>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QCheckBox>
#include <QString>
#include <QApplication>
#include <QDesktopWidget>
#include <QRadioButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QUrl>

#include "window.hpp"
#include "canvas.hpp"
#include "mobius.hpp"
#include "top_algo.hpp"
#include "input_menu.hpp"
#include "output_menu.hpp"
#include "canvas_delegate.hpp"
#include "canvas_delegate_draw_graph.hpp"
#include "canvas_delegate_draw_curve.hpp"
#include "canvas_delegate_show_cp.hpp"
#include "canvas_delegate_show_live.hpp"
#include "canvas_delegate_tests.hpp"
#include "canvas_delegate_riemann.hpp"
#include "tests.hpp"
#include "top_menu.hpp"
#include "configuration.hpp"


Window::Window()
{
    mode_flag_ = START;
    draw_graph_flag_ = DRAW_GRAPH_VERTICES;
    draw_curve_flag_ = DRAW_CURVE_NODES;
    draw_2_curves_flag_ = DRAWING_CURVES;
    create_window();
}

Window::~Window()
{
    //std::cout << "Leaving Window::~Window()" << std::endl;
}

void Window::create_window()
{
    //std::cout << "Entering Window::create_window" << std::endl;

    left_canvas_ = new Canvas(this, true);
    right_canvas_ = new Canvas(this, false);
    left_canvas_->setEnabled(false);
    right_canvas_->setEnabled(false);

    input_menu_ = new Input_Menu(this);
    output_menu_ = new Output_Menu(this);

    status_bar_= new QStatusBar(this);
    status_bar_->setSizeGripEnabled(false);
    status_bar_->setFixedHeight(25);
    status_bar_label_ = new QLabel;
    status_bar_label_->setText("Ready");
    status_bar_->insertWidget(0, status_bar_label_);


    top_menu_ = new Top_Menu(this);
    configuration_ = new Configuration(this);

    layout_ = new QGridLayout;



    layout_->setMargin(9);
    layout_->setSpacing(6);

    layout_->setMenuBar(top_menu_);

    layout_->setColumnMinimumWidth(0, menu_width());
    int left_col_menu_size = std::max(input_menu_->max_left_col_width(), output_menu_->max_left_col_width());
    input_menu_->layout_->setColumnMinimumWidth(0, left_col_menu_size);
    output_menu_->layout_->setColumnMinimumWidth(0, left_col_menu_size);


    layout_->addWidget(left_canvas_, 0, 1, 2, 1);
    layout_->addWidget(right_canvas_, 0, 2, 2, 1);
    layout_->addWidget(input_menu_, 0, 0, Qt::AlignTop);
    layout_->addWidget(output_menu_, 1, 0, Qt::AlignTop);
    layout_->addWidget(status_bar_,2, 0, 1, 3);
    setLayout(layout_);


    setWindowTitle(tr("Circle Packings"));

    QIcon icon(":/images/icon.png");
    setWindowIcon(icon);



    unsigned int w, h;
    optimal_size(w,h);
    resize(w, h);
    move(QApplication::desktop()->screen()->rect().center() - rect().center());


    image_loaded_ = false;

    //std::cout << "Leaving Window::create_window()" << std::endl;
    return;
}

void Window::restore_configuration()
{
    configuration_->restore_configuration();
    return;
}

void Window::update_configuration()
{
    configuration_->update_parameters();
    return;
}

int Window::menu_width() const
{
    return std::max(input_menu_->max_width(),output_menu_->max_width());
}

void Window::optimal_size(unsigned int &w, unsigned int &h) const
{
    unsigned int screen_width = QApplication::desktop()->width();
    unsigned int screen_height = QApplication::desktop()->height();

    int canvas_optimal_size = std::max(input_menu_->max_height() + output_menu_->max_height() +layout_->verticalSpacing(),
                                       int_round((0.9*screen_width - menu_width() - 2*layout_->margin() - 2*layout_->spacing())/2));

    h = std::max(input_menu_->max_height() + output_menu_->max_height()
                 +layout_->verticalSpacing(),canvas_optimal_size) + 2*layout_->margin() + status_bar_->height() +layout_->verticalSpacing() + top_menu_->sizeHint().height();
    if(h > screen_height)
    {
        h = 0.9*(input_menu_->max_height() + output_menu_->max_height()
                 +layout_->verticalSpacing() + 2*layout_->margin() + status_bar_->height() +layout_->verticalSpacing() + top_menu_->height());
    }
    w = std::min(menu_width() + 2*layout_->margin() + 2*canvas_optimal_size + 2*layout_->horizontalSpacing(), int_round(0.9*screen_width));

}

void Window::set_canvas_delegates(bool reset_contents, bool only_right)
{
    //std::cout << "Entering Window::set_canvas_delegates" << std::endl;

    enable_canvases_updates(false);

    if (!only_right)
    {
        delete left_canvas_->canvas_delegate_;
    }
    delete right_canvas_->canvas_delegate_;

    switch(mode_flag_)
    {
    case START:
        left_canvas_->canvas_delegate_ = new Canvas_Delegate();
        right_canvas_->canvas_delegate_ = new Canvas_Delegate();
        break;
    case DRAW_GRAPH:
        if (!only_right)
        {
            left_canvas_->canvas_delegate_ = new Canvas_Delegate_Draw_Graph(
                        left_canvas_->size_in_pixels_, &draw_graph_flag_, &maths_contents_.input_planar_graph_,
                        this, reset_contents);
        }
        right_canvas_->canvas_delegate_ = new Canvas_Delegate_Show_CP(
                    right_canvas_->size_in_pixels_, &mode_flag_, &draw_graph_flag_, &draw_curve_flag_,
                    &maths_contents_.output_circle_packing_save_1_, &maths_contents_.output_circle_packing_1_,
                    this, reset_contents);
        break;

    case DRAW_CURVE:
        if (!only_right)
        {
            left_canvas_->canvas_delegate_ = new Canvas_Delegate_Draw_Curve(
                        left_canvas_->size_in_pixels_, &draw_curve_flag_, &curve_choice_flag_,
                        &maths_contents_.curve_1_, &maths_contents_.tiling_, &maths_contents_.nodes_1_,
                        &maths_contents_.tangent_factors_1_,
                        &maths_contents_.input_circle_packing_1_, this, reset_contents);
        }
        right_canvas_->canvas_delegate_ = new Canvas_Delegate_Show_CP(
                    right_canvas_->size_in_pixels_, &mode_flag_, &draw_graph_flag_, &draw_curve_flag_,
                    &maths_contents_.output_circle_packing_save_1_, &maths_contents_.output_circle_packing_1_,
                    this, reset_contents);

        if (!reset_contents && (curve_choice_flag_==POLYGON || curve_choice_flag_==STAR))
        {
            unsigned int nb_sides;
            if (curve_choice_flag_==POLYGON)
            {
                nb_sides = input_menu_->nb_points_spinbox_->value();
            }
            else if (curve_choice_flag_==STAR)
            {
                nb_sides = input_menu_->nb_points_spinbox_->value();
            }
            left_canvas_->canvas_delegate_->set_nb_sides(nb_sides);
        }
        break;

    case DRAW_2_CURVES:
        if (!only_right)
        {
            left_canvas_->canvas_delegate_ = new Canvas_Delegate_Riemann_Mapping(
                        left_canvas_->size_in_pixels_, &draw_2_curves_flag_, &draw_curve_flag_, &curve_choice_flag_,
                        &maths_contents_.curve_1_, &maths_contents_.tiling_, &maths_contents_.nodes_1_,
                        &maths_contents_.tangent_factors_1_,
                        &maths_contents_.input_circle_packing_1_, &maths_contents_.triangulation_1_,
                        &maths_contents_.output_triangulation_1_,
                        &maths_contents_.triangulation_2_,
                        &maths_contents_.input_triangulation_search_tree_1_,
                        &maths_contents_.output_triangulation_search_tree_1_, &maths_contents_.riemann_positions_2_,
                        &maths_contents_.output_circle_packing_2_, &maths_contents_.output_circle_packing_save_2_,
                        this,
                        &maths_contents_.image_, &maths_contents_.left_image_scale_,
                        &maths_contents_.left_image_origin_x_in_pixels_, &maths_contents_.left_image_origin_y_in_pixels_,
                        &maths_contents_.right_image_scale_, &maths_contents_.right_image_origin_x_in_pixels_,
                        &maths_contents_.right_image_origin_y_in_pixels_, reset_contents);
        }

        right_canvas_->canvas_delegate_ = new Canvas_Delegate_Riemann_Mapping(
                    right_canvas_->size_in_pixels_, &draw_2_curves_flag_, &draw_curve_flag_2_, &curve_choice_flag_2_,
                    &maths_contents_.curve_2_, &maths_contents_.tiling_, &maths_contents_.nodes_2_,
                    &maths_contents_.tangent_factors_2_,
                    &maths_contents_.input_circle_packing_2_, &maths_contents_.triangulation_2_,
                    &maths_contents_.output_triangulation_2_,
                    &maths_contents_.triangulation_1_,
                    &maths_contents_.input_triangulation_search_tree_2_,
                    &maths_contents_.output_triangulation_search_tree_2_, &maths_contents_.riemann_positions_1_,
                    &maths_contents_.output_circle_packing_1_, &maths_contents_.output_circle_packing_save_1_,
                    this, &maths_contents_.image_, &maths_contents_.right_image_scale_,
                    &maths_contents_.right_image_origin_x_in_pixels_, &maths_contents_.right_image_origin_y_in_pixels_,
                    &maths_contents_.left_image_scale_, &maths_contents_.left_image_origin_x_in_pixels_,
                    &maths_contents_.left_image_origin_y_in_pixels_, reset_contents);

        if (!reset_contents && (curve_choice_flag_==POLYGON || curve_choice_flag_==STAR))
        {
            unsigned int nb_sides;
            if (curve_choice_flag_==POLYGON)
            {
                nb_sides = input_menu_->nb_points_spinbox_->value();
            }
            else if (curve_choice_flag_==STAR)
            {
                nb_sides = input_menu_->nb_points_spinbox_->value();
            }
            left_canvas_->canvas_delegate_->set_nb_sides(nb_sides);
        }
        if (!reset_contents && (curve_choice_flag_2_==POLYGON || curve_choice_flag_2_==STAR))
        {
            unsigned int nb_sides_2;
            if (curve_choice_flag_2_==POLYGON)
            {
                nb_sides_2 = input_menu_->nb_points_spinbox_2_->value();
            }
            else if (curve_choice_flag_2_==STAR)
            {
                nb_sides_2 = input_menu_->nb_points_spinbox_2_->value();
            }
            right_canvas_->canvas_delegate_->set_nb_sides(nb_sides_2);
        }
        break;

    default:
        std::cout << "ERROR in Window::set_canvas_delegates: flag problem" << std::endl;
        throw(QString("ERROR in Window::set_canvas_delegates: flag problem"));
    }

    update_draw_borders();

    left_canvas_->set_left(true);
    right_canvas_->set_left(false);

    if (mode_flag_ != START)
    {
        enable_canvases_updates(true);
        left_canvas_->rescale();
        right_canvas_->rescale();
        left_canvas_->update();
        right_canvas_->update();
    }

    //std::cout << "Leaving Window::set_canvas_delegates" << std::endl;
    return;
}

// Events

void Window::resizeEvent(QResizeEvent * event)
{
    //std::cout << "Entering Window::resizeEvent " << clock()*0.01/CLOCKS_PER_SEC <<std::endl;
    //reset_selected_stuff();

    int delta_width = abs(event->size().width() - event->oldSize().width());
    int delta_height = abs(event->size().height() - event->oldSize().height());

    if (delta_width > 1 || delta_height > 1)
    {
        enable_canvases_updates(false);
        resize_canvases();
        enable_canvases_updates(true);
    }

    return;
}

void Window::resize_canvases()
{

    int margin = layout_->margin();
    int vert_ext_margin = std::max(input_menu_->geometry().top(), margin);
    int hor_ext_margin = input_menu_->geometry().left();
    int status_bar_height = status_bar_->height();
    int input_menu_width = input_menu_->width();
    int inter_width = layout_->horizontalSpacing();
    int inter_height = layout_->verticalSpacing();

    int max_width_h = (width() - (2*hor_ext_margin + input_menu_width + 2*inter_width))/2;
    int max_height_h = height() - (status_bar_height + inter_height + vert_ext_margin + margin);
    int new_size_h, extra_width_h, extra_height_h;
    if (max_width_h < max_height_h)
    {
        new_size_h = max_width_h;
        extra_width_h = 0;
        extra_height_h = std::max(height() - (status_bar_height + inter_height + vert_ext_margin + margin) - new_size_h, 0);
    }
    else
    {
        new_size_h = max_height_h;
        extra_height_h = 0;
        extra_width_h = std::max(width() - (2*hor_ext_margin + input_menu_width + 2*inter_width) - 2*new_size_h, 0);
    }

    int max_width_v = width() - (2*hor_ext_margin + input_menu_width + inter_width);
    int max_height_v = (height() - (status_bar_height + 2*inter_height + vert_ext_margin + margin))/2;
    int new_size_v, extra_width_v, extra_height_v;
    if (max_width_v < max_height_v)
    {
        new_size_v = max_width_v;
        extra_width_v = 0;
        extra_height_v = std::max(height() - (status_bar_height + 2*inter_height + vert_ext_margin + margin) - 2*new_size_v, 0);
    }
    else
    {
        new_size_v = max_height_v;
        extra_height_v = 0;
        extra_width_v = std::max(width() - (2*hor_ext_margin + input_menu_width + inter_width) - new_size_v, 0);
    }


    int w_h = extra_width_h/3;
    int h_h = extra_height_h/2;
    int w_v = extra_width_v/2;
    int h_v = extra_height_v/3;

    int left_L, top_L, left_R, top_R, size;
    if (new_size_v > new_size_h)
    {
        left_L = (hor_ext_margin + input_menu_width + inter_width) + w_v;
        top_L = vert_ext_margin + h_v;
        left_R = (hor_ext_margin + input_menu_width + inter_width) + w_v;
        top_R = vert_ext_margin + h_v + new_size_v + inter_height + h_v;
        size = new_size_v;

    }
    else
    {
        left_L = (hor_ext_margin + input_menu_width + inter_width) + w_h;
        top_L = vert_ext_margin + h_h;
        left_R = (hor_ext_margin + input_menu_width + inter_width) + w_h + new_size_h + inter_width + w_h;
        top_R = vert_ext_margin + h_h;
        size = new_size_h;
    }

    QRect left_canvas_rect(left_L, top_L, size, size);
    QRect right_canvas_rect(left_R, top_R, size, size);


    if (left_canvas_rect != left_canvas_->geometry() || right_canvas_rect != right_canvas_->geometry())
    {
        left_canvas_->setGeometry(left_canvas_rect);
        left_canvas_->rescale();
        right_canvas_->setGeometry(right_canvas_rect);
        right_canvas_->rescale();
    }

    //print_dimensions();

    //std::cout << "Leaving Window::resizeEvent" << std::endl;
    return;
}

void Window::print_dimensions()
{
    //std::cout << "window size : " << width() << " x " << height() << std::endl;
    //std::cout << "input menu size : " << input_menu_->width() << " x " << input_menu_->height()
    //          << " col width: " << input_menu_->layout_->columnMinimumWidth(0)
    //          << ", output menu size : " << output_menu_->width() << " x " << output_menu_->height() << std::endl;
    //std::cout << "left canvas size : " << left_canvas_->width() << " x " << left_canvas_->height()
    //          << ", right canvas size : " << right_canvas_->width() << " x " << right_canvas_->height() << std::endl;
    //qDebug() << "input menu geometry: " << input_menu_->geometry();
    //qDebug() << "left canvas geometry : " << left_canvas_->geometry() << ", right canvas geometry : " << right_canvas_->geometry();
    //qDebug() << "input menu max_width: " << input_menu_->max_width();
    //qDebug() << "top_menu: " << top_menu_->geometry();

    return;
}



// Input menu actions
void Window::change_mode(int mode_flag, bool create_tiling)
{
    //std::cout << "Entering Window::change_mode " << std::endl;

    enable_canvases_updates(false);
    maths_contents_.reset_all_but_tiling();

    input_menu_->change_mode_menu(mode_flag_, mode_flag, create_tiling);
    output_menu_->change_mode_menu(mode_flag_, mode_flag);
    mode_flag_ = mode_flag;


    reset_window(false);

    set_canvas_delegates();

    resize_canvases();


    //std::cout << "Leaving Window::change_mode " << std::endl;
    return;
}

void Window::reset_window(bool reset_menus) // Only executes action of pressing 'Reset' button (in given mode)
{
    //std::cout << "Entering Window::reset_window" << std::endl;

    enable_canvases_updates(false);
    if (reset_menus)
    {
        input_menu_->reset_input_menu();
        output_menu_->reset_output_menu();
    }

    reset_flags();

    left_canvas_->reset_canvas(true);
    right_canvas_->reset_canvas(true);
    left_canvas_->setEnabled(mode_flag_ != START);
    left_canvas_->setUpdatesEnabled(true);
    right_canvas_->setEnabled(mode_flag_ == DRAW_2_CURVES);
    right_canvas_->setUpdatesEnabled(true);

    update_draw_borders();
    if (mode_flag_ == DRAW_2_CURVES)
    {
        left_canvas_->canvas_delegate_->set_image_mode(false);
        right_canvas_->canvas_delegate_->set_image_mode(false);
        triangulation_coloring_ = T_SQUARES;
    }
    reset_status_bar_message();

    return;
}

void Window::reset_status_bar_message()
{
    enable_canvases_updates(false);
    QString message;
    switch(mode_flag_)
    {
    case START:
        message = QString("Ready");
        break;

    case DRAW_GRAPH:
        message = QString("Click on left canvas to add a vertex");
        break;

    case DRAW_CURVE:
        message = QString("Click on left canvas to add a node");
        break;

    case DRAW_2_CURVES:
        message = QString("Click on left/right canvas to add a node");
        break;

    default:
        std::cout << "ERROR in Window::reset_status_bar: flag problem" << std::endl;
        throw(QString("ERROR in Window::reset_status_bar: flag problem"));
    }

    status_bar_label_->setText(message);
    enable_canvases_updates(mode_flag_ != START);
    status_bar_->clearMessage();
    return;
}

void Window::reset_selected_stuff()
{
    //std::cout << "Entering Window::reset_selected_stuff" << std::endl;
    left_canvas_->reset_selected_stuff();
    right_canvas_->reset_selected_stuff();
    return;
}

void Window::reset_flags()
{
    //std::cout << "Entering Window::reset_flags" << std::endl;

    graph_choice_flag_ = CUSTOM_GRAPH;
    draw_graph_flag_ = DRAW_GRAPH_VERTICES;

    curve_choice_flag_ = CUSTOM;
    curve_choice_flag_2_ = CUSTOM;
    draw_curve_flag_ = DRAW_CURVE_NODES;
    draw_curve_flag_2_ = DRAW_CURVE_NODES;
    draw_2_curves_flag_ = DRAWING_CURVES;

    //print_flags();
    return;
}

void Window::choose_graph(int graph_choice)
{
    enable_canvases_updates(false);
    reset_selected_stuff();

    left_canvas_->reset_canvas(true);
    right_canvas_->reset_canvas(true);
    right_canvas_->setEnabled(false);
    output_menu_->compute_button_->setEnabled(false);

    QString message;

    if (graph_choice != graph_choice_flag_)
    {
        input_menu_->empty_draw_graph_submenu();
        graph_choice_flag_ = graph_choice;
        input_menu_->choose_graph_combobox_->setCurrentIndex(graph_choice);
        input_menu_->create_draw_graph_submenu();
    }

    if (graph_choice == CUSTOM_GRAPH)
    {
        draw_graph_flag_ = DRAW_GRAPH_VERTICES;
        input_menu_->set_draw_vertices_with_edges_forbidden();
        left_canvas_->canvas_delegate_->set_points_radius(0.06);
        message = QString(tr("Click on left canvas to add a vertex"));

        status_bar_->clearMessage();
        status_bar_label_->setText(message);
        left_canvas_->update();
    }
    else if  (graph_choice == GRID)
    {
        disconnect(input_menu_->graph_parameter_spinbox_, SIGNAL(valueChanged(int)), this, SLOT(set_graph_parameter(int)));
        input_menu_->graph_parameter_spinbox_->setValue(10);
        connect(input_menu_->graph_parameter_spinbox_, SIGNAL(valueChanged(int)), this, SLOT(set_graph_parameter(int)));
        set_graph_parameter(0);
    }
    else if  (graph_choice == WEB)
    {
        disconnect(input_menu_->graph_parameter_spinbox_, SIGNAL(valueChanged(int)), this, SLOT(set_graph_parameter(int)));
        input_menu_->graph_parameter_spinbox_->setValue(10);
        connect(input_menu_->graph_parameter_spinbox_, SIGNAL(valueChanged(int)), this, SLOT(set_graph_parameter(int)));

        disconnect(input_menu_->graph_parameter_spinbox_2_, SIGNAL(valueChanged(int)), this, SLOT(set_graph_parameter_2(int)));
        input_menu_->graph_parameter_spinbox_2_->setValue(10);
        connect(input_menu_->graph_parameter_spinbox_2_, SIGNAL(valueChanged(int)), this, SLOT(set_graph_parameter_2(int)));
        set_graph_parameter_2(0);
    }
    else
    {
        std::cout << "ERROR in Window::choose_graph: flag problem" << std::endl;
        throw(QString("ERROR in Window::choose_graph: flag problem"));
    }

    enable_canvases_updates(true);
    update();

    return;
}


void Window::draw_vertices()
{
    reset_selected_stuff();
    left_canvas_->canvas_delegate_->set_vertex_highlighted(false, 0);
    left_canvas_->canvas_delegate_->set_vertex_selected(false, 0);
    right_canvas_->canvas_delegate_->set_vertex_highlighted(false, 0);

    draw_graph_flag_ = DRAW_GRAPH_VERTICES;

    status_bar_label_->setText(tr("Click on left canvas to add a vertex"));

    left_canvas_->update();

    if (maths_contents_.input_planar_graph_.nb_vertices()>1)
    {
        input_menu_->set_draw_vertices_with_edges_allowed();
    }
    else
    {
        input_menu_->set_draw_vertices_with_edges_forbidden();
    }

    return;
}

void Window::draw_edges()
{
    reset_selected_stuff();
    left_canvas_->canvas_delegate_->set_vertex_highlighted(false, 0);
    left_canvas_->canvas_delegate_->set_vertex_selected(false, 0);
    right_canvas_->canvas_delegate_->set_vertex_highlighted(false, 0);

    draw_graph_flag_ = DRAW_GRAPH_EDGES;

    input_menu_->set_draw_edges();


    status_bar_label_->setText(tr("Click on vertices to add edges"));
    return;
}

void Window::complete_graph()
{
    enable_canvases_updates(false);
    reset_selected_stuff();
    left_canvas_->canvas_delegate_->set_vertex_highlighted(false, 0);
    left_canvas_->canvas_delegate_->set_vertex_selected(false, 0);
    right_canvas_->canvas_delegate_->set_vertex_highlighted(false, 0);

    //input_menu_->completing();
    left_canvas_->setEnabled(false);

    disconnect(input_menu_->complete_, SIGNAL(clicked()), this, SLOT(complete_graph()));
    connect(input_menu_->complete_, SIGNAL(clicked()),this, SLOT(stop_completing()));
    connect(&maths_contents_.graph_completer_, SIGNAL(finished()), this, SLOT(end_of_completion()));

    QString str = QString("Completing graph...");
    status_bar_->showMessage(str);
    maths_contents_.complete_input_planar_graph();

    return;
}

void Window::stop_completing()
{
    maths_contents_.graph_completer_.set_stopped();
}

void Window::end_of_completion()
{
    input_menu_->complete_->setText("Complete");

    if (maths_contents_.graph_completer_.get_stopped())
    {
        QString str = QString("Completion stopped by user");
        status_bar_->showMessage(str, 2000);
        maths_contents_.input_planar_graph_ = maths_contents_.graph_completer_.get_graph_save();
        maths_contents_.input_algo_graph_1_.reset_graph();
        input_menu_->reset_->setEnabled(true);

        input_menu_->set_draw_vertices_with_edges_allowed();
        draw_graph_flag_ = DRAW_GRAPH_VERTICES;
    }
    else
    {
        draw_graph_flag_ = DRAW_GRAPH_READY;

        input_menu_->set_draw_neither_vertices_nor_edges();

        input_menu_->complete_->setEnabled(false);

        output_menu_->compute_button_->setEnabled(true);

        QString str = QString("Graph completed (%2 vertices, %3 edges). Click on the %4Compute%5 button to proceed.")
                .arg(maths_contents_.input_planar_graph_.nb_vertices())
                .arg(maths_contents_.input_planar_graph_.nb_edges())
                .arg(QChar(0x0022))
                .arg(QChar(0x0022));
        status_bar_->clearMessage();
        status_bar_label_->setText(str);
    }
    disconnect(input_menu_->complete_, SIGNAL(clicked()), this, SLOT(stop_completing()));
    connect(input_menu_->complete_, SIGNAL(clicked()),this, SLOT(complete_graph()));
    input_menu_->reset_->setEnabled(true);
    left_canvas_->setEnabled(true);

    enable_canvases_updates(true);
    left_canvas_->update();
}

void Window::set_graph_parameter(int)
{
    //std::cout << "Entering Window::set_graph_parameter" << std::endl;
    right_canvas_->reset_canvas(true);

    int parameter_1, parameter_2 = 0;
    if (graph_choice_flag_ == GRID)
    {
        parameter_1 = input_menu_->graph_parameter_spinbox_->value();
    }
    else if (graph_choice_flag_ == WEB)
    {
        parameter_1 = input_menu_->graph_parameter_spinbox_->value();
        parameter_2 = input_menu_->graph_parameter_spinbox_2_->value();
    }
    else
    {
        std::cout << "ERROR in Window::choose_sample_graph: flag problem" << std::endl;
        throw(QString("ERROR in Window::choose_sample_graph: flag problem"));
    }

    maths_contents_.graph_sampler_.create_sample_graph(graph_choice_flag_, parameter_1, parameter_2);
    int nb_vertices = maths_contents_.input_planar_graph_.nb_vertices();
    double radius = 0.6/sqrt(1.0*nb_vertices);
    left_canvas_->canvas_delegate_->set_points_radius(radius);

    draw_graph_flag_ = DRAW_GRAPH_READY;
    output_menu_->compute_button_->setEnabled(true);

    QString message = QString("Graph ready (%2 vertices, %3 edges). Click on the %4Compute%5 button to proceed.")
            .arg(maths_contents_.input_planar_graph_.nb_vertices())
            .arg(maths_contents_.input_planar_graph_.nb_edges())
            .arg(QChar(0x0022))
            .arg(QChar(0x0022));

    status_bar_->clearMessage();
    status_bar_label_->setText(message);
    left_canvas_->update();
    return;
}

void Window::set_graph_parameter_2(int)
{
    //std::cout << "Entering Window::set_graph_parameter_2" << std::endl;
    right_canvas_->reset_canvas(true);

    int parameter_1, parameter_2;
    if (graph_choice_flag_ == WEB)
    {
        parameter_1 = input_menu_->graph_parameter_spinbox_->value();
        parameter_2 = input_menu_->graph_parameter_spinbox_2_->value();
    }
    else
    {
        std::cout << "ERROR in Window::choose_sample_graph: flag problem" << std::endl;
        throw(QString("ERROR in Window::choose_sample_graph: flag problem"));
    }

    maths_contents_.graph_sampler_.create_sample_graph(graph_choice_flag_, parameter_1, parameter_2);
    int nb_vertices = maths_contents_.input_planar_graph_.nb_vertices();
    double radius = 0.6/sqrt(1.0*nb_vertices);
    left_canvas_->canvas_delegate_->set_points_radius(radius);

    draw_graph_flag_ = DRAW_GRAPH_READY;
    output_menu_->compute_button_->setEnabled(true);

    QString message = QString("Graph ready (%2 vertices, %3 edges). Click on the %4Compute%5 button to proceed.")
            .arg(maths_contents_.input_planar_graph_.nb_vertices())
            .arg(maths_contents_.input_planar_graph_.nb_edges())
            .arg(QChar(0x0022))
            .arg(QChar(0x0022));

    status_bar_->clearMessage();
    status_bar_label_->setText(message);
    left_canvas_->update();
    return;
}



void Window::choose_curve(int curve_choice_flag)
{
    //std::cout << "begin Window::choose_curve" << std::endl;

    enable_canvases_updates(false);
    reset_selected_stuff();


    curve_choice_flag = (curve_choice_type) curve_choice_flag;
    left_canvas_->reset_canvas(false);
    if (mode_flag_ == DRAW_CURVE)
    {
        right_canvas_->reset_canvas();
        right_canvas_->setEnabled(false);
    }
    else if (mode_flag_ == DRAW_2_CURVES)
    {
        bool inverse = (output_menu_->show_live_checkbox_->isChecked());
        left_canvas_->canvas_delegate_->set_shower(inverse);
        right_canvas_->canvas_delegate_->set_shower(!inverse);
        draw_2_curves_ready_off();
    }
    output_menu_->compute_button_->setEnabled(false);


    input_menu_->empty_curve_parameters_menu();
    curve_choice_flag_ = curve_choice_flag;
    input_menu_->create_curve_parameters_menu();


    unsigned int nb_points = 100;
    if (curve_choice_flag == POLYGON || curve_choice_flag == STAR || curve_choice_flag == KOCH ||
            curve_choice_flag == EPICYCLOID)
    {
        nb_points = input_menu_->nb_points_spinbox_->value();
    }
    left_canvas_->canvas_delegate_->set_nb_sides(nb_points);
    maths_contents_.initialize_curve_1(curve_choice_flag, nb_points);

    if (curve_choice_flag == CUSTOM)
    {
        enable_canvases_updates(true);
        left_canvas_->update();
        input_menu_->smooth_curve_->setChecked(false);
        input_menu_->smooth_curve_->setEnabled(false);
        left_canvas_->canvas_delegate_->set_smooth(false);
        draw_curve_flag_ = DRAW_CURVE_NODES;
        status_bar_label_->setText(tr("Click on left canvas to add a node"));
    }
    else
    {
        if (mode_flag_ == DRAW_CURVE)
        {
            complete_curve();
        }
        else if (mode_flag_ == DRAW_2_CURVES)
        {
            complete_curve_1();
        }
        else
        {
            std::cout << "ERROR in Window::choose_curve: flag problem" << std::endl;
            throw(QString("ERROR in Window::choose_curve: flag problem"));
        }
    }

    curve_choice_flag_ = curve_choice_flag;

    input_menu_->choose_curve_->setCurrentIndex(curve_choice_flag);
    //left_canvas_->update();
    //right_canvas_->update();

    //std::cout << "end Window::choose_curve" << std::endl;
    return;
}

void Window::choose_curve_2(int curve_choice_flag_2)
{
    enable_canvases_updates(false);
    reset_selected_stuff();

    draw_2_curves_ready_off();
    curve_choice_flag_2 = (curve_choice_type) curve_choice_flag_2;
    right_canvas_->reset_canvas(false);
    output_menu_->compute_button_->setEnabled(false);

    input_menu_->empty_curve_parameters_menu_2();
    curve_choice_flag_2_ = curve_choice_flag_2;
    input_menu_->create_curve_parameters_menu_2();

    bool inverse = (output_menu_->show_live_checkbox_->isChecked());
    left_canvas_->canvas_delegate_->set_shower(inverse);
    right_canvas_->canvas_delegate_->set_shower(!inverse);

    unsigned int nb_sides_2 = 100;
    if (curve_choice_flag_2 == POLYGON || curve_choice_flag_2 == STAR || curve_choice_flag_2 == KOCH
            || curve_choice_flag_2 == EPICYCLOID)
    {
        nb_sides_2 = input_menu_->nb_points_spinbox_2_->value();
    }
    right_canvas_->canvas_delegate_->set_nb_sides(nb_sides_2);
    maths_contents_.initialize_curve_2(curve_choice_flag_2, nb_sides_2);

    if (curve_choice_flag_2 == CUSTOM)
    {
        input_menu_->smooth_curve_2_->setChecked(false);
        input_menu_->smooth_curve_2_->setEnabled(false);
        right_canvas_->canvas_delegate_->set_smooth(false);
        draw_curve_flag_2_ = DRAW_CURVE_NODES;
        status_bar_label_->setText(tr("Click on right canvas to add a node"));

        enable_canvases_updates(true);
        right_canvas_->update();
    }
    else
    {
        complete_curve_2();
    }

    curve_choice_flag_2_ = curve_choice_flag_2;

    input_menu_->choose_curve_2_->setCurrentIndex(curve_choice_flag_2);
    //left_canvas_->update();
    //right_canvas_->update();
    return;
}

void Window::smooth_curve()
{
    //std::cout << "begin smooth curve" << std::endl;

    enable_canvases_updates(false);
    reset_selected_stuff();

    unsigned int state = input_menu_->smooth_curve_->checkState();
    maths_contents_.create_default_tangent_factors_1();
    maths_contents_.curve_1_.create_custom(maths_contents_.nodes_1_, maths_contents_.tangent_factors_1_, state);

    if (state==0)
    {
        left_canvas_->canvas_delegate_->set_smooth(false);
    }
    else
    {
        left_canvas_->canvas_delegate_->set_smooth(true);
    }

    if (mode_flag_ == DRAW_CURVE)
    {
        complete_curve();
    }
    else if (mode_flag_ == DRAW_2_CURVES)
    {
        draw_2_curves_ready_off();
        complete_curve_1();
    }
    else
    {
        std::cout << "ERROR in Window::smooth_curve: flag problem" << std::endl;
        throw(QString("ERROR in Window::smooth_curve: flag problem"));
    }
    return;
}

void Window::smooth_curve_2()
{
    reset_selected_stuff();
    enable_canvases_updates(false);

    draw_2_curves_ready_off();
    unsigned int state_2 = input_menu_->smooth_curve_2_->checkState();
    maths_contents_.create_default_tangent_factors_2();
    maths_contents_.curve_2_.create_custom(maths_contents_.nodes_2_, maths_contents_.tangent_factors_2_, state_2);

    if (state_2==0)
    {
        right_canvas_->canvas_delegate_->set_smooth(false);
    }
    else
    {
        right_canvas_->canvas_delegate_->set_smooth(true);
    }
    complete_curve_2();
    return;
}

void Window::show_inverse()
{
    reset_selected_stuff();

    bool inverse = output_menu_->show_inverse_checkbox_->isChecked();
    left_canvas_->canvas_delegate_->set_shower(inverse);
    right_canvas_->canvas_delegate_->set_shower(!inverse);
    left_canvas_->update();
    right_canvas_->update();
    return;
}

void Window::set_nb_points()
{
    enable_canvases_updates(false);
    reset_selected_stuff();

    left_canvas_->reset_canvas(false);
    if (mode_flag_ == DRAW_CURVE)
    {
        right_canvas_->reset_canvas();
    }


    draw_curve_flag_ = DRAW_CURVE_READY;

    unsigned int nb_points = input_menu_->nb_points_spinbox_->value();


    maths_contents_.initialize_curve_1(curve_choice_flag_, nb_points);

    left_canvas_->canvas_delegate_->set_nb_sides(nb_points);

    if (mode_flag_ == DRAW_CURVE)
    {
        complete_curve();
    }
    else if (mode_flag_ == DRAW_2_CURVES)
    {
        complete_curve_1();
    }
    else
    {
        std::cout << "ERROR in Window::set_nb_points: flag problem";
        throw(QString("ERROR in Window::set_nb_points: flag problem"));
    }
    return;
}

void Window::set_nb_points_2()
{
    enable_canvases_updates(false);
    reset_selected_stuff();

    right_canvas_->reset_canvas(false);

    draw_curve_flag_2_ = DRAW_CURVE_READY;

    unsigned int nb_points_2 = input_menu_->nb_points_spinbox_2_->value();


    maths_contents_.initialize_curve_2(curve_choice_flag_2_, nb_points_2);

    right_canvas_->canvas_delegate_->set_nb_sides(nb_points_2);

    complete_curve_2();
    return;
}

void Window::set_tiling_size()
{
    //std::cout << "Entering Window::set_tiling_size" << std::endl;

    enable_canvases_updates(false);
    reset_selected_stuff();

    if (mode_flag_ == DRAW_2_CURVES)
    {
        draw_2_curves_ready_off();
    }

    double tiling_step = input_menu_->tiling_size_spinbox_->value()/100.0;
    graph_tiler_.reset_for_tiling(&maths_contents_.tiling_, tiling_step);
    graph_tiler_.create_tiling();

    if (mode_flag_ == DRAW_CURVE && draw_curve_flag_ == DRAW_CURVE_NODES)
    {
        left_canvas_->update();
    }
    else if (maths_contents_.curve_1_.nb_points()>2 && (draw_curve_flag_==DRAW_CURVE_INVALID
                                                        || draw_curve_flag_ == DRAW_CURVE_READY
                                                        || draw_curve_flag_ == DRAW_CURVE_DONE))
    {
        if (mode_flag_ == DRAW_CURVE)
        {
            complete_curve();
        }
        else if (mode_flag_ == DRAW_2_CURVES)
        {
            complete_curve_1();
        }
        else
        {
            std::cout << "ERROR in Window::set_tiling_size: flag problem" << std::endl;
            throw(QString("ERROR in Window::set_tiling_size: flag problem"));
        }
    }

    if (mode_flag_ == DRAW_2_CURVES)
    {
        if (draw_curve_flag_2_ == DRAW_CURVE_NODES)
        {

        }
        else if (maths_contents_.curve_2_.nb_points()>2 && (draw_curve_flag_2_==DRAW_CURVE_INVALID
                                                            || draw_curve_flag_2_ == DRAW_CURVE_READY
                                                            || draw_curve_flag_2_ == DRAW_CURVE_DONE))
        {
            complete_curve_2();

        }
    }


    if (mode_flag_ == DRAW_CURVE)
    {
        right_canvas_->reset_canvas();
    }
    if (mode_flag_ == DRAW_2_CURVES)
    {
        draw_2_curves_flag_ = DRAWING_CURVES;
    }


    enable_canvases_updates(true);
    left_canvas_->update();
    right_canvas_->update();

    //std::cout << "end resume_set_tiling_size" << std::endl;
    return;
}

void Window::complete_curve()
{
    //std::cout << "begin complete_curve" << std::endl;

    enable_canvases_updates(false);
    reset_selected_stuff();
    draw_2_curves_flag_ = DRAWING_CURVES;

    right_canvas_->setEnabled(false);

    if (maths_contents_.curve_1_.nb_points()<3)
    {
        return;
    }
    maths_contents_.input_circle_packing_1_.reset_graph();

    bool valid = maths_contents_.create_circle_packing_inside_curve();

    QString message;
    if (valid)
    {
        message = QString("Input graph ready (%2 vertices, %3 edges). Click on the %4Compute%5 button to proceed.")
                .arg(maths_contents_.input_circle_packing_1_.nb_vertices())
                .arg(maths_contents_.input_circle_packing_1_.nb_edges())
                .arg(QChar(0x0022))
                .arg(QChar(0x0022));
        output_menu_->compute_button_->setEnabled(true);
        draw_curve_flag_ = DRAW_CURVE_READY;
    }
    else
    {
        output_menu_->compute_button_->setEnabled(false);
        draw_curve_flag_ = DRAW_CURVE_INVALID;
        if (maths_contents_.input_circle_packing_1_.nb_vertices() == 0)
        {
            message = QString("Input graph is not valid for computation: it is empty!");
        }
        else if (maths_contents_.curve_1_.is_self_intersecting())
        {
            message = QString("Input graph is not valid for computation. Note: the curve is self-intersecting!");
        }
        else
        {
            message = QString(
                        "Input graph is not valid for computation: domain is either multi-connected or some vertex disconnects it.");
        }
    }

    if (maths_contents_.input_circle_packing_1_.nb_vertices() > 0)
    {
        choose_coloring();
    }

    status_bar_->clearMessage();
    status_bar_label_->setText(message);
    enable_canvases_updates(true);
    left_canvas_->update();

    // std::cout << "end complete_curve" << std::endl;
    return;
}

void Window::complete_curve_1()
{
    //std::cout << "begin complete_curve_1" << std::endl;

    enable_canvases_updates(false);
    reset_selected_stuff();
    draw_2_curves_flag_ = DRAWING_CURVES;


    if (maths_contents_.curve_1_.nb_points()<3)
    {
        return;
    }

    maths_contents_.input_circle_packing_1_.reset_graph();
    maths_contents_.triangulation_1_temp_.reset_triangulation();

    graph_triangulater_1_.reset(&maths_contents_.input_circle_packing_1_, &maths_contents_.triangulation_1_temp_,
                                &maths_contents_.input_algo_graph_1_, &maths_contents_.curve_1_,
                                &maths_contents_.input_triangulation_search_tree_1_, false,
                                &curve_1_valid_,
                                &maths_contents_.input_algo_angle_1_,
                                &maths_contents_.tiling_, input_menu_->choose_coloring_box_2_->currentIndex());

    graph_triangulater_1_.run();

    maths_contents_.triangulation_1_ = maths_contents_.triangulation_1_temp_;

    if (curve_1_valid_)
    {
        draw_curve_flag_ = DRAW_CURVE_READY;
        if (draw_curve_flag_2_ == DRAW_CURVE_READY)
        {
            output_menu_->compute_button_->setEnabled(true);
        }
    }
    else
    {
        output_menu_->compute_button_->setEnabled(false);
        draw_curve_flag_ = DRAW_CURVE_INVALID;
    }



    finish_complete_curves();



    //std::cout << "end complete_curve_1" << std::endl;
    return;
}

void Window::complete_curve_2()
{
    //std::cout << "begin complete_curve_2" << std::endl;

    enable_canvases_updates(false);
    reset_selected_stuff();
    draw_2_curves_flag_ = DRAWING_CURVES;

    if (maths_contents_.curve_2_.nb_points()<3)
    {
        return;
    }
    maths_contents_.input_circle_packing_2_.reset_graph();
    maths_contents_.triangulation_2_temp_.reset_triangulation();

    graph_triangulater_2_.reset(&maths_contents_.input_circle_packing_2_, &maths_contents_.triangulation_2_temp_,
                                &maths_contents_.input_algo_graph_2_, &maths_contents_.curve_2_,
                                &maths_contents_.input_triangulation_search_tree_2_, false,
                                &curve_2_valid_,
                                &maths_contents_.input_algo_angle_2_,
                                &maths_contents_.tiling_,  input_menu_->choose_coloring_box_2_->currentIndex());
    graph_triangulater_2_.run();

    maths_contents_.triangulation_2_ = maths_contents_.triangulation_2_temp_;

    if (curve_2_valid_)
    {
        draw_curve_flag_2_ = DRAW_CURVE_READY;
        if (draw_curve_flag_ == DRAW_CURVE_READY)
        {
            output_menu_->compute_button_->setEnabled(true);
        }
    }
    else
    {
        output_menu_->compute_button_->setEnabled(false);
        draw_curve_flag_2_ = DRAW_CURVE_INVALID;
    }


    finish_complete_curves();

    //std::cout << "end complete_curve_2_finished" << std::endl;
    return;
}

void Window::finish_complete_curves()
{
    //std::cout << "Entering Window::finish_complete_curves" << std::endl;

    QString message_left, message_right, message;
    if (curve_1_valid_)
    {
        message_left = QString("Left input graph ready (%2 vertices)")
                .arg(maths_contents_.input_circle_packing_1_.nb_vertices());
    }
    else
    {
        if (maths_contents_.input_circle_packing_1_.nb_vertices() == 0)
        {
            message_left = QString("Left input graph is not valid (it is empty)");
        }
        else if (maths_contents_.curve_1_.is_self_intersecting())
        {
            message_left = QString("Left input graph is not valid");// (note: the curve is self-intersecting!)");
        }
        else
        {
            message_left = QString(
                        "Left input graph is not valid");// (domain is either multi-connected or some vertex disconnects it)");
        }
    }

    if (curve_2_valid_)
    {
        message_right = QString("Right input graph ready (%2 vertices)")
                .arg(maths_contents_.input_circle_packing_2_.nb_vertices());
    }
    else
    {
        if (maths_contents_.input_circle_packing_2_.nb_vertices() == 0)
        {
            message_right = QString("Right input graph is not valid (it is empty)");
        }
        else if (maths_contents_.curve_2_.is_self_intersecting())
        {
            message_right = QString("Right input graph is not valid");// (note: the curve is self-intersecting!)");
        }
        else
        {
            message_right = QString(
                        "Right input graph is not valid");// (domain is either multi-connected or some vertex disconnects it)");
        }
    }

    message = message_left;
    message.append(". ");
    message.append(message_right);
    status_bar_->clearMessage();
    status_bar_label_->setText(message);

    update_shower_bools();
    enable_canvases_updates(true);
    left_canvas_->update();
    right_canvas_->update();
    return;
}

void Window::set_canvas_delegates_show_live()
{
    switch(mode_flag_)
    {
    case START:
        std::cout << "ERROR in Window::set_canvas_delegates_show_live: flag problem" << std::endl;
        throw(QString("ERROR in Window::set_canvas_delegates_show_live: flag problem"));
        break;
    case DRAW_GRAPH:
        delete right_canvas_->canvas_delegate_;
        maths_contents_.show_live_cp_right_.reset_graph();
        right_canvas_->canvas_delegate_ = new Canvas_Delegate_Show_Live(right_canvas_->size_in_pixels_,
                                                                        &(maths_contents_.show_live_cp_right_));
        break;
    case DRAW_CURVE:
        delete left_canvas_->canvas_delegate_;
        maths_contents_.show_live_cp_left_.reset_graph();
        left_canvas_->canvas_delegate_ = new Canvas_Delegate_Show_Live(left_canvas_->size_in_pixels_,
                                                                       &(maths_contents_.show_live_cp_left_));
        delete right_canvas_->canvas_delegate_;
        maths_contents_.show_live_cp_right_.reset_graph();
        right_canvas_->canvas_delegate_ = new Canvas_Delegate_Show_Live(right_canvas_->size_in_pixels_,
                                                                        &(maths_contents_.show_live_cp_right_));
        break;
    default:
        std::cout << "ERROR in Window::set_canvas_delegates_show_live: flag problem" << std::endl;
        throw(QString("ERROR in Window::set_canvas_delegates_show_live: flag problem"));
    }

    update_draw_borders();

    left_canvas_->set_left(true);
    right_canvas_->set_left(false);
    return;
}

void Window::enable_canvases_updates(bool b)
{
    left_canvas_->setUpdatesEnabled(b);
    right_canvas_->setUpdatesEnabled(b);
    return;
}

void Window::update_shower_bools()
{
    bool inverse = output_menu_->show_inverse_checkbox_->isChecked();
    left_canvas_->canvas_delegate_->set_shower(inverse);
    right_canvas_->canvas_delegate_->set_shower(!inverse);
}

void Window::compute_circle_packing()
{
    //std::cout << "Entering Window::compute_circle_packing" << std::endl;



    if (mode_flag_ == DRAW_GRAPH || mode_flag_ == DRAW_CURVE)
    {
        reset_selected_stuff();
        right_canvas_->reset_canvas();
    }
    else if (mode_flag_ == DRAW_2_CURVES)
    {
        left_canvas_->canvas_delegate_->reset_canvas_delegate_for_computing();
        right_canvas_->canvas_delegate_->reset_canvas_delegate_for_computing();
        draw_2_curves_ready_off();
    }

    left_canvas_->setEnabled(false);
    right_canvas_->setEnabled(false);

    input_menu_->setEnabled(false);
    output_menu_->disable_all_but_stop();

    output_menu_->compute_button_->setText(tr("Stop"));

    if (output_menu_->show_live_checkbox_->checkState() != 0 && mode_flag_ != DRAW_2_CURVES)
    {
        set_canvas_delegates_show_live();
    }
    else
    {
        enable_canvases_updates(false);
    }
    disconnect(output_menu_->compute_button_, SIGNAL(clicked()),this, SLOT(compute_circle_packing()));
    connect(output_menu_->compute_button_, SIGNAL(clicked()),this, SLOT(stop_computation()));
    connect(&(maths_contents_.top_algo_), SIGNAL(end()), this, SLOT(end_of_computation()));


    maths_contents_.top_algo_.reset_algo(this);
    maths_contents_.top_algo_.launch();


    //std::cout << "Leaving Window::compute_circle_packing" << std::endl;
    return;
}


void Window::stop_computation()
{
    output_menu_->compute_button_->setEnabled(false);
    maths_contents_.top_algo_.stop();
    return;
}

void Window::end_of_computation()
{
    enable_canvases_updates(true);
    output_menu_->compute_button_->setEnabled(true);
    if (mode_flag_ == DRAW_GRAPH)
    {
        draw_graph_flag_ = DRAW_GRAPH_DONE;
    }
    else if(mode_flag_ == DRAW_CURVE)
    {
        draw_curve_flag_= DRAW_CURVE_DONE;
    }
    else if (mode_flag_ == DRAW_2_CURVES)
    {
        draw_2_curves_flag_= DRAW_2_CURVES_DONE;
    }

    disconnect(output_menu_->compute_button_, SIGNAL(clicked()),this, SLOT(stop_computation()));
    if (output_menu_->show_live_checkbox_->checkState() != 0)
    {
        if (mode_flag_ == DRAW_GRAPH)
        {
            set_canvas_delegates(false, true);
            connect(output_menu_->compute_button_, SIGNAL(clicked()),this, SLOT(compute_circle_packing()));
            output_menu_->compute_button_->setText(tr("Compute"));
            left_canvas_->setEnabled(true);
            right_canvas_->setEnabled(true);
            input_menu_->setEnabled(true);
            output_menu_->enable_all();
        }
        else if (mode_flag_ == DRAW_CURVE)
        {
            output_menu_->compute_button_->setText(tr("OK"));
            connect(output_menu_->compute_button_, SIGNAL(clicked()),this, SLOT(show_final_cp()));
        }
        else
        {
            std::cout << "ERROR in Window::end_of_computation: never supposed to reach here" << std::endl;
            throw(QString("ERROR in Window::end_of_computation: never supposed to reach here"));
        }
    }
    else
    {
        connect(output_menu_->compute_button_, SIGNAL(clicked()),this, SLOT(compute_circle_packing()));
        output_menu_->compute_button_->setText(tr("Compute"));
        left_canvas_->setEnabled(true);
        right_canvas_->setEnabled(true);
        input_menu_->setEnabled(true);
        output_menu_->enable_all();
    }

    left_canvas_->canvas_delegate_->set_vertex_highlighted(false, 0);
    right_canvas_->canvas_delegate_->set_vertex_highlighted(false, 0);

    enable_canvases_updates(true);
    left_canvas_->update();
    right_canvas_->update();

    QString message;
    if (mode_flag_ == DRAW_GRAPH || mode_flag_ == DRAW_CURVE)
    {
        if (output_menu_->show_live_checkbox_->isChecked())
        {
            message = QString(tr("Click on %1OK%2 button to proceed"))
                    .arg(QChar(0x0022))
                    .arg(QChar(0x0022));
        }
        else
        {
            message = QString(tr("Click and drag on right canvas to move the circle packing around"));
        }
    }
    else if (mode_flag_ == DRAW_2_CURVES)
    {
        message = QString(tr("Click and drag on the output triangulation to move it around"));
    }
    status_bar_label_->setText(message);
}

void Window::show_final_cp()
{
    reset_selected_stuff();

    input_menu_->setEnabled(true);
    output_menu_->enable_all();
    set_canvas_delegates(false);
    left_canvas_->canvas_delegate_->set_smooth(input_menu_->smooth_curve_->isChecked());
    left_canvas_->setEnabled(true);
    right_canvas_->setEnabled(true);
    left_canvas_->update();
    right_canvas_->update();
    disconnect(output_menu_->compute_button_, SIGNAL(clicked()),this, SLOT(show_final_cp()));
    connect(output_menu_->compute_button_, SIGNAL(clicked()),this, SLOT(compute_circle_packing()));
    output_menu_->compute_button_->setText(tr("Compute"));

    QString message = QString(tr("Click and drag on right canvas to move the circle packing around"));
    status_bar_label_->setText(message);
    return;
}

void Window::choose_coloring()
{
    enable_canvases_updates(false);
    reset_selected_stuff();

    coloring_type coloring = input_menu_->choose_coloring_box_->currentIndex();
    maths_contents_.input_circle_packing_1_.color_circle_packing(coloring);
    if (mode_flag_ == DRAW_CURVE && (draw_curve_flag_ == DRAW_CURVE_READY || draw_curve_flag_ == DRAW_CURVE_DONE))
    {
        maths_contents_.synchronise_input_colors(mode_flag_);
    }
    if (mode_flag_ == DRAW_CURVE && draw_curve_flag_ == DRAW_CURVE_DONE)
    {
        maths_contents_.synchronise_colors(mode_flag_);
    };

    update_draw_borders();

    enable_canvases_updates(true);
    left_canvas_->update();
    right_canvas_->update();
    return;
}

void Window::update_draw_borders()
{
    bool draw_borders = ((mode_flag_ == DRAW_CURVE && input_menu_->choose_coloring_box_->currentIndex() == 0) ||
                         (mode_flag_ == DRAW_2_CURVES && input_menu_->choose_coloring_box_2_->currentIndex() == 0));
    left_canvas_->canvas_delegate_->draw_borders_ = draw_borders;
    right_canvas_->canvas_delegate_->draw_borders_ = draw_borders;
    return;
}

bool Window::load_image()
{
    QImage new_image;
    QString new_image_filename = QFileDialog::getOpenFileName(this, tr("Set image..."),
                                                              (image_loaded_? image_loaded_filename_.toStdString().c_str() : ""),
                                                              tr("JPG files (*.jpg);; PNG files (*.png);;All Files (*)"));
    if(new_image_filename.isEmpty())
    {
        QMessageBox message_box_1;
        message_box_1.setWindowTitle("Set image...");
        message_box_1.setIcon(QMessageBox::Warning);
        message_box_1.setText("No file was selected.");
        message_box_1.setInformativeText("Do you want to retry?");
        message_box_1.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
        message_box_1.setDefaultButton(QMessageBox::Yes);
        int retour = message_box_1.exec();

        switch(retour)
        {
        case QMessageBox::Yes :
            return load_image();
            break;

        case QMessageBox::No :
            return false;
            break;

        default:
            std::cout << "ERROR in Window::load_image: should never have reached here" << std::endl;
            throw(QString("ERROR in Window::load_image: should never have reached here"));
        }
    }
    else
    {
        if (new_image.load(new_image_filename))
        {
            image_loaded_ = true;
            image_loaded_filename_ = new_image_filename;
            maths_contents_.image_ = new_image;
            left_canvas_->canvas_delegate_->set_image_parameters();
            right_canvas_->canvas_delegate_->set_image_parameters();
            return true;
        }
        else
        {
            QMessageBox message_box_2;
            message_box_2.setIcon(QMessageBox::Warning);
            message_box_2.setWindowTitle("Set image...");
            message_box_2.setText("Could not load image file.");
            message_box_2.setInformativeText("Do you want to retry?");
            message_box_2.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
            message_box_2.setDefaultButton(QMessageBox::Yes);
            int retour = message_box_2.exec();

            switch(retour)
            {
            case QMessageBox::Yes :
                return load_image();
                break;

            case QMessageBox::No :
                return false;
                break;

            default:
                std::cout << "ERROR in Window::load_image: should never have reached here" << std::endl;
                throw(QString("ERROR in Window::load_image: should never have reached here"));
            }
        }
    }

    std::cout << "ERROR in Window::load_image: should never have reached here" << std::endl;
    throw(QString("ERROR in Window::load_image: should never have reached here"));
    return false;
}

void Window::choose_coloring_2()
{
    enable_canvases_updates(false);
    reset_selected_stuff();

    Triangulation_coloring coloring = input_menu_->choose_coloring_box_2_->currentIndex();

    if (coloring == T_IMAGE)
    {
        if(image_loaded_)
        {
            left_canvas_->canvas_delegate_->set_image_mode(true);
            right_canvas_->canvas_delegate_->set_image_mode(true);
            triangulation_coloring_ = T_IMAGE;
        }
        else
        {
            QMessageBox message_box(this);
            message_box.setWindowTitle("Set image...");
            message_box.setIcon(QMessageBox::Question);
            QString message = QString("No image has been set yet (this can be done in the %1Settings%2 menu).")
                    .arg(QChar(0x0022))
                    .arg(QChar(0x0022));
            message_box.setText(message);
            message_box.setInformativeText("Do you want to set an image?");
            message_box.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
            message_box.setDefaultButton(QMessageBox::Yes);
            int retour = message_box.exec();

            switch(retour)
            {
            case QMessageBox::Yes :
                if (load_image())
                {
                    left_canvas_->canvas_delegate_->set_image_mode(true);
                    right_canvas_->canvas_delegate_->set_image_mode(true);
                    triangulation_coloring_ = T_IMAGE;
                }
                else
                {
                    input_menu_->choose_coloring_box_2_->setCurrentIndex((int) triangulation_coloring_);
                }
                break;

            case QMessageBox::No :
                input_menu_->choose_coloring_box_2_->setCurrentIndex((int) triangulation_coloring_);
                choose_coloring_2();
                break;

            default:
                std::cout << "ERROR in Window::choose_coloring_2: should never have reached here" << std::endl;
                throw(QString("ERROR in Window::choose_coloring_2: should never have reached here"));
            }

        }
    }
    else
    {
        if (maths_contents_.triangulation_2_.nb_triangles() > 0)
        {
            maths_contents_.triangulation_2_.color(coloring);
        }
        if (maths_contents_.triangulation_1_.nb_triangles() > 0)
        {
            maths_contents_.triangulation_1_.color(coloring);
        }

        if (draw_2_curves_flag_ == DRAW_2_CURVES_DONE)
        {
            maths_contents_.synchronise_triangulation_colors();
        }


        triangulation_coloring_ = coloring;
        update_draw_borders();
        left_canvas_->canvas_delegate_->set_image_mode(false);
        right_canvas_->canvas_delegate_->set_image_mode(false);
    }

    enable_canvases_updates(true);
    left_canvas_->update();
    right_canvas_->update();
    return;
}

void Window::signal_received(int signal, int message_timeout)
{
    switch(signal)
    {
    case GHOST:
        break;
    case VERTEX_HIGHLIGHTED_LEFT:
        right_canvas_->set_is_vertex_highlighted(left_canvas_->get_is_vertex_highlighted());
        right_canvas_->set_index_vertex_highlighted(left_canvas_->get_index_vertex_highlighted());
        right_canvas_->update();
        break;
    case VERTEX_HIGHLIGHTED_RIGHT:
        left_canvas_->set_is_vertex_highlighted(right_canvas_->get_is_vertex_highlighted());
        left_canvas_->set_index_vertex_highlighted(right_canvas_->get_index_vertex_highlighted());
        left_canvas_->update();
        break;
    case DRAW_EDGES_ON:
        input_menu_->draw_edges_->setCheckable(true);
        input_menu_->draw_edges_->setEnabled(true);
        break;
    case DRAW_EDGES_OFF:
        input_menu_->draw_edges_->setCheckable(true);
        input_menu_->draw_edges_->setEnabled(false);
        break;
    case GRAPH_COMPLETABLE_ON:
        input_menu_->complete_->setEnabled(true);
        break;
    case GRAPH_COMPLETABLE_OFF:
        input_menu_->complete_->setEnabled(false);
        break;
    case CURVE_CREATED:
        draw_curve_flag_ = DRAW_CURVE_READY;
        left_canvas_->update();
        break;
    case CURVE_READY_ON:
        if (mode_flag_ == DRAW_CURVE)
        {
            right_canvas_->reset_canvas();
            complete_curve();
        }
        else if (mode_flag_ == DRAW_2_CURVES)
        {
            draw_2_curves_ready_off();
            complete_curve_1();
        }
        input_menu_->smooth_curve_label_->setEnabled(true);
        input_menu_->smooth_curve_->setEnabled(true);
        left_canvas_->update();
        break;
    case CURVE_READY_OFF:
        if (mode_flag_ == DRAW_CURVE)
        {
            right_canvas_->reset_canvas();
        }
        else if (mode_flag_ == DRAW_2_CURVES)
        {
            draw_2_curves_ready_off();
        }
        maths_contents_.input_circle_packing_1_.reset_graph();
        maths_contents_.triangulation_1_.reset_triangulation();
        draw_curve_flag_ = DRAW_CURVE_READY;
        break;
    case CURVE_CREATED_2:
        draw_2_curves_ready_off();
        draw_curve_flag_2_ = DRAW_CURVE_READY;
        right_canvas_->update();
        break;
    case CURVE_READY_ON_2:
        draw_2_curves_ready_off();
        complete_curve_2();
        input_menu_->smooth_curve_label_2_->setEnabled(true);
        input_menu_->smooth_curve_2_->setEnabled(true);
        right_canvas_->update();
        break;
    case CURVE_READY_OFF_2:
        draw_2_curves_ready_off();
        maths_contents_.input_circle_packing_2_.reset_graph();
        maths_contents_.triangulation_2_.reset_triangulation();
        draw_curve_flag_2_ = DRAW_CURVE_READY;
    case MESSAGE:
        if (message_timeout == -1)
        {
            status_bar_->clearMessage();
        }
        else
        {
            status_bar_->showMessage(message_received_, message_timeout);
        }
        status_bar_->repaint();
        break;
    default:
        std::cout << "ERROR in Window::signal_received: flag problem" << std::endl;
        throw(QString("ERROR in Window::signal_received: flag problem"));
    }
}

void Window::draw_2_curves_ready_off()
{
    enable_canvases_updates(false);
    draw_2_curves_flag_ = DRAWING_CURVES;
    maths_contents_.output_triangulation_1_.reset_triangulation();
    maths_contents_.output_triangulation_2_.reset_triangulation();
    Triangulation_Search_Tree empty_tree;
    maths_contents_.output_triangulation_search_tree_1_ = empty_tree;
    maths_contents_.output_triangulation_search_tree_2_ = empty_tree;
    enable_canvases_updates(true);
    left_canvas_->update();
    right_canvas_->update();
    return;
}

void Window::launch_tests()
{
    reset_selected_stuff();

    bool left = left_canvas_->isEnabled();
    bool right = right_canvas_->isEnabled();

    if (left)
    {
        left_canvas_->setEnabled(false);
    }
    if (right)
    {
        right_canvas_->setEnabled(false);
    }

    /*Tests T(8, 0.01);
    T.launch_delta_tests(0.050, 0.080, 0.010);
    right_canvas_->canvas_delegate_ = new Canvas_Delegate_Tests(
                right_canvas_->size_in_pixels_, T.get_deltas(), T.get_delta_overall_scores());
    right_canvas_->update();
    right_canvas_->print_in_file("testsTS8-E5-zoom1.png");*/

    Tests T(4, 0.001);
    T.launch_mix_tests(0, 2, 0.005);
    delete right_canvas_->canvas_delegate_;
    right_canvas_->canvas_delegate_ = new Canvas_Delegate_Tests(
                right_canvas_->size_in_pixels_, T.get_alphas(), T.get_alpha_overall_scores());
    right_canvas_->update();
    right_canvas_->print_in_file("tests.png");


    if (left)
    {
        left_canvas_->setEnabled(true);
    }
    if (right)
    {
        right_canvas_->setEnabled(true);
    }
    return;
}

void Window::print_flags()
{
    if (mode_flag_ == START)
    {
        std::cout << "I am in START mode" << std::endl;
    }
    else if (mode_flag_ == DRAW_GRAPH)
    {
        std::cout << "I am in DRAW_GRAPH mode with graph choice ";
        if (graph_choice_flag_ == CUSTOM_GRAPH)
        {
            std::cout << "CUSTOM_GRAPH and subflag ";

            if (draw_graph_flag_ == DRAW_GRAPH_VERTICES)
            {
                std::cout << "DRAW_GRAPH_VERTICES" << std::endl;
            }
            else if (draw_graph_flag_ == DRAW_GRAPH_EDGES)
            {
                std::cout << "DRAW_GRAPH_EDGES" << std::endl;
            }
            else if (draw_graph_flag_ == DRAW_GRAPH_READY)
            {
                std::cout << "DRAW_GRAPH_READY" << std::endl;
            }
            else if (draw_graph_flag_ == DRAW_GRAPH_DONE)
            {
                std::cout << "DRAW_GRAPH_DONE" << std::endl;
            }
            else
            {
                std::cout << "UNDEFINED!" << std::endl;
            }
        }
        else if (graph_choice_flag_ == GRID)
        {
            std::cout << "GRID" << std::endl;
        }
        else
        {
            std::cout << "UNDEFINED!" << std::endl;
        }
    }
    else if (mode_flag_ == DRAW_CURVE)
    {
        std::cout << "I am in DRAW_CURVE mode with subflag ";
        if (draw_curve_flag_ == DRAW_CURVE_NODES)
        {
            std::cout << "DRAW_CURVE_NODES" << std::endl;
        }
        else if (draw_curve_flag_ == DRAW_CURVE_INVALID)
        {
            std::cout << "DRAW_CURVE_INVALID" << std::endl;
        }
        else if (draw_curve_flag_ == DRAW_CURVE_READY)
        {
            std::cout << "DRAW_CURVE_READY" << std::endl;
        }
        else if (draw_curve_flag_ == DRAW_CURVE_DONE)
        {
            std::cout << "DRAW_CURVE_DONE" << std::endl;
        }
        else
        {
            std::cout << "UNDEFINED!" << std::endl;
        }
    }
    else if (mode_flag_ == DRAW_2_CURVES)
    {
        std::cout << "I am in DRAW_2_CURVES mode with subflag ";
        if (draw_2_curves_flag_ == DRAWING_CURVES)
        {
            std::cout << "DRAWING_CURVES";
        }
        else if (draw_2_curves_flag_ == DRAW_2_CURVES_DONE)
        {
            std::cout << "DRAW_2_CURVES_DONE";
        }
        else
        {
            std::cout << "UNDEFINED!" << std::endl;
        }
        std::cout << ", curve 1 subflag ";
        if (draw_curve_flag_ == DRAW_CURVE_NODES)
        {
            std::cout << "DRAW_CURVE_NODES";
        }
        else if (draw_curve_flag_ == DRAW_CURVE_INVALID)
        {
            std::cout << "DRAW_CURVE_INVALID";
        }
        else if (draw_curve_flag_ == DRAW_CURVE_READY)
        {
            std::cout << "DRAW_CURVE_READY";
        }
        else if (draw_curve_flag_ == DRAW_CURVE_DONE)
        {
            std::cout << "DRAW_CURVE_DONE";
        }
        else
        {
            std::cout << "UNDEFINED!";
        }
        std::cout << ", curve 2 subflag ";
        if (draw_curve_flag_2_ == DRAW_CURVE_NODES)
        {
            std::cout << "DRAW_CURVE_NODES" << std::endl;
        }
        else if (draw_curve_flag_2_ == DRAW_CURVE_INVALID)
        {
            std::cout << "DRAW_CURVE_INVALID" << std::endl;
        }
        else if (draw_curve_flag_2_ == DRAW_CURVE_READY)
        {
            std::cout << "DRAW_CURVE_READY" << std::endl;
        }
        else if (draw_curve_flag_2_ == DRAW_CURVE_DONE)
        {
            std::cout << "DRAW_CURVE_DONE" << std::endl;
        }
        else
        {
            std::cout << "UNDEFINED!" << std::endl;
        }
    }
    else
    {
        std::cout << "I am in UNDEFINED mode!" << std::endl;
    }
}
