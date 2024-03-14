#pragma once

#include "./variadic.h"

//! \section Join

//! \subsection Rules

//! \brief Join rules (concept)
//! \note Joins two arguments into one according to the implemented rules
//! \example Transforms (abc, def) into abc_def
#define GPCL_CONCEPT_JOIN_RULES(ARG1, ARG2) ARG1 ## _ ## ARG2

//! \brief Join arguments (concept)
//! \note Joins two arguments into one using rules implemented in GPCL_CONCEPT_JOIN_RULES
//! \example Transforms (abc, def) into abc_def
//! \see GPCL_CONCEPT_JOIN_RULES
#define GPCL_CONCEPT_JOIN_ARGS(ARG1, ARG2) GPCL_CONCEPT_JOIN_RULES(ARG1, ARG2)

//! \subsection Implementation

//! \warning \todo Number of implemented arguments is limited and should be extended if needed

#define GPCL_CONCEPT_JOIN_1_ARGS(ARG1) ARG1
#define GPCL_CONCEPT_JOIN_2_ARGS(ARG1, ARG2) GPCL_CONCEPT_JOIN_ARGS(ARG1, ARG2)
#define GPCL_CONCEPT_JOIN_3_ARGS(ARG1, ARG2, ARG3) GPCL_CONCEPT_JOIN_ARGS(GPCL_CONCEPT_JOIN_1_ARGS(ARG1), \
	GPCL_CONCEPT_JOIN_2_ARGS(ARG2, ARG3))
#define GPCL_CONCEPT_JOIN_4_ARGS(ARG1, ARG2, ARG3, ARG4) GPCL_CONCEPT_JOIN_ARGS(GPCL_CONCEPT_JOIN_2_ARGS(ARG1, ARG2), \
	GPCL_CONCEPT_JOIN_2_ARGS(ARG3, ARG4))
#define GPCL_CONCEPT_JOIN_5_ARGS(ARG1, ARG2, ARG3, ARG4, ARG5) GPCL_CONCEPT_JOIN_ARGS(GPCL_CONCEPT_JOIN_2_ARGS(ARG1, ARG2), \
	GPCL_CONCEPT_JOIN_3_ARGS(ARG3, ARG4, ARG5))
#define GPCL_CONCEPT_JOIN_6_ARGS(ARG1, ARG2, ARG3, ARG4, ARG5, ARG6) GPCL_CONCEPT_JOIN_ARGS(GPCL_CONCEPT_JOIN_3_ARGS(ARG1, ARG2, ARG3), \
	GPCL_CONCEPT_JOIN_3_ARGS(ARG4, ARG5, ARG6))
#define GPCL_CONCEPT_JOIN_7_ARGS(ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7) GPCL_CONCEPT_JOIN_ARGS(GPCL_CONCEPT_JOIN_3_ARGS(ARG1, ARG2, ARG3), \
	GPCL_CONCEPT_JOIN_4_ARGS(ARG4, ARG5, ARG6, ARG7))
#define GPCL_CONCEPT_JOIN_8_ARGS(ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8) GPCL_CONCEPT_JOIN_ARGS(GPCL_CONCEPT_JOIN_4_ARGS(ARG1, ARG2, ARG3, ARG4), \
	GPCL_CONCEPT_JOIN_4_ARGS(ARG5, ARG6, ARG7, ARG8))
#define GPCL_CONCEPT_JOIN_9_ARGS(ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, ARG9) GPCL_CONCEPT_JOIN_ARGS(GPCL_CONCEPT_JOIN_4_ARGS(ARG1, ARG2, ARG3, ARG4), \
	GPCL_CONCEPT_JOIN_5_ARGS(ARG5, ARG6, ARG7, ARG8, ARG9))

//! \subsection Concepts

//! \brief Join N arguments (concept)
//! \note Transforms value N into macro GPCL_CONCEPT_JOIN_N_ARGS
//! \example Transforms 3 into GPCL_CONCEPT_JOIN_3_ARGS
//! \warning Number of implemented arguments is limited and should be extended if needed
#define GPCL_CONCEPT_JOIN_N_ARGS(N) GPCL_CONCEPT_JOIN_ ## N ## _ARGS

//! \brief Join variadic arguments (concept)
//! \note Serves as a proxy between GPCL_CONCEPT_JOIN and GPCL_CONCEPT_JOIN_N_ARGS
#define GPCL_CONCEPT_JOIN_VA_ARGS(N) GPCL_CONCEPT_JOIN_N_ARGS(N)

//! \brief Join variadic arguments (concept)
//! \note Joins variadic arguments into one using defined rules and implementation
//! \warning Number of implemented arguments is limited and should be extended if needed
//! \example Transforms () into nothing
//! \example Transforms (abcdef) into abcdef
//! \example Transforms (abc, def) into abc_def
//! \example Transforms (ab, cd, ef) into ab_cd_ef
//! \example Transforms (a, b, c, d, e, f) into a_b_c_d_e_f
#define GPCL_CONCEPT_JOIN(...) GPCL_CONCEPT_JOIN_VA_ARGS(gpcl_va_argc_fast(__VA_ARGS__))(__VA_ARGS__)

//! \subsection Aliases for the concepts

//! \brief Join variadic arguments
//! \note Joins variadic arguments into one using defined rules and implementation
//! \warning Number of implemented arguments is limited and should be extended if needed
//! \example Transforms () into nothing
//! \example Transforms (abcdef) into abcdef
//! \example Transforms (abc, def) into abc_def
//! \example Transforms (ab, cd, ef) into ab_cd_ef
//! \example Transforms (a, b, c, d, e, f) into a_b_c_d_e_f
//! \sa gpcl(...)
#define join(...) GPCL_CONCEPT_JOIN(__VA_ARGS__)

//! \brief Join variadic arguments with gpcl prefix
//! \note Joins variadic arguments into one using defined rules and implementation, adding gpcl prefix
//! \warning Number of implemented arguments is limited and should be extended if needed
//! \example Transforms (abcdef) into gpcl_abcdef
//! \example Transforms (abc, def) into gpcl_abc_def
//! \example Transforms (ab, cd, ef) into gpcl_ab_cd_ef
//! \example Transforms (a, b, c, d, e, f) into gpcl_a_b_c_d_e_f
//! \sa join(...)
#define gpcl(...) join(gpcl, __VA_ARGS__)
