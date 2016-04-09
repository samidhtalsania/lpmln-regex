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

namespace ba = boost::algorithm;
namespace io = boost::iostreams; 
namespace po = boost::program_options; 

int parse(std::string filename);

typedef std::string HEAD;
typedef std::string BODY;
typedef std::string ATOM;
typedef bool BOOLEAN;

//#define DEBUG

struct predCompare
{
	bool operator() (const std::unique_ptr<Predicate>& lhs, const std::unique_ptr<Predicate>& rhs) const
	{
		
		return std::strcmp(lhs.get()->token.c_str(),rhs.get()->token.c_str()) < 0;
	}
};

#ifdef DEBUG
std::unique_ptr<Predicate> debug1;
std::unique_ptr<Predicate> debug2;
#endif

std::unordered_multimap<HEAD, BODY> headBodyMap;
std::map<std::unique_ptr<Predicate>, BOOLEAN, predCompare> predMap;
std::string uniqueVars[] = {"_a","_b","_c","_d","_e","_f","_g","_h","_i","_j","_k","_l","_m","_n"};

int charCount = 0;

int main(int argc,char *argv[])
{
	parse("/home/samidh/Documents/lpmln/lpmln-regex/input2.txt");

	return 0;
}


// This does both matching and completion
void matchPredicate(std::string str)
{
	boost::regex exprHardrule("([a-zA-Z]+)(\\()([A-Za-z0-9,]+)(\\))([.])");
	std::string::const_iterator start, end;
	start = str.begin();
	end = str.end();
	boost::match_results<std::string::const_iterator> what;
	boost::match_flag_type flags = boost::match_default;
	std::size_t found = str.find("EXIST");
	
	Predicate* pred;
	
	if(boost::regex_match(str, exprHardrule) && found == std::string::npos)
	{
		boost::regex_search(start, end, what, exprHardrule, flags);
		
		std::string temp = what[3];
		std::vector<std::string> tokens;
		boost::split(tokens, temp, boost::is_any_of(","), boost::token_compress_on);
		int initCount = charCount;
		
		temp = "";
		for(unsigned int i=0; i<tokens.size();i++)
		{
			temp += uniqueVars[charCount++];
			temp += "=";
			ba::trim(tokens[i]);
			temp += tokens[i];
			temp += "^";
		}
		temp = temp.substr(0,temp.size()-1);
		temp.append("<=>");
		temp.append(what[1]).append(what[2]);
		std::string predName(what[1]);
		pred = new Predicate;
		#ifdef DEBUG
			std::cout << &pred <<std::endl;
		#endif
		pred->token = predName;
		pred->vars = tokens.size();
		
		predMap[std::unique_ptr<Predicate>(pred)] = true;
		

		#ifdef DEBUG
		debug1 = std::unique_ptr<Predicate>(pred);
		#endif
		
		for(unsigned int i=0; i<tokens.size();i++)
		{
			if(initCount == sizeof(uniqueVars)/sizeof(uniqueVars[0]))
			{
				initCount = charCount = 0;
			}
			temp += uniqueVars[initCount++];
			temp += ",";
		}
		temp = temp.substr(0,temp.size()-1);
		temp += what[4];
		temp += ".";
		std::cout<<temp<<std::endl;
	}
}


void matchRules(std::string str)
{
	boost::regex expr("([\\^0-9a-zA-Z\\(\\),!]+)(=>){1}([0-9a-zA-Z\\(\\),]+)([.])");
	std::string::const_iterator start, end;
	start = str.begin();
	end = str.end();
	boost::match_results<std::string::const_iterator> what;
	boost::match_flag_type flags = boost::match_default;
	Predicate* pred;
	if(boost::regex_match(str,expr))
	{
		regex_search(start, end, what, expr, flags);
		std::string first = what[3];
		std::string second = what[1];
//		ba::trim(first);
//		ba::trim(second);
		
		// If it is not a choice rule
		if(!(first.compare(second)==0))
		{
			headBodyMap.insert(std::make_pair(first ,second));
			std::vector<std::string> tokens;
			boost::split(tokens,first,boost::is_any_of("^"), boost::token_compress_on);
			
			boost::regex tokenExpr("([a-zA-Z]+)(\\()([A-Za-z0-9,]+)(\\))");
			
			for(unsigned int i = 0; i<tokens.size(); i++)
			{
				start = tokens[i].begin();
				end = tokens[i].end();
				boost::regex_search(start,end,what, tokenExpr, flags);
				std::vector<std::string> vars;
				boost::split(vars,tokens[i],boost::is_any_of(","), boost::token_compress_on);
				pred = new Predicate;
				pred->token = what[1];
				pred->vars = vars.size();
				
				try
				{
					if(!predMap[std::unique_ptr<Predicate>(pred)])
					{
						predMap[std::unique_ptr<Predicate>(pred)] = false;
					}
				}
				catch(std::exception& e)
				{
					
				}
				
				

				#ifdef DEBUG
					std::cout << &pred <<std::endl;
				#endif
//				delete pred;
			}
			boost::split(tokens,second,boost::is_any_of("^"), boost::token_compress_on);
			for(unsigned int i = 0; i<tokens.size(); i++)
			{
				start = tokens[i].begin();
				end = tokens[i].end();
				boost::regex_search(start,end,what, tokenExpr, flags);
				std::vector<std::string> vars;
				boost::split(vars,tokens[i],boost::is_any_of(","), boost::token_compress_on);
				pred = new Predicate;
				pred->token = what[1];
				pred->vars = vars.size();
				predMap[std::unique_ptr<Predicate>(pred)] = false;
				#ifdef DEBUG
					std::cout << &pred <<std::endl;
				#endif
//				delete pred;
			}
		}
	}
}
//



int parse(std::string filename)
{
	const char *input_file_name;
	input_file_name = filename.c_str();
	
	std::ifstream file(input_file_name, std::ios_base::in | std::ios_base::binary);
	std::ofstream outfile;
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
//					matchDomains(str);
					matchRules(str);
					matchPredicate(str);
				}
				else
					std::cout << "\n";
			}
		}
		catch(std::exception& e) {
	         std::cout << e.what() << '\n';
	         outfile.close();
	         return -1;
	    }
	}
	else
	{
		std::cerr << "File could not be opened!\n";
	}
	
	
	std::cout<<"\n//Completion\n";
	
	// Completion post processing
	
	Predicate* pred;
	boost::regex expr("([a-zA-Z]+)(\\()([A-Za-z0-9,]+)(\\))");
	boost::match_results<std::string::const_iterator> what;
	std::string::const_iterator start, end;
	boost::match_flag_type flags = boost::match_default;
	std::vector<std::string> preds;
	std::vector<std::string> atoms;
	
	for (auto iter=headBodyMap.begin(); iter!=headBodyMap.end(); iter=headBodyMap.equal_range(iter->first).second)
	{
			std::string uniq_key=iter->first;
			std::string comp("");
			
			comp.append(uniq_key).append("=>");
			
			
			boost::split(preds, uniq_key, boost::is_any_of("^"),boost::token_compress_on);
			
			
			for(std::vector<std::string>::iterator it = preds.begin(); it != preds.end(); ++it)
			{
				std::string temp = *it;
				start = temp.begin();
				end = temp.end();
				boost::regex_search(start, end, what, expr, flags);
				boost::split(atoms, temp, boost::is_any_of(","),boost::token_compress_on);
				pred = new Predicate;
				pred->token = what[1];
				pred->vars = atoms.size();
				predMap[std::unique_ptr<Predicate>(pred)] = true;
			}

			auto it_bounds = headBodyMap.equal_range(uniq_key);
			
			for (auto it=it_bounds.first; it!=it_bounds.second; it++)
			{
				comp.append(it->second).append(" v ");
			}
			comp = comp.substr(0,comp.length()-2);
			comp.append(".");
			std::cout<<comp<<std::endl;
	}
	
	#ifdef DEBUG
		int x = predMap.size();
		std::cout<<"Unique Predicates:"<<x<<std::endl;
	#endif

	for (auto it=predMap.begin(); it!=predMap.end(); ++it)
	{
		std::string str;
		
		if(it->second == false)
		{
			#ifdef DEBUG
			debug2 = std::unique_ptr<Predicate>(it->first.get());
			if(debug1 == debug2)
			{
				int x = 10;
			}
			#endif
			if(!((it->first).get()->token[0] == '!'))
				str.append("!");
			str.append((it->first).get()->token);
			str += "(";
			for(int i=0;i<(it->first).get()->vars;i++)
			{
				if(charCount == sizeof(uniqueVars)/sizeof(uniqueVars[0]))
				{
					charCount = 0;
				}
				str += uniqueVars[charCount++];
				str += ",";
			}
			str = str.substr(0,str.length()-1);
			str += ")";
			str += ".";
			std::cout<<str<<std::endl;
		}
	}

	#ifdef DEBUG
	for (auto it=predMap.begin(); it!=predMap.end(); ++it)
	{
		std::cout<<it->first.get()->token<<it->first.get()->vars<<it->second<<std::endl;
	}
	#endif
	if(outfile.is_open()){
		outfile.close();
	}
	return 0;
	
}
