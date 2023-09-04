import qbs

Project {
	CppApplication {
		name: "Tests"
		consoleApplication: true
		cpp.commonCompilerFlags: ["-O2", "-mfma"]
		cpp.cxxLanguageVersion: "c++20"
		cpp.dynamicLibraries: ["criterion"]
		install: true
		files: ["srcpp/*.hpp", "srcpp/tests.cpp"]
	}
}
