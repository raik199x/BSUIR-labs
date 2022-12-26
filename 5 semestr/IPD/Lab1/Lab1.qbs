import qbs

CppApplication {
	Depends {
		name: "cpp"
	}
	cpp.cxxLanguageVersion: "c++20"

	consoleApplication: true
	install: true

	excludeFiles: ["kernmod/tcfxm.c"]
	files: ["main.cpp", "battery.hpp", "battery.cpp", "Makefile", "lpalette/lpalette.hpp", "lpalette/lpalette.cpp", "daemon/batteryUpTime.sh"]
}
