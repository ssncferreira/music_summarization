#include "MarSystemManager.h"
#include <math.h>		// mathematical operations (sqrt, pow,...)
#include <vector>

using namespace std;
using namespace Marsyas;

// Global variables - DELETE
// in each position of 'reslt_vector' is a realvec, e.g.
// in position 0 of result_vector is realvec processedData of second 0
vector<realvec> result_vector;
	
	
void print_information(MarSystem* net) {
	cout << "net: " << endl;
	cout << "	- inSamples: " << net -> getctrl("mrs_natural/inSamples") << endl;
	cout << "	- israte: " << net -> getctrl("mrs_real/israte") << endl;
	cout << "	- onSamples: " << net -> getctrl("mrs_natural/onSamples") << endl;
	cout << "	- osrate: " << net -> getctrl("mrs_real/osrate") << endl << endl;

	cout << "	src: " << endl;
	cout << "		- size: " << net -> getctrl("SoundFileSource/src/mrs_natural/size") << endl;
	cout << "		- inSamples: " << net -> getctrl("SoundFileSource/src/mrs_natural/inSamples") << endl;
	cout << "		- israte: " << net -> getctrl("SoundFileSource/src/mrs_real/israte") << endl;
	cout << "		- onSamples: " << net -> getctrl("SoundFileSource/src/mrs_natural/onSamples") << endl;
	cout << "		- osrate: " << net -> getctrl("SoundFileSource/src/mrs_real/osrate") << endl << endl;
	
	cout << "	chroma: " << endl;
	cout << "		- inSamples: " << net -> getctrl("Chroma/chroma/mrs_natural/inSamples") << endl;
	cout << "		- israte: " << net -> getctrl("Chroma/chroma/mrs_real/israte") << endl;
	cout << "		- onSamples: " << net -> getctrl("Chroma/chroma/mrs_natural/onSamples") << endl;
	cout << "		- osrate: " << net -> getctrl("Chroma/chroma/mrs_real/osrate") << endl << endl;
	
	cout << "	arff: " << endl;
	cout << "		- inSamples: " << net -> getctrl("ArffFileSink/arff/mrs_natural/inSamples") << endl;
	cout << "		- israte: " << net -> getctrl("ArffFileSink/arff/mrs_real/israte") << endl;
	cout << "		- onSamples: " << net -> getctrl("ArffFileSink/arff/mrs_natural/onSamples") << endl;
	cout << "		- osrate: " << net -> getctrl("ArffFileSink/arff/mrs_real/osrate") << endl << endl;
	
	cout << "	g: " << endl;
	cout << "		- inSamples: " << net -> getctrl("Gain/g/mrs_natural/inSamples") << endl;
	cout << "		- israte: " << net -> getctrl("Gain/g/mrs_real/israte") << endl;
	cout << "		- onSamples: " << net -> getctrl("Gain/g/mrs_natural/onSamples") << endl;
	cout << "		- osrate: " << net -> getctrl("Gain/g/mrs_real/osrate") << endl << endl;
}

void print_result_vector() {
	cout << "---------------RESULT VECTOR---------------" << endl;
	
	for(int i = 0; i < result_vector.size(); i++) {
		cout << "result_vector[" << i << "]" << endl;
		for(int x = 0; x < result_vector[i].getSize(); x++) {
			cout << "	- " << x << " = " << result_vector[i].getValueFenced(x) << endl;
		}
	}
	
	cout << "---------------RESULT VECTOR---------------" << endl << endl;
}

void print_matrix(int vector_size) {
	cout << "---------------MATRIX---------------" << endl;
	
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
			cout << matrix[rows][columns] << " ";
			
			result = 0;
			magnitude1 = 0;
			magnitude2 = 0;
			dot_product = 0;
		}
		cout << endl;
	}
	
	cout << "---------------MATRIX---------------" << endl << endl;
}

void segmenter(string sfName) {
	MarSystemManager mng;
	MarSystem* net = mng.create("Series", "net");
	
	net -> addMarSystem(mng.create("SoundFileSource", "src"));
	net -> updctrl("SoundFileSource/src/mrs_string/filename", sfName);
	
	net -> addMarSystem(mng.create("Spectrum","spc"));

/* Using spectrum - too high values at the end	
	net -> addMarSystem(mng.create("PowerSpectrum", "ps"));
	net -> updctrl("PowerSpectrum/ps/mrs_string/spectrumType", "magnitude");
	net -> addMarSystem(mng.create("Spectrum2Chroma", "s2c"));
*/	
	
	net -> addMarSystem(mng.create("Chroma", "chroma"));
	
	net -> addMarSystem(mng.create("Gain", "g"));	// necessary to obtain concrete values
	
	// Creates an arff file to use .dat on gnuplot
	net -> addMarSystem(mng.create("ArffFileSink", "arff"));
	net -> updctrl("ArffFileSink/arff/mrs_string/filename", "result/result.arff");
	
/* Test audio signal - play filename
	net -> addMarSystem(mng.create("AudioSink", "dest"));
	net -> updctrl("AudioSink/dest/mrs_bool/initAudio", true);
*/

	// mrs_real/israte: rate (number of samples per second) of input
	mrs_natural israte = net -> getctrl("mrs_real/israte") -> to<mrs_real>();
	
	// Associate each one-second segment window of the musical piece
	// number of input samples must be equivalent at number of samples per second (israte)
	// multiplied by 2 (2 channels) - NECESSARY the multiplicaiton???
	net -> updctrl("mrs_natural/inSamples", israte*2);
	
	print_information(net);

	int second = 0;
	mrs_natural position = net -> getctrl("SoundFileSource/src/mrs_natural/pos") -> to<mrs_natural>();
		
	cout << "Initial position: " << position << endl << endl;
	
	// print processedData
	cout << "---------------PROCESSED DATA---------------" << endl;
	
	// processed data by Spectrum2Chroma
	realvec processedData;
	
	while(net -> getctrl("SoundFileSource/src/mrs_bool/hasData") -> to<mrs_bool>()) {
		net -> tick();
		
		position = net -> getctrl("SoundFileSource/src/mrs_natural/pos") -> to<mrs_natural>();
		
		/*const - When const is used, instruction 'delete net;' results in segmentation
		 * fault - necessary because processedData is of type const realvec& */ 
		processedData = net -> getctrl("Chroma/chroma/mrs_realvec/processedData") -> to<mrs_realvec>();
		cout << "ChromaFilter second " << second <<  " position " << position << " = " << processedData << endl;
		
		result_vector.push_back(processedData);
		
		second++;
		
		// end after 5º second - test only
		/*if(second == 4) {
			break;
		}*/
	}
	
	cout << "---------------PROCESSED DATA---------------" << endl << endl;
	
	// print result_vector
	//print_result_vector();
	
	// calculate and print matrix 
	print_matrix(result_vector.size());
	
	//delete net;	// Segmentation fault (see instruction 'const realvec& processedData = ...)'
}

int main(int argc, const char **argv) {
	string fileName;
	
	if (argc < 2) {
		cout << "Please enter a filename to segment." << endl;
		exit(1);
	}
	else {
		fileName = argv[1];
	}
	
	segmenter(fileName);
	exit(0);
}

