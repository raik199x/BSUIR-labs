import qbs

Project {
	CppApplication {
		name: "Lab2"
		consoleApplication: true
		cpp.commonCompilerFlags: ["-O1"]
		cpp.cxxLanguageVersion: "c++20"
		cpp.dynamicLibraries: ["criterion"]
		install: true
		files: ["srcpp/*.hpp", "srcpp/main.cpp"]
	}
}
