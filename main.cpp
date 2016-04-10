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
std::string uniqueVars[] = {"_a","_b","_c","_d","_e","_f","_g","_h","_i","_j","_k","_l","_m","_n"};

int charCount = 0;

int main(int argc,char *argv[])
{
	parse("/home/samidh/Documents/lpmln/lpmln-regex/input.txt");

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
		
		MAP::iterator it = predMapStr.find(predName);
		if(it != predMapStr.end())
			predMapStr[predName] = std::vector<lint>{TRUE,tokens.size()};
		else
			predMapStr.insert(std::pair<std::string, std::vector<lint>>(predName,std::vector<lint>{TRUE,tokens.size()})) ;

		
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
//	Predicate* pred;
	if(boost::regex_match(str,expr))
	{
		boost::regex_search(start, end, what, expr, flags);
		std::string first = what[3];
		std::string second = what[1];
		
		// If it is not a choice rule
		if(!(first.compare(second)==0))
		{
			headBodyMap.insert(std::make_pair(first ,second));
			std::vector<std::string> tokens;
			
			//Split conjunctive head terms
			boost::split(tokens,first,boost::is_any_of("^"), boost::token_compress_on);
			boost::regex tokenExpr("([a-zA-Z]+)(\\()([A-Za-z0-9,]+)(\\))");
			
			for(unsigned int i = 0; i<tokens.size(); i++)
			{
				start = tokens[i].begin();
				end = tokens[i].end();
				
				//Find local variables in each of the conjunctive head terms
				boost::regex_search(start,end,what, tokenExpr, flags);
				
				std::vector<std::string> vars;
				
				//Split local variables by comma
				boost::split(vars,tokens[i],boost::is_any_of(","), boost::token_compress_on);
				
				MAP::iterator it = predMapStr.find(what[1]);
				if(it == predMapStr.end())
					predMapStr.insert(std::pair<std::string, std::vector<lint>>(what[1],std::vector<lint>{TRUE,tokens.size()})) ;
				

			}
			boost::split(tokens,second,boost::is_any_of("^"), boost::token_compress_on);
			for(unsigned int i = 0; i<tokens.size(); i++)
			{
				start = tokens[i].begin();
				end = tokens[i].end();
				boost::regex_search(start,end,what, tokenExpr, flags);
				std::vector<std::string> vars;
				boost::split(vars,tokens[i],boost::is_any_of(","), boost::token_compress_on);

				MAP::iterator it = predMapStr.find(what[1]);
				if(it == predMapStr.end())
					predMapStr.insert(std::pair<std::string, std::vector<lint>>(what[1],std::vector<lint>{FALSE,vars.size()})) ;
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
				predMapStr[what[1]] = std::vector<lint>{1, atoms.size()};				
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
	

	for (auto it=predMapStr.begin(); it!=predMapStr.end(); ++it)
	{
		std::string str;
		
		if(it->second[0] == FALSE)
		{
			str.append("!");
			str.append((it->first));
			str += "(";
			for(unsigned int i=0;i<(it->second)[1];i++)
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

	return 0;
	
}
