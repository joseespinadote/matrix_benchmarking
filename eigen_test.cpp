#include <iostream>
#include <chrono>
#include <matio.h>
#include <Eigen/Eigenvalues>
#include <Eigen/Dense>

using Eigen::MatrixXd;
using Eigen::EigenSolver;

int main(int argc, char* argv[])
{
	std::cout << "Using Eigen to solve linear algebra problems" << std::endl;
	MatrixXd eigenMat, eigenTemp;
	
	// Seccion para leer matriz binaria
	matvar_t *matvar;
	size_t i, j, stride;
	mat_t    *mat;
	double tempValue;
	
	std::cout << "Reading a Matlab binary matrix and translating it to Eigen matrix..." << std::endl;
	auto begin = std::chrono::high_resolution_clock::now();
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
			eigenMat = MatrixXd(matvar->dims[0], matvar->dims[1]);
			for ( i = 0; i < matvar->dims[0]; i++ )
			{
				for ( j = 0; j < matvar->dims[1]; j++ )
				{
					size_t idx = matvar->dims[0]*j+i;
					switch ( matvar->data_type )
					{
						case MAT_T_DOUBLE:
							eigenMat(i,j) = *(double*)(data+idx*stride);
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
	auto end = std::chrono::high_resolution_clock::now();
	std::cout << "Done! tookme " << std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count() << " milliseconds" << std::endl << std::endl;
	
	begin = std::chrono::high_resolution_clock::now();
	std::cout << "Calculating trace..." << std::endl;
	begin = std::chrono::high_resolution_clock::now();
	tempValue = eigenMat.trace();
	end = std::chrono::high_resolution_clock::now();
	std::cout << "Done! tookme " << std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count() << " milliseconds" << std::endl << std::endl;
	
	std::cout << "Calculating transpose..." << std::endl;
	begin = std::chrono::high_resolution_clock::now();
	eigenTemp = eigenMat.transpose();
	end = std::chrono::high_resolution_clock::now();
	std::cout << "Done! tookme " << std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count() << " milliseconds" << std::endl << std::endl;
	
	std::cout << "Calculating determinant..." << std::endl;
	begin = std::chrono::high_resolution_clock::now();
	tempValue = eigenMat.determinant();
	end = std::chrono::high_resolution_clock::now();
	std::cout << "Done! tookme " << std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count() << " milliseconds" << std::endl << std::endl;
	
	std::cout << "Inverting the matrix..." << std::endl;
	begin = std::chrono::high_resolution_clock::now();
	eigenTemp = eigenMat.inverse();
	end = std::chrono::high_resolution_clock::now();
	std::cout << "Done! tookme " << std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count() << " milliseconds" << std::endl << std::endl;
	
	std::cout << "Calculating eigen decomposition..." << std::endl;
	begin = std::chrono::high_resolution_clock::now();
	EigenSolver<MatrixXd> es(eigenMat);
	end = std::chrono::high_resolution_clock::now();
	std::cout << "Done! tookme " << std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count() << " milliseconds" << std::endl  << std::endl;
	
	std::cout << "Multiplying the matrix by itself" << std::endl;
	begin = std::chrono::high_resolution_clock::now();
	eigenTemp = eigenMat*eigenMat;
	end = std::chrono::high_resolution_clock::now();
	std::cout << "Done! tookme " << std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count() << " milliseconds" << std::endl;
	
	return 0;
}
