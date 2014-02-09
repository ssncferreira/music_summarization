#include <marsyas/MarSystemManager.h>
#include <math.h>
#include <vector>
#include <iostream>
#include <fstream>
#include "outputPrinter.h"
#include "matrix.h"

using namespace std;
using namespace Marsyas;



vector<realvec> segmenter(string sfName) {
	vector<realvec> result_vector;
	MarSystemManager mng;
	MarSystem* net = mng.create("Series", "net");

	net -> addMarSystem(mng.create("SoundFileSource", "src"));
	net -> updctrl("SoundFileSource/src/mrs_string/filename", sfName);

	net -> addMarSystem(mng.create("Spectrum", "spc1"));
	net -> addMarSystem(mng.create("PowerSpectrum", "pw1"));
	net -> updctrl("PowerSpectrum/pw1/mrs_string/spectrumType", "power");

	net -> addMarSystem(mng.create("Chroma", "chroma")); 
	net -> addMarSystem(mng.create("Gain", "g"));

	//mrs_natural israte = net -> getctrl("mrs_real/israte") -> to<mrs_real>();
	//mrs_natural inSamples = net -> getctrl("SoundFileSource/src/mrs_natural/inSamples") -> to<mrs_natural>();
	//net -> updctrl("SoundFileSource/src/mrs_real/israte", (mrs_real)inSamples);

	mrs_natural insamples = net -> getctrl("mrs_natural/inSamples") -> to<mrs_natural>(); 
	net -> updctrl("mrs_natural/inSamples", (mrs_natural)insamples*12);
	mrs_natural insamples2 = net -> getctrl("mrs_natural/inSamples") -> to<mrs_natural>();

	cout << "inSamples: " << insamples2 << endl;
	cout << "israte: " << net -> getctrl("mrs_real/israte") -> to<mrs_real>() << endl;

	cout << "src/inSamples: " << net -> getctrl("SoundFileSource/src/mrs_natural/inSamples") -> to<mrs_natural>() << endl; 
	cout << "src/israte: " << net -> getctrl("SoundFileSource/src/mrs_real/israte") -> to<mrs_real>() << endl;

	// processed data by Chroma
	realvec processedData;

	while(net -> getctrl("SoundFileSource/src/mrs_bool/hasData") -> to<mrs_bool>()) {
		net -> tick();

		/*const - When const is used, instruction 'delete net;' results in segmentation
		 * fault - necessary because processedData is of type const realvec& */
		processedData = net -> getctrl("Chroma/chroma/mrs_realvec/processedData") -> to<mrs_realvec>();
		result_vector.push_back(processedData);
	}

	cout << "size result_vector: " << result_vector.size() << endl;
	return result_vector;
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

	/* Printer object initialization */
	OutputPrinter printer(file_location);

	/* Matrix object initialization with result_vector, creates matrix */
	Matrix m(segmenter(printer.getCurr_File_Location()));

	printer.write_matrix_PPM(m.getMatrix(), printer.getFilename_Matrix());
	printer.write_matrix(m.getMatrix(), "raw_Matrix.dat"); 

	/* SSM enhancement operations */
	// ssmEnhance(matrix);

	/* Path extraction operations */
	m.remove_upper_triangle();
	m.path_extraction();

	/* Output operations */
	printer.write_path_PGM(m.getMatrix_Size(),m.getPaths(), "OnlyPaths.pgm");
	printer.write_matrix_PPM(m.getMatrix(), "PathExtractionMatrix.ppm");
	printer.write_matrix(m.getMatrix(), "end_Matrix.dat");

	exit(0);
}
