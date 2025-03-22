#pragma once
//CAREFUL - some .h in this project have ifndef, some have pragma once
//Q_OBJECT macro only seems to work with pragma once

#include <vector>
#include "Planet.h"
#include <QObject>

class Backend : public QObject
{
	Q_OBJECT

	//UNITS OF MEASUREMENT FOR THIS DATA:
	//number_of_engines - unitless positive integer
	//acceleration_per_engine - m/s^2 positive real number
	//earth_mass - kg positive real number
	//AU - km positive real number
	//planetary diameters - km positive real number
	//planetary masses - kg positive real number
	//planetary periods - days positive real number
	//planetary orbital radii - km positive real number

private:
	std::vector<Planet> planets;
	int number_of_engines;
	long double acceleration_per_engine;

	long double earth_mass;
	long double AU;
	long double G;

	bool default_planets = true;
	bool default_rocket = true;
	bool default_solar_system = true;

public:
	Backend();

	void pop_up(std::string s);

	int load_rocket_data();
	int load_planetary_data();
	int load_solar_system_data();

	std::string get_rocket_data();
	std::string get_planetary_data();
	std::string get_solar_system_data();

	std::string get_planetary_computations();
	std::string get_positions_after_time(std::string time);
	std::string get_trajectories(std::string planet_1, std::string planet_2);
};