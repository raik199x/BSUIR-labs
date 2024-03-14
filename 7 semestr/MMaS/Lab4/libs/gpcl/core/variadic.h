#pragma once

//! \section Variadic

//! \subsection Concepts

//! \brief Stringify (concept)
//! \example Transforms (ab, cd, ef) into "ab cd ef"
#define GPCL_CONCEPT_STRINGIFY(...) #__VA_ARGS__

//! \brief Variadic arguments (are) empty (concept)
//! \example Transforms () into true
//! \example Transforms (abcdef), (abc, def), (ab, cd, ef), etc. into false
#define GPCL_CONCEPT_VA_EMPTY(...) (sizeof((char[]){#__VA_ARGS__}) == sizeof((char[]){""}))

//! \brief Variadic arguments filler (concept)
//! \note Used to expand variadic arguments and fill them with descending numbers
//! \warning The number of arguments heavily depends on the compiler and the target environment
#define GPCL_CONCEPT_VA_FILLER \
	64,63,62,61,60,59,58,57,56,55,54,53,52,51,50,49, \
	48,47,46,45,44,43,42,41,40,39,38,37,36,35,34,33, \
	32,31,30,29,28,27,26,25,24,23,22,21,20,19,18,17, \
	16,15,14,13,12,11,10, 9, 8, 7, 6, 5, 4, 3, 2, 1

//! \brief Get last variadic argument (concept)
//! \note Used to return the last variadic argument
//! \warning The number of arguments heavily depends on the compiler and the target environment
#define GPCL_CONCEPT_VA_GET_LAST( \
	_1, _2, _3, _4, _5, _6, _7, _8, _9, _10,_11,_12,_13,_14,_15,_16, \
	_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32, \
	_33,_34,_35,_36,_37,_38,_39,_40,_41,_42,_43,_44,_45,_46,_47,_48, \
	_49,_50,_51,_52,_53,_54,_55,_56,_57,_58,_59,_60,_61,_62,_63,_64, \
	_65, ...) _65

//! \brief Variadic arguments count getter (concept)
//! \note Serves as a proxy between GPCL_CONCEPT_VA_ARGC and GPCL_CONCEPT_VA_GET_LAST
#define GPCL_CONCEPT_VA_GET_ARGC(...) GPCL_CONCEPT_VA_GET_LAST(__VA_ARGS__)

//! \brief Variadic arguments count (concept)
//! \note Used to return the number of variadic arguments
//! \warning The number of arguments heavily depends on the compiler and the target environment
//! \warning Treats no arguments as one argument. This behavior is intensional, solutions introduce a variety of issues
//! \example Transforms () and (abcdef) into 1. See warning for note
//! \example Transforms (abc, def) into 2
//! \example Transforms (ab, cd, ef) into 3
#define GPCL_CONCEPT_VA_ARGC(...) GPCL_CONCEPT_VA_GET_ARGC(__VA_ARGS__, GPCL_CONCEPT_VA_FILLER)

//! \subsection Aliases for the concepts

//! \brief Stringify
//! \example Transforms (ab, cd, ef) into "ab cd ef"
#define gpcl_stringify(...) GPCL_CONCEPT_STRINGIFY(__VA_ARGS__)

//! \brief Variadic arguments (are) empty
//! \example Transforms () into true
//! \example Transforms (abcdef), (abc, def), (ab, cd, ef), etc. into false
#define gpcl_va_empty(...) GPCL_CONCEPT_VA_EMPTY(__VA_ARGS__)

//! \brief Variadic arguments count (fast)
//! \note Used to return the number of variadic arguments
//! \warning Treats no arguments as one argument. This behavior is intensional, solutions introduce a variety of issues
//! \see gpcl_va_argc_precise for a no-arguments-aware version
//! \example Transforms () and (abcdef) into 1. See warning for note
//! \example Transforms (abc, def) into 2
//! \example Transforms (ab, cd, ef) into 3
#define gpcl_va_argc_fast(...) GPCL_CONCEPT_VA_ARGC(__VA_ARGS__)

//! \brief Variadic arguments count (precise)
//! \note Used to return the number of variadic arguments
//! \warning Distinguishes no arguments from one argument, using gpcl_va_empty
//! \bug Causes issues with preprocessor directives
//! \see gpcl_va_argc_fast for a no-arguments-unaware version
//! \see gpcl_va_empty to understand why this solution works (and why it doesn't)
//! \example Transforms () into 0
//! \example Transforms (abcdef) into 1
//! \example Transforms (abc, def) into 2
//! \example Transforms (ab, cd, ef) into 3
#define gpcl_va_argc_precise(...) (gpcl_va_empty(__VA_ARGS__) ? 0 : gpcl_va_argc_fast(__VA_ARGS__))

//! \brief Variadic arguments count
//! \note Used to return the number of variadic arguments
//! \warning The default implementation is gpcl_va_argc_precise which is no-arguments-aware
//! \bug Causes issues with preprocessor directives (inherited from gpcl_va_argc_precise)
//! \see gpcl_va_argc_fast for a no-arguments-unaware version
//! \see gpcl_va_argc_precise for a no-arguments-aware version
//! \example Transforms () into 0
//! \example Transforms (abcdef) into 1
//! \example Transforms (abc, def) into 2
//! \example Transforms (ab, cd, ef) into 3
#define gpcl_va_argc(...) gpcl_va_argc_precise(__VA_ARGS__)
