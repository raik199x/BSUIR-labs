#include <iostream>
#include <string>
#include <fstream>

using namespace std;

int main(){
	int startNumber;
	string fileName;
	cout << "Enter file path" << endl;
	cin >> fileName;
	cout << "Enter start number" << endl;
	cin >> startNumber;

	ifstream file(fileName);
	if (!file.is_open()){
		cout << "Error file" << endl;
		return -1;
	}
	string line;
	while(getline(file,line)){
		if (startNumber < 10)
			cout << "00" << startNumber << ". " << line << endl;
		else if (startNumber < 100)
			cout << "0" << startNumber << ". " << line << endl;
		else
			cout << startNumber << ". " << line << endl;
		startNumber++;
	}
	file.close();
	return 0;
}
