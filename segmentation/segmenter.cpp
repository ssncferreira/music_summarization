#include "MarSystemManager.h"
#include <math.h>		// mathematical operations (sqrt, pow,...)
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;
using namespace Marsyas;

// Global variables - DELETE
// in each position of 'reslt_vector' is a realvec, e.g.
// in position 0 of result_vector is realvec processedData of second 0
vector<realvec> result_vector;

// Name of file to process
string filename_matrix;

// filename_matrix has format result/filename_matrix.dat
void create_filename_matrix(string file_location) {
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

	filename_matrix.insert(0, "result/");

	filename_matrix.append("_matrix.dat");
}

void print_information(MarSystem* net) {
	cout << "net: " << endl;
	cout << "	- inSamples: " << net -> getctrl("mrs_natural/inSamples") << endl;
	cout << "	- israte: " << net -> getctrl("mrs_real/israte") << endl;
	cout << "	- onSamples: " << net -> getctrl("mrs_natural/onSamples") << endl;
	cout << "	- osrate: " << net -> getctrl("mrs_real/osrate") << endl << endl;
/*
	cout << "	src: " << endl;
	cout << "		- size: " << net -> getctrl("SoundFileSource/src/mrs_natural/size") << endl;
	cout << "		- inSamples: " << net -> getctrl("SoundFileSource/src/mrs_natural/inSamples") << endl;
	cout << "		- israte: " << net -> getctrl("SoundFileSource/src/mrs_real/israte") << endl;
	cout << "		- onSamples: " << net -> getctrl("SoundFileSource/src/mrs_natural/onSamples") << endl;
	cout << "		- osrate: " << net -> getctrl("SoundFileSource/src/mrs_real/osrate") << endl << endl;
*/
	cout << "	chroma: " << endl;
	cout << "		- inSamples: " << net -> getctrl("Chroma/chroma/mrs_natural/inSamples") << endl;
	cout << "		- israte: " << net -> getctrl("Chroma/chroma/mrs_real/israte") << endl;
	cout << "		- onSamples: " << net -> getctrl("Chroma/chroma/mrs_natural/onSamples") << endl;
	cout << "		- osrate: " << net -> getctrl("Chroma/chroma/mrs_real/osrate") << endl << endl;
/*
	cout << "	arff: " << endl;
	cout << "		- inSamples: " << net -> getctrl("ArffFileSink/arff/mrs_natural/inSamples") << endl;
	cout << "		- israte: " << net -> getctrl("ArffFileSink/arff/mrs_real/israte") << endl;
	cout << "		- onSamples: " << net -> getctrl("ArffFileSink/arff/mrs_natural/onSamples") << endl;
	cout << "		- osrate: " << net -> getctrl("ArffFileSink/arff/mrs_real/osrate") << endl << endl;

	cout << "	g: " << endl;
	cout << "		- inSamples: " << net -> getctrl("Gain/g/mrs_natural/inSamples") << endl;
	cout << "		- israte: " << net -> getctrl("Gain/g/mrs_real/israte") << endl;
	cout << "		- onSamples: " << net -> getctrl("Gain/g/mrs_natural/onSamples") << endl;
	cout << "		- osrate: " << net -> getctrl("Gain/g/mrs_real/osrate") << endl << endl;*/
}

void print_result_vector() {
	for(int i = 0; i < result_vector.size(); i++) {
		cout << "result_vector[" << i << "]" << endl;
		for(int x = 0; x < result_vector[i].getSize(); x++) {
			cout << "	- " << x << " = " << result_vector[i].getValueFenced(x) << endl;
		}
	}

}

// calculate and outputs matrix to filename_matrix
void print_matrix(int vector_size) {

	cout << "Creating file " << filename_matrix << "..." << endl;

	ofstream matrix_file;
	matrix_file.open(filename_matrix.c_str());

	// every element of matrix matrix[i,j] represents the similarity between two onde-second
	// segments of the piece at different times (euclidean distance is used)
	mrs_real matrix[vector_size][vector_size];

	mrs_real result = 0;
	mrs_real dot_product = 0;
	mrs_real magnitude1 = 0;
	mrs_real magnitude2 = 0;

	// Optimization: calculate only half-matrix, since the other half is equal
	for(int rows = 0; rows < vector_size; rows++) {
		for(int columns = 0; columns < vector_size; columns++) {
			if(rows == columns) {
				// optimization -> always equal to 0 in euclidean distance
				// 1 in cosine distance
				matrix[rows][columns] = 1;
			}
			else {
				// Euclidean distance 
				/*for(int x = 0; x < result_vector[rows].getSize(); x++) {
					result += pow(result_vector[rows].getValueFenced(x) - 
						result_vector[columns].getValueFenced(x), 2);
				}
				
				matrix[rows][columns] = sqrt(result);*/

				// Cosine distance (similarity)
				// resulting similarity ranges from -1 meaning exactly opposite,
				// to 1 meaning exactly the same, 0 indicates independence
				for(int x = 0; x < result_vector[rows].getSize(); x++) {
					dot_product += result_vector[rows].getValueFenced(x) * 
						result_vector[columns].getValueFenced(x);
					magnitude1 += pow(result_vector[rows].getValueFenced(x),2);
					magnitude2 += pow(result_vector[columns].getValueFenced(x),2);
				}
				magnitude1 = sqrt(magnitude1);
				magnitude2 = sqrt(magnitude2);
				magnitude1 = magnitude1*magnitude2;

				result = dot_product/magnitude1;

				matrix[rows][columns] = result;
			}
			matrix_file << matrix[rows][columns] << " ";

			result = 0;
			magnitude1 = 0;
			magnitude2 = 0;
			dot_product = 0;
		}
		result = 0;
		magnitude1 = 0;
		magnitude2 = 0;
		dot_product = 0;

		matrix_file << endl;
	}

	matrix_file.close();
}

void segmenter(string sfName) {
	MarSystemManager mng;
	MarSystem* net = mng.create("Series", "net");
	MarSystem* parallel = mng.create("Parallel", "p");
	MarSystem* parallel_serie1 = mng.create("Series", "s1");
	MarSystem* parallel_serie2 = mng.create("Series", "s2");

	net -> addMarSystem(mng.create("SoundFileSource", "src"));
	net -> updctrl("SoundFileSource/src/mrs_string/filename", sfName);

	parallel_serie1 -> addMarSystem(mng.create("Spectrum", "spc1"));
	parallel_serie1 -> addMarSystem(mng.create("PowerSpectrum", "pw1"));
	parallel_serie1 -> updctrl("PowerSpectrum/pw1/mrs_string/spectrumType", "power");
	
	parallel_serie2 -> addMarSystem(mng.create("Spectrum", "spc2"));
	parallel_serie2 -> addMarSystem(mng.create("PowerSpectrum", "pw2"));
	parallel_serie2 -> updctrl("PowerSpectrum/pw2/mrs_string/spectrumType", "power");
	
	parallel -> addMarSystem(parallel_serie1);
	parallel -> addMarSystem(parallel_serie2);
	
	net -> addMarSystem(parallel);
	
	net -> addMarSystem(mng.create("StereoSpectrum", "ss"));

	net -> addMarSystem(mng.create("Chroma", "chroma"));
	
	net -> addMarSystem(mng.create("ArffFileSink", "arff"));
	net -> updctrl("ArffFileSink/arff/mrs_string/filename", "output.arff");

	// mrs_real/israte: rate (number of samples per second) of input
	mrs_natural israte = net -> getctrl("mrs_real/israte") -> to<mrs_real>();

	// Associate each one-second segment window of the musical piece
	// number of input samples must be equivalent at number of samples per second (israte)
	// multiplied by 2 (2 channels) - NECESSARY the multiplicaiton??? Yes, see self similarity matrix
	net -> updctrl("mrs_natural/inSamples", israte*2);

	int second = 0;
	mrs_natural position = net -> getctrl("SoundFileSource/src/mrs_natural/pos") -> to<mrs_natural>();

	// processed data by Chroma
	realvec processedData;

	while(net -> getctrl("SoundFileSource/src/mrs_bool/hasData") -> to<mrs_bool>()) {
		net -> tick();

		position = net -> getctrl("SoundFileSource/src/mrs_natural/pos") -> to<mrs_natural>();

		/*const - When const is used, instruction 'delete net;' results in segmentation
		 * fault - necessary because processedData is of type const realvec& */ 
		processedData = net -> getctrl("Chroma/chroma/mrs_realvec/processedData") -> to<mrs_realvec>();

		result_vector.push_back(processedData);

		second++;
	}
	
	print_result_vector();

	print_matrix(result_vector.size());

	//delete net;	// Segmentation fault (see instruction 'const realvec& processedData = ...)'
}

int main(int argc, const char **argv) {
	string file_location;

	if (argc < 2) {
		cout << "Please enter a filename to segment." << endl;
		exit(1);
	}
	else {
		file_location = argv[1];
	}

	create_filename_matrix(file_location);

	segmenter(file_location);
	exit(0);
}
