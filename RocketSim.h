#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_RocketSim.h"
#include "Backend.h"

#include <QGridLayout>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>

class RocketSim : public QMainWindow
{
    Q_OBJECT

public:
    RocketSim(QWidget *parent = nullptr);
    ~RocketSim();

private:
    Ui::RocketSimClass ui;

    Backend backend = Backend();

    QWidget* wrapper;
    QGridLayout* grid;
    QLabel* label_selector_1;
    QLabel* label_selector_2;
    QLabel* label_selector_3;
    QPushButton* button_selector_1;
    QPushButton* button_selector_2;
    QPushButton* button_selector_3;
    QLabel* label_planets_computations;
    QLineEdit* line_edit_time_for_rotation;
    QPushButton* button_time_for_rotation;
    QLabel* label_time_for_rotation;
    QLineEdit* line_edit_planet_1;
    QLineEdit* line_edit_planet_2;
    QPushButton* button_compute_trajectory;
    QLabel* label_compute_trajectory;

    void update_displyed_data();
    void update_rotation_positions();
    void update_trajectory_calculations();
};
