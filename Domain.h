#pragma once
#include <vector>
#include <string>

class Domain
{
public:
	Domain(std::string str): domainVar(str){}
	Domain();
	~Domain();
	
	void setVars(std::vector<std::string> domainVariables)
	{
		vars = domainVariables;
	}
	
	std::vector<std::string> getVars()
	{
		return vars;
	}
	
	void setDomainVar(std::string domainVariable)
	{
		domainVar = domainVariable;
	}
	
	std::string getDomainVar() const
	{
		return domainVar;
	}
	
	bool operator< (const Domain& right) const
	{
		return domainVar < right.getDomainVar();
	}
	
	bool operator== (const Domain& right) const
	{
		return domainVar == right.getDomainVar();
	}
	
private :
	std::vector<std::string> vars;
	std::string domainVar;

};

