#include <iostream>
#include <chrono>
#include <matio.h>
#include <armadillo>

using namespace arma;

int main(int argc, char* argv[])
{	
	std::cout << std::endl << "Using Armadillo to solve linear algebra problems" << std::endl << std::endl;
	mat matrix, temp;
	double tempValue;
	
	std::cout << "Reading the matrix from " << argv[1] << " ASCII Matlab matrix file..." << std::endl;
	auto begin = std::chrono::high_resolution_clock::now();
	matrix.load(argv[1], raw_ascii);
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
	
	return 0;
}
