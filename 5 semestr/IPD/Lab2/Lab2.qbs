import qbs

CppApplication {
	Depends {
		name: "cpp"
	}
	cpp.cxxLanguageVersion: "c++20"

	consoleApplication: true
	install: true

	excludeFiles: ["kernmod/tcfxm.c"]
	files: ["main.cpp", "kernmod.cpp", "kernmod.hpp", "kernmod/tcfxm.link", "kernmod/Makefile", "lpalette/lpalette.cpp", "lpalette/lpalette.hpp", "pcitable.cpp", "pcitable.hpp"]
}
