#include "RocketSim.h"

#include <QGridLayout>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>

RocketSim::RocketSim(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    wrapper = new QWidget();
    QGridLayout* grid = new QGridLayout();

    label_selector_1 = new QLabel(QString("Rocket data"));
    grid->addWidget(label_selector_1, 0, 0, Qt::AlignCenter);

    label_selector_2 = new QLabel(QString("Planetary data"));
    grid->addWidget(label_selector_2, 0, 1, Qt::AlignCenter);

    label_selector_3 = new QLabel(QString("Solar System data"));
    grid->addWidget(label_selector_3, 0, 2, Qt::AlignCenter);

    button_selector_1 = new QPushButton(QString("Load new rocket data"));
    grid->addWidget(button_selector_1, 1, 0, Qt::AlignCenter);

    QObject::connect(button_selector_1, &QPushButton::pressed, &backend, &Backend::load_rocket_data);
    QObject::connect(button_selector_1, &QPushButton::pressed, this, &RocketSim::update_displyed_data);

    button_selector_2 = new QPushButton(QString("Load new planetary data"));
    grid->addWidget(button_selector_2, 1, 1, Qt::AlignCenter);

    QObject::connect(button_selector_2, &QPushButton::pressed, &backend, &Backend::load_planetary_data);
    QObject::connect(button_selector_2, &QPushButton::pressed, this, &RocketSim::update_displyed_data);

    button_selector_3 = new QPushButton(QString("Load new Solar System data"));
    grid->addWidget(button_selector_3, 1, 2, Qt::AlignCenter);

    QObject::connect(button_selector_3, &QPushButton::pressed, &backend, &Backend::load_solar_system_data);
    QObject::connect(button_selector_3, &QPushButton::pressed, this, &RocketSim::update_displyed_data);

    label_planets_computations = new QLabel("Planet computations");
    grid->addWidget(label_planets_computations, 2, 0, 1, 3, Qt::AlignCenter);

    line_edit_time_for_rotation = new QLineEdit();
    grid->addWidget(line_edit_time_for_rotation, 3, 0, Qt::AlignCenter);

    button_time_for_rotation = new QPushButton(QString::fromStdString("Input a time in days and click to calculate\nplanet positions after that time passed"));
    grid->addWidget(button_time_for_rotation, 3, 1, Qt::AlignCenter);

    QObject::connect(button_time_for_rotation, &QPushButton::pressed, this, &RocketSim::update_rotation_positions);

    label_time_for_rotation = new QLabel("Result: ");
    grid->addWidget(label_time_for_rotation, 3, 2, Qt::AlignCenter);

    line_edit_planet_1 = new QLineEdit();
    grid->addWidget(line_edit_planet_1, 4, 0, Qt::AlignCenter);

    line_edit_planet_2 = new QLineEdit();
    grid->addWidget(line_edit_planet_2, 4, 1, Qt::AlignCenter);

    button_compute_trajectory = new QPushButton("Input two planets and click to compute the three journeys' data");
    grid->addWidget(button_compute_trajectory, 4, 2, Qt::AlignCenter);

    QObject::connect(button_compute_trajectory, &QPushButton::pressed, this, &RocketSim::update_trajectory_calculations);

    label_compute_trajectory = new QLabel(QString::fromStdString("Result: "));
    grid->addWidget(label_compute_trajectory, 4, 3, Qt::AlignCenter);

    wrapper->setLayout(grid);
    this->setCentralWidget(wrapper);

    this->update_displyed_data();
}

RocketSim::~RocketSim()
{}

void RocketSim::update_displyed_data()
{
    label_selector_1->setText(QString::fromStdString(backend.get_rocket_data()));
    label_selector_2->setText(QString::fromStdString(backend.get_planetary_data()));
    label_selector_3->setText(QString::fromStdString(backend.get_solar_system_data()));
    label_planets_computations->setText(QString::fromStdString(backend.get_planetary_computations()));
    line_edit_time_for_rotation->setText(QString::fromStdString(""));
    label_time_for_rotation->setText(QString::fromStdString(""));
}

void RocketSim::update_rotation_positions()
{
    std::string ret = backend.get_positions_after_time(line_edit_time_for_rotation->text().toStdString());

    label_time_for_rotation->setText(QString::fromStdString("Result: " + ret));
}

void RocketSim::update_trajectory_calculations()
{
    std::string planet_1 = line_edit_planet_1->text().toStdString();
    std::string planet_2 = line_edit_planet_2->text().toStdString();
    std::string ret = backend.get_trajectories(planet_1, planet_2);

    label_compute_trajectory->setText(QString::fromStdString("Result: " + ret));
}