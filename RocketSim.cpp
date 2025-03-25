#include "RocketSim.h"

#include <QGridLayout>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPainter>
#include <QMessageBox>

RocketSim::RocketSim(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    positions_window = nullptr;
    trajectory_window = nullptr;

    wrapper = new QWidget();
    grid = new QVBoxLayout();
    wrapper->setLayout(grid);

    QWidget* spacer;

    QWidget* superframe1 = new QWidget();
    QHBoxLayout* hbox_layout_superframe_1 = new QHBoxLayout();
    superframe1->setLayout(hbox_layout_superframe_1);
    QWidget* frame1 = new QWidget();
    frame1->setStyleSheet("QWidget {border : 1px solid black; } QPushButton {background-color: #ADD8E6;}");
    QVBoxLayout* vbox_layout_frame_1 = new QVBoxLayout();
    frame1->setLayout(vbox_layout_frame_1);
    QWidget* frame2 = new QWidget();
    frame2->setStyleSheet("QWidget {border : 1px solid black; } QPushButton {background-color: #ADD8E6;}");
    QVBoxLayout* vbox_layout_frame_2 = new QVBoxLayout();
    frame2->setLayout(vbox_layout_frame_2);
    QWidget* frame3 = new QWidget();
    frame3->setStyleSheet("QWidget {border : 1px solid black; } QPushButton {background-color: #ADD8E6;}");
    QVBoxLayout* vbox_layout_frame_3 = new QVBoxLayout();
    frame3->setLayout(vbox_layout_frame_3);

    hbox_layout_superframe_1->addWidget(frame1);
    hbox_layout_superframe_1->addWidget(frame2);
    hbox_layout_superframe_1->addWidget(frame3);
    grid->addWidget(superframe1);

    label_selector_1 = new QLabel(QString("--- ROCKET DATA ---"));
    vbox_layout_frame_1->addWidget(label_selector_1);
    label_2_selector_1 = new QLabel(QString("Rocket data"));
    vbox_layout_frame_1->addWidget(label_2_selector_1);

    label_selector_2 = new QLabel(QString("Planetary data"));
    vbox_layout_frame_2->addWidget(label_selector_2);
    table_selector_2 = new QTableWidget();
    vbox_layout_frame_2->addWidget(table_selector_2);

    label_selector_3 = new QLabel(QString("Solar System data"));
    vbox_layout_frame_3->addWidget(label_selector_3);
    table_selector_3 = new QTableWidget();
    vbox_layout_frame_3->addWidget(table_selector_3);

    button_selector_1 = new QPushButton(QString("Click to load new rocket data"));
    vbox_layout_frame_1->addWidget(button_selector_1);

    QObject::connect(button_selector_1, &QPushButton::pressed, &backend, &Backend::load_rocket_data);
    QObject::connect(button_selector_1, &QPushButton::pressed, this, &RocketSim::update_displyed_data);

    button_selector_2 = new QPushButton(QString("Click to load new planetary data"));
    vbox_layout_frame_2->addWidget(button_selector_2);

    QObject::connect(button_selector_2, &QPushButton::pressed, &backend, &Backend::load_planetary_data);
    QObject::connect(button_selector_2, &QPushButton::pressed, this, &RocketSim::update_displyed_data);

    button_selector_3 = new QPushButton(QString("Click to load new Solar System data"));
    vbox_layout_frame_3->addWidget(button_selector_3);

    QObject::connect(button_selector_3, &QPushButton::pressed, &backend, &Backend::load_solar_system_data);
    QObject::connect(button_selector_3, &QPushButton::pressed, this, &RocketSim::update_displyed_data);

    QWidget* frame6 = new QWidget();
    frame6->setStyleSheet("QWidget {border : 1px solid black; }");
    QVBoxLayout* vbox_layout_frame_6 = new QVBoxLayout();
    frame6->setLayout(vbox_layout_frame_6);
    label_planets_computations = new QLabel("Planet computations");
    vbox_layout_frame_6->addWidget(label_planets_computations);
    table_planet_computations = new QTableWidget();
    vbox_layout_frame_6->addWidget(table_planet_computations);
    grid->addWidget(frame6);

    QWidget* frame4 = new QWidget();
    QHBoxLayout* hbox_layout_frame_4 = new QHBoxLayout();
    frame4->setLayout(hbox_layout_frame_4);
    frame4->setStyleSheet("QWidget {border : 1px solid black; } QPushButton {background-color: #ADD8E6;}");

    line_edit_time_for_rotation = new QLineEdit();
    hbox_layout_frame_4->addWidget(line_edit_time_for_rotation);

    button_time_for_rotation = new QPushButton(QString::fromStdString("Input a time in days and click to calculate\nplanet positions after that time passed"));
    hbox_layout_frame_4->addWidget(button_time_for_rotation);

    QObject::connect(button_time_for_rotation, &QPushButton::pressed, this, &RocketSim::update_rotation_positions);

    grid->addWidget(frame4);

    QWidget* frame5 = new QWidget();
    QHBoxLayout* hbox_layout_frame_5 = new QHBoxLayout();
    frame5->setLayout(hbox_layout_frame_5);
    frame5->setStyleSheet("QWidget {border : 1px solid black; } QPushButton {background-color: #ADD8E6;}");
    grid->addWidget(frame5);

    line_edit_planet_1 = new QLineEdit();
    hbox_layout_frame_5->addWidget(line_edit_planet_1);

    line_edit_planet_2 = new QLineEdit();
    hbox_layout_frame_5->addWidget(line_edit_planet_2);

    button_compute_trajectory = new QPushButton("Input two planets and click to compute the three journeys' data");
    hbox_layout_frame_5->addWidget(button_compute_trajectory);

    QObject::connect(button_compute_trajectory, &QPushButton::pressed, this, &RocketSim::update_trajectory_calculations);

    this->setCentralWidget(wrapper);

    this->update_displyed_data();
}

RocketSim::~RocketSim()
{}

void RocketSim::update_displyed_data()
{
    std::string s;
    s += "--- ROCKET DATA ---\n";
    if (backend.default_rocket)
    {
        s += "Caution! This data is the application default,\nyou might want to load your own data\n";
    }
    label_selector_1->setText(QString::fromStdString(s));

    s = "";
    s += "Number of engines --- " + std::to_string(backend.number_of_engines) + "\n";
    s += "Acceleration per engine --- " + std::to_string(backend.acceleration_per_engine);
    int pos = s.size() - 1;
    while (s.at(pos) == '0')
    {
        s.erase(pos, std::string::npos);
        pos -= 1;
    }
    s += "0 m/s^2";
    label_2_selector_1->setText(QString::fromStdString(s));

    if (backend.default_planets)
    {
        label_selector_2->setText(QString::fromStdString(" --- PLANETARY DATA ---\nCaution! This data is the application default,\nyou might want to load your own data\n"));
    }
    else
    {
        label_selector_2->setText(QString::fromStdString(" --- PLANETARY DATA ---\n"));
    }

    if (backend.default_solar_system)
    {
        label_selector_3->setText(QString::fromStdString(" --- SOLAR SYSTEM DATA ---\nCaution! This data is the application default,\nyou might want to load your own data\n"));
    }
    else
    {
        label_selector_3->setText(QString::fromStdString(" --- SOLAR SYSTEM DATA ---\n"));
    }

    label_planets_computations->setText(QString::fromStdString(" --- PLANETARY COMPUTATIONS --- \n"));

    //fill the 3 tables
    table_selector_2->setRowCount(9);
    table_selector_2->setColumnCount(2);

    table_selector_2->setHorizontalHeaderItem(0, new QTableWidgetItem(QString::fromStdString("Diameter")));
    table_selector_2->setHorizontalHeaderItem(1, new QTableWidgetItem(QString::fromStdString("Mass")));

    int counter = 0;
    for (auto& each : backend.planets)
    {
        table_selector_2->setVerticalHeaderItem(counter, new QTableWidgetItem(QString::fromStdString(each.get_name())));
        std::string s = std::to_string(each.get_diameter());
        int pos = s.size() - 1;
        while (s.at(pos) == '0')
        {
            s.erase(pos, std::string::npos);
            pos -= 1;
        }
        s += "0 km";
        table_selector_2->setItem(counter, 0, new QTableWidgetItem(QString::fromStdString(s)));

        s = std::to_string(each.get_mass() / backend.planets.at(2).get_mass());
        if (each.get_name() != "Earth")
        {
            int pos = s.size() - 1;
            while (s.at(pos) == '0')
            {
                s.erase(pos, std::string::npos);
                pos -= 1;
            }
            s += "0 Earths";
        }
        else
        {
            s = QString::number(each.get_mass(), 'E', 2).toStdString() + " kg";
        }
        table_selector_2->setItem(counter, 1, new QTableWidgetItem(QString::fromStdString(s)));

        counter += 1;
    }

    table_selector_3->setRowCount(9);
    table_selector_3->setColumnCount(2);

    table_selector_3->setHorizontalHeaderItem(0, new QTableWidgetItem(QString::fromStdString("Period")));
    table_selector_3->setHorizontalHeaderItem(1, new QTableWidgetItem(QString::fromStdString("Orbital radius")));

    counter = 0;
    for (auto& each : backend.planets)
    {
        table_selector_3->setVerticalHeaderItem(counter, new QTableWidgetItem(QString::fromStdString(each.get_name())));
        std::string s = std::to_string(each.get_period());
        int pos = s.size() - 1;
        while (s.at(pos) == '0')
        {
            s.erase(pos, std::string::npos);
            pos -= 1;
        }
        s += "0 days";
        table_selector_3->setItem(counter, 0, new QTableWidgetItem(QString::fromStdString(s)));

        s = std::to_string(each.get_orbital_radius() / backend.AU);
        pos = s.size() - 1;
        while (s.at(pos) == '0')
        {
            s.erase(pos, std::string::npos);
            pos -= 1;
        }
        s += "0 AU";
        table_selector_3->setItem(counter, 1, new QTableWidgetItem(QString::fromStdString(s)));

        counter += 1;
    }

    table_planet_computations->setRowCount(3);
    table_planet_computations->setColumnCount(9);

    table_planet_computations->setVerticalHeaderItem(0, new QTableWidgetItem(QString::fromStdString("Escape velocity")));
    table_planet_computations->setVerticalHeaderItem(1, new QTableWidgetItem(QString::fromStdString("Escape time")));
    table_planet_computations->setVerticalHeaderItem(2, new QTableWidgetItem(QString::fromStdString("Escape distance")));

    counter = 0;
    for (auto& each : backend.planets)
    {
        table_planet_computations->setHorizontalHeaderItem(counter, new QTableWidgetItem(QString::fromStdString(each.get_name())));
        std::string s = std::to_string(sqrt(static_cast<long double>(2 * backend.G * each.get_mass() / (each.get_diameter() * 500))) / 1000);
        int pos = s.size() - 5;
        s.erase(pos, std::string::npos);
        s += " km/s";
        table_planet_computations->setItem(0, counter, new QTableWidgetItem(QString::fromStdString(s)));

        s = std::to_string(static_cast<long double>(sqrt(static_cast<long double>(2 * backend.G * each.get_mass() / (each.get_diameter() * 500))) / (backend.number_of_engines * backend.acceleration_per_engine)));
        pos = s.size() - 5;
        s.erase(pos, std::string::npos);
        s += " s";
        table_planet_computations->setItem(1, counter, new QTableWidgetItem(QString::fromStdString(s)));

        long double time = static_cast<long double>(sqrt(static_cast<long double>(2 * backend.G * each.get_mass() / (each.get_diameter() * 500))) / (backend.number_of_engines * backend.acceleration_per_engine));
        s = std::to_string(static_cast<long double>(backend.acceleration_per_engine * backend.number_of_engines * time * time / 2000));
        pos = s.size() - 5;
        s.erase(pos, std::string::npos);
        s += " km";
        table_planet_computations->setItem(2, counter, new QTableWidgetItem(QString::fromStdString(s)));

        counter += 1;
    }

    line_edit_time_for_rotation->setText(QString::fromStdString(""));
    line_edit_planet_1->setText(QString::fromStdString(""));
    line_edit_planet_2->setText(QString::fromStdString(""));

    //clear any pictures
    if (positions_window != nullptr)
    {
        positions_window->done(0);
        delete(positions_window);
        positions_window = nullptr;
    }

    if (trajectory_window != nullptr)
    {
        trajectory_window->done(0);
        delete(trajectory_window);
        trajectory_window = nullptr;
    }
}

class Draw_planet_positions : public QWidget
{
public:
    Draw_planet_positions(QWidget* parent = nullptr) : QWidget(parent) {}

protected:
    void paintEvent(QPaintEvent* event) override {
        if (polar_positions.size() == 9)
        {
            QPainter painter(this);

            long double canvas_size = 700;
            long double scaling_factor = canvas_size / 2.0 / polar_positions.at(8).second;
            long double start_point = canvas_size / 2 + 50;

            painter.fillRect(rect(), Qt::black);

            for (int i = 0; i < 9; ++i)
            {
                long double orbit_size = 2.0 * scaling_factor * polar_positions.at(i).second;

                painter.setPen(QPen(Qt::white, 1));
                painter.drawEllipse(start_point - scaling_factor * polar_positions.at(i).second, start_point - scaling_factor * polar_positions.at(i).second, orbit_size, orbit_size);
            }

            for (int i = 0; i < 9; ++i)
            {
                long double orbit_size = 2.0 * scaling_factor * polar_positions.at(i).second;

                painter.setPen(QPen());
                if (i == 0)
                {
                    painter.setBrush(QBrush(QColorConstants::Svg::darkgreen, Qt::SolidPattern));
                }
                if (i == 1)
                {
                    painter.setBrush(QBrush(Qt::yellow, Qt::SolidPattern));
                }
                if (i == 2)
                {
                    painter.setBrush(QBrush(Qt::blue, Qt::SolidPattern));
                }
                if (i == 3)
                {
                    painter.setBrush(QBrush(Qt::red, Qt::SolidPattern));
                }
                if (i == 4)
                {
                    painter.setBrush(QBrush(QColorConstants::Svg::darkorange, Qt::SolidPattern));
                }
                if (i == 5)
                {
                    painter.setBrush(QBrush(QColorConstants::Svg::gold, Qt::SolidPattern));
                }
                if (i == 6)
                {
                    painter.setBrush(QBrush(QColorConstants::Svg::darkblue, Qt::SolidPattern));
                }
                if (i == 7)
                {
                    painter.setBrush(QBrush(QColorConstants::Svg::aqua, Qt::SolidPattern));
                }
                if (i == 8)
                {
                    painter.setBrush(QBrush(QColorConstants::Svg::darkgray, Qt::SolidPattern));
                }
                painter.drawEllipse(start_point - 4 + cos(polar_positions.at(i).first * M_PI / 180.0) * scaling_factor * (polar_positions.at(i).second), start_point - 4 + sin(polar_positions.at(i).first * M_PI / 180.0) * scaling_factor * (polar_positions.at(i).second), 8, 8);
                painter.setBrush(QBrush());
            }
        }
    }
public:
    std::vector<std::pair<long double, long double>> polar_positions; //angle in degrees and radius in AU
    void add_position(std::pair<long double, long double> where)
    {
        polar_positions.push_back(where);
    }
};

class Draw_trajectory : public QWidget
{
public:
    Draw_trajectory(QWidget* parent = nullptr) : QWidget(parent) {}

protected:
    void paintEvent(QPaintEvent* event) override {
        if (polar_positions.size() == 9)
        {
            QPainter painter(this);

            long double canvas_size = 700;
            long double scaling_factor = canvas_size / 2.0 / polar_positions.at(8).second;
            long double start_point = canvas_size / 2 + 50;

            painter.fillRect(rect(), Qt::black);

            for (int i = 0; i < 9; ++i)
            {
                long double orbit_size = 2.0 * scaling_factor * polar_positions.at(i).second;

                painter.setPen(QPen(Qt::white, 1));
                painter.drawEllipse(start_point - scaling_factor * polar_positions.at(i).second, start_point - scaling_factor * polar_positions.at(i).second, orbit_size, orbit_size);
            }

            for (int i = 0; i < 9; ++i)
            {
                long double orbit_size = 2.0 * scaling_factor * polar_positions.at(i).second;

                painter.setPen(QPen());
                if (i == 0)
                {
                    painter.setBrush(QBrush(QColorConstants::Svg::darkgreen, Qt::SolidPattern));
                }
                if (i == 1)
                {
                    painter.setBrush(QBrush(Qt::yellow, Qt::SolidPattern));
                }
                if (i == 2)
                {
                    painter.setBrush(QBrush(Qt::blue, Qt::SolidPattern));
                }
                if (i == 3)
                {
                    painter.setBrush(QBrush(Qt::red, Qt::SolidPattern));
                }
                if (i == 4)
                {
                    painter.setBrush(QBrush(QColorConstants::Svg::darkorange, Qt::SolidPattern));
                }
                if (i == 5)
                {
                    painter.setBrush(QBrush(QColorConstants::Svg::gold, Qt::SolidPattern));
                }
                if (i == 6)
                {
                    painter.setBrush(QBrush(QColorConstants::Svg::darkblue, Qt::SolidPattern));
                }
                if (i == 7)
                {
                    painter.setBrush(QBrush(QColorConstants::Svg::aqua, Qt::SolidPattern));
                }
                if (i == 8)
                {
                    painter.setBrush(QBrush(QColorConstants::Svg::darkgray, Qt::SolidPattern));
                }
                painter.drawEllipse(start_point - 4 + cos(polar_positions.at(i).first * M_PI / 180.0) * scaling_factor * (polar_positions.at(i).second), start_point - 4 + sin(polar_positions.at(i).first * M_PI / 180.0) * scaling_factor * (polar_positions.at(i).second), 8, 8);
                painter.setBrush(QBrush());
            }
            painter.setPen(Qt::red);
            painter.drawLine(start_point + cos(polar_positions.at(p_1).first * M_PI / 180.0) * scaling_factor * (polar_positions.at(p_1).second), start_point + sin(polar_positions.at(p_1).first * M_PI / 180.0) * scaling_factor * (polar_positions.at(p_1).second), start_point + cos(polar_positions.at(p_2).first * M_PI / 180.0) * scaling_factor * (polar_positions.at(p_2).second), start_point + sin(polar_positions.at(p_2).first * M_PI / 180.0) * scaling_factor * (polar_positions.at(p_2).second));
        }
    }
public:
    std::vector<std::pair<long double, long double>> polar_positions; //angle in degrees and radius in AU
    int p_1 = 0, p_2 = 0;
    void add_position(std::pair<long double, long double> where)
    {
        polar_positions.push_back(where);
    }
    void set_planet_counters(int p1, int p2)
    {
        p_1 = p1;
        p_2 = p2;
    }
};

void RocketSim::update_rotation_positions()
{
    std::pair<std::string, std::vector<long double>> ret = backend.get_positions_after_time(line_edit_time_for_rotation->text().toStdString());

    if (ret.first != "")
    {
        if (positions_window != nullptr)
        {
            positions_window->done(0);
            delete(positions_window);
            positions_window = nullptr;
        }
        positions_window = new QDialog(this);
        positions_window->setFixedSize(1200, 800);
        
        positions_window->setLayout(new QHBoxLayout());
        Draw_planet_positions* dp = new Draw_planet_positions();
        for (int i = 0; i < 9; ++i)
        {
            dp->add_position(std::make_pair(ret.second.at(i), backend.planets.at(i).get_orbital_radius() / backend.AU));
        }
        (dynamic_cast<QHBoxLayout*>(positions_window->layout()))->addWidget(dp);

        QTableWidget* legend = new QTableWidget();
        legend->setRowCount(9);
        legend->setColumnCount(2);
        legend->setHorizontalHeaderItem(0, new QTableWidgetItem(QString("Color")));
        legend->setHorizontalHeaderItem(1, new QTableWidgetItem(QString("Angle")));
        dp->setLayout(new QGridLayout());
        (dynamic_cast<QGridLayout*>(dp->layout()))->addWidget(legend, 0, 0, 3, 1, Qt::AlignTop | Qt::AlignRight);
        legend->setFixedSize(300, 300);
        legend->setVerticalHeaderItem(0, new QTableWidgetItem(QString("Mercury")));
        legend->setVerticalHeaderItem(1, new QTableWidgetItem(QString("Venus")));
        legend->setVerticalHeaderItem(2, new QTableWidgetItem(QString("Earth")));
        legend->setVerticalHeaderItem(3, new QTableWidgetItem(QString("Mars")));
        legend->setVerticalHeaderItem(4, new QTableWidgetItem(QString("Jupiter")));
        legend->setVerticalHeaderItem(5, new QTableWidgetItem(QString("Saturn")));
        legend->setVerticalHeaderItem(6, new QTableWidgetItem(QString("Uranus")));
        legend->setVerticalHeaderItem(7, new QTableWidgetItem(QString("Neptune")));
        legend->setVerticalHeaderItem(8, new QTableWidgetItem(QString("Pluto")));

        QTableWidgetItem* item = new QTableWidgetItem();
        item->setBackground(QBrush(QColorConstants::Svg::darkgreen));
        legend->setItem(0, 0, item);
        item = new QTableWidgetItem();
        item->setBackground(QBrush(Qt::yellow));
        legend->setItem(1, 0, item);
        item = new QTableWidgetItem();
        item->setBackground(QBrush(Qt::blue));
        legend->setItem(2, 0, item);
        item = new QTableWidgetItem();
        item->setBackground(QBrush(Qt::red));
        legend->setItem(3, 0, item);
        item = new QTableWidgetItem();
        item->setBackground(QBrush(QColorConstants::Svg::darkorange));
        legend->setItem(4, 0, item);
        item = new QTableWidgetItem();
        item->setBackground(QBrush(QColorConstants::Svg::gold));
        legend->setItem(5, 0, item);
        item = new QTableWidgetItem();
        item->setBackground(QBrush(QColorConstants::Svg::darkblue));
        legend->setItem(6, 0, item);
        item = new QTableWidgetItem();
        item->setBackground(QBrush(QColorConstants::Svg::aqua));
        legend->setItem(7, 0, item);
        item = new QTableWidgetItem();
        item->setBackground(QBrush(QColorConstants::Svg::darkgrey));
        legend->setItem(8, 0, item);

        for (int i = 0; i < 9; ++i)
        {
            std::string transformed = std::to_string(ret.second.at(i));
            int pos = transformed.size() - 5;
            transformed.erase(pos, std::string::npos);
            transformed += "0 degrees";
            legend->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(transformed)));
        }

        positions_window->show();
        positions_window->update();
    }
}

void RocketSim::update_trajectory_calculations()
{
    std::string planet_1 = line_edit_planet_1->text().toStdString();
    std::string planet_2 = line_edit_planet_2->text().toStdString();

    std::pair<std::vector<std::string>, std::vector<long double>> ret = backend.get_trajectories(planet_1, planet_2, this);

    if (ret.first.size() != 0)
    {
        if (trajectory_window != nullptr)
        {
            trajectory_window->done(0);
            delete(trajectory_window);
            trajectory_window = nullptr;
        }
        trajectory_window = new QDialog(this);
        trajectory_window->setFixedSize(1200, 800);

        trajectory_window->setLayout(new QHBoxLayout());
        Draw_trajectory* dt = new Draw_trajectory();
        for (int i = 0; i < 9; ++i)
        {
            dt->add_position(std::make_pair(ret.second.at(i), backend.planets.at(i).get_orbital_radius() / backend.AU));
        }
        dt->set_planet_counters(ret.second.at(9), ret.second.at(10));
        (dynamic_cast<QHBoxLayout*>(trajectory_window->layout()))->addWidget(dt);

        QTableWidget* legend = new QTableWidget();
        legend->setRowCount(9);
        legend->setColumnCount(2);
        legend->setHorizontalHeaderItem(0, new QTableWidgetItem(QString("Color")));
        legend->setHorizontalHeaderItem(1, new QTableWidgetItem(QString("Angle")));
        dt->setLayout(new QGridLayout());
        (dynamic_cast<QGridLayout*>(dt->layout()))->addWidget(legend, 0, 0, Qt::AlignTop | Qt::AlignRight);
        legend->setFixedSize(300, 300);
        legend->setVerticalHeaderItem(0, new QTableWidgetItem(QString("Mercury")));
        legend->setVerticalHeaderItem(1, new QTableWidgetItem(QString("Venus")));
        legend->setVerticalHeaderItem(2, new QTableWidgetItem(QString("Earth")));
        legend->setVerticalHeaderItem(3, new QTableWidgetItem(QString("Mars")));
        legend->setVerticalHeaderItem(4, new QTableWidgetItem(QString("Jupiter")));
        legend->setVerticalHeaderItem(5, new QTableWidgetItem(QString("Saturn")));
        legend->setVerticalHeaderItem(6, new QTableWidgetItem(QString("Uranus")));
        legend->setVerticalHeaderItem(7, new QTableWidgetItem(QString("Neptune")));
        legend->setVerticalHeaderItem(8, new QTableWidgetItem(QString("Pluto")));

        QTableWidgetItem* item = new QTableWidgetItem();
        item->setBackground(QBrush(QColorConstants::Svg::darkgreen));
        legend->setItem(0, 0, item);
        item = new QTableWidgetItem();
        item->setBackground(QBrush(Qt::yellow));
        legend->setItem(1, 0, item);
        item = new QTableWidgetItem();
        item->setBackground(QBrush(Qt::blue));
        legend->setItem(2, 0, item);
        item = new QTableWidgetItem();
        item->setBackground(QBrush(Qt::red));
        legend->setItem(3, 0, item);
        item = new QTableWidgetItem();
        item->setBackground(QBrush(QColorConstants::Svg::darkorange));
        legend->setItem(4, 0, item);
        item = new QTableWidgetItem();
        item->setBackground(QBrush(QColorConstants::Svg::gold));
        legend->setItem(5, 0, item);
        item = new QTableWidgetItem();
        item->setBackground(QBrush(QColorConstants::Svg::darkblue));
        legend->setItem(6, 0, item);
        item = new QTableWidgetItem();
        item->setBackground(QBrush(QColorConstants::Svg::aqua));
        legend->setItem(7, 0, item);
        item = new QTableWidgetItem();
        item->setBackground(QBrush(QColorConstants::Svg::darkgrey));
        legend->setItem(8, 0, item);
        //legend->selectRow(ret.second.at(9));
        //legend->selectRow(ret.second.at(10));

        for (int i = 0; i < 9; ++i)
        {
            std::string transformed = std::to_string(ret.second.at(i));
            int pos = transformed.size() - 5;
            transformed.erase(pos, std::string::npos);
            transformed += "0 degrees";
            legend->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(transformed)));
        }

        //add a journey table
        QTableWidget* table_journey = new QTableWidget();
        (dynamic_cast<QGridLayout*>(dt->layout()))->addWidget(table_journey, 2, 0, Qt::AlignTop | Qt::AlignRight);
        table_journey->setRowCount(7);
        table_journey->setColumnCount(2);
        table_journey->setHorizontalHeaderItem(0, new QTableWidgetItem(QString("Straight journey")));
        table_journey->setHorizontalHeaderItem(1, new QTableWidgetItem(QString("Dynamic journey")));

        table_journey->setFixedSize(400, 280);
        table_journey->setColumnWidth(0, 130);
        table_journey->setColumnWidth(1, 130);

        table_journey->setVerticalHeaderItem(0, new QTableWidgetItem(QString("Total time")));
        table_journey->setVerticalHeaderItem(1, new QTableWidgetItem(QString("Cruising time")));
        table_journey->setVerticalHeaderItem(2, new QTableWidgetItem(QString("Acceleration time")));
        table_journey->setVerticalHeaderItem(3, new QTableWidgetItem(QString("Acceleration distance")));
        table_journey->setVerticalHeaderItem(4, new QTableWidgetItem(QString("Deceleration time")));
        table_journey->setVerticalHeaderItem(5, new QTableWidgetItem(QString("Deceleration distance")));
        table_journey->setVerticalHeaderItem(6, new QTableWidgetItem(QString("Start time")));

        table_journey->setItem(0, 0, new QTableWidgetItem(QString::fromStdString(ret.first.at(0))));
        table_journey->setItem(1, 0, new QTableWidgetItem(QString::fromStdString(ret.first.at(3))));
        table_journey->setItem(2, 0, new QTableWidgetItem(QString::fromStdString(ret.first.at(1))));
        table_journey->setItem(3, 0, new QTableWidgetItem(QString::fromStdString(ret.first.at(2))));
        table_journey->setItem(4, 0, new QTableWidgetItem(QString::fromStdString(ret.first.at(1))));
        table_journey->setItem(5, 0, new QTableWidgetItem(QString::fromStdString(ret.first.at(2))));
        table_journey->setItem(6, 0, new QTableWidgetItem(QString::fromStdString("N/A")));

        table_journey->setItem(0, 1, new QTableWidgetItem(QString::fromStdString(ret.first.at(9))));
        table_journey->setItem(1, 1, new QTableWidgetItem(QString::fromStdString(ret.first.at(8))));
        table_journey->setItem(2, 1, new QTableWidgetItem(QString::fromStdString(ret.first.at(1))));
        table_journey->setItem(3, 1, new QTableWidgetItem(QString::fromStdString(ret.first.at(2))));
        table_journey->setItem(4, 1, new QTableWidgetItem(QString::fromStdString(ret.first.at(1))));
        table_journey->setItem(5, 1, new QTableWidgetItem(QString::fromStdString(ret.first.at(2))));
        table_journey->setItem(6, 1, new QTableWidgetItem(QString::fromStdString(ret.first.at(4))));

        trajectory_window->show();
        trajectory_window->update();
    }
}
