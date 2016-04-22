#pragma once
#include <string>
#include <vector>
class Predicate
{
public:
	Predicate(std::string _var, std::vector<std::string> _tokens): var(_var), tokens(_tokens){};
	Predicate();
	~Predicate();
	
	friend bool operator< (const Predicate &left, const Predicate &right);
	friend bool operator== (const Predicate &left, const Predicate &right);
	
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

