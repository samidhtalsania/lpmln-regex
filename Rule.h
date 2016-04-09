#pragma once
#include <string>

class Rule
{
public:
	Rule();
	Rule(std::string _head, std::string _body):head(_head),body(_body){}
	~Rule();
	std::string getHead(){ return head;}
	std::string getBody(){ return body;}
private:
	std::string head;
	std::string body;

};

