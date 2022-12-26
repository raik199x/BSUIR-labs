import qbs

CppApplication {
	Depends {
		name: "cpp"
	}
	cpp.cxxLanguageVersion: "c++20"

	consoleApplication: true
	install: true

	files: ["port/ata.hpp", "port/io.hpp", "port/io.cpp", "main.cpp", "tlibrary/tlibrary.hpp", "tlibrary/lpalette.cpp", "tlibrary/lpalette.hpp", "tlibrary/tstream.hpp"]
}
