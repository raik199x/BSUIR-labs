import qbs

CppApplication {
	Depends {
		name: "cpp"
	}
	cpp.cxxLanguageVersion: "c++20"
	cpp.includePaths: ["tlibrary"]

	cpp.rpaths: "/usr/local/lib"
	cpp.libraryPaths: "/usr/local/lib"
	cpp.dynamicLibraries: ["ncurses"]

	consoleApplication: true
	install: true

	files: ["main.cpp", "tlibrary/tlibrary.hpp", "tlibrary/tstream.hpp", "tlibrary/tcurses.hpp", "tlibrary/tcurses.cpp"]
}
