#include "Backend.h"
#include <iostream>
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <cmath>
#include <QGridLayout>
#include <QLabel>

Backend::Backend()
{
	//add some default values for the fields
	this->number_of_engines = 4;
	this->acceleration_per_engine = 10;

	this->earth_mass = 6 * pow(static_cast<long double>(10), static_cast<long double>(24));
	this->AU = 149597870.7;
	this->G = static_cast<long double>(6.67) * pow(static_cast<long double>(10), static_cast<long double>(-11));

	this->planets.push_back(Planet("Mercury", 4900, 0.06 * earth_mass, 88, 0.39 * AU));
	this->planets.push_back(Planet("Venus", 12100, 0.82 * earth_mass, 225, 0.72 * AU));
	this->planets.push_back(Planet("Earth", 12800, earth_mass, 365, AU));
	this->planets.push_back(Planet("Mars", 5800, 0.11 * earth_mass, 687, 1.52 * AU));
	this->planets.push_back(Planet("Jupiter", 142800, 318 * earth_mass, 4329, 5.2 * AU));
	this->planets.push_back(Planet("Saturn", 120000, 95 * earth_mass, 10753, 9.54 * AU));
	this->planets.push_back(Planet("Uranus", 52400, 15 * earth_mass, 30660, 19.18 * AU));
	this->planets.push_back(Planet("Neptune", 48400, 17 * earth_mass, 60148, 30.06 * AU));
	this->planets.push_back(Planet("Pluto", 2450, 0.002 * earth_mass, 90560, 39.6 * AU));

}

int Backend::load_rocket_data()
{
	//will return either - 
	//0 on no change because no file selected
	//1 on error in file contents
	//2 on success

	//format allowed - as in the model .txt, it allows for an endline on the second line
	//and trailing space on both lines; numbers must not start with a 0 and must
	//contain only digits

	try
	{
		QString file_path = QFileDialog::getOpenFileName(nullptr, "Open File", "", "*.txt");
		if (!file_path.isEmpty())
		{
			QFile file_descriptor = QFile(file_path);
			file_descriptor.open(QIODevice::ReadOnly | QIODevice::Text);
			QString message = file_descriptor.readAll();

			QStringTokenizer lines = message.tokenize(u'\n');
			if (lines.toContainer().size() != 2)
			{
				if (lines.toContainer().size() == 3)
				{
					if (lines.toContainer().at(2).size() > 0)
					{
						pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (should have only 2 lines)");
						return 1;
					}
				}
				else
				{
					pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (should have only 2 lines)");
					return 1;
				}
			}

			QString message1 = lines.toContainer().at(0).toString();
			QString message2 = lines.toContainer().at(1).toString();

			//validate line 1
			QStringTokenizer tokens = message1.tokenize(u' ');
			if (tokens.toContainer().size() != 5)
			{
				if (tokens.toContainer().size() > 5)
				{
					int i = 5;
					while (i < tokens.toContainer().size())
					{
						if (tokens.toContainer().at(i).size() == 1)
						{
							if (tokens.toContainer().at(i).toString() != '\n')
							{
								pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (first line different than 5 words)");
								return 1;
							}
						}
						if (tokens.toContainer().at(i).size() > 1)
						{
							pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (first line longer than 5 words)");
							return 1;
						}
						i += 1;
					}
				}
				else
				{
					pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (first line less than 5 words)");
					return 1;
				}
			}
			if (!(tokens.toContainer().at(0).toString() == "Number"))
			{
				pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (first word on first line not 'Number')");
				return 1;
			}
			if (!(tokens.toContainer().at(1).toString() == "of"))
			{
				pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (second word on first line not 'of')");
				return 1;
			}
			if (!(tokens.toContainer().at(2).toString() == "rocket"))
			{
				pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (third word on first line not 'rocket')");
				return 1;
			}
			if (!(tokens.toContainer().at(3).toString() == "engines:"))
			{
				pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (fourth word on first line not 'engines:')");
				return 1;
			}
			QString number = tokens.toContainer().at(4).toString();
			for (auto each : number)
			{
				if (!(each.isDigit()))
				{
					pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (final number on first line contains non-digits or a dot)");
					return 1;
				}
			}
			if (number.at(0).digitValue() == 0)
			{
				pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (final number on first line starts with 0 or is just 0)");
				return 1;
			}

			int output_1 = std::stoi(number.toStdString());

			//validate line 2
			tokens = message2.tokenize(u' ');
			if (tokens.toContainer().size() != 5)
			{
				if (tokens.toContainer().size() > 5)
				{
					int i = 5;
					while (i < tokens.toContainer().size())
					{
						if (tokens.toContainer().at(i).size() == 1)
						{
							if (tokens.toContainer().at(i).toString() != '\n')
							{
								pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (second line different than 5 words)");
								return 1;
							}
						}
						if (tokens.toContainer().at(i).size() > 1)
						{
							pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (second line longer than 5 words)");
							return 1;
						}
						i += 1;
					}
				}
				else
				{
					pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (second line less than 5 words)");
					return 1;
				}
			}
			if (!(tokens.toContainer().at(0).toString() == "Acceleration"))
			{
				pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (first word on second line not 'Acceleration')");
				return 1;
			}
			if (!(tokens.toContainer().at(1).toString() == "per"))
			{
				pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (second word on second line not 'per')");
				return 1;
			}
			if (!(tokens.toContainer().at(2).toString() == "engine:"))
			{
				pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (third word on second line not 'engine:')");
				return 1;
			}
			if (!(tokens.toContainer().at(4).toString() == "m/s^2"))
			{
				pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (fifth word on second line not 'm/s^2')");
				return 1;
			}
			number = tokens.toContainer().at(3).toString();

			bool dot_found = false;
			int dot_pos = -1;
			int counter = 0;
			for (auto each : number)
			{
				if (!(each.isDigit()))
				{
					if (dot_found)
					{
						pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (final number is written wrong)");
						return 1;
					}
					else
					{
						if (each != '.')
						{
							pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (final number is written wrong)");
							return 1;
						}
						else
						{
							if (counter == 0 || counter == number.size() - 1)
							{
								pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (final number is written wrong)");
								return 1;
							}
							else
							{
								dot_pos = counter;
								dot_found = true;
							}
						}
					}
				}
				counter += 1;
			}
			if (number.at(0).digitValue() == 0 && dot_pos != 1)
			{
				pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (final number on second line starts with 0 or is just 0)");
				return 1;
			}

			long double output_2 = 0;
			int sum = 0;
			for (auto each : number)
			{
				if (each == '.')
				{
					break;
				}
				sum *= 10;
				sum += each.digitValue();
			}
			output_2 = sum;

			if (dot_found)
			{
				int aux = 0;
				QString s = "0.";
				for (auto each : number)
				{
					if (aux > dot_pos)
					{
						s += each;
					}
					aux += 1;
				}
				output_2 += s.toDouble();
			}

			this->number_of_engines = output_1;
			this->acceleration_per_engine = output_2;

			pop_up("New rocket data loaded successfully!");

			this->default_rocket = false;
			file_descriptor.close();

			return 2;
		}

		pop_up("You didn't choose a file! The previous rocket data is unchanged");

		return 0;
	}
	catch (...)
	{
		pop_up("An error occured while reading the data! Some data may have been loaded, check its correctness, and check the input file");
		return 0;
	}
}

int Backend::load_planetary_data()
{
	//will return either - 
	//0 on no change because no file selected
	//1 on error in file contents
	//2 on success

	//format allowed - as in the model .txt, it allows for an endline on the last line
	//and trailing space on any lines; numbers must not start with a 0 and must
	//contain only digits and maybe a .

	try
	{
		QString file_path = QFileDialog::getOpenFileName(nullptr, "Open File", "", "*.txt");
		if (!file_path.isEmpty())
		{
			QFile file_descriptor = QFile(file_path);
			file_descriptor.open(QIODevice::ReadOnly | QIODevice::Text);
			QString message = file_descriptor.readAll();

			QStringTokenizer lines = message.tokenize(u'\n');

			//validate number of input lines
			if (lines.toContainer().size() != 9)
			{
				if (lines.toContainer().size() == 10)
				{
					if (lines.toContainer().at(9).size() > 0)
					{
						pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (should have only 9 lines)");
						return 1;
					}
				}
				else
				{
					pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (should have only 9 lines)");
					return 1;
				}
			}

			//the storage for the input data
			std::vector<std::pair<long double, long double>> numbers;
			std::pair<long double, long double> earth_numbers;
			int earth_mass_power = 0;

			//make exception for Earth
			int earth_counter = 0;
			//validate line-by-line
			for (auto each : lines.toContainer())
			{
				//ignore a possibly empty last row (notepad sometimes adds a \n)
				if (each.size() == 0)
				{
					continue;
				}

				//validate

				QStringTokenizer words = each.tokenize(u' ');

				//validate, with exception for Earth
				if (earth_counter != 2)
				{
					//validate word count
					if (words.toContainer().size() != 9)
					{
						if (words.toContainer().size() > 9)
						{
							int i = 9;
							while (i < words.toContainer().size())
							{
								if (words.toContainer().at(i).size() == 1)
								{
									if (words.toContainer().at(i).at(0).toLatin1() != '\n')
									{
										pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (should have exactly 9 words per line except for Earth with 11)");
										return 1;
									}
								}
								if (words.toContainer().at(i).size() > 1)
								{
									pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (should have exactly 9 words per line except for Earth with 11)");
									return 1;
								}
								i += 1;
							}
						}
						else
						{
							pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (should have exactly 9 words per line except for Earth with 11)");
							return 1;
						}
					}

					//validate words themselves

					if (words.toContainer().at(1) != "diameter")
					{
						pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (word 2 should be 'diameter' on every line)");
						return 1;
					}
					if (words.toContainer().at(2) != "=")
					{
						pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (word 3 should be '=' on every line)");
						return 1;
					}
					if (words.toContainer().at(4) != "km,")
					{
						pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (word 5 should be 'km,' on every line)");
						return 1;
					}
					if (words.toContainer().at(5) != "mass")
					{
						pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (word 6 should be 'mass' on every line)");
						return 1;
					}
					if (words.toContainer().at(6) != "=")
					{
						pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (word 7 should be '=' on every line)");
						return 1;
					}
					if (words.toContainer().at(8) != "Earths")
					{
						pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (word 9 should be 'Earths' on every line except for Earth's)");
						return 1;
					}

					if (earth_counter == 0 && words.toContainer().at(0) != "Mercury:")
					{
						pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (word 1 on line 1 should be 'Mercury:')");
						return 1;
					}
					if (earth_counter == 1 && words.toContainer().at(0) != "Venus:")
					{
						pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (word 1 on line 2 should be 'Venus:')");
						return 1;
					}
					if (earth_counter == 3 && words.toContainer().at(0) != "Mars:")
					{
						pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (word 1 on line 4 should be 'Mars:')");
						return 1;
					}
					if (earth_counter == 4 && words.toContainer().at(0) != "Jupiter:")
					{
						pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (word 1 on line 5 should be 'Jupiter:')");
						return 1;
					}
					if (earth_counter == 5 && words.toContainer().at(0) != "Saturn:")
					{
						pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (word 1 on line 6 should be 'Saturn:')");
						return 1;
					}
					if (earth_counter == 6 && words.toContainer().at(0) != "Uranus:")
					{
						pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (word 1 on line 7 should be 'Uranus:')");
						return 1;
					}
					if (earth_counter == 7 && words.toContainer().at(0) != "Neptune:")
					{
						pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (word 1 on line 8 should be 'Neptune:')");
						return 1;
					}
					if (earth_counter == 8 && words.toContainer().at(0) != "Pluto:")
					{
						pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (word 1 on line 9 should be 'Pluto:')");
						return 1;
					}

					//validate and read the numbers

					QString number = words.toContainer().at(3).toString();

					bool dot_found = false;
					int dot_pos = -1;
					int counter = 0;
					for (auto each : number)
					{
						if (!(each.isDigit()))
						{
							if (dot_found)
							{
								pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (first number is written wrong on line " + std::to_string(earth_counter + 1) + ")");
								return 1;
							}
							else
							{
								if (each != '.')
								{
									pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (first number is written wrong on line " + std::to_string(earth_counter + 1) + ")");
									return 1;
								}
								else
								{
									if (counter == 0 || counter == number.size() - 1)
									{
										pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (first number is written wrong on line " + std::to_string(earth_counter + 1) + ")");
										return 1;
									}
									else
									{
										dot_pos = counter;
										dot_found = true;
									}
								}
							}
						}
						counter += 1;
					}
					if (number.at(0).digitValue() == 0 && dot_pos != 1)
					{
						pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (first number starts with 0 or is just 0 on line " + std::to_string(earth_counter + 1) + ")");
						return 1;
					}

					long double output = 0;
					int sum = 0;
					for (auto each : number)
					{
						if (each == '.')
						{
							break;
						}
						sum *= 10;
						sum += each.digitValue();
					}
					output = sum;

					if (dot_found)
					{
						int aux = 0;
						QString s = "0.";
						for (auto each : number)
						{
							if (aux > dot_pos)
							{
								s += each;
							}
							aux += 1;
						}
						output += s.toDouble();
					}

					std::pair<long double, long double> proto_pair;
					proto_pair.first = output;

					number = words.toContainer().at(7).toString();

					dot_found = false;
					dot_pos = -1;
					counter = 0;
					for (auto each : number)
					{
						if (!(each.isDigit()))
						{
							if (dot_found)
							{
								pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (second number is written wrong on line " + std::to_string(earth_counter + 1) + ")");
								return 1;
							}
							else
							{
								if (each != '.')
								{
									pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (second number is written wrong on line " + std::to_string(earth_counter + 1) + ")");
									return 1;
								}
								else
								{
									if (counter == 0 || counter == number.size() - 1)
									{
										pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (second number is written wrong on line " + std::to_string(earth_counter + 1) + ")");
										return 1;
									}
									else
									{
										dot_pos = counter;
										dot_found = true;
									}
								}
							}
						}
						counter += 1;
					}
					if (number.at(0).digitValue() == 0 && dot_pos != 1)
					{
						pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (second number starts with 0 or is just 0 on line " + std::to_string(earth_counter + 1) + ")");
						return 1;
					}

					output = 0;
					sum = 0;
					for (auto each : number)
					{
						if (each == '.')
						{
							break;
						}
						sum *= 10;
						sum += each.digitValue();
					}
					output = sum;

					if (dot_found)
					{
						int aux = 0;
						QString s = "0.";
						for (auto each : number)
						{
							if (aux > dot_pos)
							{
								s += each;
							}
							aux += 1;
						}
						output += s.toDouble();
					}

					proto_pair.second = output;
					numbers.push_back(proto_pair);

				}
				else
				{
					//validate word count
					if (words.toContainer().size() != 11)
					{
						if (words.toContainer().size() > 11)
						{
							int i = 11;
							while (i < words.toContainer().size())
							{
								if (words.toContainer().at(i).size() == 1)
								{
									if (words.toContainer().at(0).toLatin1() != '\n')
									{
										pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (should have exactly 11 words for Earth)");
										return 1;
									}
								}
								if (words.toContainer().at(i).size() > 1)
								{
									pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (should have exactly 11 words for Earth)");
									return 1;
								}
								i += 1;
							}
						}
						else
						{
							pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (should have exactly 11 words for Earth)");
							return 1;
						}
					}

					//validate words themselves

					if (words.toContainer().at(0) != "Earth:")
					{
						pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (word 1 on line 3 should be 'Earth:')");
						return 1;
					}
					if (words.toContainer().at(1) != "diameter")
					{
						pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (word 2 should be 'diameter' on every line)");
						return 1;
					}
					if (words.toContainer().at(2) != "=")
					{
						pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (word 3 should be '=' on every line)");
						return 1;
					}
					if (words.toContainer().at(4) != "km,")
					{
						pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (word 5 should be 'km,' on every line)");
						return 1;
					}
					if (words.toContainer().at(5) != "mass")
					{
						pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (word 6 should be 'mass' on every line)");
						return 1;
					}
					if (words.toContainer().at(6) != "=")
					{
						pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (word 7 should be '=' on every line)");
						return 1;
					}
					if (words.toContainer().at(8) != "*")
					{
						pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (word 9 should be '*' on line 3)");
						return 1;
					}
					if (words.toContainer().at(10) != "kg")
					{
						pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (word 11 should be 'kg' on line 3)");
						return 1;
					}
					if (words.toContainer().at(9).size() < 4)
					{
						pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (Earth's mass should be specified as 'number * 10^some-whole-number')");
						return 1;
					}
					else
					{
						if (words.toContainer().at(9).at(0) != '1' || words.toContainer().at(9).at(1) != '0' || words.toContainer().at(9).at(2) != '^')
						{
							pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (Earth's mass should be specified as 'number * 10^some-whole-number')");
							return 1;
						}
						if (words.toContainer().at(9).at(3) == '0')
						{
							pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (Earth's mass should be specified as 'number * 10^some-whole-number-not-starting-with-0-and-not-0')");
							return 1;
						}
						int pos = 3;
						int sum = 0;
						while (pos < words.toContainer().at(9).size())
						{
							if (!words.toContainer().at(9).at(pos).isDigit())
							{
								pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (Earth's mass should be specified as 'number * 10^some-whole-number')");
								return 1;
							}
							sum *= 10;
							sum += words.toContainer().at(9).at(pos).digitValue();
							pos += 1;
						}
						earth_mass_power = sum;
					}

					//validate and read the numbers for Earth

					QString number = words.toContainer().at(3).toString();

					bool dot_found = false;
					int dot_pos = -1;
					int counter = 0;
					for (auto each : number)
					{
						if (!(each.isDigit()))
						{
							if (dot_found)
							{
								pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (first number is written wrong on line " + std::to_string(earth_counter + 1) + ")");
								return 1;
							}
							else
							{
								if (each != '.')
								{
									pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (first number is written wrong on line " + std::to_string(earth_counter + 1) + ")");
									return 1;
								}
								else
								{
									if (counter == 0 || counter == number.size() - 1)
									{
										pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (first number is written wrong on line " + std::to_string(earth_counter + 1) + ")");
										return 1;
									}
									else
									{
										dot_pos = counter;
										dot_found = true;
									}
								}
							}
						}
						counter += 1;
					}
					if (number.at(0).digitValue() == 0 && dot_pos != 1)
					{
						pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (first number starts with 0 or is just 0 on line " + std::to_string(earth_counter + 1) + ")");
						return 1;
					}

					long double output = 0;
					int sum = 0;
					for (auto each : number)
					{
						if (each == '.')
						{
							break;
						}
						sum *= 10;
						sum += each.digitValue();
					}
					output = sum;

					if (dot_found)
					{
						int aux = 0;
						QString s = "0.";
						for (auto each : number)
						{
							if (aux > dot_pos)
							{
								s += each;
							}
							aux += 1;
						}
						output += s.toDouble();
					}

					std::pair<long double, long double> proto_pair;
					proto_pair.first = output;

					number = words.toContainer().at(7).toString();

					dot_found = false;
					dot_pos = -1;
					counter = 0;
					for (auto each : number)
					{
						if (!(each.isDigit()))
						{
							if (dot_found)
							{
								pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (the number after 'mass =' is written wrong on line " + std::to_string(earth_counter + 1) + ")");
								return 1;
							}
							else
							{
								if (each != '.')
								{
									pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (the number after 'mass =' is written wrong on line " + std::to_string(earth_counter + 1) + ")");
									return 1;
								}
								else
								{
									if (counter == 0 || counter == number.size() - 1)
									{
										pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (the number after 'mass =' is written wrong on line " + std::to_string(earth_counter + 1) + ")");
										return 1;
									}
									else
									{
										dot_pos = counter;
										dot_found = true;
									}
								}
							}
						}
						counter += 1;
					}
					if (number.at(0).digitValue() == 0 && dot_pos != 1)
					{
						pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (the number after 'mass =' starts with 0 or is just 0 on line " + std::to_string(earth_counter + 1) + ")");
						return 1;
					}

					output = 0;
					sum = 0;
					for (auto each : number)
					{
						if (each == '.')
						{
							break;
						}
						sum *= 10;
						sum += each.digitValue();
					}
					output = sum;

					if (dot_found)
					{
						int aux = 0;
						QString s = "0.";
						for (auto each : number)
						{
							if (aux > dot_pos)
							{
								s += each;
							}
							aux += 1;
						}
						output += s.toDouble();
					}

					proto_pair.second = output;

					earth_numbers = proto_pair;
				}

				earth_counter += 1;
			}

			//actually save the data

			this->earth_mass = static_cast<long double>(earth_numbers.second * pow(static_cast<long double>(10), static_cast<long double>(earth_mass_power)));

			this->planets.at(0).set_diameter(numbers.at(0).first);
			this->planets.at(0).set_mass(numbers.at(0).second * earth_mass);

			this->planets.at(1).set_diameter(numbers.at(1).first);
			this->planets.at(1).set_mass(numbers.at(1).second * earth_mass);

			this->planets.at(2).set_diameter(earth_numbers.first);
			this->planets.at(2).set_mass(static_cast<long double>(earth_numbers.second * pow(static_cast<long double>(10), static_cast<long double>(earth_mass_power))));

			this->planets.at(3).set_diameter(numbers.at(2).first);
			this->planets.at(3).set_mass(numbers.at(2).second * earth_mass);

			this->planets.at(4).set_diameter(numbers.at(3).first);
			this->planets.at(4).set_mass(numbers.at(3).second * earth_mass);

			this->planets.at(5).set_diameter(numbers.at(4).first);
			this->planets.at(5).set_mass(numbers.at(4).second * earth_mass);

			this->planets.at(6).set_diameter(numbers.at(5).first);
			this->planets.at(6).set_mass(numbers.at(5).second * earth_mass);

			this->planets.at(7).set_diameter(numbers.at(6).first);
			this->planets.at(7).set_mass(numbers.at(6).second * earth_mass);

			this->planets.at(8).set_diameter(numbers.at(7).first);
			this->planets.at(8).set_mass(numbers.at(7).second * earth_mass);

			std::string p;
			p += "Successfully loaded new planetary data!";
			pop_up(p);

			this->default_planets = false;
			file_descriptor.close();

			return 2;
		}

		pop_up("You didn't choose a file! The previous planetary data is unchanged");

		return 0;
	}
	catch (...)
	{
		pop_up("An error occured while reading the data! Some data may have been loaded, check its correctness, and check the input file");
		return 0;
	}
}

int Backend::load_solar_system_data()
{
	//will return either - 
	//0 on no change because no file selected
	//1 on error in file contents
	//2 on success

	//format allowed - as in the model .txt, it allows for an endline on the last line
	//and trailing space on any lines; numbers must not start with a 0 and must
	//contain only digits and maybe a .

	try
	{
		QString file_path = QFileDialog::getOpenFileName(nullptr, "Open File", "", "*.txt");
		if (!file_path.isEmpty())
		{
			QFile file_descriptor = QFile(file_path);
			file_descriptor.open(QIODevice::ReadOnly | QIODevice::Text);
			QString message = file_descriptor.readAll();

			QStringTokenizer lines = message.tokenize(u'\n');

			//validate number of input lines
			if (lines.toContainer().size() != 9)
			{
				if (lines.toContainer().size() == 10)
				{
					if (lines.toContainer().at(9).size() > 0)
					{
						pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (should have only 9 lines)");
						return 1;
					}
				}
				else
				{
					pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (should have only 9 lines)");
					return 1;
				}
			}

			//the storage for the input data
			std::vector<std::pair<long double, long double>> numbers;

			//make exception for Earth
			int earth_counter = 0;
			//validate line-by-line
			for (auto each : lines.toContainer())
			{
				//ignore a possibly empty last row (notepad sometimes adds a \n)
				if (each.size() == 0)
				{
					continue;
				}

				//validate

				QStringTokenizer words = each.tokenize(u' ');

				//validate word count
				if (words.toContainer().size() != 10)
				{
					if (words.toContainer().size() > 10)
					{
						int i = 10;
						while (i < words.toContainer().size())
						{
							if (words.toContainer().at(i).size() == 1)
							{
								if (words.toContainer().at(i).at(0).toLatin1() != '\n')
								{
									pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (should have exactly 10 words per line)");
									return 1;
								}
							}
							if (words.toContainer().at(i).size() > 1)
							{
								pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (should have exactly 10 words per line)");
								return 1;
							}
							i += 1;
						}
					}
					else
					{
						pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (should have exactly 10 words per line)");
						return 1;
					}
				}

				//validate words themselves

				if (words.toContainer().at(1) != "period")
				{
					pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (word 2 should be 'period' on every line)");
					return 1;
				}
				if (words.toContainer().at(2) != "=")
				{
					pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (word 3 should be '=' on every line)");
					return 1;
				}
				if (words.toContainer().at(4) != "days,")
				{
					pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (word 5 should be 'days,' on every line)");
					return 1;
				}
				if (words.toContainer().at(5) != "orbital")
				{
					pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (word 6 should be 'orbital' on every line)");
					return 1;
				}
				if (words.toContainer().at(6) != "radius")
				{
					pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (word 7 should be 'radius' on every line)");
					return 1;
				}
				if (words.toContainer().at(7) != "=")
				{
					pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (word 8 should be '=' on every line)");
					return 1;
				}
				if (words.toContainer().at(9) != "AU")
				{
					pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (word 10 should be 'AU' on every line)");
					return 1;
				}

				if (earth_counter == 0 && words.toContainer().at(0) != "Mercury:")
				{
					pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (word 1 on line 1 should be 'Mercury:')");
					return 1;
				}
				if (earth_counter == 1 && words.toContainer().at(0) != "Venus:")
				{
					pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (word 1 on line 2 should be 'Venus:')");
					return 1;
				}
				if (earth_counter == 2 && words.toContainer().at(0) != "Earth:")
				{
					pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (word 1 on line 3 should be 'Earth:')");
					return 1;
				}
				if (earth_counter == 3 && words.toContainer().at(0) != "Mars:")
				{
					pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (word 1 on line 4 should be 'Mars:')");
					return 1;
				}
				if (earth_counter == 4 && words.toContainer().at(0) != "Jupiter:")
				{
					pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (word 1 on line 5 should be 'Jupiter:')");
					return 1;
				}
				if (earth_counter == 5 && words.toContainer().at(0) != "Saturn:")
				{
					pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (word 1 on line 6 should be 'Saturn:')");
					return 1;
				}
				if (earth_counter == 6 && words.toContainer().at(0) != "Uranus:")
				{
					pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (word 1 on line 7 should be 'Uranus:')");
					return 1;
				}
				if (earth_counter == 7 && words.toContainer().at(0) != "Neptune:")
				{
					pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (word 1 on line 8 should be 'Neptune:')");
					return 1;
				}
				if (earth_counter == 8 && words.toContainer().at(0) != "Pluto:")
				{
					pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (word 1 on line 9 should be 'Pluto:')");
					return 1;
				}

				//validate and read the numbers

				QString number = words.toContainer().at(3).toString();

				bool dot_found = false;
				int dot_pos = -1;
				int counter = 0;
				for (auto each : number)
				{
					if (!(each.isDigit()))
					{
						if (dot_found)
						{
							pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (first number is written wrong on line " + std::to_string(earth_counter + 1) + ")");
							return 1;
						}
						else
						{
							if (each != '.')
							{
								pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (first number is written wrong on line " + std::to_string(earth_counter + 1) + ")");
								return 1;
							}
							else
							{
								if (counter == 0 || counter == number.size() - 1)
								{
									pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (first number is written wrong on line " + std::to_string(earth_counter + 1) + ")");
									return 1;
								}
								else
								{
									dot_pos = counter;
									dot_found = true;
								}
							}
						}
					}
					counter += 1;
				}
				if (number.at(0).digitValue() == 0 && dot_pos != 1)
				{
					pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (first number starts with 0 or is just 0 on line " + std::to_string(earth_counter + 1) + ")");
					return 1;
				}

				long double output = 0;
				int sum = 0;
				for (auto each : number)
				{
					if (each == '.')
					{
						break;
					}
					sum *= 10;
					sum += each.digitValue();
				}
				output = sum;

				if (dot_found)
				{
					int aux = 0;
					QString s = "0.";
					for (auto each : number)
					{
						if (aux > dot_pos)
						{
							s += each;
						}
						aux += 1;
					}
					output += s.toDouble();
				}

				std::pair<long double, long double> proto_pair;
				proto_pair.first = output;

				number = words.toContainer().at(8).toString();

				dot_found = false;
				dot_pos = -1;
				counter = 0;
				for (auto each : number)
				{
					if (!(each.isDigit()))
					{
						if (dot_found)
						{
							pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (second number is written wrong on line " + std::to_string(earth_counter + 1) + ")");
							return 1;
						}
						else
						{
							if (each != '.')
							{
								pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (second number is written wrong on line " + std::to_string(earth_counter + 1) + ")");
								return 1;
							}
							else
							{
								if (counter == 0 || counter == number.size() - 1)
								{
									pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (second number is written wrong on line " + std::to_string(earth_counter + 1) + ")");
									return 1;
								}
								else
								{
									dot_pos = counter;
									dot_found = true;
								}
							}
						}
					}
					counter += 1;
				}
				if (number.at(0).digitValue() == 0 && dot_pos != 1)
				{
					pop_up("Error in the text from " + file_path.toStdString() + "! Check it and try again (second number starts with 0 or is just 0 on line " + std::to_string(earth_counter + 1) + ")");
					return 1;
				}

				output = 0;
				sum = 0;
				for (auto each : number)
				{
					if (each == '.')
					{
						break;
					}
					sum *= 10;
					sum += each.digitValue();
				}
				output = sum;

				if (dot_found)
				{
					int aux = 0;
					QString s = "0.";
					for (auto each : number)
					{
						if (aux > dot_pos)
						{
							s += each;
						}
						aux += 1;
					}
					output += s.toDouble();
				}

				proto_pair.second = output;
				numbers.push_back(proto_pair);

				earth_counter += 1;
			}

			//actually save the data

			this->planets.at(0).set_period(numbers.at(0).first);
			this->planets.at(0).set_orbital_radius(numbers.at(0).second * AU);

			this->planets.at(1).set_period(numbers.at(1).first);
			this->planets.at(1).set_orbital_radius(numbers.at(1).second * AU);

			this->planets.at(2).set_period(numbers.at(2).first);
			this->planets.at(2).set_orbital_radius(numbers.at(2).second * AU);

			this->planets.at(3).set_period(numbers.at(3).first);
			this->planets.at(3).set_orbital_radius(numbers.at(3).second * AU);

			this->planets.at(4).set_period(numbers.at(4).first);
			this->planets.at(4).set_orbital_radius(numbers.at(4).second * AU);

			this->planets.at(5).set_period(numbers.at(5).first);
			this->planets.at(5).set_orbital_radius(numbers.at(5).second * AU);

			this->planets.at(6).set_period(numbers.at(6).first);
			this->planets.at(6).set_orbital_radius(numbers.at(6).second * AU);

			this->planets.at(7).set_period(numbers.at(7).first);
			this->planets.at(7).set_orbital_radius(numbers.at(7).second * AU);

			this->planets.at(8).set_period(numbers.at(8).first);
			this->planets.at(8).set_orbital_radius(numbers.at(8).second * AU);

			std::string p;
			p += "Successfully loaded new Solar System data!\n";

			pop_up(p);

			this->default_solar_system = false;
			file_descriptor.close();

			return 2;
		}

		pop_up("You didn't choose a file! The previous Solar System data is unchanged");

		return 0;
	}
	catch (...)
	{
		pop_up("An error occured while reading the data! Some data may have been loaded, check its correctness, and check the input file");
		return 0;
	}
}

void Backend::pop_up(std::string s)
{
	QMessageBox mb = QMessageBox();
	mb.setText(QString::fromStdString(s));
	mb.exec();
	return;
}

std::string Backend::get_rocket_data()
{
	std::string s;
	s += "--- ROCKET DATA ---\n";
	if (default_rocket)
	{
		s += "Caution! This data is the application default,\nyou might want to load your own data\n";
	}
	s += "Number of engines --- " + std::to_string(number_of_engines) + "\n";
	s += "Acceleration per engine --- " + std::to_string(acceleration_per_engine);
	int pos = s.size() - 1;
	while (s.at(pos) == '0')
	{
		s.erase(pos, std::string::npos);
		pos -= 1;
	}
	s += "0 m/s^2";
	return s;
}

std::pair<std::string, std::vector<long double>> Backend::get_positions_after_time(std::string time)
{
	try
	{
		std::string s = time;

		if (s.size() == 0)
		{
			pop_up("Error! You must type a number of days");
			return std::make_pair("", std::vector<long double>());
		}

		bool dot_found = false;
		int dot_pos = -1;
		int counter = 0;
		for (auto each : s)
		{
			if (each < '0' || each > '9')
			{
				if (dot_found)
				{
					pop_up("Error in the number of days you typed!");
					return std::make_pair("", std::vector<long double>());
				}
				else
				{
					if (each != '.')
					{
						pop_up("Error in the number of days you typed!");
						return std::make_pair("", std::vector<long double>());
					}
					else
					{
						if (counter == 0 || counter == s.size() - 1)
						{
							pop_up("Error in the number of days you typed!");
							return std::make_pair("", std::vector<long double>());
						}
						else
						{
							dot_pos = counter;
							dot_found = true;
						}
					}
				}
			}
			counter += 1;
		}
		if (s.at(0) == '0' && dot_pos != 1)
		{
			pop_up("Error in the number of days you typed! It starts with a 0 or is just a 0");
			return std::make_pair("", std::vector<long double>());
		}
		if (s.size() >= 8 && !dot_found)
		{
			pop_up("The number of days you typed is too large!(maximum 9 999 999)");
			return std::make_pair("", std::vector<long double>());
		}

		long double output = 0;
		int sum = 0;
		std::vector<long double> angles_res;

		for (auto each : s)
		{
			if (each == '.')
			{
				break;
			}
			sum *= 10;
			sum += each - '0';
		}
		output = sum;

		if (dot_found)
		{
			int aux = 0;
			QString st = "0.";
			for (auto each : s)
			{
				if (aux > dot_pos)
				{
					st += each;
				}
				aux += 1;
			}
			output += st.toDouble();
		}
		
		std::string res = "After " + std::to_string(output);
		int pos = res.size() - 1;
		while (res.at(pos) == '0')
		{
			res.erase(pos, std::string::npos);
			pos -= 1;
		}
		res += +"0 days, the Solar System looks like this:\n";

		for (Planet each : planets)
		{
			long double degs_per_day = static_cast<long double>(360.0) / each.get_period();
			long double degs_traveled = fmod((output * degs_per_day), 360.0);

			angles_res.push_back(degs_traveled);

			res += each.get_name() + ": " + std::to_string(degs_traveled);
			int pos = res.size() - 5;
			res.erase(pos, std::string::npos);
			res += " degrees; ";
			if (each.get_name() == "Mars")
			{
				res += '\n';
			}
		}

		return std::make_pair(res, angles_res);
	}
	catch (...)
	{
		pop_up("Error in computing planet positions! Retry");
		return std::make_pair("", std::vector<long double>());
	}
}

std::pair<std::vector<std::string>, std::vector<long double>> Backend::get_trajectories(std::string planet_1, std::string planet_2, QWidget* parent)
{
	try
	{
		std::string p1 = planet_1;
		std::string p2 = planet_2;

		//validate the planets
		int p1_counter = -1, p2_counter = -1, counter = 0;
		for (auto each : planets)
		{
			if (each.get_name() == p1)
			{
				p1_counter = counter;
			}
			if (each.get_name() == p2)
			{
				p2_counter = counter;
			}
			counter += 1;
		}
		if (p1 == "" || p2 == "")
		{
			pop_up("Error! You must type one planet in each field");
			return std::pair<std::vector<std::string>, std::vector<long double>>();
		}
		if (p1_counter == p2_counter)
		{
			pop_up("Error! You typed the same planet twice");
			return std::pair<std::vector<std::string>, std::vector<long double>>();
		}
		if (p1_counter == -1 || p2_counter == -1)
		{
			pop_up("Error! You typed the planets wrong, retry");
			return std::pair<std::vector<std::string>, std::vector<long double>>();
		}
		
		QDialog* wait_for_computations = new QDialog(parent);
		wait_for_computations->setWindowModality(Qt::ApplicationModal);
		wait_for_computations->setFixedSize(180, 20);
		wait_for_computations->setWindowTitle("Computing...");
		wait_for_computations->show();

		//do the straight line computations
		long double p1_escape_vel = sqrt(2 * G * planets.at(p1_counter).get_mass() / (planets.at(p1_counter).get_diameter() * 500)); //in m/s
		long double p2_escape_vel = sqrt(2 * G * planets.at(p2_counter).get_mass() / (planets.at(p2_counter).get_diameter() * 500)); //in m/s
		long double max_speed = p1_escape_vel > p2_escape_vel ? p1_escape_vel : p2_escape_vel; // in m/s
		long double total_acceleration = acceleration_per_engine * number_of_engines; // in m/s^2
		long double straight_distance = (abs(planets.at(p1_counter).get_orbital_radius() - planets.at(p2_counter).get_orbital_radius()) - planets.at(p1_counter).get_diameter() / 2 - planets.at(p2_counter).get_diameter() / 2) * 1000; //in m

		long double time_of_acceleration = max_speed / total_acceleration; //in s
		long double distance_of_acceleration = total_acceleration * time_of_acceleration * time_of_acceleration / 2.0; //in m
		long double cruising_time = (straight_distance - 2.0 * distance_of_acceleration) / max_speed; //in s
		long double journey_time = cruising_time + 2 * time_of_acceleration; //in s

		std::vector<std::string> ret = std::vector<std::string>();
		ret.push_back(seconds_to_days(journey_time));
		int pos;
		ret.push_back(std::to_string(time_of_acceleration));
		pos = ret.at(1).size() - 5;
		ret.at(1).erase(pos, std::string::npos);
		ret.at(1) += " s";
		ret.push_back(std::to_string(distance_of_acceleration / 1000));
		pos = ret.at(2).size() - 5;
		ret.at(2).erase(pos, std::string::npos);
		ret.at(2) += " km";
		ret.push_back(seconds_to_days(cruising_time));

		//do the moving planets computations
		std::vector<long double> planet_positions; //in degrees
		for (auto each : planets)
		{
			planet_positions.push_back(0.0);
		}
		//let 100 years pass
		counter = 0;
		for (auto& each : planet_positions)
		{
			each += fmod(360.0 / planets.at(counter).get_period() * 365.0 * 100.0, 360.0);
			counter += 1;
		}
		//do step calculations over 10 years and find a minimum
		int step_limit = 10 * 365; //1 calculation per day
		int current_step = 0;

		long double start_of_launch_window = 0.0;
		long double minimum_interplanetary_distance = 900000000000.0; //in km
		long double current_distance = 0.0;

		std::vector<std::pair<long double, long double>> orthogonal_positions; //x and y orthogonal positions of the planets in km
		for (int i = 0; i < 9; ++i)
		{
			orthogonal_positions.push_back(std::make_pair(0.0, 0.0));
		}

		std::pair<int, int> ret_planet_counters;
		ret_planet_counters.first = p1_counter;
		ret_planet_counters.second = p2_counter;
		std::vector<long double> ret_polar_planet_positions;
		for (int i = 0; i < 9; ++i)
		{
			ret_polar_planet_positions.push_back(0.0);
		}
		long double ret_total_journey_time; //in s
		long double ret_cruising_journey_time; //in s

		while (current_step < step_limit)
		{
			//progress the system by one step
			int counter = 0;
			for (auto& each : planet_positions)
			{
				each += 360.0 / planets.at(counter).get_period() * (10 * 365) / step_limit; //add the corresponding degrees
				each = fmod(each, 360.0); //truncate
				counter += 1;
			}
			//compute optimality
			//convert polar to orthogonal planet positions
			counter = 0;
			while (counter < 9)
			{
				orthogonal_positions.at(counter) = std::make_pair(planets.at(counter).get_orbital_radius() * cos(planet_positions.at(counter) * M_PI / 180.0), planets.at(counter).get_orbital_radius() * sin(planet_positions.at(counter) * M_PI / 180.0)); //in km

				counter += 1;
			}
			//check smashing into planets and compare with current shortest distance
			bool smashes = false;

			long double p1_x = orthogonal_positions.at(p1_counter).first; //in km
			long double p1_y = orthogonal_positions.at(p1_counter).second; //in km
			long double p2_x = orthogonal_positions.at(p2_counter).first; //in km
			long double p2_y = orthogonal_positions.at(p2_counter).second; //in km

			current_distance = hypot(abs(p1_x - p2_x), abs(p1_y - p2_y)); //in km
			std::pair<long double, long double> current_global_position; //in km
			long double current_travel_time; //in s
			long double dynamic_journey_distance = (current_distance - planets.at(p1_counter).get_diameter() / 2 - planets.at(p2_counter).get_diameter() / 2); //in km
			long double dynamic_journey_time = (dynamic_journey_distance * 1000.0 - 2 * distance_of_acceleration) / max_speed; //in s
			long double journey_angle = atan2(p1_y - p2_y, p1_x - p2_x); //in radians

			long double other_x; //in km
			long double other_y; //in km
			
			//check smashing
			if (current_distance < minimum_interplanetary_distance)
			{
				//static smashing
				int counter = 0;
				long double distance_center_of_planet_to_line = 0.0; //in km
				for (auto each : orthogonal_positions)//in km
				{
					if (counter != p1_counter && counter != p2_counter)
					{
						//compute smashing - formula for distance from point to line
						distance_center_of_planet_to_line = abs((p1_y - p2_y) * each.first - (p1_x - p2_x) * each.second + p2_x * p1_y - p1_x * p2_y) / current_distance; //in km
						if (distance_center_of_planet_to_line <= planets.at(counter).get_diameter() / 2) //no smash detected, in km
						{
							smashes = true;
						}
					}
					counter += 1;
				}
				
				//check dynamic smashing into planets numerically
				current_travel_time = time_of_acceleration; //in s
				current_global_position.first = p1_x + (distance_of_acceleration / 1000.0 + planets.at(p1_counter).get_diameter() / 2) * cos(journey_angle);//in km
				current_global_position.second = p1_y + (distance_of_acceleration / 1000.0 + planets.at(p1_counter).get_diameter() / 2) * sin(journey_angle);//in km

				while (current_travel_time < (dynamic_journey_time - 60 * 60 * 24))
				{
					current_global_position.first += cos(journey_angle) * max_speed * 60 * 60 * 24 / 1000.0; //in km/day
					current_global_position.second += sin(journey_angle) * max_speed * 60 * 60 * 24 / 1000.0; //in km/day

					//compare rocket position to positions of planets at this time
					int counter = 0;
					for (auto& each : planet_positions)
					{
						if (counter != p1_counter && counter != p2_counter)
						{
							//check linear distance to planet
							other_x = cos(fmod(each + 360.0 / planets.at(counter).get_period() * (dynamic_journey_time / (60 * 60 * 24)), 360.0)) * planets.at(counter).get_orbital_radius(); //in km
							other_y = sin(fmod(each + 360.0 / planets.at(counter).get_period() * (dynamic_journey_time / (60 * 60 * 24)), 360.0)) * planets.at(counter).get_orbital_radius(); //in km
						
							if (hypot(abs(other_x - current_global_position.first), abs(other_y - current_global_position.second)) < planets.at(counter).get_diameter() / 2 * 10) //safe margin of 10 planetary radii in km
							{
								smashes = true;
								break;
							}
						}
						counter += 1;
					}
					if (smashes)
					{
						break;
					}

					current_travel_time += 60 * 60 * 24; //add a days' worth of seconds
				}

				//update if no smash
				if (!smashes)
				{
					minimum_interplanetary_distance = current_distance;
					start_of_launch_window = current_step * (10 * 365 / step_limit);
					for (int i = 0; i < 9; ++i)
					{
						ret_polar_planet_positions.at(i) = planet_positions.at(i);
					}
					ret_cruising_journey_time = dynamic_journey_time;
					ret_total_journey_time = dynamic_journey_time + 2 * time_of_acceleration;
				}
			}

			current_step += 1;
		}

		std::string temp = std::to_string(start_of_launch_window);
		int temp_pos = temp.size() - 5;
		temp.erase(temp_pos, std::string::npos);
		ret.push_back(temp + " days");
		ret.push_back(std::to_string(start_of_launch_window));
		ret.push_back(std::to_string(time_of_acceleration) + " s");
		temp = std::to_string(distance_of_acceleration / 1000.0);
		temp_pos = temp.size() - 5;
		temp.erase(temp_pos, std::string::npos);
		ret.push_back(temp + " km");
		ret.push_back(seconds_to_days(ret_cruising_journey_time));
		ret.push_back(seconds_to_days(ret_total_journey_time));

		ret_polar_planet_positions.push_back(ret_planet_counters.first);
		ret_polar_planet_positions.push_back(ret_planet_counters.second);

		wait_for_computations->close();

		return std::make_pair(ret, ret_polar_planet_positions);
	}
	catch (...)
	{
		pop_up("Error in computing rocket trajectories! Retry");
		return std::pair<std::vector<std::string>, std::vector<long double>>();
	}
}

std::string Backend::seconds_to_days(long double seconds)
{
	unsigned long long to_int = static_cast<unsigned long long>(seconds);
	unsigned int days = to_int / (60 * 60 * 24);
	to_int -= days * (60 * 60 * 24);
	unsigned int hours = to_int / (60 * 60);
	to_int -= hours * 60 * 60;
	return std::to_string(days) + " d, " + std::to_string(hours) + " h, " + std::to_string(to_int) + " s";
}