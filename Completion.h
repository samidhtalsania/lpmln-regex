#pragma once
#include "Predicate.h"

class Completion
{
public:
	Completion();
	Completion(Predicate _head, std::vector<Predicate> _body) : head(_head), body(_body){};
	~Completion();
	
	Predicate head;
	std::vector<Predicate> body;
};

