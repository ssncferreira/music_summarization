#include "MarSystemManager.h"

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

	int i = 0;		// DELETE
	mrs_natural size = net -> getctrl("SoundFileSource/src/mrs_natural/size") -> to<mrs_natural>();
	mrs_natural position = 0;
	
	while(net -> getctrl("SoundFileSource/src/mrs_bool/hasData") -> to<mrs_bool>()) {
		net -> tick();
		
		i++;
		cout << "i: " << i << endl;
		position = net -> getctrl("SoundFileSource/src/mrs_natural/pos") -> to<mrs_natural>();
		cout << "src -> pos: " << position << endl;
		
		/*const - When const is used, instruction 'delete net;' results in segmentation
		 * fault - necessary because processedData is of type const realvec& */ 
		const realvec& processedData = 
			net -> getctrl("Spectrum2Chroma/s2c/mrs_realvec/processedData") -> to<mrs_realvec>();
		cout << "Spectrum2Chroma = " << processedData << endl;
		cout << "######################" << endl;
	}
	
	if(position < size) {
		cout << "ERROR: information lost..." << endl;
	}

	delete net;	// Segmentation fault (see instruction 'const realvec& processedData = ...)'
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

