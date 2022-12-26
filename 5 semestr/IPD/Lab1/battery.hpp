#pragma once

#include <string>

using std::string;

/*!
 * \namespace Battery
 * \brief Interface for communicating with battery
 */
namespace Battery
{
	void Run(void);
	void Sleep(void);
	void Hibernate(void);
	string GetSavingStatus();
};
