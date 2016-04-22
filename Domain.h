#pragma once
#include <vector>
#include <string>

class Domain
{
public:
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
	
	std::string getDomainVar()
	{
		return domainVar;
	}
	
private :
	std::vector<std::string> vars;
	std::string domainVar;

};

