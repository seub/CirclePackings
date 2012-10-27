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



#include "top_menu.hpp"


#include <QMenu>
#include <QMessageBox>
#include <QFileDialog>
#include <QDialog>
#include <QLineEdit>
#include <QGridLayout>
#include <QLabel>
#include <QComboBox>
#include <QSpinBox>
#include <QPushButton>
#include <QCheckBox>

#include "window.hpp"
#include "canvas.hpp"
#include "canvas_delegate.hpp"
#include "input_menu.hpp"
#include "output_menu.hpp"
#include "configuration.hpp"
#include "help_browser.hpp"

Top_Menu::Top_Menu()
{

}

Top_Menu::~Top_Menu()
{

}

Top_Menu::Top_Menu(Window* window)

{
    setParent(window);
    window_ = window;
    file_menu_ = new QMenu(tr("&File"), this);

    save_action_ = file_menu_->addAction(tr("&Save"),this, SLOT(save()), QKeySequence(Qt::CTRL + Qt::Key_S));
    save_as_action_ = file_menu_->addAction(tr("Save &as..."),this, SLOT(save_as()), QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_S));
    load_action_ = file_menu_->addAction(tr("&Open..."),this, SLOT(open()), QKeySequence(Qt::CTRL + Qt::Key_O));
    print_action_ = file_menu_->addAction(tr("&Print..."),this, SLOT(print()), QKeySequence(Qt::CTRL + Qt::Key_P));
    exit_action_ = file_menu_->addAction(tr("E&xit"),window_, SLOT(close()), QKeySequence(Qt::CTRL + Qt::Key_Q));
    addMenu(file_menu_);

    setting_menu_ = new QMenu(tr("&Settings"), this);

    load_image_action_ = setting_menu_->addAction(tr("&Set image..."),window_, SLOT(load_image()), QKeySequence(Qt::CTRL + Qt::Key_I));
    addMenu( setting_menu_);
    help_menu_ = new QMenu(tr("&Help"), this);

    help_action_ = help_menu_->addAction(tr("&Help"),this,SLOT(help()), QKeySequence(Qt::Key_F1));
    about_action_ = help_menu_->addAction(tr("&About"), this, SLOT(about()));
    addMenu(help_menu_);


    show_save_image_warning_ = true;
}

void Top_Menu::print()
{
    print_dialog_ = new QDialog;
    print_dialog_->setWindowTitle("Print in file...");
    QGridLayout *layout = new QGridLayout;
    QLabel *canvas_label = new QLabel;
    canvas_choice_ = new QComboBox;
    QLabel *quality_label = new QLabel;
    quality_spinbox_ = new QSpinBox;
    QLabel *file_label = new QLabel;
    QPushButton *browse_button = new QPushButton;
    print_button_ = new QPushButton;
    QPushButton *cancel_button = new QPushButton;
    file_name_ = new QLineEdit;
    file_name_->setText("CP.png");

    canvas_label->setText(tr("Canvas: "));
    canvas_choice_->addItem("left", 0);
    canvas_choice_->addItem("right",1);
    quality_label->setText(tr("Quality:  "));
    quality_spinbox_->setRange(0,100);
    quality_spinbox_->setValue(80);
    file_label->setText("Filename: ");
    browse_button->setText(tr("Browse..."));
    print_button_->setText(tr("Print"));
    cancel_button->setText(tr("Cancel"));
    connect(browse_button,SIGNAL(clicked()),this,SLOT(browse()));
    connect(cancel_button,SIGNAL(clicked()),print_dialog_,SLOT(close()));
    connect(print_button_,SIGNAL(clicked()),this,SLOT(print_in_file()));
    connect(file_name_,SIGNAL(textChanged(QString)),this,SLOT(print_file_name_changed(QString)));
    layout->setVerticalSpacing(15);
    layout->setRowMinimumHeight(3, 20);
    layout->setColumnMinimumWidth(4, 60);

    layout->addWidget(canvas_label,0,0,1,1);
    layout->addWidget(canvas_choice_,0,1);
    layout->addWidget(quality_label,0,5,1,1,Qt::AlignRight);
    layout->addWidget(quality_spinbox_,0,6,1,1);
    layout->addWidget(file_label,2,0,1,1);
    layout->addWidget(file_name_,2,1,1,5);
    layout->addWidget(browse_button,2,6,1,2);

    layout->addWidget(print_button_,4,6,1,2);
    layout->addWidget(cancel_button,4,5,1,1);
    print_dialog_->setLayout(layout);

    print_button_->setDefault(true);
    print_dialog_->show();
}

void Top_Menu::print_in_file()
{

    bool success = false;
    if(canvas_choice_->currentIndex()==0)
    {
        success = window_->left_canvas_->print_in_file(file_name_->text(),quality_spinbox_->value());
    }
    else
    {
        success = window_->right_canvas_->print_in_file(file_name_->text(),quality_spinbox_->value());
    }


    if (success)
    {
        print_dialog_->close();
    }
    else
    {

        QMessageBox message_box_1;
        message_box_1.setWindowTitle("Print...");
        message_box_1.setIcon(QMessageBox::Warning);
        message_box_1.setText("Failed to print the image with the given extension.");
        message_box_1.setInformativeText("Do you want to retry?");
        message_box_1.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
        message_box_1.setDefaultButton(QMessageBox::Yes);
        int retour = message_box_1.exec();

        switch(retour)
        {
        case QMessageBox::Yes :
            return;
            break;

        case QMessageBox::No :
            print_dialog_->close();
            break;

        default:
            std::cout << "ERROR in Top_Menu::print_in_file: should never have reached here" << std::endl;
            throw(QString("ERROR in Top_Menu::print_in_file: should never have reached here"));
        }
    }

    return;
}

void Top_Menu::print_file_name_changed(QString filename)
{
    print_button_->setDisabled(filename.isEmpty());
    return;
}

void Top_Menu::browse()
{

    QString new_file_name =
            QFileDialog::getSaveFileName(print_dialog_,
                                         tr("Print in file..."),
                                         "CP.png",
                                         tr("PNG files (*.png);; JPG files (*.jpg);;All Files (*)"));

    if (!new_file_name.isEmpty())
    {
        file_name_->setText(new_file_name);
    }

    print_button_->setDefault(true);

    return;
}

void Top_Menu::save_as()
{
    if (window_->mode_flag_ == DRAW_2_CURVES && window_->input_menu_->choose_coloring_box_2_->currentIndex() == T_IMAGE
            && show_save_image_warning_)
    {
        QMessageBox message_box;
        message_box.setWindowTitle("Save...");
        message_box.setIcon(QMessageBox::Information);
        QString message = QString("Saving the image is not supported.");
        message_box.setText(message);
        message_box.setInformativeText("You may need to set an image again when you load the file.");
        message_box.setStandardButtons(QMessageBox::Ok);
        message_box.exec();

        show_save_image_warning_ = false;
    }

    last_loaded_file_ =
            QFileDialog::getSaveFileName(this,
                                         tr("Save as..."),
                                         (last_loaded_file_.isEmpty()?"save.cp":last_loaded_file_.toStdString().c_str()),
                                         tr("CP files (*.cp);;All Files (*)"));
    if(last_loaded_file_.isEmpty())
    {
        return;

    }
    else
    {
        save();
        return;
    }
}

void Top_Menu::save()
{
    if (window_->mode_flag_ == DRAW_2_CURVES && window_->input_menu_->choose_coloring_box_2_->currentIndex() == T_IMAGE
            && show_save_image_warning_)
    {
        QMessageBox message_box;
        message_box.setWindowTitle("Save...");
        message_box.setIcon(QMessageBox::Information);
        QString message = QString("Saving the image is not supported.");
        message_box.setText(message);
        message_box.setInformativeText("You may need to set an image again when you load the file.");
        message_box.setStandardButtons(QMessageBox::Ok);
        message_box.exec();

        show_save_image_warning_ = false;
    }


    if(last_loaded_file_.isEmpty())
    {
        save_as();
    }
    std::ofstream save_file(last_loaded_file_.toStdString().c_str(), std::ios::out | std::ios::trunc);
    window_->configuration_->update_parameters();
    window_->configuration_->write_in_file(save_file);
    save_file.close();
    return;
}

void Top_Menu::open()
{
    //std::cout << "Entering in Top_Menu::load" << std::endl;
    int load_success = 1;
    last_loaded_file_ =
            QFileDialog::getOpenFileName(this,
                                         tr("Open file..."),
                                         (last_loaded_file_.isEmpty()?" ":last_loaded_file_.toStdString().c_str()),
                                         tr("CP files (*.cp);;All Files (*)"));
    if(last_loaded_file_.isEmpty())
    {
        return;
    }
    std::ifstream load_file(last_loaded_file_.toStdString().c_str(), std::ios::in);
    load_success = window_->configuration_->read_from_file(load_file);
    if(load_success == 0)
    {
        window_->configuration_->restore_configuration();
    }
    else if(load_success == 1)
    {
        QMessageBox message_box_1;
        message_box_1.setWindowTitle("Open file...");
        message_box_1.setIcon(QMessageBox::Warning);
        message_box_1.setText("The selected file is corrupted.");
        message_box_1.setInformativeText("Do you want to retry?");
        message_box_1.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
        message_box_1.setDefaultButton(QMessageBox::Yes);
        int retour = message_box_1.exec();

        switch(retour)
        {
        case QMessageBox::Yes :
            open();
            return;
            break;

        case QMessageBox::No :
            return;
            break;

        default:
            std::cout << "ERROR in Top_Menu::open: should never have reached here" << std::endl;
            throw(QString("ERROR in Top_Menu::open: should never have reached here"));
        }
    }
    else if(load_success == 2)
    {
        QMessageBox message_box_1;
        message_box_1.setWindowTitle("Open file...");
        message_box_1.setIcon(QMessageBox::Warning);
        message_box_1.setText("The selected file is not a valid CP file.");
        message_box_1.setInformativeText("Do you want to retry?");
        message_box_1.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
        message_box_1.setDefaultButton(QMessageBox::Yes);
        int retour = message_box_1.exec();

        switch(retour)
        {
        case QMessageBox::Yes :
            open();
            return;
            break;

        case QMessageBox::No :
            return;
            break;

        default:
            std::cout << "ERROR in Top_Menu::open: should never have reached here" << std::endl;
            throw(QString("ERROR in Top_Menu::open: should never have reached here"));
        }
    }

    load_file.close();
    return;
}


void Top_Menu::help()
{
    Help_Browser::show_page();
    return;
}


void Top_Menu::about()
{
    QString text(tr("<p> Circle Packings v1.0" \
                    "<p> Created by Benjamin Beeker and Brice Loustau " \
                    "<br> (using Qt framework)" \
                    "<p> Based on an algorithm by C. R. Collins and K. Stephenson"));
    QMessageBox::about(this, "About", text);
    return;
}
