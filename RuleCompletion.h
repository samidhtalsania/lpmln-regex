#pragma once
#include "Completion.h"
#include "Predicate.h"
#include <set>

class RuleCompletion : public Completion
{
public:
	RuleCompletion(Predicate _head,
					std::vector<Predicate> _body,
					bool _isOrphan,
					std::set<std::string> _orphanVars):super(_head, _body), isOrphan(_isOrphan), orphanVars(_orphanVars){};
	~RuleCompletion();
	
private:
	typedef Completion super;
	bool isOrphan = false;
	std::set<std::string> orphanVars; 
};

