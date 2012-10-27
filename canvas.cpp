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

#include <QPixmap>
#include <QPainter>
#include <QMouseEvent>
#include <QApplication>
#include <QImage>

#include "canvas.hpp"
#include "canvas_delegate.hpp"
#include "window.hpp"

Canvas::Canvas(Window *const window, bool left):
    window_(window)
{
    setParent(window);
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
    setFocusPolicy(Qt::WheelFocus);

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    size_in_pixels_ = width();
    canvas_delegate_ = new Canvas_Delegate();
    canvas_delegate_->left_canvas_ = left;

    left_canvas_ = left;

    reset_canvas();
}

Canvas::~Canvas()
{
    delete canvas_delegate_;
}

void Canvas::reset_canvas(bool repaint_canvas)
{
    canvas_delegate_->reset_canvas_delegate(true);
    setMouseTracking(true);
    if (repaint_canvas)
    {
        update();
    }
    return;
}

void Canvas::rescale()
{
    /*if (left_canvas_)
    {
        std::cout << "Entering left Canvas::rescale() at t = " << clock()*0.01/CLOCKS_PER_SEC << std::endl;
    }*/

    size_in_pixels_ = width();
    canvas_delegate_->rescale(size_in_pixels_);
    return;
}

void Canvas::reset_selected_stuff()
{
    canvas_delegate_->reset_selected_stuff();
    update();
    return;
}

void Canvas::paintEvent(QPaintEvent *)
{
    /*if (left_canvas_)
    {
        std::cout << "Entering left Canvas::paintEvent" << clock() << std::endl;
    }
    else
    {
        //std::cout << "Entering right Canvas::paintEvent" << std::endl;
    }*/
    //clock_t t_0 = clock();

    QImage canvas_image(size_in_pixels_, size_in_pixels_, QImage::Format_RGB32);
    canvas_delegate_->canvas_image_width_ = canvas_image.width();
    canvas_delegate_->canvas_image_data_ = (QRgb*) canvas_image.bits();

    QPainter painter(&canvas_image);
    painter.setPen(canvas_delegate_->pen_);
    canvas_delegate_->brush_.setStyle(Qt::SolidPattern);
    painter.setBrush(canvas_delegate_->brush_);
    painter.setRenderHint(QPainter::Antialiasing, true);
    canvas_delegate_->painter_ = &painter;

    painter.eraseRect(0, 0, size_in_pixels_, size_in_pixels_);
    canvas_delegate_->paint_event();

    QPainter canvas_painter(this);
    canvas_painter.drawImage(0, 0, canvas_image);


    /*clock_t t_1 = clock();
    if (left_canvas_)
    {
        //std::cout << "Left paintEvent in " << (t_1- t_0)*100.0/CLOCKS_PER_SEC << "cs" << std::endl;
    }
    else
    {
        std::cout << "Right paintEvent in " << (t_1- t_0)*100.0/CLOCKS_PER_SEC << "cs" << std::endl;
    }*/
    //std::cout << "Leaving Canvas::paintEvent" << std::endl;
    return;
}

void Canvas::wheelEvent(QWheelEvent *event)
{
    int nb = 5;
    int i;
    for(i = 0; i < nb; i++)
    {
        integer_coordinates point_coordinates_in_pixels(event->x(), event->y());
        canvas_delegate_->wheel_event(int_round(event->delta()*1.0/nb), point_coordinates_in_pixels);
        update();
    }
    return;
}

void Canvas::mousePressEvent(QMouseEvent *event)
{
    //window_->print_flags();

    canvas_delegate_->set_point_clicked(integer_coordinates(event->x(), event->y()));
    canvas_delegate_->click_on_ = true;
    canvas_delegate_->mouse_press_event();
    update();
    return;
}

void Canvas::mouseReleaseEvent(QMouseEvent *event)
{
    canvas_delegate_->set_point_released(integer_coordinates(event->x(), event->y()));
    canvas_delegate_->click_on_ = false;
    canvas_delegate_->mouse_release_event();
    update();

    return;
}

void Canvas::mouseMoveEvent(QMouseEvent *event)
{
    canvas_delegate_->set_point_under_mouse(integer_coordinates(event->x(), event->y()));

    if(QApplication::keyboardModifiers() == Qt::ShiftModifier)
    {
        if(canvas_delegate_->click_on_)
        {
            canvas_delegate_->shift_event();
        }
    }
    else
    {
        canvas_delegate_->mouse_move_event();
    }
    update();

    return;
}

void Canvas::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Control :
        canvas_delegate_->ctrl_pressed();
        break;

    case Qt::Key_Left :
        canvas_delegate_->shift_left();
        update();
        break;

    case Qt::Key_Right :
        canvas_delegate_->shift_right();
        update();
        break;

    case Qt::Key_Up :
        canvas_delegate_->shift_up();
        update();
        break;

    case Qt::Key_Down :
        canvas_delegate_->shift_down();
        update();
        break;

    case Qt::Key_Plus :
        canvas_delegate_->zoom(2);
        update();
        break;

    case Qt::Key_Minus :
        canvas_delegate_->zoom(0.5);
        update();
        break;

    /*case Qt::Key_T:
        std::cout << "throw test in Canvas::keyPressEvent" << std::endl;
        throw(QString("ERROR in Canvas::keyPressEvent: throw test"));
        break;*/

    }
    return;
}

void Canvas::resizeEvent(QResizeEvent *event)
{
        /*if (left_canvas_)
        {
            std::cout << "Entering left Canvas::resizeEvent" << std::endl;
        }
        else
        {
            std::cout << "Entering right Canvas::resizeEvent" << std::endl;
        }*/

        int delta_width = abs(event->size().width() - event->oldSize().width());
        int delta_height = abs(event->size().height() - event->oldSize().height());

        if (delta_width > 1 || delta_height > 1)
        {
            window_->resize_canvases();
        }

        canvas_delegate_->set_image_parameters();

    return;
}

void Canvas::enterEvent(QEvent *)
{
    setFocus();
    return;
}

void Canvas::leaveEvent(QEvent *)
{
    //std::cout << "Entering Canvas::leaveEvent" << std::endl;

    reset_selected_stuff();
    update();

    window_->signal_received(MESSAGE, -1);
    return;
}

void Canvas::keyReleaseEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Control :
        canvas_delegate_->ctrl_released();
        break;
    }
    return;
}

bool Canvas::print_in_file(const QString &filename, int quality)
{
    QPixmap pixmap(size());
    render(&pixmap);
    return pixmap.save(filename, 0, quality);
}

void Canvas::set_left(bool left)
{
    left_canvas_ = left;
    canvas_delegate_->set_left(left);
    return;
}


bool Canvas::get_is_vertex_highlighted() const
{
    return canvas_delegate_->is_vertex_highlighted_;
}

const unsigned int & Canvas::get_index_vertex_highlighted() const
{
    return canvas_delegate_->index_vertex_highlighted_;
}

void Canvas::set_is_vertex_highlighted(bool b)
{
    canvas_delegate_->is_vertex_highlighted_ = b;
    return;
}

void Canvas::set_index_vertex_highlighted(const unsigned int &index)
{
    canvas_delegate_->index_vertex_highlighted_ = index;
    return;
}
