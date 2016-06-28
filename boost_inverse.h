#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/vector_proxy.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/triangular.hpp>
#include <boost/numeric/ublas/lu.hpp>
#include <boost/numeric/ublas/io.hpp>

namespace ublas = boost::numeric::ublas;

template<class T>
bool InvertMatrix (const ublas::matrix<T>& input, ublas::matrix<T>& inverse) {
	using namespace boost::numeric::ublas;
	typedef permutation_matrix<std::size_t> pmatrix;
	matrix<T> A(input);
	pmatrix pm(A.size1());
	int res = lu_factorize(A,pm);
		if( res != 0 ) return false;
	inverse.assign(ublas::identity_matrix<T>(A.size1()));
	lu_substitute(A, pm, inverse);
	return true;
}
