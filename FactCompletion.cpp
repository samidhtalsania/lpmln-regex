#include <string>
#include "FactCompletion.h"

FactCompletion::~FactCompletion()
{
}

FactCompletion::FactCompletion()
{
}

bool FactCompletion::operator<(const FactCompletion& rhs) const
{
	std::string tokenStringLhs;
	std::string tokenStringRhs;
	
	for(unsigned int i=0;i<rhs.head.getTokens().size();i++)
		tokenStringRhs.append(rhs.head.getTokens().at(i));
		
	for(unsigned int i=0;i<this->head.getTokens().size();i++)
		tokenStringLhs.append(this->head.getTokens().at(i));
		
	return tokenStringLhs<tokenStringRhs;
	
	
}

bool FactCompletion::operator==(const FactCompletion& rhs) const
{
	std::string tokenStringLhs;
	std::string tokenStringRhs;
	
	for(unsigned int i=0;i<rhs.head.getTokens().size();i++)
		tokenStringRhs.append(rhs.head.getTokens().at(i));
		
	for(unsigned int i=0;i<this->head.getTokens().size();i++)
		tokenStringLhs.append(this->head.getTokens().at(i));
		
	return tokenStringLhs==tokenStringRhs;
}

