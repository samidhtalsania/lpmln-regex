#pragma once
#include <string>
#include <vector>
#include <map>
class Predicate
{
public:
	Predicate(std::string _var, std::vector<std::string> _tokens): var(_var), tokens(_tokens){};
	Predicate();
	~Predicate();
	
	bool operator< (const Predicate &right) const;
	bool operator== (const Predicate &right) const;
	
	std::string getVar() const { return var; }
	
	std::vector<std::string> getTokens() const {return tokens;}
	

private:
	std::string var;
	std::vector<std::string> tokens;
	
};

