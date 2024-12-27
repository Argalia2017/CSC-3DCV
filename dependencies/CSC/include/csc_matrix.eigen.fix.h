

#ifdef _HAS_DEPRECATED_NEGATORS
#if _HAS_DEPRECATED_NEGATORS
#undef EIGEN_HAS_STD_NEGATORS
#endif
#endif

#ifdef EIGEN_HAS_STD_NEGATORS
#if !EIGEN_HAS_STD_NEGATORS
namespace std {
template<class _Fn>
class unary_negate {	// functor adapter !_Func(left)
public:
	typedef typename _Fn::argument_type argument_type;
	typedef bool result_type;

	constexpr explicit unary_negate (const _Fn &_Func)
		: _Functor (_Func) {	// construct from functor
	}

	constexpr bool operator()(const argument_type &_Left) const {	// apply functor to operand
		return (!_Functor (_Left));
	}

private:
	_Fn _Functor;	// the functor to apply
};

template<class _Fn>
class binary_negate {	// functor adapter !_Func(left, right)
public:
	typedef typename _Fn::first_argument_type first_argument_type;
	typedef typename _Fn::second_argument_type second_argument_type;
	typedef bool result_type;

	constexpr explicit binary_negate (const _Fn &_Func)
		: _Functor (_Func) {	// construct from functor
	}

	constexpr bool operator()(const first_argument_type &_Left ,
		const second_argument_type &_Right) const {	// apply functor to operands
		return (!_Functor (_Left ,_Right));
	}

private:
	_Fn _Functor;	// the functor to apply
};
} ;
#endif
#endif