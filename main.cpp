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

#include "Rule.h"

namespace ba = boost::algorithm;
namespace io = boost::iostreams; 
namespace po = boost::program_options; 



#define OUTPUT_EXTN ".lpm\0"

int parse(std::string filename);

typedef std::string HEAD;
typedef std::string BODY;
typedef std::string ATOM;
typedef std::string PREDICATE;
//typedef std::string DOMAIN;
typedef bool BOOLEAN;

struct predicate
{
	std::string token;
	int vars; 
};

std::unordered_multimap<HEAD, BODY> headBodyMap;
std::map<ATOM, BOOLEAN> atomMap;
//std::map<PREDICATE, struct predicate> predMap;


int charCount = 0;



int main(int argc,char *argv[])
{
	parse("/home/samidh/Documents/lpmln/lpmln-regex/input.txt");

	return 0;
}


// This does both matching and completion
void matchPredicate(std::string str)
{
	std::string uniqueVars[] = {"a","b","c","d","e","f","g","h","i","j","k","l","m","n"};
	boost::regex exprHardrule("([a-zA-Z]+)(\\()([A-Za-z0-9,]+)(\\))([.])");
	boost::regex exprPredicateDecl("([a-zA-Z]+)(\\()([A-Za-z0-9,]+)(\\))");
	std::string::const_iterator start, end;
	start = str.begin();
	end = str.end();
	boost::match_results<std::string::const_iterator> what;
	boost::match_flag_type flags = boost::match_default;
	std::size_t found = str.find("EXIST");
	
//	if(boost::regex_match(str,exprPredicateDecl))
//	{
//		boost::regex_search(start, end, what, exprPredicateDecl, flags);
//		struct predicate pred;
//		pred.token = what[1];
//		std::string temp = what[3];
//		std::vector<std::string> tokens;
//		boost::split(tokens, temp, boost::is_any_of(","), boost::token_compress_on);
//		pred.vars = tokens.size();
//		predMap[pred.token] = pred;
//	}
	
	if(boost::regex_match(str, exprHardrule) && found == std::string::npos)
	{
		boost::regex_search(start, end, what, exprHardrule, flags);
		
		std::string temp = what[3];
		std::vector<std::string> tokens;
		boost::split(tokens, temp, boost::is_any_of(","), boost::token_compress_on);
		int initCount = charCount;
		if(initCount == sizeof(uniqueVars)/sizeof(uniqueVars[0]))
		{
			initCount = charCount = 0;
		}
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
		atomMap[predName] = true;
		for(unsigned int i=0; i<tokens.size();i++)
		{
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
	if(boost::regex_match(str,expr))
	{
		regex_search(start, end, what, expr, flags);
		std::string first = what[3];
		std::string second = what[1];
		ba::trim(first);
		ba::trim(second);
		
		// If it is not a choice rule
		if(!(first.compare(second)==0))
		{
			headBodyMap.insert(std::make_pair(first ,second));
			std::vector<std::string> tokens;
			boost::split(tokens,first,boost::is_any_of("^"), boost::token_compress_on);
			for(unsigned int i = 0; i<tokens.size(); i++)
			{
				atomMap[tokens[i]] = false;
			}
			boost::split(tokens,second,boost::is_any_of("^"), boost::token_compress_on);
			for(unsigned int i = 0; i<tokens.size(); i++)
			{
				atomMap[tokens[i]] = false;
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
	for (auto iter=headBodyMap.begin(); iter!=headBodyMap.end(); iter=headBodyMap.equal_range(iter->first).second)
	{
			std::string uniq_key=iter->first;
			std::string comp("");
			
			comp.append(uniq_key).append("=>");
			
			atomMap[uniq_key] = true;
			
			auto it_bounds = headBodyMap.equal_range(uniq_key);
			
			for (auto it=it_bounds.first; it!=it_bounds.second; it++)
			{
				comp.append(it->second).append(" v ");
			}
			comp = comp.substr(0,comp.length()-2);
			comp.append(".");
			std::cout<<comp<<std::endl;
	}
	
	for (std::map<ATOM, BOOLEAN>::iterator it=atomMap.begin(); it!=atomMap.end(); ++it)
	{
		std::string str;
		
		if(it->second == false)
		{
			if(!((it->first)[0] == '!'))
				str.append("!");
			str.append(it->first);
			str.append(".");
			std::cout<<str<<std::endl;
		}
	}
	if(outfile.is_open()){
		outfile.close();
	}
	return 0;
	
}


//void matchDomains(std::string str)
//{
//	
//	boost::regex expr("([a-zA-Z ]+)(=){1}([{}a-zA-Z, ]+)");
//	std::string::const_iterator start, end;
//	start = str.begin();
//	end = str.end();
//	boost::match_results<std::string::const_iterator> what;
//	boost::match_flag_type flags = boost::match_default;
//	
//	if(boost::regex_match(str, expr ))
//	{
//		boost::regex_search(start, end, what, expr, flags);
////		what[3] has {jo, bob}
//		std::string temp = what[3];
//		ba::trim(temp);
//		std::vector<std::string> tokens;
//		temp = temp.substr(1,temp.length()-2);
//		boost::split(tokens, temp, boost::is_any_of(","), boost::token_compress_on);
//		
//		for(unsigned int i = 0; i < tokens.size(); i++)
//		{
//			ba::trim(tokens[i]);
//			domainMap[tokens[i]] = true;
//		}
//	
//	}
//}