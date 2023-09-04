#include <fstream>
#include <iostream>
#include <mpich/mpi.h>
#include <mpich/mpi_proto.h>
#include <string>

std::string GetSystemName() {
	std::string command = "neofetch | grep OS | cut -d \":\" -f 2 > temp.txt";
	system(command.c_str());
	std::ifstream outputFile("temp.txt");

	std::string output((std::istreambuf_iterator<char>(outputFile)),
	                   std::istreambuf_iterator<char>());

	// Remove the output file
	remove("temp.txt");
	return output;
}

int main(int argc, char **argv) {

	MPI_Init(&argc, &argv);
	int myId, amountOfProcesses;
	MPI_Comm_rank(MPI_COMM_WORLD, &myId);
	MPI_Comm_size(MPI_COMM_WORLD, &amountOfProcesses);

	if (myId == 0) {
		for (int i = 1; i < amountOfProcesses; i++) {
			int sizeOfLine;
			MPI_Recv(&sizeOfLine, 1, MPI_INT, i, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			char *data = new char[sizeOfLine];
			MPI_Recv(data, sizeOfLine, MPI_CHAR, i, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			std::string result(data);
			std::cout << "Node " << i << " is" << result;
			delete[] data;
		}
	} else {
		std::string sysName = GetSystemName();
		int size            = sysName.length();
		MPI_Send(&size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
		MPI_Send(sysName.c_str(), size, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
	}
	MPI_Finalize();
	return 0;
}