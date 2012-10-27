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

#include "circle_packings_application.hpp"
#include "window.hpp"
#include "canvas.hpp"
#include "canvas_delegate.hpp"

#include <QMessageBox>
#include <QPlastiqueStyle>

Circle_Packings_Application::Circle_Packings_Application(int& argc, char ** argv) : QApplication(argc, argv)
{
    //setStyle(new QPlastiqueStyle);

    error_caught_ = false;
    window_ = new Window;
    connect(window_, SIGNAL(destroyed()), this, SLOT(quit()));
    window_->setAttribute(Qt::WA_DeleteOnClose, true);
    window_->show();
    /*window_->enable_canvases_updates(true);
    window_->left_canvas_->repaint();
    window_->right_canvas_->repaint();
    window_->enable_canvases_updates(false);*/
}

Circle_Packings_Application::~Circle_Packings_Application()
{

}

bool Circle_Packings_Application::notify(QObject * receiver, QEvent * event)
{
    try
    {
        return QApplication::notify(receiver, event);
    }
    catch(QString error_message)
    {
        if (error_caught_)
        {
            return QApplication::notify(receiver, event);
        }
        else
        {
            disconnect(window_, SIGNAL(destroyed()), this, SLOT(quit()));
            error_caught_ = true;
            window_->setEnabled(false);

            switch (show_dialog_box(error_message))
            {
            case EXIT:
                exit_choice();
                break;

            case RESTART:
                restart_choice();
                break;

            case RESTORE:
                restore_choice();
                break;

            case IGNORE:
                ignore_choice();
                break;

            default:
                std::cout << "ERROR in Circle_Packings_Application::notify: should never have reached here" << std::endl;
            }
        }
    }
    return false;
}

user_choice Circle_Packings_Application::show_dialog_box(QString error_message)
{
    QString oops = QString(tr("<b> Oops! </b>"));
    QString sentence = QString(tr("The program caught an unexpected error:"));

    QString message = oops
            .append(QString(tr("<br> <br>")))
            .append(sentence)
            .append(QString(tr("<br> <i>")))
            .append(error_message)
            .append(QString(tr("</i>")));

    QMessageBox message_box;
    message_box.setWindowTitle("Error");
    message_box.setIcon(QMessageBox::Critical);
    message_box.setText(message);
    message_box.setInformativeText("What do you want to do? <br>");


    QPushButton *restore_button = message_box.addButton(tr("Restore"), QMessageBox::RejectRole);
    QPushButton *ham_button = message_box.addButton(tr("Ham sandwich"), QMessageBox::RejectRole);
    QPushButton *ignore_button = message_box.addButton(tr("Ignore"), QMessageBox::RejectRole);
    QPushButton *restart_button = message_box.addButton(tr("Restart"), QMessageBox::RejectRole);
    QPushButton *exit_button = message_box.addButton(tr("Exit"), QMessageBox::RejectRole);

    message_box.setDefaultButton(restore_button);
    message_box.setModal(Qt::ApplicationModal);

    message_box.exec();

    if ((QPushButton *) message_box.clickedButton() == exit_button)
    {
        return EXIT;
    }
    else if ((QPushButton *) message_box.clickedButton() == restart_button)
    {
        return RESTART;
    }
    else if ((QPushButton *) message_box.clickedButton() == restore_button)
    {
        return RESTORE;
    }
    else if ((QPushButton *) message_box.clickedButton() == ham_button)
    {
        QMessageBox ham_message_box(&message_box);
        ham_message_box.setWindowTitle("Ham Sandwich");
        ham_message_box.setIcon(QMessageBox::Information);
        QString message = QString("Making a ham sandwich");
        ham_message_box.setText(message);
        ham_message_box.setInformativeText("<i>Waiting for ham in CD drive...</i>");
        ham_message_box.setStandardButtons(QMessageBox::Cancel);
        message_box.setModal(Qt::ApplicationModal);
        ham_message_box.exec();

        return show_dialog_box(error_message);
    }
    else if ((QPushButton *) message_box.clickedButton() == ignore_button)
    {
        return IGNORE;
    }
    else
    {
        std::cout << "ERROR in Circle_Packings_Application::show_dialog_box: should never have reached here" << std::endl;
        return -1;
    }
}

void Circle_Packings_Application::restore_choice()
{

    setQuitOnLastWindowClosed(false);
    window_->update_configuration();

    Configuration configuration_save = *(window_->configuration_);
    closeAllWindows();


    error_caught_ = false;

    window_ = new Window;
    connect(window_, SIGNAL(destroyed()), this, SLOT(quit()));
    window_->show();

    *(window_->configuration_) = configuration_save;
    window_->configuration_->reset_window_pointer(window_);
    window_->restore_configuration();



    setQuitOnLastWindowClosed(true);
    return;
}

void Circle_Packings_Application::exit_choice()
{
    window_->close();

    return;
}

void Circle_Packings_Application::restart_choice()
{
    setQuitOnLastWindowClosed(false);
    closeAllWindows();

    window_ = new Window;
    connect(window_, SIGNAL(destroyed()), this, SLOT(quit()));
    window_->show();

    window_->enable_canvases_updates(true);
    window_->left_canvas_->repaint();
    window_->right_canvas_->repaint();
    window_->enable_canvases_updates(false);

    error_caught_ = false;
    setQuitOnLastWindowClosed(true);

    return;
}

void Circle_Packings_Application::ignore_choice()
{
    error_caught_ = false;
    connect(window_, SIGNAL(destroyed()), this, SLOT(quit()));
    window_->setEnabled(true);
    window_->left_canvas_->canvas_delegate_->click_on_ = false;
    window_->right_canvas_->canvas_delegate_->click_on_ = false;
    return;
}
