#include "multiply.hpp"
#include "rand_gen.hpp"

int display_options(const char* const title)
{
	cout << "Lab 1:" << endl
		 << "  Four-dimensional table (matrix) multiplication" << endl << endl
		 << "Usage:" << endl
		 << "  " << title << " OPTION" << endl << endl
		 << "Available Options:" << endl
		 << "  auto" << "\t\t" << "with automatic vectorization (if enabled)" << endl
		 << "  manual" << "\t" << "with manual vectorization" << endl
		 << "  all" << "\t\t" << "with both methods" << endl;
	return 0;
}

int invalid_option(const char* const title, const char* const option)
{
	cout << "ERROR: Unexpected option '" << option << "'." << endl
		 << "Try to run'" << title << "' to see the list of all available options..." << endl;
	return 1;
}

int main(const int argc, const char* const argv[]) {
	if (argc == 1)
		return display_options(argv[0]);

	string argument = argv[1];
	cout << boolalpha;

	if (argument != "all" && argument != "auto" && argument != "manual")
		return invalid_option(argv[0], argv[1]);

	auto TableOne = table::create();
	auto TableTwo = table::create();
	auto ResultTable = table::create();
	auto CorrectTable = table::create();

	for(size_t i = 0; i < auto_table_size; i++)
		for(size_t j = 0 ; j < auto_table_size; j++ )
			for(size_t k = 0; k < auto_matrix_size; k++)
				for(size_t l = 0; l < auto_matrix_size; l++)
					TableOne[i][j][k][l] = TableTwo[i][j][l][k] = rand_gen<int, uniform_int_distribution<int>>(0, 5);

	if(argument == "all" || argument == "auto") {
		table::mul(CorrectTable, TableOne, TableTwo);
		auto period = table::mul_auto_vector(ResultTable, TableOne, TableTwo);
		const bool correctness = table::equal(ResultTable, CorrectTable);
		cout << "It took me " << period.count() << " seconds" << endl
			 << "Correctness: " << correctness << endl;
		if (!correctness)
			cout << "Result: " << matrix::to_string(ResultTable[0][0]) << endl
				 << "Correct: " << matrix::to_string(CorrectTable[0][0]) << endl;
	}

	if (argument == "all" || argument == "manual") {
		table::mul(CorrectTable, TableOne, TableTwo);
		auto period = table::mul_manual_vector(ResultTable, TableOne, TableTwo).count();
		const bool correctness = table::equal(ResultTable, CorrectTable);
		cout << "It took me " << period << " seconds" << endl
			 << "Correctness: " << correctness << endl;
		if (!correctness)
			cout << "Result: " << matrix::to_string(ResultTable[0][0]) << endl
				 << "Correct: " << matrix::to_string(CorrectTable[0][0]) << endl;
	}

	table::destroy(CorrectTable);
	table::destroy(ResultTable);
	table::destroy(TableTwo);
	table::destroy(TableOne);

	return 0;
}
