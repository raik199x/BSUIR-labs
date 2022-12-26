import qbs.FileInfo

Project {
	CppApplication {
		Depends {
			name: "cpp"
		}
		cpp.defines: []
		cpp.cxxLanguageVersion: "c++20"
		cpp.includePaths: ["/usr/include/opencv4"]
		cpp.rpaths: "/usr/lib"
		cpp.libraryPaths: "/usr/lib"
		cpp.dynamicLibraries: ["ncurses", "opencv_core", "opencv_videoio", "opencv_imgcodecs"]

		name: "SilentPhoto"
		files: ["SilentPhoto.cpp"]

		install: true
		installDir: qbs.targetOS.contains("qnx") ? FileInfo.joinPaths(
													   "/tmp", name,
													   "bin") : base
	}
	CppApplication {
		Depends {
			name: "cpp"
		}
		cpp.defines: []
		cpp.cxxLanguageVersion: "c++20"
		cpp.includePaths: ["/usr/include/opencv4"]
		cpp.rpaths: "/usr/lib"
		cpp.libraryPaths: "/usr/lib"
		cpp.dynamicLibraries: ["ncurses", "opencv_core", "opencv_videoio", "opencv_imgcodecs"]

		name: "SilentVideo"
		files: ["SilentVideo.cpp"]

		install: true
		installDir: qbs.targetOS.contains("qnx") ? FileInfo.joinPaths(
													   "/tmp", name,
													   "bin") : base
	}
}
