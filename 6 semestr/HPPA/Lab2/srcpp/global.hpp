#pragma once

#include <iomanip>
#include <iostream>

/*!
 * \file global.hpp
 * \brief General global definitions for commonly used components from the standard library
 */

using std::string;
using std::to_string;

using std::cerr;
using std::cin;
using std::clog;
using std::cout;

using std::boolalpha;
using std::setw;

/*!
 * \note I don't like the idea of using std::endl in the code because of its performance issues, caused by flushing the output stream.
 * \note So I insist on using '\\n' instead.
 */
constexpr static const auto endl = '\n';
