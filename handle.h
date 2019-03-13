#ifndef HANDLE_H_GUARD
#define HANDLE_H_GUARD
#include <stdexcept>

template <class T> class Handle{
	
	//Can't define functions for template classes outside of the header file, unless you use explicit initialisation.
	//The classes that you use handle with must have a clone() function
	
	public:
		//Constructors
		Handle():pointer(0){}
		Handle(T* pointer):pointer(pointer){}
		//Copy control
		Handle(const Handle& rhs){
			if(rhs.pointer)
				pointer = rhs->clone();
		}
		Handle& operator=(const Handle& rhs){
			if(&rhs!=this){
				delete pointer;
				pointer = rhs.pointer? rhs->clone() : 0;
			}
			return *this;
		}
		~Handle(){delete pointer;}
		
		//Operator functions
		T& operator*()const{
			if(pointer)
				return *pointer;
			throw std::runtime_error("Unbound handle");
		}
		T* operator->()const{
			if(pointer)
				return pointer;
			throw std::runtime_error("Unbound handle");
		}
		operator bool()const{return pointer!=0;}
		
	private:
	
		T* pointer;
		
};

#endif