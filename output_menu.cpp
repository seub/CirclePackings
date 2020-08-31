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

#include "output_menu.hpp"
#include "window.hpp"

#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QSpinBox>
#include <QCheckBox>

Output_Menu::Output_Menu(Window* window)
{
    setParent(window);
    window_ = window;
    setTitle(tr("Output"));

    compute_button_ = new QPushButton(tr("Compute"));

    vert_space_ = 10;
    button_height_ = compute_button_->sizeHint().height();
    compute_button_->setFixedHeight(button_height_);
    layout_ = new QGridLayout;
    layout_->setSpacing(0);
    layout_->setRowMinimumHeight(0, vert_space_);
    layout_->setRowMinimumHeight(1,button_height_);
    layout_->setRowMinimumHeight(2, vert_space_);
    layout_->setRowMinimumHeight(3,button_height_);



    connect(compute_button_, SIGNAL(clicked()), window_, SLOT(compute_circle_packing()));

    show_live_label_ = new QLabel("Show live ");
    show_live_checkbox_ = new QCheckBox;
    show_live_label_->setFixedHeight(button_height_);
    show_live_checkbox_->setFixedHeight(button_height_);
    show_inverse_label_ = new QLabel("Inverse ");
    show_inverse_checkbox_ = new QCheckBox;
    show_inverse_label_->setFixedHeight(button_height_);
    show_inverse_checkbox_->setFixedHeight(button_height_);
    connect(show_inverse_checkbox_, SIGNAL(clicked()), window_, SLOT(show_inverse()));


    layout_->setColumnMinimumWidth(0,max_left_col_width());
    setLayout(layout_);
    reset_output_menu();
}

void Output_Menu::resizeEvent(QResizeEvent *)
{
    if(height() < max_height())
    {
        setMinimumHeight(max_height());
    }

    return;
}

void Output_Menu::reset_output_menu()
{
    compute_button_->setEnabled(false);

    show_live_label_->setEnabled(true);
    show_live_checkbox_->setEnabled(true);
    show_live_checkbox_->setChecked(false);

    show_inverse_label_->setEnabled(true);
    show_inverse_checkbox_->setEnabled(true);
    show_inverse_checkbox_->setChecked(false);
    return;
}

void Output_Menu::disable_all_but_stop()
{
    show_live_label_->setEnabled(false);
    show_live_checkbox_->setEnabled(false);

    show_inverse_label_->setEnabled(false);
    show_inverse_checkbox_->setEnabled(false);

    return;
}

void Output_Menu::enable_all()
{
    compute_button_->setEnabled(true);

    show_live_label_->setEnabled(true);
    show_live_checkbox_->setEnabled(true);

    show_inverse_label_->setEnabled(true);
    show_inverse_checkbox_->setEnabled(true);

    return;
}

void Output_Menu::change_mode_menu(const mode_type &old_mode_flag, const mode_type &new_mode_flag)
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
        std::cout << "ERROR in Output_Menu::change_mode_menu: flag problem" << std::endl;
        throw(QString("ERROR in Output_Menu::change_mode_menu: flag problem"));
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
        std::cout << "ERROR in Output_Menu::change_mode_menu: flag problem" << std::endl;
        throw(QString("ERROR in Output_Menu::change_mode_menu: flag problem"));
    }

    return;
}

void Output_Menu::create_draw_graph_menu()
{
    layout_->addWidget(compute_button_, 1, 0, 1, 2);
    compute_button_->setVisible(true);

    layout_->addWidget(show_live_label_, 3, 0, 1, 1,Qt::AlignRight);
    show_live_label_->setVisible(true);

    layout_->addWidget(show_live_checkbox_, 3, 1, 1, 1);
    show_live_checkbox_->setVisible(true);

    compute_button_->setEnabled(false);
    show_live_checkbox_->setChecked(false);
    return;
}

void Output_Menu::empty_draw_graph_menu()
{
    layout_->removeWidget(compute_button_);
    compute_button_->setVisible(false);

    layout_->removeWidget(show_live_label_);
    show_live_label_->setVisible(false);

    layout_->removeWidget(show_live_checkbox_);
    show_live_checkbox_->setVisible(false);
    show_live_checkbox_->setChecked(false);
    return;
}

void Output_Menu::create_draw_curve_menu()
{
    layout_->addWidget(compute_button_, 1, 0, 1, 2);
    compute_button_->setVisible(true);

    layout_->addWidget(show_live_label_, 3, 0, 1, 1,Qt::AlignRight);
    show_live_label_->setVisible(true);

    layout_->addWidget(show_live_checkbox_, 3, 1, 1, 1);
    show_live_checkbox_->setVisible(true);

    compute_button_->setEnabled(false);
    show_live_checkbox_->setChecked(false);
    return;
}

void Output_Menu::empty_draw_curve_menu()
{
    layout_->removeWidget(compute_button_);
    compute_button_->setVisible(false);

    layout_->removeWidget(show_live_label_);
    show_live_label_->setVisible(false);

    layout_->removeWidget(show_live_checkbox_);
    show_live_checkbox_->setVisible(false);
    show_live_checkbox_->setChecked(false);

    return;
}

void Output_Menu::create_draw_two_curves_menu()
{
    layout_->addWidget(compute_button_, 1, 0, 1, 2);
    compute_button_->setVisible(true);

    layout_->addWidget(show_inverse_label_, 3, 0, 1, 1, Qt::AlignRight);
    show_inverse_label_->setVisible(true);

    layout_->addWidget(show_inverse_checkbox_, 3, 1, 1, 1);
    show_inverse_checkbox_->setVisible(true);

    compute_button_->setEnabled(false);
    return;
}

void Output_Menu::empty_draw_two_curves_menu()
{
    layout_->removeWidget(compute_button_);
    compute_button_->setVisible(false);

    layout_->removeWidget(show_inverse_label_);
    show_inverse_label_->setVisible(false);

    layout_->removeWidget(show_inverse_checkbox_);
    show_inverse_checkbox_->setVisible(false);
    show_inverse_checkbox_->setChecked(false);

    return;
}

int Output_Menu::max_left_col_width() const
{
    int left_max = show_live_label_->sizeHint().width();
    left_max = std::max(left_max, show_inverse_label_->sizeHint().width());

    //std::cout << "output left_max: " << left_max << std::endl;

    return left_max;
}

int Output_Menu::max_right_col_width() const
{
    int right_max = show_live_checkbox_->sizeHint().width();
    right_max = std::max(right_max, show_inverse_checkbox_->sizeHint().width());

    //std::cout << "output right max: " << right_max << std::endl;

    return right_max;
}

int Output_Menu::max_width() const
{
    int maxi = max_left_col_width() + max_right_col_width() + layout_->horizontalSpacing();
    maxi = std::max(maxi, compute_button_->sizeHint().width());

    return maxi + layout_->margin() + QStyle::CE_MenuHMargin;
}

int Output_Menu::max_height() const
{
    int absurd_margin = 1;
    return QStyle::CE_HeaderLabel + 2*button_height_ + 2*vert_space_ + absurd_margin;
}

void Output_Menu::enterEvent(QEvent *)
{
    window_->reset_selected_stuff();
    return;
}
