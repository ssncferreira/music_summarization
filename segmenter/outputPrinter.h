#ifndef  __OUTPUTWRITER_H_INCLUDED__   
#define  __OUTPUTWRITER_H_INCLUDED__

#include <iostream>
#include <math.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <marsyas/MarSystemManager.h>


using namespace std;
using namespace Marsyas;

class OutputPrinter {

	public:
		/* constructors */
		OutputPrinter();
		OutputPrinter(string file_location);

		/* getters */
		string getFilename_Matrix();
		string getCurr_File_Location();

		/* methods */

		/* filename_matrix has format result/rmatrixNew.pgm, automatically called on constructor */
		void create_filename_matrix(string file_location);

		/* outputs the number of paths and size of each path found */
		void print_path_stats();

		/* outputs every position of a given matrix */	
	        void print_matrix(vector< vector < mrs_real> >  matrix);

	  	/* writes to file with 'fileName' the given matrix */	
		void write_matrix(vector< vector < mrs_real> > matrix, string fileName);

		/* writes to PGM(P2) file with 'fileName' a given path matrix */
		void write_path_PGM(mrs_real matrix_size,vector< vector <mrs_real> > paths, string fileName);

		/* writes to PPM(P3) file with 'fileName' a given matrix */	
		void write_matrix_PPM(const vector< vector < mrs_real> > &matrix, const string &fileName); 
			
	       	private:
		/* current audio file location */
		string curr_file_location;

		/* Name of audio file to segment */
		string filename_matrix;

};

#endif
