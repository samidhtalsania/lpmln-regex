#pragma once
#include "Completion.h"
#include "Predicate.h"

#include <vector>


class FactCompletion : public Completion
{
public:
	FactCompletion();
	FactCompletion(Predicate _head): super(_head, std::vector<Predicate> ()){};
	~FactCompletion();

	bool operator== (const FactCompletion&) const;

	bool operator< (const FactCompletion&) const;
	
private:
	typedef Completion super;
};
