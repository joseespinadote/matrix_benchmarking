#include <iostream>
#include <chrono>
#include <matio.h>
#include <fstream>
#include "boost_inverse.h"
#include "EigenvalueDecomposition.hpp"

class MatrixOps {
    public:
        template<class T>
        bool InvertMatrix(const ublas::matrix<T>& input, ublas::matrix<T>& inverse)
        {
            typedef ublas::permutation_matrix<std::size_t> pmatrix;
            ublas::matrix<T> A(input);
            pmatrix pm(A.size1());
            int res = ublas::lu_factorize(A, pm);
            if (res != 0)
                    return false;
            inverse.assign(ublas::identity_matrix<T> (A.size1()));
            ublas::lu_substitute(A, pm, inverse);
            return true;
        }

        double determinant(ublas::matrix<double>& m ) {
            ublas::permutation_matrix<std ::size_t> pm(m.size1());
            double det = 1.0;
            if( ublas::lu_factorize(m,pm) ) {
                det = 0.0;
            } else {
                for(int i = 0; i < m.size1(); i++)
                    det *= m(i,i); // multiply by elements on diagonal
                det = det * determinant_sign( pm );
            }
            return det;
        }
    private:
        int determinant_sign(const ublas::permutation_matrix<std ::size_t>& pm)
        {
            int pm_sign=1;
            std::size_t size = pm.size();
            for (std::size_t i = 0; i < size; ++i)
                if (i != pm(i))
                    pm_sign *= -1.0; // swap_rows would swap a pair of rows here, so we change sign
            return pm_sign;
        }
};

int main(int argc, char* argv[])
{
	std::cout << "Using Boost to solve linear algebra problems" << std::endl;
	ublas::matrix<double> boostMat, boostMatTemp;
	MatrixOps matOps;
	
	// Seccion para leer matriz binaria
	matvar_t *matvar;
	size_t i, j, stride;
	mat_t    *mat;
	
	std::cout << "Reading the matrix from " << argv[1] << " Matlab binary file, and traslating it to boost matrix..." << std::endl;
	auto begin = std::chrono::high_resolution_clock::now();
	mat = Mat_Open(argv[1], MAT_ACC_RDONLY );
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
			boostMat = ublas::matrix<double>(matvar->dims[0], matvar->dims[1]);
			for ( i = 0; i < matvar->dims[0]; i++ )
			{
				for ( j = 0; j < matvar->dims[1]; j++ )
				{
					size_t idx = matvar->dims[0]*j+i;
					switch ( matvar->data_type )
					{
						case MAT_T_DOUBLE:
							boostMat(i,j) = *(double*)(data+idx*stride);
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
	double trace = 0;
	for(int i=0; i < boostMat.size1(); i++)
		trace+=boostMat(i,i);
	end = std::chrono::high_resolution_clock::now();
	std::cout << "Done! tookme " << std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count() << " milliseconds" << std::endl << std::endl;
	
	std::cout << "Calculating transpose..." << std::endl;
	begin = std::chrono::high_resolution_clock::now();
	//boostMatTemp = ublas::trans(boostMat);
	end = std::chrono::high_resolution_clock::now();
	std::cout << "Done! tookme " << std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count() << " milliseconds" << std::endl << std::endl;
	
	std::cout << "Calculating determinant..." << std::endl;
	begin = std::chrono::high_resolution_clock::now();
	//matOps.determinant(boostMat);
	end = std::chrono::high_resolution_clock::now();
	std::cout << "Done! tookme " << std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count() << " milliseconds" << std::endl << std::endl;
	
	std::cout << "Inverting the matrix..." << std::endl;
	begin = std::chrono::high_resolution_clock::now();
	matOps.InvertMatrix(boostMat, boostMatTemp);
	end = std::chrono::high_resolution_clock::now();
	std::cout << "Done! tookme " << std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count() << " milliseconds" << std::endl << std::endl;
	
	std::cout << "Calculating eigen decomposition..." << std::endl;
	begin = std::chrono::high_resolution_clock::now();
	//ublas::EigenvalueDecomposition<double> Eig(boostMat);
	end = std::chrono::high_resolution_clock::now();
	std::cout << "Done! tookme " << std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count() << " milliseconds" << std::endl  << std::endl;
	
	std::cout << "Multiplying the matrix by itself" << std::endl;
	begin = std::chrono::high_resolution_clock::now();
	//boostMatTemp = ublas::prod(boostMat, boostMat);
	end = std::chrono::high_resolution_clock::now();
	std::cout << "Done! tookme " << std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count() << " milliseconds" << std::endl;
	
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