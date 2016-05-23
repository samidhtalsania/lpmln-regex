#pragma once
#include <string>
#include <vector>
class Predicate
{
public:
	Predicate(std::string _var, std::vector<std::string> _tokens): var(_var), tokens(_tokens){};
	Predicate();
	~Predicate();
	
	bool operator< (const Predicate &right) const;
	bool operator== (const Predicate &right) const;
	
	std::string getVar() const { return var; }
	
	void setHasConstant() {	hasConstant = true;	}
	
	void setIsCompleted() {	isCompleted = true; }
	
	bool getIsCompleted() const {return isCompleted;}
	
	std::vector<std::string> getTokens() const {return tokens;}

private:
	std::string var;
	std::vector<std::string> tokens;
	bool appearsInHead = false;
	bool hasConstant = false;
	bool isCompleted = false;
	
};

