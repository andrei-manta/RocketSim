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
}