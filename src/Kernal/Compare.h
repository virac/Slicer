#ifndef __COMAPRE_H__
#define __COMPARE_H__

template<class T>
T inline LEAST(T first, T second) //!< Will return the lesser of 2 variables so long as the < comparator is defined.
		{
	return first < second ? first : second;
}

template<class T>
T inline MOST(T first, T second) //!< Will return the greater of 2 variables so long as the > comparator is defined.
		{
	return first > second ? first : second;
}

#endif
