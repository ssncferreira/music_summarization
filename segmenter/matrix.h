#ifndef  __MATRIX_H_INCLUDED__   
#define  __MATRIX_H_INCLUDED__

#include <iostream>
#include <math.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <marsyas/MarSystemManager.h>


using namespace std;
using namespace Marsyas;

class Matrix {

	public:
		/* Constructor */
		Matrix();
		Matrix(vector<realvec> result);

		/* getters */
		vector<realvec> getResult_Vector();
		mrs_natural getMatrix_Size();
		vector < vector < mrs_real > > getMatrix();
		vector< vector <mrs_real> > getPaths();

		/* Create Self-Similarty Matrix: every element of matrix matrix[i,j] represents the similarity 
		   between two onde-second segments of the piece at different times with euclidean distance	*/
		void create_matrix();

		/* Removes all monotonically decreasing values in horizontal/vertical "rays" from each cell in path*/
		void remove_cell_neighborhood(int row, int column);

		/* Removes the upper triangle of the ssm matrix */
		void  remove_upper_triangle();

		/* Given a row and column verifies if the path already exists */
		bool path_exists(int row, int column);

		/*  Tries to expand up and down a path in positions [i+1,j+1] , [i+1,j+2] , [i+2, j+1] */
		void expandpath(int i, int j);

		/* Starts the path extraction algorithm */
		void path_extraction();

	private:
		/* result_vector contains the maryas processed data of the audio file */
		vector<realvec> result_vector;

		/* Number of columns (or lines) of the squared matrix */
		mrs_natural matrix_size;

		/* Self-similarity matrix */
		vector < vector < mrs_real > > matrix;

		/* 2D vector of pahts, first dimension represents the number of the path and the second the path itself */  
		vector< vector <mrs_real> > paths;
};

#endif
