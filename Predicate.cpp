#include "Predicate.h"
#include <set>
#include <algorithm>
#include <string>

bool operator< (const Predicate &left, const Predicate &right) 
{
	if(left.var != right.var)
		return left.var < right.var;
	else
	{
		// If they are the same then they might have different constants.
		if(left.hasConstant && right.hasConstant)
		{
			std::set<std::string> s1(left.tokens.begin(), left.tokens.end());
			std::set<std::string> s2(right.tokens.begin(), right.tokens.end());
			
			std::set<std::string> res;
			std::set_difference(s1.begin(), s1.end(), s2.begin(), s2.end(),std::inserter(res, res.end()));
			
			if(res.size() > 0)
				return true;
			return false;
		}
		return false;
	}
    
}

bool operator== (const Predicate &left, const Predicate &right) 
{
	return left.var == right.var;
//	if(left.var != right.var && !left.hasConstant && !right.hasConstant)
//		return left.var == right.var;
//	else if(left.var == right.var)
//	{
//		// If they are the same then they might have different constants.
//		if(left.hasConstant && right.hasConstant)
//		{
//			std::set<std::string> s1(left.tokens.begin(), left.tokens.end());
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

