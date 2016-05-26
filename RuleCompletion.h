#pragma once
#include "Completion.h"
#include "Predicate.h"
#include <set>

class RuleCompletion : public Completion
{
public:
	RuleCompletion(Predicate _head,
					std::vector<Predicate> _body,
					std::set<std::pair<std::string, std::string>> _orphanVars,
					std::map<int,std::pair<int, std::string>> _map):super(_head, _body), orphanVars(_orphanVars), varMap(_map)
					{
						_orphanVars.size()>0 ? isOrphan = true : isOrphan = false;
					};
	~RuleCompletion();
	RuleCompletion();
	
	std::map<int,std::pair<int, std::string>> getConstantMap() const {return varMap;}
	bool checkOrphan() { return isOrphan;}
	std::set<std::pair<std::string, std::string>> getOrphanVars() {return orphanVars;}
	
private:
	typedef Completion super;
	bool isOrphan = false;
	std::set<std::pair<std::string, std::string>> orphanVars; 
	std::map<int,std::pair<int, std::string>> varMap;
};

