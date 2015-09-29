#ifndef SELF_HPP
#define SELF_HPP


template<class T>
class Self {
protected:
	const T &self() {
		return static_cast<const T &>(*this);
	}
};


#endif
