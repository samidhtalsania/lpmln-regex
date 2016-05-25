#include "Predicate.h"
#include <set>
#include <algorithm>
#include <string>

bool Predicate::operator< (const Predicate &right) const
{
	if(this->var != right.var)
		return this->var < right.var;
	else
	{
		// If they are the same then they might have different constants.
//		if(this->hasConstant && right.hasConstant)
//		{
			std::set<std::string> s1(this->tokens.begin(), this->tokens.end());
			std::set<std::string> s2(right.tokens.begin(), right.tokens.end());
			
			std::set<std::string> res;
			std::set_difference(s1.begin(), s1.end(), s2.begin(), s2.end(),std::inserter(res, res.end()));
			
			if(res.size() > 0)
				return true;
			return false;
//		}
//		return false;
	}
    
}

bool Predicate::operator== (const Predicate &right) const
{
	return this->var == right.var;
//	if(this->var != right.var && !this->hasConstant && !right.hasConstant)
//		return this->var == right.var;
//	else if(this->var == right.var)
//	{
//		// If they are the same then they might have different constants.
//		if(this->hasConstant && right.hasConstant)
//		{
//			std::set<std::string> s1(this->tokens.begin(), this->tokens.end());
//			std::set<std::string> s2(right.tokens.begin(), right.tokens.end());
//			
//			std::set<std::string> res;
//			std::set_difference(s1.begin(), s1.end(), s2.begin(), s2.end(),std::inserter(res, res.end()));
//			
//			if(res.size() > 0)
//				return false;
//			return true;
//		}
//		return false;
//	}
    
}

Predicate::Predicate()
{
}


Predicate::~Predicate()
{
}

