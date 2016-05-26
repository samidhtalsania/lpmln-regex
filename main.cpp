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
#include <boost/algorithm/string.hpp>

#include <memory>
#include <map>

#include "Predicate.h"
#include "Domain.h"
#include "FactCompletion.h"
#include "RuleCompletion.h"
#include "Variable.h"


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


std::set<std::string> domainNamesList;
std::set<Domain> domains;
std::set<std::string> domainList;
std::set<Variable> variables;
std::multimap<std::string, FactCompletion> facts;
std::multimap<std::string, RuleCompletion> rules;


std::string uniqueVars[] = {"_a","_b","_c","_d","_e","_f","_g","_h","_i","_j","_k","_l","_m","_n"};
std::string constantVars[] = {"_c1","_c2","_c3","_c4","_c5","_c6","_c7","_c8","_c9","_c10","_c11","_c12","_c13","_c14"};

int charCount = 0;

int main(int argc,char *argv[])
{
	if(!(argv[1] == NULL))
		parse(argv[1]);
	else 
		std::cerr<<"A single input file required\n";
	return 0;
}

inline void error(std::string str)
{
	std::cout << str;
}

void matchDeclarations(std::string str)
{
	boost::regex expr("([a-zA-Z]+)(\\(){1}([_0-9a-zA-Z,]+)(\\)){1}");
	std::string::const_iterator start, end;
	start = str.begin();
	end = str.end();
	boost::match_results<std::string::const_iterator> what;
	boost::match_flag_type flags = boost::match_default;
	if(boost::regex_match(str,expr))
	{
		boost::regex_search(start, end, what, expr, flags);
		std::vector<std::string> domainVars;
		std::string var = what[1];
		std::string domain(what[3]);
		boost::split(domainVars,domain,boost::is_any_of(","));
		
		std::set<Domain>::iterator itr;
		
		//search for domains to check if we have already seen them otherwise show error for each such domain
		int i = 0;
		Variable v;
		std::map<int,Domain> posMap;
		for(auto &name : domainVars)
		{
			itr = domains.find(name);
			if(itr == domains.end())
			{
				error("Error:Domain:"+name+" not found.\n");
				exit(EXIT_FAILURE);
			}
			else
			{
				itr = domains.find(name); 
				v.setVar(var);
				posMap[i++] = *itr;
			}
		}
		
		v.setPosMap(posMap);
		variables.insert(v);
	}
}



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
		facts.insert(std::pair<std::string,FactCompletion>(f.head.getVar(),f));
	}
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

bool isConstant(std::string str)
{
	std::set<std::string>::iterator itr = domainList.find(str);
	if(itr != domainList.end())
		return true;
	else
		return false;
}

void removeConstants(std::set<std::string>& vec)
{
	std::set<std::string>::iterator itr; 
	for(auto &v : vec)
	{
		itr = domainList.find(v);
		if(itr != domainList.end())
			vec.erase(v);
	}
}

void removeConstantsPair(std::set<std::pair<std::string, std::string>>& vec)
{
	std::set<std::string>::iterator itr; 
	for(auto &v : vec)
	{
		itr = domainList.find(v.first);
		if(itr != domainList.end())
			vec.erase(v);
	}
}

struct cmp
{
	bool operator()(const std::pair<std::string, std::string>& left, const std::pair<std::string, std::string>& right) const
	{
		return left.first < right.first;
	}
};

/* 
 * limitation: 
 * RHS of => can only contain one predicate. 
 * No disjunction in body.
 * Constants must all be declared before rules.
 * 
 */
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

		std::vector<std::string> tokens;
		
		//Split conjunctive body terms
		boost::split(tokens,body,boost::is_any_of("^"), boost::token_compress_on);
//		std::set<std::string> orphanVars;
		std::set<std::pair<std::string,std::string>> orphanVarsMap;
		std::set<std::pair<std::string,std::string>> orphanVarsHeadMap;
		std::vector<Predicate> predList;
		
		for(unsigned int i = 0; i<tokens.size(); i++)
		{
			start = tokens[i].begin();
			end = tokens[i].end();
			
			//Find variables in each of the conjunctive body terms
			boost::regex_search(start,end,what, tokenExpr, flags);
			
			std::vector<std::string> vars;
			
			std::string strTemp(what[3]);
			//Split local variables by comma
			boost::split(vars,strTemp,boost::is_any_of(","), boost::token_compress_on);
			
			Predicate p(what[1],vars);
			predList.push_back(p);
			int tempCount = 0;
			for(auto &var : vars)
				orphanVarsMap.insert(std::pair<std::string, std::string>(var,variables.find(p.getVar())->getPosMap().at(tempCount++).getDomainVar()));
//			orphanVars.insert(vars.begin(), vars.end());
		}
		
		
		
//		std::set<std::string> orphanVarsHead;
		start = head.begin();
		end = head.end();
		boost::regex_search(start,end,what, tokenExpr, flags);
		std::vector<std::string> vars;
		std::string strTemp(what[3]);
		//Split local variables by comma
		boost::split(vars,strTemp,boost::is_any_of(","), boost::token_compress_on);
		
		//Replace constants in vars with variables and add constants in body. 
		//For each local variable in head, check if it is a constant. 
		//If it is then in that case, replace constant with a variable
		//and add constant as a <variable,constant> pair in that rule's map.

		int count = 0;
		std::map<int,std::pair<int, std::string>> varMap;
		int tempCount = 0;
		for(auto &str : vars)
		{
			if(isConstant(str))
			{
//				varMap[count] = std::pair<std::string, std::string>(constantVars[count], str);
				varMap[count] = std::pair<int, std::string>(count, str);
				str = constantVars[count++];
			}
			else
				count++;
				
			orphanVarsHeadMap.insert(std::pair<std::string, std::string>(str,variables.find(std::string(what[1]))->getPosMap().at(tempCount++).getDomainVar()));
		}
		
		Predicate p(what[1],vars);
		
//		orphanVarsHead.insert(vars.begin(), vars.end());

		std::set<std::string> result;
		
//		removeConstants(orphanVarsHead);
//		removeConstants(orphanVars);
		removeConstantsPair(orphanVarsMap);
		removeConstantsPair(orphanVarsHeadMap);
		
		std::set<std::pair<std::string,std::string>> resultMap;

//		std::set_difference(orphanVars.begin(), orphanVars.end(), orphanVarsHead.begin(), orphanVarsHead.end(),std::inserter(result, result.end()));
		std::set_difference(orphanVarsMap.begin(), orphanVarsMap.end(), orphanVarsHeadMap.begin(), orphanVarsHeadMap.end(),std::inserter(resultMap, resultMap.end()), cmp());

		RuleCompletion r(p,predList, resultMap, varMap);
		rules.insert(std::pair<std::string,RuleCompletion>(r.head.getVar(),r));

	}
}
//

void matchDomains(std::string str)
{
	boost::regex expr("([a-zA-Z0-9_]+)(=){1}(\\{){1}([_0-9a-zA-Z,.]+)(\\}){1}");
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
		domains.insert(p);
		domainNamesList.insert(p.getDomainVar());
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
					matchDeclarations(str);
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
	
	// Completion post processing
	
	//Fact completion
	  
	/* Create vector of deduplicated entries: */
	std::vector<std::pair<std::string,FactCompletion>> keys_dedup;
	std::unique_copy(std::begin(facts),
				  std::end(facts),
					std::back_inserter(keys_dedup),
				  [](const std::pair<std::string,FactCompletion> &entry1,
					 const std::pair<std::string,FactCompletion> &entry2) {
					   return (entry1.first == entry2.first);
				   }
				 );

	// All unique keys are in keys_dedup now.
	for(auto &key : keys_dedup)
	{
		std::string strLhs;
		std::string strRhs; 
		std::pair <std::multimap<std::string,FactCompletion>::iterator, std::multimap<std::string, FactCompletion>::iterator> ret;
		ret = facts.equal_range(key.first);
		
		int count = 0;
		FactCompletion f;
		for (std::multimap<std::string,FactCompletion>::iterator it=ret.first; it!=ret.second; ++it)
		{
			//it.second are FactCompletions
			f = it->second;
			std::vector<std::string> v = f.head.getTokens();
			count = 0;
			strRhs.append("(");
			for(auto &str : v)
			{
				strRhs.append(uniqueVars[count++]);
				strRhs.append("=");
				strRhs.append(str);
				strRhs.append(" ^ ");
			}
			strRhs = strRhs.substr(0,strRhs.size()-3);
			strRhs.append(")");
			strRhs.append(" v ");
		}
		strRhs = strRhs.substr(0,strRhs.size()-3);
		strLhs.append(key.first);
		strLhs.append("(");
		count=0;
		for(unsigned int i=0;i<f.head.getTokens().size();i++)
			strLhs.append(uniqueVars[count++]).append(",");
		strLhs = strLhs.substr(0,strLhs.size()-1);
		strLhs.append(")");
		std::cout<<strLhs<<" => "<<strRhs<<"."<<std::endl;
	}
	
	// End Fact Completion
	
	// Rule Completion
	std::vector<std::pair<std::string,RuleCompletion>> keys_dedup2;
	std::unique_copy(std::begin(rules),
				  std::end(rules),
					std::back_inserter(keys_dedup2),
				  [](const std::pair<std::string,RuleCompletion> &entry1,
					 const std::pair<std::string,RuleCompletion> &entry2) {
					   return (entry1.first == entry2.first);
				   }
				 );
//	
	for(auto &key : keys_dedup2)
	{
		std::string strLhs;
		std::string strRhs;
		
		std::pair<int,int> auxPos;
		
		std::pair <std::multimap<std::string,RuleCompletion>::iterator, std::multimap<std::string, RuleCompletion>::iterator> ret;
		ret = rules.equal_range(key.first);
		
		//Based on key, from varaibles set find out its domains.
		//Assign a variable to each of the domain of the key.
		//Use this variable in constructing strLhs and strRhs 
		std::set<Variable>::iterator itr = variables.find(key.first);
		std::map<std::string, std::pair<int, std::string> > varMap;
		std::set<std::pair<std::string,std::string>> orphanVarsSet;
		
		int count = 0;
		for(auto &var : itr->getPosMap())
		{
			varMap[var.second.getDomainVar()] = std::pair<int, std::string> (var.first,uniqueVars[count++]);
		}
		
		RuleCompletion r;
		
		
		int auxCount = 0;
		for (std::multimap<std::string,RuleCompletion>::iterator it=ret.first; it!=ret.second; ++it)
		{
			r = it->second;
			
			strRhs.append("(");
			
			//append exist and associated variables to string.
			if(r.checkOrphan())
			{
				orphanVarsSet = r.getOrphanVars();
				strRhs.append("EXIST ");
				for(auto &orphanVars : r.getOrphanVars())
				{
					strRhs.append(orphanVars.first).append(",");
				}
				strRhs = strRhs.substr(0,strRhs.size()-1);
				strRhs.append(" (");
			}
			
			auxPos.first = strRhs.size() ;
//			std::vector<std::pair<int, std::string>> constantPos;
			//Append constants to strRhs
			for(auto &constant : r.getConstantMap())
			{
//				constantPos.push_back(std::pair<int,std::string>(constant.first, constant.second.first));
//				localPos[constant.first] = constant.second.first;
//				strRhs.append("=").append(" ^ ");
				for(auto &constantInner : varMap)
				{
					if(constantInner.second.first == constant.first)
					{
						strRhs.append(constantInner.second.second).append("=").append(constant.second.second).append(" ^ ");
					}
//					int x=0;
				}
			}
			
			for(auto &pred : r.body)
			{
				strRhs.append(pred.getVar());
				strRhs.append("(");
				//If pred.getVar is the same as that of our key
				//Then in that case we make use of localPos to fill variables.
				//Otherwise we just use its own variables
				
//				std::set<std::string>::iterator itr;
//				auto itr;
				int pos = 0;
				for(auto &vars : pred.getTokens())
				{
//					auto itr = orphanVarsSet.find(vars);
					auto itr = std::find_if(orphanVarsSet.begin(), orphanVarsSet.end(), [&](const std::pair<std::string, std::string>& val) -> bool{
						return val.first == vars;
					});
					
					if(pred.getVar().compare(key.first) == 0)
					{
						if(itr != orphanVarsSet.end())
						{
							strRhs.append(vars).append(",");
						}
						
						else
						{
							for(auto &innerVar : varMap)
							{
								if(innerVar.second.first == pos)
								{
									strRhs.append(innerVar.second.second).append(",");
								}
							}
						}
					}
					else
					{
						if(itr != orphanVarsSet.end())
						{
							strRhs.append(vars).append(",");
						}
						else
						{
							std::set<Variable>::iterator itrInner = variables.find(Variable(pred.getVar()));
							//Finds variable with var=next
							Domain d = (*itrInner).getPosMap().at(pos);
							std::string varType(d.getDomainVar());
							std::pair<int,std::string> p(varMap[varType]);
							strRhs.append(p.second).append(",");
						}
					}
					
					pos++;
				}
				
				strRhs = strRhs.substr(0,strRhs.size()-1);
				strRhs.append(")");
				strRhs.append(" ^ ");
			}
			

			strRhs = strRhs.substr(0,strRhs.size()-3);
			strRhs.append(")");
			if(r.checkOrphan())
			{
				auxPos.second = strRhs.size()-1;
				strRhs.append(")");
			}
			strRhs.append(" v ");
			
			//Add code for aux stuff
			//Aux variables needed only in case of orphans
			if(r.checkOrphan())
			{
				std::string auxVar;
				std::map<std::string,std::string> auxDeclMap;
				
				//First add declaration
				auxVar.append("aux_").append(key.first).append("_").append(std::to_string(auxCount++));
				std::string auxLhs(auxVar);
				std::string auxRhs(auxVar);
				std::string auxDecl(auxVar);
				
				
				//Figure out domains to be added in aux declaration
				//parse strRhs. Add all variables to auxLhs such that
				// ex: (EXIST t1 (_b=False ^ shoot(t1) ^ next(t1,_c) ^ target(_a,_b,t1)))
				// _b,t1,_c,_a should be added.
				
				auxDecl.append("(");
				auxLhs.append("(");
				
				for(auto &auxDeclVar : varMap)
				{
					auxDecl.append(auxDeclVar.first).append(",");
					auxLhs.append(auxDeclVar.second.second).append(",");
				}
				
				for(auto &auxDeclVar : r.getOrphanVars())
				{
					auxDecl.append(auxDeclVar.second).append(",");
					auxLhs.append(auxDeclVar.first).append(",");
				}
				
				auxDecl = auxDecl.substr(0,auxDecl.size()-1);
				auxLhs = auxLhs.substr(0,auxLhs.size()-1);
				
				auxDecl.append(")");
				auxLhs.append(")");
				
				auxRhs = strRhs.substr(auxPos.first, auxPos.second-auxPos.first);
				
				strRhs.replace(auxPos.first,auxPos.second-auxPos.first, auxLhs);
				
				
				std::cout << auxDecl << "\n";
				std::cout << auxLhs << " <=> " << auxRhs << ".\n";
				
				
//				boost::regex aux_expr_eql("([a-zA-Z0-9_]+)(=){1}([_0-9a-zA-Z]+)");
//				boost::regex aux_expr_var("([_0-9a-zA-Z])([(])([_0-9a-zA-Z,])([)])");
				
				
				//((([a-zA-Z0-9_]+)(=){1}([_0-9a-zA-Z]+))|(([_0-9a-zA-Z])([(])([_0-9a-zA-Z,])([)])))
				
				
				//Second add equivalence
				
				
			}
		}
		
		strRhs = strRhs.substr(0,strRhs.size()-3);
		
		
		strLhs.append(key.first).append("(");
		
		std::vector<std::string> v(varMap.size());
		
		for(auto &vm : varMap)
			v[vm.second.first] = vm.second.second;
			
		for(auto &vm : v)
			strLhs.append(vm).append(",");
		
		strLhs = strLhs.substr(0,strLhs.size()-1);
		strLhs.append(")");

		std::cout<<strLhs<<" => "<<strRhs<<"."<<std::endl;
	}
	return 0;
}
