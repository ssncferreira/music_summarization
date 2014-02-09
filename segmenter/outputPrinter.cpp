#include "outputPrinter.h"

/* Constructors */
OutputPrinter::OutputPrinter(){ }

OutputPrinter::OutputPrinter(string file_location) {
	create_filename_matrix(file_location);
	curr_file_location = file_location;
}

/* getters */
string OutputPrinter::getFilename_Matrix() { return filename_matrix; }
string OutputPrinter::getCurr_File_Location() { return curr_file_location; }


void OutputPrinter::create_filename_matrix(string file_location) {
	// flag: true if symbol '/' is found, false otherwise
	bool folder = false;

	// iterate string until '.' (format type) is found
	for(string::iterator it = file_location.begin(); *it != '.'; ++it) {
		if(folder == true) {
			filename_matrix.push_back(*it);
		}
		if(*it == '/') {
			folder = true;
			filename_matrix.clear();   
		}
	}

	filename_matrix.insert(0, "../result/");
	filename_matrix.append("rmatrixNew.ppm");
}

void OutputPrinter::print_path_stats() {
	cout << "******************************" << endl;	
	//cout << "Number of paths: " << paths.size() << endl;
	/*	for(int i = 0; i < paths.size(); i++) {
		cout << " Path N:   " << i << " size: " << paths[i].size() <<  endl;
		for(int j = 0; j < paths[i].size(); j++) {
		cout << " " << paths[i][j];
		}
		cout << endl;
		}*/
	cout << "******************************" << endl;
}


void OutputPrinter::print_matrix(vector< vector < mrs_real> >  matrix) {
	cout << "Matrix size: " << matrix.size() << endl;
	for(unsigned int i = 0; i < matrix.size(); i++) {
		for(unsigned int j = 0; j < matrix.size(); j++) {
			cout << matrix[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
}

void OutputPrinter::write_matrix(vector< vector < mrs_real> > matrix, string fileName) {
	ofstream matrix_file;
	matrix_file.open(fileName.c_str());
	for(unsigned int i = 0; i < matrix.size(); i++) {
		for(unsigned int j = 0; j < matrix.size(); j++) {
			matrix_file << matrix[i][j] << " ";
		}
		matrix_file << endl;
	}
	matrix_file.close();
}

void OutputPrinter::write_path_PGM(mrs_real matrix_size,vector< vector <mrs_real> > paths, string fileName) {
	ofstream matrix_file;
	vector<vector <mrs_real> > pathMatrix (matrix_size, vector<mrs_real>(matrix_size, 0));
	matrix_file.open(fileName.c_str());
	matrix_file << "P2" << endl << pathMatrix.size() << " " << pathMatrix.size() << endl;
	matrix_file << "2500" << endl;
	mrs_real tempRow = 0;
	mrs_real tempCol = 0;

	for(unsigned int i = 0; i < paths.size(); i++) {
		for(unsigned int j = 0; j < paths[i].size(); j=j+3) {
			tempRow = paths[i][j];
			tempCol = paths[i][j+1];
			pathMatrix[tempRow][tempCol] = paths[i][j+2]; // Cada posicao de paths é um triplo Linha,Coluna,Valor
		}   
	}   

	for(unsigned int i = 0; i < pathMatrix.size(); i++) {
		for(unsigned int j = 0; j < pathMatrix.size(); j++) {
			matrix_file << pathMatrix[i][j] << " ";
		}   
		matrix_file << endl;
	}   
	matrix_file.close();
}

void OutputPrinter::write_matrix_PPM(const vector< vector < mrs_real> > &matrix, const string &fileName) {
	ofstream matrix_file;
	matrix_file.open(fileName.c_str());
	matrix_file << "P3" << endl << matrix.size() << " " << matrix.size() << endl;
	matrix_file << "2500" << endl;
	for(unsigned int i = 0; i < matrix.size(); i++) {
		for(unsigned int j = 0; j < matrix.size(); j++) {
			matrix_file << matrix[i][j] << " " << 0 << " " << 0 << "  ";
		}   
		matrix_file << endl;
	}   
	matrix_file.close();
}

