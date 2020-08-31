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

#include "input_menu.hpp"

#include <QGridLayout>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QCheckBox>
#include <QSpinBox>
#include <QStatusBar>
#include <QRadioButton>

#include "window.hpp"

Input_Menu::Input_Menu(Window* window)
{
    setParent(window);
    window_ = window;

    setTitle(tr("Input"));

    layout_ = new QGridLayout(this);
    layout_->setSpacing(0);
    layout_->setAlignment(Qt::AlignTop);

    reset_ = new QPushButton(tr("Reset"));
    choose_mode_ = new QComboBox();

    vert_space_ = 10;
    button_height_ = std::min(choose_mode_->sizeHint().height(),reset_->sizeHint().height());


    create_choose_mode_combobox(); // does not include the "new" (above)
    create_reset_button(); // does not include the "new" (above)

    layout_->addWidget(choose_mode_,1,0,1,2);
    layout_->addWidget(reset_,3,0,1,2);

    create_choose_graph_combobox();
    create_draw_vertices_and_edges_radioboxes();
    create_complete_button();
    create_graph_paramaters_items();
    create_graph_paramaters_items_2();

    create_choose_curve_combobox();
    create_choose_curve_2_combobox();
    create_tiling_items();
    create_smooth_items();
    create_smooth_2_items();
    create_choose_nb_points_items();
    create_choose_nb_points_2_items();
    create_choose_coloring_label();
    create_choose_coloring_combobox();
    create_choose_coloring_2_combobox();

    set_row_heights();
    setLayout(layout_);
}


// Functions for constructor

void Input_Menu::set_row_heights()
{
    layout_->setRowMinimumHeight(0, vert_space_);
    layout_->setRowMinimumHeight(1, button_height_);
    layout_->setRowMinimumHeight(2, vert_space_);
    layout_->setRowMinimumHeight(3, button_height_);
    layout_->setRowMinimumHeight(4, vert_space_);
    layout_->setRowMinimumHeight(5, button_height_);
    layout_->setRowMinimumHeight(6, vert_space_);
    layout_->setRowMinimumHeight(7, button_height_);
    layout_->setRowMinimumHeight(8, vert_space_);
    layout_->setRowMinimumHeight(9, button_height_);
    layout_->setRowMinimumHeight(10, vert_space_);
    layout_->setRowMinimumHeight(11, button_height_);
    layout_->setRowMinimumHeight(12, 3*vert_space_);
    layout_->setRowMinimumHeight(13, vert_space_);
    layout_->setRowMinimumHeight(14, 3*vert_space_);
    layout_->setRowMinimumHeight(15, button_height_);
    layout_->setRowMinimumHeight(16, 3*vert_space_);
    return;
}

void Input_Menu::create_choose_mode_combobox()
{
    choose_mode_->setFixedHeight(button_height_);

    choose_mode_->addItem(tr("Choose Mode"), START);
    choose_mode_->addItem(tr("Draw Graph"), DRAW_GRAPH);
    choose_mode_->addItem(tr("Draw Curve"), DRAW_CURVE);
    choose_mode_->addItem(tr("Draw Two Curves"), DRAW_2_CURVES);
    choose_mode_->setCurrentIndex(START);

    connect(choose_mode_, SIGNAL(activated(int)), window_, SLOT(change_mode(int)));

    return;
}

void Input_Menu::create_reset_button()
{
    reset_->setFixedHeight(button_height_);
    connect(reset_, SIGNAL(clicked()), window_, SLOT(reset_window()));
    return;
}

void Input_Menu::create_choose_graph_combobox()
{
    choose_graph_combobox_ = new QComboBox;
    choose_graph_combobox_->setFixedHeight( button_height_);
    choose_graph_combobox_->addItem(tr("Custom"), CUSTOM_GRAPH);
    choose_graph_combobox_->addItem(tr("Grid"), GRID);
    choose_graph_combobox_->addItem(tr("Web"), WEB);
    connect(choose_graph_combobox_, SIGNAL(activated(int)), window_, SLOT(choose_graph(int)));
    return;
}

void Input_Menu::create_draw_vertices_and_edges_radioboxes()
{
    draw_vertices_ = new QRadioButton(tr("Add vertices"));
    draw_vertices_->setAutoExclusive(false);
    draw_vertices_->setCheckable(true);
    draw_vertices_->setFixedHeight( button_height_);
    connect(draw_vertices_, SIGNAL(clicked()), window_, SLOT(draw_vertices()));

    draw_edges_ = new QRadioButton(tr("Add edges"));
    draw_edges_->setAutoExclusive(false);
    draw_edges_->setCheckable(true);
    draw_edges_->setFixedHeight( button_height_);
    connect(draw_edges_, SIGNAL(clicked()), window_, SLOT(draw_edges()));
    return;
}

void Input_Menu::create_complete_button()
{
    complete_ = new QPushButton(tr("Complete"));
    complete_->setFixedHeight( button_height_);
    connect(complete_, SIGNAL(clicked()), window_, SLOT(complete_graph()));
}

void Input_Menu::create_graph_paramaters_items()
{
    graph_parameter_spinbox_ = new QSpinBox;
    graph_parameter_spinbox_->setFixedHeight( button_height_);
    graph_parameter_label_ = new QLabel;
    graph_parameter_label_->setFixedHeight( button_height_);
    connect(graph_parameter_spinbox_, SIGNAL(valueChanged(int)), window_, SLOT(set_graph_parameter(int)));
    return;
}

void Input_Menu::create_graph_paramaters_items_2()
{
    graph_parameter_spinbox_2_ = new QSpinBox;
    graph_parameter_spinbox_2_->setFixedHeight( button_height_);
    graph_parameter_label_2_ = new QLabel;
    graph_parameter_label_2_->setFixedHeight( button_height_);
    connect(graph_parameter_spinbox_2_, SIGNAL(valueChanged(int)), window_, SLOT(set_graph_parameter_2(int)));
    return;
}

void Input_Menu::create_choose_curve_combobox()
{
    choose_curve_ = new QComboBox;
    choose_curve_->setFixedHeight( button_height_);
    choose_curve_->addItem(tr("Custom"), CUSTOM);
    choose_curve_->addItem(tr("Rectangle"), RECTANGLE);
    choose_curve_->addItem(tr("Polygon"), POLYGON);
    choose_curve_->addItem(tr("Star"), STAR);
    choose_curve_->addItem(tr("Ellipse"), ELLIPSE);
    choose_curve_->addItem(tr("Epicycloid"), EPICYCLOID);
    choose_curve_->addItem(tr("Koch Curve"), KOCH);
    choose_curve_->setToolTip("Choose left curve");
    connect(choose_curve_, SIGNAL(activated(int)), window_, SLOT(choose_curve(int)));
    return;
}

void Input_Menu::create_choose_curve_2_combobox()
{
    choose_curve_2_ = new QComboBox;
    choose_curve_2_->setFixedHeight( button_height_);
    choose_curve_2_->addItem(tr("Custom"), CUSTOM);
    choose_curve_2_->addItem(tr("Rectangle"), RECTANGLE);
    choose_curve_2_->addItem(tr("Polygon"), POLYGON);
    choose_curve_2_->addItem(tr("Star"), STAR);
    choose_curve_2_->addItem(tr("Ellipse"), ELLIPSE);
    choose_curve_2_->addItem(tr("Epicycloid"), EPICYCLOID);
    choose_curve_2_->addItem(tr("Koch Curve"), KOCH);
    choose_curve_2_->setToolTip("Choose right curve");
    connect(choose_curve_2_, SIGNAL(activated(int)), window_, SLOT(choose_curve_2(int)));
    return;
}

void Input_Menu::create_tiling_items()
{
    tiling_size_label_ = new QLabel("Tiling size ");
    tiling_size_label_->setFixedHeight( button_height_);

    tiling_size_spinbox_ = new QSpinBox();
    tiling_size_spinbox_->setFixedHeight( button_height_);
    tiling_size_spinbox_->setRange(3, 30);
    connect(tiling_size_spinbox_, SIGNAL(valueChanged(int)), window_, SLOT(set_tiling_size()));
    return;
}

void Input_Menu::create_smooth_items()
{
    smooth_curve_label_ = new QLabel("Smooth ");
    smooth_curve_label_->setFixedHeight( button_height_);
    smooth_curve_ = new QCheckBox;
    smooth_curve_->setFixedHeight( button_height_);
    connect(smooth_curve_, SIGNAL(clicked()), window_, SLOT(smooth_curve()));
    return;
}

void Input_Menu::create_smooth_2_items()
{
    smooth_curve_label_2_ = new QLabel("Smooth ");
    smooth_curve_label_2_->setFixedHeight( button_height_);
    smooth_curve_2_ = new QCheckBox;
    smooth_curve_2_->setFixedHeight( button_height_);
    connect(smooth_curve_2_, SIGNAL(clicked()), window_, SLOT(smooth_curve_2()));

    return;
}

void Input_Menu::create_choose_nb_points_items()
{
    nb_points_label_ = new QLabel;
    nb_points_label_->setFixedHeight( button_height_);
    nb_points_spinbox_= new QSpinBox;
    nb_points_spinbox_->setFixedHeight( button_height_);
    connect(nb_points_spinbox_, SIGNAL(valueChanged(int)), window_, SLOT(set_nb_points()));
    return;
}

void Input_Menu::create_choose_nb_points_2_items()
{
    nb_points_label_2_ = new QLabel;
    nb_points_label_2_->setFixedHeight( button_height_);
    nb_points_spinbox_2_= new QSpinBox;
    nb_points_spinbox_2_->setFixedHeight( button_height_);
    connect(nb_points_spinbox_2_, SIGNAL(valueChanged(int)), window_, SLOT(set_nb_points_2()));
    return;
}

void Input_Menu::create_choose_coloring_label()
{
    choose_coloring_label_ = new QLabel("Coloring ");
    choose_coloring_label_->setFixedHeight(button_height_);
    return;
}

void Input_Menu::create_choose_coloring_combobox()
{
    choose_coloring_box_ = new QComboBox;
    choose_coloring_box_->setFixedHeight(button_height_);
    choose_coloring_box_->addItem("Plain", PLAIN);
    choose_coloring_box_->addItem("Stripes 1", STRIPES1);
    choose_coloring_box_->addItem("Stripes 2", STRIPES2);
    choose_coloring_box_->addItem("Stripes 3", STRIPES3);
    choose_coloring_box_->addItem("Flowers", FLOWERS);
    choose_coloring_box_->addItem("3 Colors", TILING);
    choose_coloring_box_->addItem("Shading", SHADING);
    choose_coloring_box_->addItem("Erdos", ERDOS);
    choose_coloring_box_->setCurrentIndex((int) PLAIN);
    connect(choose_coloring_box_, SIGNAL(activated(int)), window_, SLOT(choose_coloring()));
    return;
}

void Input_Menu::create_choose_coloring_2_combobox()
{
    choose_coloring_box_2_ = new QComboBox;
    choose_coloring_box_2_->setFixedHeight(button_height_);
    choose_coloring_box_2_->addItem("Plain", T_PLAIN);
    choose_coloring_box_2_->addItem("Stripes 1", T_STRIPES_1);
    choose_coloring_box_2_->addItem("Stripes 2", T_STRIPES_2);
    choose_coloring_box_2_->addItem("Stripes 3", T_STRIPES_3);
    choose_coloring_box_2_->addItem("Triangles", T_TRIANGLES);
    choose_coloring_box_2_->addItem("Squares", T_SQUARES);
    choose_coloring_box_2_->addItem("Squares 2", T_SQUARES_2);
    choose_coloring_box_2_->addItem("Shading", T_SHADING);
    choose_coloring_box_2_->addItem("Image", T_IMAGE);
    choose_coloring_box_2_->setCurrentIndex((int) T_SQUARES);
    connect(choose_coloring_box_2_, SIGNAL(activated(int)), window_, SLOT(choose_coloring_2()));
    return;
}



//

void Input_Menu::change_mode_menu(const mode_type &old_mode_flag, const mode_type &new_mode_flag, bool create_tiling)
{

    switch (old_mode_flag)
    {
    case START:
        break;
    case DRAW_GRAPH:
        empty_draw_graph_menu();
        break;
    case DRAW_CURVE:
        empty_draw_curve_menu();
        break;
    case DRAW_2_CURVES:
        empty_draw_two_curves_menu();
        break;
    default:
        std::cout << "ERROR in Input_Menu::change_mode_menu: flag problem" << std::endl;
        throw(QString("ERROR in Input_Menu::change_mode_menu: flag problem"));
    }

    switch (new_mode_flag)
    {
    case START:
        break;
    case DRAW_GRAPH:
        create_draw_graph_menu();
        break;
    case DRAW_CURVE:
        create_draw_curve_menu();
        break;
    case DRAW_2_CURVES:
        create_draw_two_curves_menu();
        break;
    default:
        std::cout << "ERROR in Input_Menu::change_mode_menu: flag problem" << std::endl;
        throw(QString("ERROR in Input_Menu::change_mode_menu: flag problem"));
    }


    if (new_mode_flag == DRAW_CURVE && create_tiling)
    {
        //disconnect(tiling_size_spinbox_, SIGNAL(valueChanged(int)), window_, SLOT(set_tiling_size()));
        tiling_size_spinbox_->setValue(10);
        //connect(tiling_size_spinbox_, SIGNAL(valueChanged(int)), window_, SLOT(set_tiling_size()));
    }
    else if (new_mode_flag == DRAW_2_CURVES && create_tiling)
    {
        //disconnect(tiling_size_spinbox_, SIGNAL(valueChanged(int)), window_, SLOT(set_tiling_size()));
        tiling_size_spinbox_->setValue(10);
        //connect(tiling_size_spinbox_, SIGNAL(valueChanged(int)), window_, SLOT(set_tiling_size()));
    }
    return;
}

void Input_Menu::empty_draw_graph_menu()
{
    empty_draw_graph_submenu();

    layout_->removeWidget(choose_graph_combobox_);
    choose_graph_combobox_->setVisible(false);
    return;
}

void Input_Menu::empty_draw_graph_submenu()
{
    if (window_->graph_choice_flag_ == CUSTOM_GRAPH)
    {
        layout_->removeWidget(complete_);
        complete_->setVisible(false);

        layout_->removeWidget(draw_edges_);
        draw_edges_->setVisible(false);

        layout_->removeWidget(draw_vertices_);
        draw_vertices_->setVisible(false);

        set_draw_neither_vertices_nor_edges();
    }
    else
    {
        choose_graph_combobox_->setCurrentIndex(CUSTOM_GRAPH);
        empty_graph_parameters_menu();
    }
    return;
}

void Input_Menu::empty_graph_parameters_menu()
{
    switch(window_->graph_choice_flag_)
    {
    case CUSTOM_GRAPH:
        break;

    case GRID :
        layout_->removeWidget(graph_parameter_spinbox_);
        graph_parameter_spinbox_->setVisible(false);

        layout_->removeWidget(graph_parameter_label_);
        graph_parameter_label_->setVisible(false);
        break;

    case WEB:
        layout_->removeWidget(graph_parameter_spinbox_2_);
        graph_parameter_spinbox_2_->setVisible(false);

        layout_->removeWidget(graph_parameter_label_2_);
        graph_parameter_label_2_->setVisible(false);

        layout_->removeWidget(graph_parameter_spinbox_);
        graph_parameter_spinbox_->setVisible(false);

        layout_->removeWidget(graph_parameter_label_);
        graph_parameter_label_->setVisible(false);
        break;

    default:
        std::cout << "ERROR in Input_Menu::empty_graph_parameters_menu: flag problem" << std::endl;
        throw(QString("ERROR in Input_Menu::empty_graph_parameters_menu: flag problem"));
    }
    return;
}

void Input_Menu::empty_draw_curve_menu()
{
    layout_->removeWidget(choose_coloring_label_);
    choose_coloring_label_->setVisible(false);

    layout_->removeWidget(choose_coloring_box_);
    choose_coloring_box_->setVisible(false);

    layout_->removeWidget(tiling_size_spinbox_);
    tiling_size_spinbox_->setVisible(false);

    layout_->removeWidget(tiling_size_label_);
    tiling_size_label_->setVisible(false);

    empty_curve_parameters_menu();

    layout_->removeWidget(choose_curve_);
    choose_curve_->setVisible(false);
    return;
}

void Input_Menu::empty_draw_two_curves_menu()
{    
    layout_->removeWidget(choose_coloring_label_);
    choose_coloring_label_->setVisible(false);

    layout_->removeWidget(choose_coloring_box_2_);
    choose_coloring_box_2_->setVisible(false);

    layout_->removeWidget(tiling_size_spinbox_);
    tiling_size_spinbox_->setVisible(false);

    layout_->removeWidget(tiling_size_label_);
    tiling_size_label_->setVisible(false);

    empty_curve_parameters_menu_2();

    layout_->removeWidget(choose_curve_2_);
    choose_curve_2_->setVisible(false);

    empty_curve_parameters_menu();

    layout_->removeWidget(choose_curve_);
    choose_curve_->setVisible(false);
    return;
}

void Input_Menu::empty_curve_parameters_menu()
{
    curve_choice_type curve_choice_flag = window_->curve_choice_flag_;

    if (curve_choice_flag == CUSTOM)
    {
        layout_->removeWidget(smooth_curve_);
        smooth_curve_->setVisible(false);
        smooth_curve_->setChecked(false);

        layout_->removeWidget(smooth_curve_label_);
        smooth_curve_label_->setVisible(false);
    }
    else if (curve_choice_flag == POLYGON || curve_choice_flag == STAR
             || curve_choice_flag == KOCH || curve_choice_flag == EPICYCLOID)
    {
        layout_->removeWidget(nb_points_spinbox_);
        nb_points_spinbox_->setVisible(false);

        layout_->removeWidget(nb_points_label_);
        nb_points_label_->setVisible(false);
    }
    return;
}

void Input_Menu::empty_curve_parameters_menu_2()
{
    curve_choice_type curve_choice_flag_2 = window_->curve_choice_flag_2_;

    if (curve_choice_flag_2 == CUSTOM)
    {
        layout_->removeWidget(smooth_curve_2_);
        smooth_curve_2_->setVisible(false);
        smooth_curve_2_->setChecked(false);

        layout_->removeWidget(smooth_curve_label_2_);
        smooth_curve_label_2_->setVisible(false);
    }
    else if (curve_choice_flag_2 == POLYGON || curve_choice_flag_2 == STAR
             || curve_choice_flag_2 == KOCH || curve_choice_flag_2 == EPICYCLOID)
    {
        layout_->removeWidget(nb_points_spinbox_2_);
        nb_points_spinbox_2_->setVisible(false);

        layout_->removeWidget(nb_points_label_2_);
        nb_points_label_2_->setVisible(false);
    }
    return;
}

void Input_Menu::reset_input_menu()
{
    switch(choose_mode_->currentIndex())
    {
    case START:
        break;
    case DRAW_GRAPH:
        reset_draw_graph_menu();
        break;
    case DRAW_CURVE:
        reset_draw_curve_menu();
        break;
    case DRAW_2_CURVES:
        reset_draw_two_curves_menu();
        break;
    default:
        std::cout << "ERROR in Input_Menu::reset_input_menu: flag problem" << std::endl;
        throw(QString("ERROR in Input_Menu::reset_input_menu: flag problem"));
    }
    return;
}

void Input_Menu::reset_draw_graph_menu()
{
    if (choose_graph_combobox_->currentIndex() == CUSTOM_GRAPH)
    {

    }
    else
    {
        complete_->setEnabled(false);

        empty_graph_parameters_menu();

        layout_->addWidget(draw_vertices_, 7, 0, 1, 2);
        draw_vertices_->setVisible(true);

        layout_->addWidget(draw_edges_, 8, 0, 1, 2);
        draw_edges_->setVisible(true);

        layout_->addWidget(complete_, 11, 0, 1, 2);
        complete_->setVisible(true);

        choose_graph_combobox_->setCurrentIndex(CUSTOM);
    }

    set_draw_vertices_with_edges_forbidden();

    return;
}

void Input_Menu::reset_draw_curve_menu()
{
    if (choose_curve_->currentIndex() == CUSTOM)
    {
        smooth_curve_->setChecked(false);
        smooth_curve_label_->setEnabled(false);
        smooth_curve_->setEnabled(false);
    }
    else
    {
        empty_curve_parameters_menu();
        choose_curve_->setCurrentIndex(CUSTOM);
        create_curve_parameters_menu();
    }

    choose_coloring_label_->setEnabled(true);
    choose_coloring_box_->setEnabled(true);
    choose_coloring_box_->setCurrentIndex((int) PLAIN);

    return;
}

void Input_Menu::reset_draw_two_curves_menu()
{
    if (choose_curve_->currentIndex() == CUSTOM)
    {
        smooth_curve_->setChecked(false);
        smooth_curve_label_->setEnabled(false);
        smooth_curve_->setEnabled(false);
    }
    else
    {
        empty_curve_parameters_menu();
        choose_curve_->setCurrentIndex(CUSTOM);
        create_curve_parameters_menu();
    }
    if (choose_curve_2_->currentIndex() == CUSTOM)
    {
        smooth_curve_2_->setChecked(false);
        smooth_curve_label_2_->setEnabled(false);
        smooth_curve_2_->setEnabled(false);
    }
    else
    {
        empty_curve_parameters_menu_2();
        choose_curve_2_->setCurrentIndex(CUSTOM);
        create_curve_parameters_menu_2();

    }


    choose_coloring_label_->setEnabled(true);
    choose_coloring_box_2_->setEnabled(true);
    choose_coloring_box_2_->setCurrentIndex((int) T_SQUARES);
    return;
}



//

void Input_Menu::set_draw_vertices_with_edges_forbidden()
{
    draw_vertices_->setEnabled(true);
    draw_vertices_->setCheckable(true);
    draw_vertices_->setChecked(true);

    draw_edges_->setCheckable(true);
    draw_edges_->setChecked(false);
    draw_edges_->setEnabled(false);

    complete_->setEnabled(false);
    return;
}

void Input_Menu::set_draw_vertices_with_edges_allowed()
{
    draw_vertices_->setEnabled(true);
    draw_vertices_->setCheckable(true);
    draw_vertices_->setChecked(true);

    draw_edges_->setCheckable(true);
    draw_edges_->setChecked(false);
    draw_edges_->setEnabled(true);

    complete_->setEnabled(true);
    return;
}

void Input_Menu::set_draw_edges()
{
    draw_vertices_->setCheckable(true);
    draw_vertices_->setChecked(false);
    draw_vertices_->setEnabled(true);

    draw_edges_->setEnabled(true);
    draw_edges_->setCheckable(true);
    draw_edges_->setChecked(true);

    complete_->setEnabled(true);
    return;
}

void Input_Menu::set_draw_neither_vertices_nor_edges()
{
    draw_vertices_->setCheckable(true);
    draw_vertices_->setChecked(false);
    draw_vertices_->setEnabled(false);

    draw_edges_->setCheckable(true);
    draw_edges_->setChecked(false);
    draw_edges_->setEnabled(false);
    return;
}



//

void Input_Menu::create_draw_graph_menu()
{
    layout_->addWidget(choose_graph_combobox_, 5, 0, 1, 2);
    choose_graph_combobox_->setVisible(true);

    create_draw_graph_submenu();
    return;
}

void Input_Menu::create_draw_graph_submenu()
{
    if (choose_graph_combobox_->currentIndex() == CUSTOM_GRAPH)
    {
        layout_->addWidget(draw_vertices_, 7, 0, 1, 2);
        draw_vertices_->setVisible(true);

        layout_->addWidget(draw_edges_, 8, 0, 1, 2);
        draw_edges_->setVisible(true);

        set_draw_vertices_with_edges_forbidden();

        layout_->addWidget(complete_, 11, 0, 1, 2);
        complete_->setEnabled(false);
        complete_->setVisible(true);
    }
    else
    {
        create_graph_parameters_menu();
    }
    return;
}

void Input_Menu::create_graph_parameters_menu()
{

    switch(choose_graph_combobox_->currentIndex())
    {
    case CUSTOM_GRAPH:
        break;
    case GRID:
        disconnect(graph_parameter_spinbox_,SIGNAL(valueChanged(int)),window_,SLOT(set_graph_parameter(int)));
        graph_parameter_spinbox_->setRange(2,40);
        graph_parameter_spinbox_->setValue(10);
        connect(graph_parameter_spinbox_,SIGNAL(valueChanged(int)),window_,SLOT(set_graph_parameter(int)));
        layout_->addWidget(graph_parameter_spinbox_, 7,1,1,1);
        graph_parameter_spinbox_->setVisible(true);

        graph_parameter_label_->setText("Size ");
        layout_->addWidget(graph_parameter_label_,7,0,1,1, Qt::AlignRight);
        graph_parameter_label_->setVisible(true);
        break;

    case WEB:
        disconnect(graph_parameter_spinbox_,SIGNAL(valueChanged(int)),window_,SLOT(set_graph_parameter(int)));
        graph_parameter_spinbox_->setRange(1, 30);
        graph_parameter_spinbox_->setValue(10);
        connect(graph_parameter_spinbox_,SIGNAL(valueChanged(int)),window_,SLOT(set_graph_parameter(int)));
        layout_->addWidget(graph_parameter_spinbox_, 7,1,1,1);
        graph_parameter_spinbox_->setVisible(true);

        graph_parameter_label_->setText("Depth ");
        layout_->addWidget(graph_parameter_label_,7,0,1,1, Qt::AlignRight);
        graph_parameter_label_->setVisible(true);

        disconnect(graph_parameter_spinbox_2_,SIGNAL(valueChanged(int)),window_,SLOT(set_graph_parameter_2(int)));
        graph_parameter_spinbox_2_->setRange(3, 30);
        graph_parameter_spinbox_2_->setValue(10);
        connect(graph_parameter_spinbox_2_,SIGNAL(valueChanged(int)),window_,SLOT(set_graph_parameter_2(int)));
        layout_->addWidget(graph_parameter_spinbox_2_, 9,1,1,1);
        graph_parameter_spinbox_2_->setVisible(true);

        graph_parameter_label_2_->setText("Angle ");
        layout_->addWidget(graph_parameter_label_2_,9,0,1,1, Qt::AlignRight);
        graph_parameter_label_2_->setVisible(true);
        break;
    default:
        std::cout << "ERROR in Input_Menu::create_graph_parameters_menu : flag problem" << std::endl;
        throw(QString("ERROR in Input_Menu::create_graph_parameters_menu : flag problem"));
    }
    return;
}

void Input_Menu::create_draw_curve_menu()
{
    layout_->addWidget(choose_curve_, 5, 0, 1, 2);
    choose_curve_->setVisible(true);
    choose_curve_->setCurrentIndex(CUSTOM);

    create_curve_parameters_menu();

    layout_->addWidget(tiling_size_label_, 15, 0, 1, 1, Qt::AlignRight);
    tiling_size_label_->setVisible(true);

    tiling_size_spinbox_->setEnabled(true);
    layout_->addWidget(tiling_size_spinbox_, 15, 1, 1, 1, Qt::AlignBottom);
    tiling_size_spinbox_->setVisible(true);


    layout_->addWidget(choose_coloring_label_, 13, 0, 1, 1, Qt::AlignRight);
    choose_coloring_label_->setVisible(true);

    layout_->addWidget(choose_coloring_box_, 13, 1, 1, 1);
    choose_coloring_box_->setVisible(true);
    choose_coloring_box_->setCurrentIndex((int) PLAIN);

    return;
}

void Input_Menu::create_draw_two_curves_menu()
{
    layout_->addWidget(choose_curve_, 5, 0, 1, 2);
    choose_curve_->setVisible(true);
    choose_curve_->setCurrentIndex(CUSTOM);

    create_curve_parameters_menu();

    layout_->addWidget(choose_curve_2_, 9, 0, 1, 2);
    choose_curve_2_->setVisible(true);
    choose_curve_2_->setCurrentIndex(CUSTOM);

    create_curve_parameters_menu_2();

    layout_->addWidget(tiling_size_label_, 15, 0, 1, 1, Qt::AlignRight);
    tiling_size_label_->setVisible(true);

    tiling_size_spinbox_->setEnabled(true);
    layout_->addWidget(tiling_size_spinbox_, 15, 1, 1, 1, Qt::AlignBottom);
    tiling_size_spinbox_->setVisible(true);

    layout_->addWidget(choose_coloring_label_, 13, 0, 1, 1, Qt::AlignRight);
    choose_coloring_label_->setVisible(true);

    layout_->addWidget(choose_coloring_box_2_, 13, 1, 1, 1);
    choose_coloring_box_2_->setVisible(true);
    choose_coloring_box_2_->setCurrentIndex((int) T_SQUARES);

    return;
}

void Input_Menu::create_curve_parameters_menu()
{
    switch(choose_curve_->currentIndex())
    {
    case CUSTOM:
        layout_->addWidget(smooth_curve_label_, 7, 0, 1, 1, Qt::AlignRight);
        smooth_curve_label_->setVisible(true);
        smooth_curve_label_->setEnabled(false);

        layout_->addWidget(smooth_curve_, 7, 1, 1, 1);
        smooth_curve_->setChecked(false);
        smooth_curve_->setVisible(true);
        smooth_curve_->setEnabled(false);
        break;
    case RECTANGLE:
        break;
    case POLYGON:
        nb_points_label_->setText("Sides: ");
        layout_->addWidget(nb_points_label_, 7, 0, 1, 1, Qt::AlignRight);
        nb_points_label_->setVisible(true);

        layout_->addWidget(nb_points_spinbox_, 7, 1, 1, 1);
        disconnect(nb_points_spinbox_, SIGNAL(valueChanged(int)), window_, SLOT(set_nb_points()));
        nb_points_spinbox_->setRange(3, 20);
        nb_points_spinbox_->setValue(5);
        connect(nb_points_spinbox_, SIGNAL(valueChanged(int)), window_, SLOT(set_nb_points()));
        nb_points_spinbox_->setVisible(true);
        break;
    case STAR:
        nb_points_label_->setText("Points: ");
        layout_->addWidget(nb_points_label_, 7, 0, 1, 1, Qt::AlignRight);
        nb_points_label_->setVisible(true);

        layout_->addWidget(nb_points_spinbox_, 7, 1, 1, 1);
        disconnect(nb_points_spinbox_, SIGNAL(valueChanged(int)), window_, SLOT(set_nb_points()));
        nb_points_spinbox_->setRange(3, 20);
        nb_points_spinbox_->setValue(5);
        connect(nb_points_spinbox_, SIGNAL(valueChanged(int)), window_, SLOT(set_nb_points()));
        nb_points_spinbox_->setVisible(true);
        break;
    case ELLIPSE:
        break;
    case EPICYCLOID:
        nb_points_label_->setText("Cusps ");
        layout_->addWidget(nb_points_label_, 7, 0, 1, 1, Qt::AlignRight);
        nb_points_label_->setVisible(true);

        layout_->addWidget(nb_points_spinbox_, 7, 1, 1, 1);
        disconnect(nb_points_spinbox_, SIGNAL(valueChanged(int)), window_, SLOT(set_nb_points()));
        nb_points_spinbox_->setRange(1, 10);
        nb_points_spinbox_->setValue(1);
        connect(nb_points_spinbox_, SIGNAL(valueChanged(int)), window_, SLOT(set_nb_points()));
        nb_points_spinbox_->setVisible(true);
        break;
    case KOCH:
        nb_points_label_->setText("Depth ");
        layout_->addWidget(nb_points_label_, 7, 0, 1, 1, Qt::AlignRight);
        nb_points_label_->setVisible(true);

        layout_->addWidget(nb_points_spinbox_, 7, 1, 1, 1);
        disconnect(nb_points_spinbox_, SIGNAL(valueChanged(int)), window_, SLOT(set_nb_points()));
        nb_points_spinbox_->setRange(0, 5);
        nb_points_spinbox_->setValue(3);
        connect(nb_points_spinbox_, SIGNAL(valueChanged(int)), window_, SLOT(set_nb_points()));
        nb_points_spinbox_->setVisible(true);
        break;
    default:
        std::cout << "ERROR in Input_Menu::create_curve_parameters_menu: flag problem" << std::endl;
        throw(QString("ERROR in Input_Menu::create_curve_parameters_menu: flag problem"));
    }

    return;
}

void Input_Menu::create_curve_parameters_menu_2()
{
    switch(choose_curve_2_->currentIndex())
    {
    case CUSTOM:
        layout_->addWidget(smooth_curve_label_2_, 11, 0, 1, 1, Qt::AlignRight);
        smooth_curve_label_2_->setVisible(true);
        smooth_curve_label_2_->setEnabled(false);

        layout_->addWidget(smooth_curve_2_, 11, 1, 1, 1);
        smooth_curve_2_->setChecked(false);
        smooth_curve_2_->setVisible(true);
        smooth_curve_2_->setEnabled(false);
        break;
    case RECTANGLE:
        break;
    case POLYGON:
        nb_points_label_2_->setText("Sides: ");
        layout_->addWidget(nb_points_label_2_, 11, 0, 1, 1, Qt::AlignRight);
        nb_points_label_2_->setVisible(true);

        layout_->addWidget(nb_points_spinbox_2_, 11, 1, 1, 1);
        disconnect(nb_points_spinbox_2_, SIGNAL(valueChanged(int)), window_, SLOT(set_nb_points_2()));
        nb_points_spinbox_2_->setRange(3, 20);
        nb_points_spinbox_2_->setValue(5);
        connect(nb_points_spinbox_2_, SIGNAL(valueChanged(int)), window_, SLOT(set_nb_points_2()));
        nb_points_spinbox_2_->setVisible(true);
        break;
    case STAR:
        nb_points_label_2_->setText("Points: ");
        layout_->addWidget(nb_points_label_2_, 11, 0, 1, 1, Qt::AlignRight);
        nb_points_label_2_->setVisible(true);

        layout_->addWidget(nb_points_spinbox_2_, 11, 1, 1, 1);
        disconnect(nb_points_spinbox_2_, SIGNAL(valueChanged(int)), window_, SLOT(set_nb_points_2()));
        nb_points_spinbox_2_->setRange(3, 20);
        nb_points_spinbox_2_->setValue(5);
        connect(nb_points_spinbox_2_, SIGNAL(valueChanged(int)), window_, SLOT(set_nb_points_2()));
        nb_points_spinbox_2_->setVisible(true);
        break;
    case ELLIPSE:
        break;
    case EPICYCLOID:
        nb_points_label_2_->setText("Cusps: ");
        layout_->addWidget(nb_points_label_2_, 11, 0, 1, 1, Qt::AlignRight);
        nb_points_label_2_->setVisible(true);

        layout_->addWidget(nb_points_spinbox_2_, 11, 1, 1, 1);
        disconnect(nb_points_spinbox_2_, SIGNAL(valueChanged(int)), window_, SLOT(set_nb_points_2()));
        nb_points_spinbox_2_->setRange(1, 10);
        nb_points_spinbox_2_->setValue(1);
        connect(nb_points_spinbox_2_, SIGNAL(valueChanged(int)), window_, SLOT(set_nb_points_2()));
        nb_points_spinbox_2_->setVisible(true);
        break;
    case KOCH:
        nb_points_label_2_->setText("Depth: ");
        layout_->addWidget(nb_points_label_2_, 11, 0, 1, 1, Qt::AlignRight);
        nb_points_label_2_->setVisible(true);

        layout_->addWidget(nb_points_spinbox_2_, 11, 1, 1, 1);
        disconnect(nb_points_spinbox_2_, SIGNAL(valueChanged(int)), window_, SLOT(set_nb_points_2()));
        nb_points_spinbox_2_->setRange(0, 5);
        nb_points_spinbox_2_->setValue(3);
        connect(nb_points_spinbox_2_, SIGNAL(valueChanged(int)), window_, SLOT(set_nb_points_2()));
        nb_points_spinbox_2_->setVisible(true);
        break;
    default:
        std::cout << "ERROR in Input_Menu::create_curve_parameters_menu: flag problem" << std::endl;
        throw(QString("ERROR in Input_Menu::create_curve_parameters_menu: flag problem"));
    }

    return;
}

void Input_Menu::completing()
{
    reset_->setEnabled(false);
    set_draw_neither_vertices_nor_edges();
    complete_->setText("Stop");
    return;
}


//

int Input_Menu::max_left_col_width() const
{
    int left_max = graph_parameter_label_->sizeHint().width();
    left_max = std::max(left_max, smooth_curve_label_->sizeHint().width());
    left_max = std::max(left_max, smooth_curve_label_2_->sizeHint().width());
    left_max = std::max(left_max, nb_points_label_->sizeHint().width());
    left_max = std::max(left_max, nb_points_label_2_->sizeHint().width());
    left_max = std::max(left_max, tiling_size_label_->sizeHint().width());
    left_max = std::max(left_max, choose_coloring_label_->sizeHint().width());
    //std::cout << "left_max: " << left_max << std::endl;
    return left_max;
}

int Input_Menu::max_right_col_width() const
{
    int right_max = graph_parameter_spinbox_->sizeHint().width();
    right_max = std::max(right_max,smooth_curve_->sizeHint().width());
    right_max = std::max(right_max,smooth_curve_2_->sizeHint().width());
    right_max = std::max(right_max,nb_points_spinbox_->sizeHint().width());
    right_max = std::max(right_max,nb_points_spinbox_2_->sizeHint().width());
    right_max = std::max(right_max,tiling_size_spinbox_->sizeHint().width());
    right_max = std::max(right_max, choose_coloring_box_->sizeHint().width());
    right_max = std::max(right_max, choose_coloring_box_2_->sizeHint().width());

    //std::cout << "choose col box " << choose_coloring_box_->sizeHint().width() << std::endl;
    //std::cout << "choose col box 2 " << choose_coloring_box_2_->sizeHint().width() << std::endl;

    //std::cout << "right_max: " << right_max << std::endl;

    return right_max;
}

int Input_Menu::max_width() const
{
    //std::cout << "Entering Input_Menu::max_width" << std::endl;

    int maxi = max_left_col_width() + max_right_col_width() + layout_->horizontalSpacing();
    maxi = std::max(maxi, choose_mode_->sizeHint().width());
    maxi = std::max(maxi, reset_->sizeHint().width());
    maxi = std::max(maxi, choose_graph_combobox_->sizeHint().width());
    maxi = std::max(maxi, draw_vertices_->sizeHint().width());
    maxi = std::max(maxi, draw_edges_->sizeHint().width());
    maxi = std::max(maxi, complete_->sizeHint().width());
    maxi = std::max(maxi, choose_curve_->sizeHint().width());
    maxi = std::max(maxi, choose_curve_2_->sizeHint().width());

    return QStyle::CE_MenuHMargin + maxi + layout_->margin();

}

int Input_Menu::max_height() const
{
    int absurd_margin = 1;
    return QStyle::CE_HeaderLabel + 8*button_height_ + 16*vert_space_ + absurd_margin;
}


//

void Input_Menu::resizeEvent(QResizeEvent *)
{
    int max = max_height();
    //std::cout << "Entering Input_Menu::resizeEvent with height = " << height() << " and max height = " << max << std::endl;
    if(height() < max)
    {
        setMinimumHeight(max);
    }
    return;
}

void Input_Menu::enterEvent(QEvent *)
{
    window_->reset_selected_stuff();
    return;
}

void Input_Menu::set_nb_points_without_signal(int nb_points)
{
    disconnect(nb_points_spinbox_, SIGNAL(valueChanged(int)), window_, SLOT(set_nb_points()));
    nb_points_spinbox_->setValue(nb_points);
    connect(nb_points_spinbox_, SIGNAL(valueChanged(int)), window_, SLOT(set_nb_points()));

    return;
}

void Input_Menu::set_nb_points_2_without_signal(int nb_points_2)
{
    disconnect(nb_points_spinbox_2_, SIGNAL(valueChanged(int)), window_, SLOT(set_nb_points_2()));
    nb_points_spinbox_2_->setValue(nb_points_2);
    connect(nb_points_spinbox_2_, SIGNAL(valueChanged(int)), window_, SLOT(set_nb_points_2()));

    return;
}

void Input_Menu::set_graph_parameter_1_without_signal(int parameter)
{
    disconnect(graph_parameter_spinbox_, SIGNAL(valueChanged(int)), window_, SLOT(set_graph_parameter(int)));
    graph_parameter_spinbox_->setValue(parameter);
    connect(graph_parameter_spinbox_, SIGNAL(valueChanged(int)), window_, SLOT(set_graph_parameter(int)));

    return;
}

void Input_Menu::set_graph_parameter_2_without_signal(int parameter)
{
    disconnect(graph_parameter_spinbox_2_, SIGNAL(valueChanged(int)), window_, SLOT(set_graph_parameter_2(int)));
    graph_parameter_spinbox_2_->setValue(parameter);
    connect(graph_parameter_spinbox_2_, SIGNAL(valueChanged(int)), window_, SLOT(set_graph_parameter_2(int)));

    return;
}
