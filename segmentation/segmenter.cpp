#include "MarSystemManager.h"

using namespace std;
using namespace Marsyas;

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

	while(net -> getctrl("SoundFileSource/src/mrs_bool/hasData") -> to<mrs_bool>()) {
		net -> tick();
		
		const realvec& processedData = 
			net -> getctrl("Spectrum2Chroma/s2c/mrs_realvec/processedData") -> to<mrs_realvec>();
		cout << "Spectrum2Chroma = " << processedData << endl;
		cout << "######################" << endl;
	}

	delete net;
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

