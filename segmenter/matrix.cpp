#include "matrix.h"

/* Constructors */
Matrix::Matrix() { }

Matrix::Matrix(vector<realvec> result) {
	result_vector = result;
	matrix_size = result.size();
	matrix.resize(matrix_size, (vector<mrs_real> (matrix_size,0)));
	create_matrix();
}

/* getters */
vector<realvec> Matrix::getResult_Vector() { return result_vector; }
mrs_natural Matrix::getMatrix_Size() { return matrix_size; }
vector < vector < mrs_real > > Matrix::getMatrix() { return matrix; }
vector< vector <mrs_real> > Matrix::getPaths() { return paths; } 

void Matrix::create_matrix() {
	mrs_real dot_product = 0;
	mrs_real magnitude1 = 0;
	mrs_real magnitude2 = 0;
	mrs_real cosineDistance = 0;
	mrs_natural temp = 0;     

	// Optimization: calculate only half-matrix, since the other half is equal
	for(int rows = 0; rows < matrix_size; rows++) {
		for(int columns = 0; columns < matrix_size; columns++) {
			if(rows == columns) {
				// optimization -> always equal to  1 in cosine distance
				temp = 1;
			}
			else {
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

				// To scale a range x0...x1 to a new range y0...y1:
				// y = y0 + (y1 - y0) * (x - x0) / (x1 - x0)
				// changing scale from -1 1 to 0 20
				// 200 - x to invert color scale
				// result = 5000 - (5000*((dot_product/magnitude1)+1) / 2);

				cosineDistance =  dot_product/magnitude1;	
				temp = ((cosineDistance+1)*5000)/2;
				temp = temp - 2500;
			}
			if(temp == 1 ) {
				temp = 2500;
			}
			else{
				if(temp == 0 || temp > 5000) 
					temp = 0;
			}
			matrix[rows][columns] = temp;

			magnitude1 = 0;
			magnitude2 = 0;
			dot_product = 0;
			temp = 0;
		}
		magnitude1 = 0;
		magnitude2 = 0;
		dot_product = 0;
	}
}

/*
   PATH EXTRACTION OPERATIONS
 */
void Matrix::remove_cell_neighborhood(int row, int column) {
	mrs_real original_center = matrix[row][column];
	mrs_real moving_center = original_center;
	mrs_natural sz = matrix.size();

	for(int i = 1; row + i < sz; i++) { // Example: matrix real size: 0..731, matrix.size() = 732
		if(matrix[row+i][column] < moving_center) {
			moving_center = matrix[row+i][column]; 
			matrix[row+i][column] = 0;	 //remove decreasing values down in row
		}
	}
	for(int i = 1, moving_center = original_center; row - i >= 0; i++) {
		if(matrix[row-i][column] < moving_center) {
			moving_center = matrix[row-i][column]; 
			matrix[row-i][column] = 0;	 //remove decreasing values up in row
		}
	}
	for(int j = 1, moving_center = original_center; column + j < sz; j++) {
		if(matrix[row][column + j] < moving_center) {
			moving_center = matrix[row][column + j]; 
			matrix[row][column + j] = 0;	 //remove decreasing values right in column
		}
	}
	for(int j = 1, moving_center = original_center; column - j >= 0; j++) {
		if(matrix[row][column-j] < moving_center) {
			moving_center = matrix[row][column-j]; 
			matrix[row][column-j] = 0;	 //remove decreasing values left in column
		}
	}
}

void Matrix::remove_upper_triangle() {
	int j = 0;
	for(int i = 0; i < matrix_size; i++) {
		j = i;
		for(; j < matrix_size; j++) {
			matrix[i][j] = 0;
		}
	}
}

bool Matrix::path_exists(int row, int column) {
	int tempRow, tempCol;
	for(unsigned int i = 0; i < paths.size(); i++) {
		for(unsigned int j = 0; j < paths[i].size(); j = j+3) {
			tempRow = paths[i][j];
			tempCol = paths[i][j+1]; 
			if(tempRow == row && tempCol == column) {
				return true;
			}
		}
	}
	return false;
}


void Matrix::expandpath(int i, int j) {
	mrs_natural threshold = 2200;
	unsigned int d = 1;
	mrs_real ext1, ext2, ext3;
	vector<mrs_real> path; // cada 3 posições são 1 elemento do caminho, triplo (i,j,matrix)

	//	cout << " ExpandingPath...";
	remove_cell_neighborhood(i, j); //First we remove the neighborhood from the max similarity value
	path.push_back(i);
	path.push_back(j);
	path.push_back(matrix[i][j]);

	do {
		ext1 = matrix[i+d][j+d];
		ext2 = matrix[i+d][j+d+1];
		ext3 = matrix[i+d+1][j+d];	

		if((ext2 > ext1) & (ext2 > ext3)) {
			path.push_back(i+d);
			path.push_back(j+d+1);
			path.push_back(matrix[i+d][j+d+1]);
			remove_cell_neighborhood(i+d, j+d);
		} else {
			if((ext3 > ext1) & (ext3 > ext2)) {
				path.push_back(i+d+1);
				path.push_back(j+d);
				path.push_back(matrix[i+d+1][j+d]);
				remove_cell_neighborhood(i+d, j+d);
			}
			else {
				path.push_back(i+d);
				path.push_back(j+d);
				path.push_back(matrix[i+d][j+d]);
				remove_cell_neighborhood(i+d, j+d);
			}
		}

		d++;								
	}
	while((ext1 > threshold || ext2 > threshold || ext3 > threshold) && ((i+d + 2 < matrix.size()) && (j+d + 2 < matrix.size()))); 

	//Valor empirico, apenas adiciona-se o caminho se for relevante , i.e. com pelo menos 3 celulas
	if(d > 20) {
		paths.push_back(path);	
	}
}

/*é necessário uma estrutura de dados para guardar os caminhos*/
/* falta a remoção da cell neibouhood */
/*vector 2d de caminhos, 1ºdim -> nº do caminho, 2ºdim -> caminho, que contém
  posição i, j, valor. falta inicialização */
void Matrix::path_extraction() {
	mrs_natural maxSimilarity;
	unsigned int sz = matrix.size();
	for(maxSimilarity = 2500; maxSimilarity >= 2485; maxSimilarity--) {
		for(unsigned int i = 0; i < sz; i++) {
			for(unsigned int j = 0; j < sz; j++) {
				if(matrix[i][j] == maxSimilarity) {
					//inicio de um caminho p
					if((i+2) < sz && (j+2) < sz  && (!path_exists(i,j))) {
						//	cout << "Inicio de um caminho... i: " << i << " j: " << j << " " << endl;
						expandpath(i, j); //The matrix is returned with neighborhood cleaning
					}    
				}
			}
		}
	}
}

