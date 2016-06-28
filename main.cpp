#include <iostream>
#include <chrono>
#include <matio.h>

#define GI_BOOST 1
#define GI_EIGEN 2
#define GI_ARMA 3

#ifndef MATH_ENGINE

#pragma GCC error MATH_ENGINE variable must be defined

#endif

#if MATH_ENGINE == GI_BOOST

#include <fstream>
#include "boost_inverse.h"

void function(std::string fileMatrix)
{
	std::cout << "Using Boost to solve linear algebra problems" << std::endl;
	std::cout << "Opening " << fileMatrix << "..." << std::endl;
	ublas::matrix<double> boostMat;
	ublas::matrix<double> boostMatInv;
	
	
	
	std::cout << boostMat;
	
	/*Mat_Close(mat);
	begin = std::chrono::high_resolution_clock::now();
	InvertMatrix(mat, matInv);
	end = std::chrono::high_resolution_clock::now();
	std::cout << "Matrix inverted, tookme " << std::chrono::duration_cast<std::chrono::seconds>(end-begin).count() << " seconds" << std::endl;*/
}
	
#elif MATH_ENGINE == GI_EIGEN

#include "eigen_inverse.h"

void function(std::string fileMatrix)
{
	std::cout << "Using Eigen to solve linear algebra problems" << std::endl;
	const int N=3;
	MatrixXd m = MatrixXd::Random(N,N);
	std::cout << m.inverse() << std::endl;
}

#elif MATH_ENGINE == GI_ARMA

#include "arma_inverse.h"

void function(std::string fileMatrix)
{
	std::cout << std::endl << "Using Armadillo to solve linear algebra problems" << std::endl << std::endl;
	mat matrix, temp;
	double tempValue;
	
	std::cout << "Reading the matrix from " << fileMatrix << " ASCII file..." << std::endl;
	auto begin = std::chrono::high_resolution_clock::now();
	matrix.load(fileMatrix, raw_ascii);
	auto end = std::chrono::high_resolution_clock::now();
	std::cout << "Done! tookme " << std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count() << " milliseconds" << std::endl << std::endl;
	
	begin = std::chrono::high_resolution_clock::now();
	std::cout << "Calculating trace..." << std::endl;
	begin = std::chrono::high_resolution_clock::now();
	tempValue = trace(matrix);
	end = std::chrono::high_resolution_clock::now();
	std::cout << "Done! tookme " << std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count() << " milliseconds" << std::endl << std::endl;
	
	std::cout << "Calculating transpose..." << std::endl;
	begin = std::chrono::high_resolution_clock::now();
	temp = matrix.t();
	end = std::chrono::high_resolution_clock::now();
	std::cout << "Done! tookme " << std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count() << " milliseconds" << std::endl << std::endl;
	
	std::cout << "Calculating determinant..." << std::endl;
	begin = std::chrono::high_resolution_clock::now();
	tempValue = det(matrix);
	end = std::chrono::high_resolution_clock::now();
	std::cout << "Done! tookme " << std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count() << " milliseconds" << std::endl << std::endl;
	
	std::cout << "Inverting the matrix..." << std::endl;
	begin = std::chrono::high_resolution_clock::now();
	temp = inv(matrix);
	end = std::chrono::high_resolution_clock::now();
	std::cout << "Done! tookme " << std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count() << " milliseconds" << std::endl << std::endl;
	
	std::cout << "Calculating eigen decomposition..." << std::endl;
	cx_vec eigval;
	cx_mat eigvec;
	begin = std::chrono::high_resolution_clock::now();
	eig_gen(eigval, eigvec, matrix);
	end = std::chrono::high_resolution_clock::now();
	std::cout << "Done! tookme " << std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count() << " milliseconds" << std::endl  << std::endl;
	
	std::cout << "Multiplying the matrix by itself" << std::endl;
	begin = std::chrono::high_resolution_clock::now();
	temp = matrix*matrix;
	end = std::chrono::high_resolution_clock::now();
	std::cout << "Done! tookme " << std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count() << " milliseconds" << std::endl;
}

#else
	
void function(std::string fileMatrix)
{
	std::cout << "Linear algebra engine not specified" << std::endl << "Please, recompile the project indicating a math engine";
}
	
#endif

int main(int argc, char* argv[])
{
	//function(argv[1]);
	
	// Seccion para leer matriz binaria
	matvar_t *matvar;
	size_t i, j, stride;
	mat_t    *mat;
	mat = Mat_Open( argv[1],MAT_ACC_RDONLY );
	while ( (matvar = Mat_VarReadNext(mat)) != NULL ) {
		if ( NULL == matvar )
			return 0;
		if ( NULL == matvar->data )
			return 0;
		stride = Mat_SizeOf(matvar->data_type);
		if ( matvar->isComplex )
		{
			std::cout << "Matrix is complex" << std::cout;
			return 0;
		} else {
			char *data = (char*)matvar->data;
			//boostMat = ublas::matrix<double>(matvar->dims[0], matvar->dims[1]);
			std::cout << matvar->dims[0] << " " << matvar->dims[1];
			for ( i = 0; i < matvar->dims[0]; i++ )
			{
				for ( j = 0; j < matvar->dims[1]; j++ )
				{
					size_t idx = matvar->dims[0]*j+i;
					switch ( matvar->data_type )
					{
						case MAT_T_DOUBLE:
							//boostMat(i,j) = *(double*)(data+idx*stride);
							break;
						default:
							std::cout << "can't determine data type";
					}
				}
			}
		}	
		Mat_VarFree(matvar);
		matvar = NULL;
	}
	Mat_Close(mat);
	return 0;
}

/* code snippet para el caso de lectura de matrices en ascii en formato boost
	std::ifstream s(fileMatrix);
	if (s.is_open())
	{
		auto begin = std::chrono::high_resolution_clock::now();
		s >> mat;
		auto end = std::chrono::high_resolution_clock::now();
		std::cout << "Matrix of size " << mat.size1() << " by " << mat.size2() << " loaded. Tookme " << std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count() << " milliseconds" << std::endl;
	} else {
		std::cout << "Can't open " << fileMatrix << std::endl;
	}*/