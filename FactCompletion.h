#pragma once
#include "Completion.h"
#include "Predicate.h"

#include <vector>


class FactCompletion : public Completion
{
public:
	FactCompletion(Predicate _head): super(_head, std::vector<Predicate> ()){};
	~FactCompletion();
	
private:
	typedef Completion super;
};
