#include <fstream>
#include <streambuf>
#include <string>
#include <cerrno>
#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>

#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/program_options.hpp>

#include <memory>

#include "Predicate.h"
#include "Domain.h"
#include "FactCompletion.h"
#include "RuleCompletion.h"


//TODO comment handling. //next(0,1) will result in fact completion

#define TRUE 1
#define FALSE 0

namespace ba = boost::algorithm;
namespace io = boost::iostreams; 
namespace po = boost::program_options; 

int parse(std::string filename);

typedef std::string HEAD;
typedef std::string BODY;
typedef std::string ATOM;
typedef bool BOOLEAN;
typedef long unsigned int lint;
typedef std::map<std::string, std::vector<lint>> MAP;


std::unordered_multimap<HEAD, BODY> headBodyMap;
std::map<std::string, std::vector<lint>> predMapStr;


std::vector<Domain> domains;
std::set<std::string> domainList;
std::vector<FactCompletion> facts;
std::vector<RuleCompletion> rules;
std::set<Predicate> predicates;

std::string uniqueVars[] = {"_a","_b","_c","_d","_e","_f","_g","_h","_i","_j","_k","_l","_m","_n"};

int charCount = 0;

int main(int argc,char *argv[])
{
	if(!(argv[1] == NULL))
		parse(argv[1]);
	else 
		std::cerr<<"A single input file required\n";
	return 0;
}


// This does both matching and completion
void matchPredicate(std::string str)
{
	boost::regex exprHardrule("([a-zA-Z]+)(\\()([A-Za-z0-9,]+)(\\))([.])");
//	boost::regex exprSoftrule("([0-9.]+)([a-zA-Z]+)(\\()([A-Za-z0-9,]+)(\\))");
	std::string::const_iterator start, end;
	start = str.begin();
	end = str.end();
	boost::match_results<std::string::const_iterator> what;
	boost::match_flag_type flags = boost::match_default;
	std::size_t found = str.find("EXIST");
	
	// Taking in non exist clauses
	if(boost::regex_match(str, exprHardrule) && found == std::string::npos)
	{
		//Ex ResidentBird(a,b,c).
		boost::regex_search(start, end, what, exprHardrule, flags);
		
		std::string temp = what[3]; // temp = a,b,c 
		std::vector<std::string> tokens;
		boost::split(tokens, temp, boost::is_any_of(","), boost::token_compress_on);
		Predicate p(std::string(what[1]), tokens);
		FactCompletion f(p);
		facts.push_back(f);
		
//		int initCount = charCount;
//		
//		temp = "";
//		for(unsigned int i=0; i<tokens.size();i++)
//		{
//			temp += uniqueVars[charCount++];
//			temp += "=";
//			ba::trim(tokens[i]);
//			temp += tokens[i];
//			temp += "^";
//		}
//		temp = temp.substr(0,temp.size()-1);
//		temp.append("<=>");
//		temp.append(what[1]).append(what[2]);
//		std::string predName(what[1]);
//		
//		MAP::iterator it = predMapStr.find(predName);
//		if(it != predMapStr.end())
//			predMapStr[predName] = std::vector<lint>{TRUE,tokens.size()};
//		else
//			predMapStr.insert(std::pair<std::string, std::vector<lint>>(predName,std::vector<lint>{TRUE,tokens.size()})) ;
//
//		
//		for(unsigned int i=0; i<tokens.size();i++)
//		{
//			if(initCount == sizeof(uniqueVars)/sizeof(uniqueVars[0]))
//			{
//				initCount = charCount = 0;
//			}
//			temp += uniqueVars[initCount++];
//			temp += ",";
//		}
//		temp = temp.substr(0,temp.size()-1);
//		temp += what[4];
//		temp += ".";
//		std::cout<<temp<<std::endl;
	}

//	if(boost::regex_match(str, exprSoftrule) && found == std::string::npos)
//	{
//		boost::regex_search(start, end, what, exprSoftrule, flags);
//		std::string temp = what[2];
//		temp = what[1];
//		temp = what[3];
//		temp = what[4];
//		temp = what[5];
//		temp = what[6];
//		temp = what[7];
//		
//		
//	}
}


bool hasConstant(Predicate p)
{
	for(unsigned int i=0;i<p.getTokens().size();i++)
	{
		std::set<std::string>::iterator itr = domainList.find(p.getTokens().at(i));
		if(itr != domainList.end())
			return true;
	}
	return false;
}

void matchRules(std::string str)
{
	boost::regex expr("([\\^0-9a-zA-Z_\\(\\),!]+)(=>){1}([0-9a-zA-Z\\(\\),]+)([.])");
	std::string::const_iterator start, end;
	start = str.begin();
	end = str.end();
	boost::match_results<std::string::const_iterator> what;
	boost::match_flag_type flags = boost::match_default;

	if(boost::regex_match(str,expr))
	{
		boost::regex_search(start, end, what, expr, flags);
		std::string head = what[3]; //Head
		std::string body = what[1]; //Body
		boost::regex tokenExpr("([a-zA-Z]+)(\\()([A-Za-z0-9,]+)(\\))");
		// If it is not a choice rule
		if(!(head.compare(body)==0))
		{
//			headBodyMap.insert(std::make_pair(first ,second));
			std::vector<std::string> tokens;
			
			//Split conjunctive body terms
			boost::split(tokens,body,boost::is_any_of("^"), boost::token_compress_on);
			std::set<std::string> orphanVars;
			std::vector<Predicate> predList;
			for(unsigned int i = 0; i<tokens.size(); i++)
			{
				start = tokens[i].begin();
				end = tokens[i].end();
				
				//Find local variables in each of the conjunctive head terms
				boost::regex_search(start,end,what, tokenExpr, flags);
				
				std::vector<std::string> vars;
				
				std::string strTemp(what[3]);
				//Split local variables by comma
				boost::split(vars,strTemp,boost::is_any_of(","), boost::token_compress_on);
				
				Predicate p(what[1],vars);
				predList.push_back(p);
				orphanVars.insert(vars.begin(), vars.end());
//				MAP::iterator it = predMapStr.find(what[1]);
//				if(it == predMapStr.end())
//					predMapStr.insert(std::pair<std::string, std::vector<lint>>(what[1],std::vector<lint>{TRUE,tokens.size()})) ;
			}
			
			std::set<std::string> orphanVarsHead;
			start = head.begin();
			end = head.end();
			boost::regex_search(start,end,what, tokenExpr, flags);
			std::vector<std::string> vars;
			std::string strTemp(what[3]);
			//Split local variables by comma
			boost::split(vars,strTemp,boost::is_any_of(","), boost::token_compress_on);
			Predicate p(what[1],vars);
			orphanVarsHead.insert(vars.begin(), vars.end());
			bool isOrphan = true;
			std::set<std::string> result;
			if(orphanVarsHead == orphanVars)
				isOrphan = false;
			else
				// Assuming that head always has less variables then body
				std::set_difference(orphanVars.begin(), orphanVars.end(), orphanVarsHead.begin(), orphanVarsHead.end(),std::inserter(result, result.end()));
			
			//Determine if P has any constant in it.
			if(hasConstant(p))
				p.setHasConstant();
			RuleCompletion r(p,predList, isOrphan, result);
			rules.push_back(r);
			
//			boost::split(tokens,second,boost::is_any_of("^"), boost::token_compress_on);
//			for(unsigned int i = 0; i<tokens.size(); i++)
//			{
//				start = tokens[i].begin();
//				end = tokens[i].end();
//				boost::regex_search(start,end,what, tokenExpr, flags);
//				std::vector<std::string> vars;
//				boost::split(vars,tokens[i],boost::is_any_of(","), boost::token_compress_on);
//
//				MAP::iterator it = predMapStr.find(what[1]);
//				if(it == predMapStr.end())
//					predMapStr.insert(std::pair<std::string, std::vector<lint>>(what[1],std::vector<lint>{FALSE,vars.size()})) ;
//			}
		
		}
	}
}
//

void matchDomains(std::string str)
{
	boost::regex expr("([a-zA-Z]+)(=){1}(\\{){1}([_0-9a-zA-Z,]+)(\\}){1}");
	std::string::const_iterator start, end;
	start = str.begin();
	end = str.end();
	boost::match_results<std::string::const_iterator> what;
	boost::match_flag_type flags = boost::match_default;
	if(boost::regex_match(str,expr))
	{
		boost::regex_search(start, end, what, expr, flags);
		std::vector<std::string> domainVars;
		std::string domain = what[1];
		std::string domainStr(what[4]);
		boost::split(domainVars,domainStr,boost::is_any_of(","));
		Domain p;
		p.setVars(domainVars);
		p.setDomainVar(domain);
		domains.push_back(p);
		
		domainList.insert(domainVars.begin(),domainVars.end());
	}
	
}

int parse(std::string filename)
{
	const char *input_file_name;
	input_file_name = filename.c_str();
	
	std::ifstream file(input_file_name, std::ios_base::in | std::ios_base::binary);
	std::vector<std::string> keys;
	

    if(file)
    {
	    try 
		{
	        io::filtering_istream in;
	        
	        in.push(file);
			
			for(std::string str; std::getline(in, str); )
			{
				std::cout<<str<<std::endl;
				if(!str.empty())
	            {
					str.erase(std::remove(str.begin(),str.end(),' '),str.end());
					matchDomains(str);
					matchRules(str);
					matchPredicate(str);
				}
				else
					std::cout << "\n";
			}
		}
		catch(std::exception& e) {
	         std::cout << e.what() << '\n';
	         //outfile.close();
	         return -1;
	    }
	}
	else
	{
		std::cerr << "File could not be opened!\n";
	}
	
	
	std::cout<<"\n//Completion\n";
//	
//	int a = domains.size();
//	int b = facts.size();
//	int c = rules.size();
//	
	// Completion post processing
	
	//Pre processing => We need to know how many completion rules are needed before we try completion. 
	
	
	// 1) Get all heads from rules R such that the signature of all rules are different if it has constants in them and add them to predicates set.
	//  ex L(A,b,C) is different from L(D,b,C) A,C,D are constants b is a variable,
	//     L(a,b,c) is same as L(d,e,f) a,b,c,d,e,f are variables.
	
	for(unsigned int i=0; i<rules.size(); i++)
	{
		predicates.insert(rules.at(i).head);
		predicates.insert(rules.at(i).body.begin(), rules.at(i).body.end());
	}
	
	// Predicates set has rules that need to be completed. 
	// First complete facts
	// Then complete rules while also checking for orphan vars
	
//	int d = predicates.size();
	
	
	// Fact completion
	for(unsigned int i=0;i<facts.size();i++)
	{


		int count = 0;
		std::string op("");

		op.append(facts.at(i).head.getVar());
		op.append("(");
		count = 0;
		for(unsigned int j=0;j<facts.at(i).head.getTokens().size();j++)
		{
			op.append(uniqueVars[count++]);
			op.append(",");
		}
		op = op.substr(0,op.size()-1);
		op.append(")");
		
		op.append("=>");
		count=0;
		for(unsigned int j=0;j<facts.at(i).head.getTokens().size();j++)
		{
			op.append(uniqueVars[count++]);
			op.append("=");
			op.append(facts.at(i).head.getTokens().at(j));
			op.append(" ^ ");
		}
		
		op = op.substr(0,op.size()-3);
		op.append(".");
		std::cout<<op<<std::endl;
		
		//Since this paricular predicate is completed we set its completion bit to true if this is found in the predicates set. 
		Predicate p = facts.at(i).head;
		std::set<Predicate>::iterator it = predicates.find(p);
		if(it != predicates.end())
		{
			predicates.erase(it);
			p.setIsCompleted();
			predicates.insert(p);
		}
		
	}
	
	std::set<Predicate> predComp;
	
	
	// Rule completion
	// Needs to be redone
	// Follow 4 step procedure.
	for (std::set<Predicate>::iterator it=predicates.begin(); it!=predicates.end(); ++it)
	{
		//Now for each predicate, get its body from the rules set concat it take care of orphan variables and output the results
		
		//Take the predicate
		Predicate p = *it;
		
		// Ignore if it is already completed
		if(!p.getIsCompleted())
		{
			
			std::string op("");
			
			op += p.getVar();
			op += "(";
			std::vector<std::string> t = p.getTokens();
			for(auto tit = t.begin(); tit != t.end(); ++tit)
			{
				op += (*tit);
				op += ",";
			}
			op = op.substr(0, op.size()-1);
			op += ") => ";
			
			// Collect all rules here
			std::vector<RuleCompletion> comp;
			
			// Iterate over all the rules to find this predicate
			for(std::vector<RuleCompletion>::iterator rit = rules.begin(); rit != rules.end() ; ++rit)
			{
				if(p == rit->head)
					comp.push_back(*rit);
			}
			
			// This predicate does not appear as the head of any rule.
			//  Ignore
			if(comp.size() == 0)
				continue;
			
			
			predComp.insert(p);
			// comp vector will have all the rules which have p in its head.
			// First determine the variables inside p.
			
			// Set of variables inside predicate p
			std::vector<std::string> pVars = p.getTokens();
			std::set<std::string> hSet(pVars.begin(), pVars.end());
			
			for (auto i=pVars.begin(); i!=pVars.end(); ++i)
			{
//				std::cout << *i <<"\n";
				hSet.insert(*i);
			}
				
			
			//Set of variables inside the body of all predicates here p is head
			std::set<std::string> bSet;
			
			// Fill bSet
			// Determine the variables inside all of its body predicates
			for(auto bit = comp.begin(); bit != comp.end(); ++bit)
			{
				// jt is a vector of predicates
				for(auto jt = bit->body.begin(); jt != bit->body.end(); ++jt)
				{
					std::vector<std::string> temp = jt->getTokens();
					std::copy(temp.begin(),temp.end(), std::inserter(bSet, bSet.end()));
				}
			}

			// Remove all constants from head set
			for(auto hit = hSet.begin(); hit != hSet.end(); ++hit)
			{
				if(domainList.find(*hit) != domainList.end())
					hSet.erase(*hit);
			}
			
			// Remove all constants from body set
			for(auto bit = bSet.begin(); bit != bSet.end(); ++bit)
			{
				if(domainList.find(*bit) != domainList.end())
					bSet.erase(*bit);
			}
			
			// Head can have orphan variables, body cannot
			
			// Orphan variables set
			std::set<std::string> oSet;
			std::set_difference(bSet.begin(), bSet.end(), hSet.begin(), hSet.end(), std::inserter(oSet, oSet.end()));
			
			if(oSet.size() == 0)
			{
				
				// Just write out disjunctions of all the body predicates
				// loops through all the rules
				for(auto bit = comp.begin(); bit != comp.end() ; ++bit)
				{
					std::vector<Predicate> vec = bit->body;
					
					//loops through all the body predicates
					for(auto jt = vec.begin(); jt!= vec.end(); ++jt)
					{
						std::vector<std::string> str = jt->getTokens();
						op += jt->getVar();
						op += "(";
					
						// Lopps through all the variables in the body
						for(auto kt = str.begin(); kt!= str.end(); ++kt)
						{
							op += (*kt);
							op += ",";
						}
						op = op.substr(0,op.size()-1);
						op += ")";
						op += "^";
					}
					op = op.substr(0,op.size()-1);
					op += " v ";
				}
				op = op.substr(0,op.size() - 3);
				op += "."; 
				std::cout<<op<<"\n";
			}
			
			else
			{
				// Use exist 
				
				//First take a predicate. Check out its variables. See if any of it is an orphan variable. 
				//If you find one add Exist orphanVAr and that predicate.
				
				//Read predicate
				
				for(auto bit = comp.begin(); bit != comp.end() ; ++bit)
				{
					std::vector<Predicate> vec = bit->body;
					bool existUsed = false;
					std::string existStr("Exist ");
					std::set<std::string> existSet;
					std::string temp("");
					//loops through all the body predicates
					for(auto jt = vec.begin(); jt!= vec.end(); ++jt)
					{
						std::vector<std::string> str = jt->getTokens();
						temp += jt->getVar();
						temp += "(";
						
						for(auto kt = str.begin(); kt!= str.end(); ++kt)
						{
							temp += (*kt);
							if(oSet.find(*kt) != oSet.end())
							{
								existSet.insert(*kt);
								
								existUsed = true;
							}
							temp += ",";
						}
						temp = temp.substr(0,temp.size()-1);
						temp += ")";
						temp += "^";
					}
					temp = temp.substr(0,temp.size()-1);
					
					if(existUsed)
					{
						op += existStr;
						for(auto eit = existSet.begin(); eit != existSet.end(); ++eit)
						{
							op += *eit + " ,"; 
						}
						
						op = op.substr(0,op.size()-1);
						op += temp;
					}
						 
					else
						op += temp;
					op += " v ";
					
				}
				op = op.substr(0,op.size() - 3);
				op += "."; 
				std::cout<<op<<"\n";
			}
		}
	}
	
	//PredComp has all the predicates that have been completed. So mark their bit as completed.
	Predicate p;
	for(auto it = predComp.begin(); it!=predComp.end(); ++it)
	{
		auto pit = predicates.find(*it);
		predicates.erase(pit);
		p = *it;
		p.setIsCompleted();
		predicates.insert(p);
	}
	
	
	predComp.clear();
	
	// Final sanity check to see all predicates ahve been completed.
	// For all those predicates not completed add negation in front.
	for (std::set<Predicate>::iterator it=predicates.begin(); it!=predicates.end(); ++it)
	{
		
		//TODO: Body might contain predicates which might only have constants. No need to complete that predicate. 
		//Take the predicate
		Predicate p = *it;
		std::string op("");
		// Ignore if it is already completed
		if(!p.getIsCompleted())
		{
			//if it isnt then add negation in front of it
			op += "!";
			op += p.getVar();
			op += "(";
			
			std::vector<std::string> t = p.getTokens();
			
			for(auto pit = t.begin(); pit != t.end(); ++pit)
			{
				op += *pit;
				op += ",";
			}
			op = op.substr(0, op.size()-1);
			op += ").\n";
			std::cout << op;
			
			predComp.insert(p);
		}
	}
	
	//This entire part is redundant
	//Final checking to see if all predicates have been completed. Theu should be by now.
	
//	for(auto it = predComp.begin(); it!=predComp.end(); ++it)
//	{
//		auto pit = predicates.find(*it);
//		predicates.erase(pit);
//		p = *it;
//		p.setIsCompleted();
//		predicates.insert(p);
//	}
//	
//	predComp.clear();
//	
//	int count=0;
//	for(auto it = predicates.begin(); it!=predicates.end(); ++it)
//	{
//		if(it->getIsCompleted())
//			count++;
//	}
//	
//	if(count == predicates.size())
//		std::cout<<"//Correct";
//	else
//		std::cout<<"//InCorrect";

	return 0;
	
}
