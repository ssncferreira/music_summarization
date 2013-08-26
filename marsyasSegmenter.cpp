#include "MarSystemManager.h"

#include <math.h>		
#include <vector>
#include "SelfSimilarityMatrix.h"

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
                MarSystem* netGain = mng.create("Series", "netGain");
                MarSystem* net1 = mng.create("Series", "net1");
                MarSystem* self = mng.create("SelfSimilarityMatrix", "self");
                
                
//                 self -> addMarSystem(mng.create("PowerSpectrum", "ps"));
//                 self -> updctrl("PowerSpectrum/ps/mrs_string/spectrumType", "magnitude");
//                 self -> addMarSystem(mng.create("Spectrum2Chroma", "s2c"));
//                 self -> addMarSystem(mng.create("Gain", "g"));
//                 self -> updctrl("Gain/g2/mrs_real/gain", 1.0);
        
                
                net -> addMarSystem(mng.create("SoundFileSource", "src"));
                net -> updctrl("SoundFileSource/src/mrs_string/filename", sfName);
                net1 -> addMarSystem(mng.create("Gain", "g"));
                net1 -> updctrl("Gain/g/mrs_real/gain", 1.0);
                netGain -> addMarSystem(mng.create("Spectrum","spc"));
                
                net->addMarSystem(net1);
                net1->addMarSystem(self);
                self -> addMarSystem(netGain);
             
                mrs_natural israte = net -> getctrl("mrs_real/israte") -> to<mrs_real>();
                cout << "Input rate: " << israte << endl;
                cout << "Before Input Samples: " << net -> getctrl("mrs_natural/inSamples") << endl;
                cout << "After Input Samples: " << net -> getctrl("mrs_natural/inSamples") << endl;
                mrs_natural oSamples = net -> getctrl("mrs_natural/onObservations")->to<mrs_natural>();
                cout << "Ouput Observations: " << oSamples << endl;
                
                int seconds = 0;
                
                while(net -> getctrl("SoundFileSource/src/mrs_bool/hasData") -> to<mrs_bool>()) {
                    net->tick();
                    cout << "Seconds: " << seconds << endl;
                    seconds++;    
                    const realvec& processedData = net1->getctrl("SelfSimilarityMatrix/self/mrs_realvec/processedData")->to<mrs_realvec>();
                    cout << "SelfSimilarityMatrix: = " << endl << processedData << endl;																				
                }
                cout << "Seconds Total: " << seconds << endl;
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


