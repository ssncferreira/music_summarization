#include "MarSystemManager.h"
#include <math.h>		// mathematical operations (sqrt, pow,...)
#include <vector>

using namespace std;
using namespace Marsyas;

void printInformation(MarSystem* net) {
	cout << "net -> inSamples: " << net -> getctrl("mrs_natural/inSamples") -> to<mrs_natural>() << endl;
	cout << "net -> onSamples: " << net -> getctrl("mrs_natural/onSamples") -> to<mrs_natural>() << endl;
	cout << "net -> israte: " << net -> getctrl("mrs_real/israte") -> to<mrs_real>() << endl;
	cout << "net -> osrate: " << net -> getctrl("mrs_real/osrate") -> to<mrs_real>() << endl;
	cout << "##############################" << endl;
	cout << "src -> inSamples: " << net -> getctrl("SoundFileSource/src/mrs_natural/inSamples") -> to<mrs_natural>() << endl;
	cout << "src -> onSamples: " << net -> getctrl("SoundFileSource/src/mrs_natural/onSamples") -> to<mrs_natural>() << endl;
	cout << "src -> israte: " << net -> getctrl("SoundFileSource/src/mrs_real/israte") -> to<mrs_real>() << endl;
	cout << "src -> osrate: " << net -> getctrl("SoundFileSource/src/mrs_real/osrate") -> to<mrs_real>() << endl;
	cout << "##############################" << endl;
}

void segmenter(string sfName) {
	MarSystemManager mng;
	MarSystem* net = mng.create("Series", "net");

	net -> addMarSystem(mng.create("SoundFileSource", "src"));
	net -> updctrl("SoundFileSource/src/mrs_string/filename", sfName);

	net -> addMarSystem(mng.create("Spectrum","spc"));
	
	net -> addMarSystem(mng.create("PowerSpectrum", "ps"));
	net -> updctrl("PowerSpectrum/ps/mrs_string/spectrumType", "magnitude");
	
	net -> addMarSystem(mng.create("Spectrum2Chroma", "s2c"));
	
	net -> addMarSystem(mng.create("Gain", "g"));

/* Test audio signal
	net -> addMarSystem(mng.create("AudioSink", "dest"));
	net -> updctrl("AudioSink/dest/mrs_bool/initAudio", true);
*/

	// mrs_real/israte: rate (number of samples per second) of input
	mrs_natural israte = net -> getctrl("mrs_real/israte") -> to<mrs_real>();
	
	// Associate each one-second segment window (israte/2) of the musical piece
	// number of input samples in a tick must be equivalent at number of samples in a second (israte)
	// divided by 2 (2 channels) -> wrong?
	net -> updctrl("mrs_natural/inSamples", israte/2);

	//printInformation(net);

	int second = 0;
	mrs_natural size = net -> getctrl("SoundFileSource/src/mrs_natural/size") -> to<mrs_natural>();
	mrs_natural position = 0;
	
	// in each position of 'reslt_vector' is a realvec, e.g.
	// in position 0 of result_vector is realvec processedData of second 0
	vector<realvec> result_vector;

	
	
	while(net -> getctrl("SoundFileSource/src/mrs_bool/hasData") -> to<mrs_bool>()) {
		net -> tick();
		
		position = net -> getctrl("SoundFileSource/src/mrs_natural/pos") -> to<mrs_natural>();
		//cout << "src -> pos: " << position << endl;
		
		/*const - When const is used, instruction 'delete net;' results in segmentation
		 * fault - necessary because processedData is of type const realvec& */ 
		const realvec& processedData = 
			net -> getctrl("Spectrum2Chroma/s2c/mrs_realvec/processedData") -> to<mrs_realvec>();
		cout << "Spectrum2Chroma second " << second <<  " = " << processedData << endl;
		
		result_vector.push_back(processedData);
		
		/*
		cout << "Size: " << processedData.getSize() << ": " << endl;
		cout << "Columns: " << processedData.getCols() << ": " << endl;
		cout << "Rows: " << processedData.getRows() << ": " << endl;
		*/
		// Usage of euclidean distance
		//mrs_real result = 0;
		
		
		
		// processedData values at index i (1 column and 12 rows)
		//for(i = 0; i < processedData.getSize(); i++) {
			//cout << "	-posicao " << i << ": " << processedData.getValueFenced(i) << endl;
			// euclidean distance between 2 equal vectors -> must be 0 
			//result += pow(processedData.getValueFenced(i) - processedData.getValueFenced(i), 2);
		//}

		// square root of 'result'
		//result = sqrt(result);
		//cout << "final result: " << result << endl;
		second++;
		/*
		if (second == 2)
			break;*/
	}
	
	// print result_vector
	/*for(int i = 0; i < result_vector.size(); i++) {
		cout << "result_vector[" << i << "]" << endl;
		for(int x = 0; x < result_vector[i].getSize(); x++) {
			cout << "	- " << x << " = " << result_vector[i].getValueFenced(x) << endl;
		}
	}*/
	
	int vector_size = result_vector.size();
	cout << "vector_size: " << vector_size << endl;
	
	// every element of matrix matrix[i,j] represents the similarity between two onde-second
	// segments of the piece at different times (euclidean distance is used)
	mrs_real matrix[vector_size][vector_size];
	
	
	// print matrix
	// calculate matrix
	cout << "  |";
	for(int i = 0; i < vector_size; i++) {
		cout << i << "|";
	}
	cout << endl << "---";
	for(int j = 0; j < vector_size*2; j++) {
		cout << "-";
	}
	cout << endl;
			
			
	mrs_real result = 0;
		
	for(int rows = 0; rows < vector_size; rows++) {
		if(rows < 10) {
			cout << rows << " |";
		}
		else {
			cout << rows << "|";
		}
		for(int columns = 0; columns < vector_size; columns++) {
			if(rows == columns) {
				// optimization -> always equal to 0
				matrix[rows][columns] = 0.0;
			}
			else {
			
				// Euclidean distance 
				for(int x = 0; x < result_vector[rows].getSize(); x++) {
					result += pow(result_vector[rows].getValueFenced(x) - 
						result_vector[columns].getValueFenced(x), 2);
				}
				
				matrix[rows][columns] = sqrt(result);
			}
			cout << matrix[rows][columns];
			
			if(columns < 10) {
				cout << "|";
			}
			else {
				cout << " |";
			}
		}
		cout << endl;
	}
	
	
	if(position < size) {
		cout << "ERROR: information lost..." << endl;
	}

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

