#ifndef PLANET_H
#define PLANET_H

#include <string>
#include <QString>

class Planet
{
private:
	std::string name = "";
	long double diameter = 0;
	long double mass = 0;
	long double period = 0;
	long double orbital_radius = 0;

public:
	std::string get_name()
	{
		return name;
	}
	void set_name(std::string n)
	{
		name = n;
	}

	long double get_diameter()
	{
		return diameter;
	}
	void set_diameter(long double v)
	{
		diameter = v;
	}

	long double get_mass()
	{
		return mass;
	}
	void set_mass(long double v)
	{
		mass = v;
	}

	long double get_period()
	{
		return period;
	}
	void set_period(long double v)
	{
		period = v;
	}

	long double get_orbital_radius()
	{
		return orbital_radius;
	}
	void set_orbital_radius(long double v)
	{
		orbital_radius = v;
	}

	Planet(std::string n, long double d, long double m, long double p, long double o_r) : name{ n }, diameter{ d }, mass{ m }, period{ p },
		orbital_radius{ o_r } {
	};

	std::string toStringPlanet(long double earth_mass)
	{
		std::string s;
		s += name + ":";
		s += "Diameter---" + std::to_string(diameter) + " km, ";
		if (name == "Earth")
		{
			s += "Mass---" + QString::number(mass, 'E', 6).toStdString() + " kg";
		}
		else
		{
			s += "Mass---" + std::to_string(mass / earth_mass) + " Earths";
		}
		return s;
	}

	std::string toStringSolarSystem(long double AU)
	{
		std::string s;
		s += name + ":";
		s += "Period---" + std::to_string(period) + " days, ";
		s += "Orbital radius---" + std::to_string(orbital_radius / AU) + " AU";
		return s;
	}
};

#endif