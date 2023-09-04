#pragma once

#include <random>

using std::uniform_real_distribution;
using std::uniform_int_distribution;

/*!
 * \brief rand_gen Random number generator
 * \tparam T Type of generated random number
 * \tparam distribution Type of distribution, e.g. uniform_int_distribution, uniform_real_distribution, etc.
 * \tparam arg_package Package of arguments, required by selected distribution
 * \param args Package of arguments of type T, required by selected distribution. `args` will be passed to distribution(`args`) without any change, e.g.:
 * \example rand_gen<int, uniform_int_distribution<int>>() => uniform_int_distribution<int>()
 * \example rand_gen<long, uniform_int_distribution<long>>(1) => uniform_int_distribution<long>(1)
 * \example rand_gen<double, uniform_real_distribution<double>>(2.0, 3.0) => uniform_real_distribution<double>(2.0, 3.0)
 * \note exact values and meaning of `args` depends on selected distribution. See documentation for selected distribution for more information.
 * \return Generated random number of type T
 */
template<typename T, class distribution,
		 typename... arg_package>
T rand_gen(arg_package... args)
{
	std::random_device rdev;
	return distribution(args...)(rdev);
}
