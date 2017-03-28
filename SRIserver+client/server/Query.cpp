/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Query.cpp
 * Author: Nicole
 * 
 * Created on February 26, 2017, 7:47 PM
 */
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <thread>
#include "Query.h"
#include "RB.h"
#include "KB.h"
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <map>
#include "Rule.h"
#include "Fact.h" 


//method to strip whitespace and newlines from strings 
std::string removeSpace(std::string &S) {
    
    S.erase(std::remove_if(S.begin(), S.end(), 
            [](char c){
                return (c=='\r'||c=='\n'||c==' ');
            }), S.end() );
    return S;
}

//method to remove the first word of a string 
std::string removeFirst(std::string& S) {
    std::string::size_type x =0;
    x=S.find_first_not_of(" \t", x);
    x=S.find_first_of(" \t", x);
    S.erase(0, S.find_first_not_of(" \t", x));
    
    std::cout<<S;
    return S;
}
//*******************************************************************

//calls the query class, passing a string argument
void INFERENCE(std::string obj, RB *rb, KB *kb) {
    //wrapper call query 
	vector<Fact*> facts=inquire(obj, rb, kb);

//case where there is a new rule name to save into kb
	if (obj.find(")")+1 < obj.size()) {
		std::cout<<"Saving new facts to kb"<<std::endl;
		std::string SUB = obj.substr(obj.find(")")+2, obj.size()-obj.find(")"));
		//if second word, rename all to that second word
		//for (int i = 0; i < facts.size(); i++) {
		//	kb->add(new Fact(SUB, facts.at(i)->param.at(0), facts.at(i)->param.at(1)) );
		//}

	} else {
//END BAD CODE 
	removeSpace(obj);
	for (int i = 0; i < facts.size(); i++)
	{
		std::cout << i <<"\n";
		std::cout << facts.at(i)->Name << "(";
		std::cout << facts.at(i)->param.at(0) << ",";
		std::cout << facts.at(i)->param.at(1) << ")\n";
	}
	}
}

//deletes fact or rule
void DROP(std::string obj, KB *kb, RB *rb) {
    //search through KB and RB for name, then delete 
    //STRIP WHITESPACE
    removeSpace(obj);
    kb->del(obj);
    rb->delRule(obj);
}

//***********************************************************

//Dumps the contents of KB and RB to an SRI file 
void DUMP(std::string filename, KB *kb, RB *rb) {
    std::cout << "Dumping to " << filename << std::endl;
    //STRIP WHITESPACE
    removeSpace(filename);
    
    //create and open file with name filename
    std::ofstream file;
    file.open(filename);
    
    
    //for each member of rb.allRules, dump the rule
    for(std::map<std::string, Rule*>::iterator x=(rb->allRules)->begin();
            x!=(rb->allRules)->end(); ++x ) {
 
        //if we implement the vector in each rule,
        //dump first section of rule 
        //for x in kb, 
            //dump rulename and associated vector 
        file<<"RULE ";
        file<<(x->first);
        file<<"(";
        for (std::vector<std::string>::iterator P=((x->second)->param).begin();
                 P!=((x->second)->param).end(); ++P) {
                    file <<*P;
                 if (P!=((x->second)->param).end()-1) {
                   file <<",";
                 }
         }
        file<<"):- ";
	if ((x->second)->op == 'O') {
		file <<"OR ";
	} else {
		file <<"AND ";
	}


        file<<(x->second)->Name1<<"(";
         //dump second rule name and associated vector
         for (std::vector<std::string>::iterator P=((x->second)->param1).begin();
                 P!=((x->second)->param1).end(); ++P) {
                file<<*P;
                if (P!=((x->second)->param1).end()-1) {
                   file <<",";
                } 
         }
        //dump third rule name and associated vector
        file<<") "<<(x->second)->Name2<<"(";
         for (std::vector<std::string>::iterator P=((x->second)->param2).begin();
                P!=((x->second)->param2).end(); ++P) {
                    file<<*P;
                if (P!=((x->second)->param2).end()-1) {
                   file <<",";
                } 
         }
        file<<")\n";
    }
    

    //for each member of kb.array, dump the fact
    for(std::vector<Fact*>::iterator x=(kb->allFacts)->begin(); 
            x!=(kb->allFacts)->end(); ++x) {
        file<<"FACT ";
	file<<(*x)->Name;		//x pointer?
	file<<"(";
        for (auto P=((*x)->param).begin(); P!=((*x)->param).end(); ++P) {
            file <<*P;
            if (P!=((*x)->param).end()-1) {
                file<<",";
            }
        }
		file<<")\n";
    }
    //close the file
    file.close();
}


//loads the KB and RB with contents of given SRI file 
void LOAD(std::string filename, KB *kb, RB* rb) {
    removeSpace(filename);
    if (filename == "") {
	std::cout<<"No filename Specified. \n";
        throw(false);
    }
    std::cout << "Loading from" << filename << std::endl;
    
    std::string line = ""; //string to read lines from input 
    std::ifstream readFile(filename); //opens the input file 
    std::string word =""; //fact or rule prefix
    std::size_t SECOND; //index of word after the prefix
    
    //line by line add rules/facts to the RB/KB
    while(std::getline(readFile, line)) {
        std::stringstream iss(line); //load into string stream
    
        //if fact, add to KB
        //if rule, add to RB
        if (iss>>word) {
            SECOND = line.find(word);
            if (word=="RULE") {
                //not sure if this works
                rb->add( new Rule(removeFirst(line))); //New?
            } else if (word == "FACT") {
                kb->add(new Fact(removeFirst(line)));
            }
        }
        
    }
    
    
}



// Carries out queries, scans RB and KB with given parameters
std::vector<Fact*> inquire(std::string n, RB *rb, KB *kb)
{
	std::vector<Fact*> unfilterfacts;
	std::vector<std::string> param; //from inference
	string Name = n.substr(0, n.find("("));

	std::string namestr = n.substr(0, n.find(")") + 1);
	n = n.substr(n.find(")") + 1, n.size() - n.find(")") + 1);

	//counts the number of the commas/parmas
	int count = 1;
	for (int i = 0; i < namestr.length(); i++)
	{
		std::string temp = namestr.substr(i, 1);
		if (temp.find(",") == 0)
		{
			count++;
		}
	}

	//parse the chuck from the main string
	for (int i = 0; i < count; i++)
	{
		if (i == 0)
		{
			param.push_back(
					namestr.substr(namestr.find("(") + 1,
							namestr.find(",") - namestr.find("(") - 1));
			namestr = namestr.substr(namestr.find(","),
					namestr.length() - namestr.find(","));
		}
		else if (i == count - 1)
		{
			param.push_back(
					namestr.substr(namestr.find(",") + 1,
							namestr.find(")") - namestr.find(",") - 1));
		}
		else
		{
			param.push_back(namestr.substr(1, namestr.find(",", 2) - 1));
			namestr = namestr.substr(namestr.find(",", 2),
					namestr.length() - namestr.find(",", 2));
		}

	}

	//Builds a collection of unfiltered facts, then after going through the SRI algorithm it will be run thru filter
	//std::thread thread1 (&KB::search,kb, Name);
	//thread1.join();
	unfilterfacts = merge(unfilterfacts, kb->search(Name));
	Rule *rule = rb->search(Name);
	if(rule != NULL){

		unfilterfacts = merge(unfilterfacts, infer(rule,rb,kb,param.at(0),param.at(1)));
	      
	}



	std::string test = param.at(0);
    return filter(unfilterfacts, param.at(0), param.at(1)); //returns final filter query
}

void makeFacts1(std::vector<Fact*>& facts1, RB* rb, KB* kb, Rule* rule ){
	// search Rule param1
	Rule* found1 = rb->search(rule->Name1);
	// fact found, not rule
	if(found1 == NULL){
	    // search KB for fact
	    facts1 = merge(facts1, kb->search(rule->Name1));
	}
	else if(found1 != NULL){
	    facts1 = merge(facts1, infer(found1, rb, kb, rule->param.at(0), rule->param.at(1)));
	    facts1  = merge(facts1, kb->search(rule->Name1));
	}


}

void makeFacts2(std::vector<Fact*>& facts2, RB* rb, KB* kb, Rule* rule ){
    Rule* found2 = rb->search(rule->Name2);
    if(found2 == NULL){
        facts2 = merge(facts2, kb->search(rule->Name2));
    }
    else if(found2 != NULL){
        facts2 = merge(facts2, infer(found2, rb, kb, rule->param.at(0), rule->param.at(1)));
        facts2 = merge(facts2, kb->search(rule->Name2));
    }

}
//Takes in Rule that is being infered and the params of inference
std::vector<Fact*> infer(Rule *rule, RB *rb, KB *kb, std::string param1,
		std::string param2)
{
	std::vector<Fact*> facts1;
	std::vector<Fact*> facts2;

	int l;
	int k;
	
	std::thread thread1(makeFacts1, std::ref(facts1), rb, kb, rule);
	std::cout<<"Thread 1 started"<<std::endl;
	std::thread thread2(makeFacts2, std::ref(facts2), rb, kb, rule);
	std::cout<<"Thread 2 started"<<std::endl;

	//join both threads to main thread 
	thread1.join();
	std::cout<<"Thread 1 finished"<<std::endl;
	thread2.join();
	std::cout<<"Thread 2 finished"<<std::endl;


	if (rule->op == 'O')
	/*{
		std::vector<Fact*> temp = merge(facts1, facts2);
		for (int i = 0; i < temp.size(); i++)
			{
				temp.at(i)->Name = rule->Name;
			}
		return temp;
	}
	*/
	return merge(facts1, facts2);
	else // op == 'A'
	{
		       std::vector<Fact*> facts3;
       // 1 == 3, 2 == 4
       if( (rule->param1.at(0)==rule->param2.at(0)) && (rule->param1.at(1)==rule->param2.at(1)) ){
    		   //std::cout << "case1 i: " << i << "  facts2.size(): " << facts2.size() << "\n";
    	       facts3 = merge(facts1, facts2);
       // 1 == 3, 2 != 4
       }else if( (rule->param1.at(0)==rule->param2.at(0)) && (rule->param1.at(1)!=rule->param2.at(1)) ){
    	   for (int i = 0; i < facts1.size(); i++)
           {
    		   //std::cout << "case2 i: " << i << "  facts2.size(): " << facts2.size() << "\n";
    	       facts3 = merge(facts3, filter(facts2, facts1.at(i)->param.at(0), "$Q"));
    	   }
       // 1 != 3, 2 == 4
       }else if( (rule->param1.at(0)!=rule->param2.at(0)) && (rule->param1.at(1)==rule->param2.at(1)) ){
    	   for (int i = 0; i < facts1.size(); i++)
           {
    		   //std::cout << "case3 i: " << i << "  facts2.size(): " << facts2.size() << "\n";
    	       facts3 = merge(facts3, filter(facts2, "$Q", facts1.at(i)->param.at(1)));
    	   }
       }else{ // 1 != 3, 2 != 4
    	   // 2 == 3
    	   if( rule->param1.at(1)==rule->param2.at(0) ){
        	   for (int i = 0; i < facts1.size(); i++)
               {
        		   //std::cout << "case4/2=3 i: " << i << "  facts2.size(): " << facts2.size() << "\n";
        	       facts3 = merge(facts3, filter(facts2, facts1.at(i)->param.at(1), "$Q"));
        	   }
    	   } // 1 == 4
    	   else if( rule->param1.at(0)==rule->param2.at(1) ){
        	   for (int i = 0; i < facts1.size(); i++)
               {
        		   //std::cout << "case4/1=4 i: " << i << "  facts2.size(): " << facts2.size() << "\n";
        	       facts3 = merge(facts1, filter(facts2, "Q", facts1.at(i)->param.at(0)));
        	   }
    	   }
       }
    return facts3;
	}

}

//doesnt handle dynamic number of params
std::vector<Fact*> filter(std::vector<Fact*> unfilterfacts, std::string param1,
		std::string param2)
{
	//if both params are variables do nothing and return
	if (param1.substr(0, 1) == "$" && param2.substr(0, 1) == "$")
	{
		return unfilterfacts;
	}
	//if param1 is var
	else if (param1.substr(0, 1) == "$" && param2.substr(0, 1) != "$")
	{
		for (int i = 0; i < unfilterfacts.size(); i++)
		{
		    //checks if param2 from interference and Fact param2; remember Fact object holds dynamic number of params in a vector
			if (unfilterfacts.at(i)->param.at(1) != param2)
			{
			    // if not match, take out of unfilteredfacts
				unfilterfacts.erase(unfilterfacts.begin() + i);
				i--;
			}
		}
	}
	//if param2 is var
	else if (param2.substr(0, 1) == "$" && param1.substr(0, 1) != "$")
	{
		for (int i = 0; i < unfilterfacts.size(); i++)
		{
			if (unfilterfacts.at(i)->param.at(0) != param1)
			{
				unfilterfacts.erase(unfilterfacts.begin() + i);
				i--;
			}
		}
	}
	else //if both param are vars
	{
		for (int i = 0; i < unfilterfacts.size(); i++)
		{
			if (unfilterfacts.at(i)->param.at(0) != param1)
			{
				unfilterfacts.erase(unfilterfacts.begin() + i);
				i--;
			}
		}

		for (int i = 0; i < unfilterfacts.size(); i++)
		{

			if (unfilterfacts.at(i)->param.at(1) != param2)
			{
				unfilterfacts.erase(unfilterfacts.begin() + i);
			}
		}
	}

	return unfilterfacts;
}

// Joins two Fact vectors and checks for duplicates
std::vector<Fact*> merge(std::vector<Fact*> facts, std::vector<Fact*> facts1){

	std::vector<Fact*> merge;

	for (int i = 0; i < facts.size(); i++)
    {
        merge.push_back(facts.at(i));
    }

    int size = merge.size();
    for (int i = 0; i < facts1.size(); i++)
    {
        bool dup = 0;

        for (int j = 0; j < size; j++)
        {
            if (facts1.at(i)->Name == merge.at(j)->Name
                    && facts1.at(i)->param.at(0) == merge.at(j)->param.at(0)
                    && facts1.at(i)->param.at(1) == merge.at(j)->param.at(1))
            {
                dup = 1;
                break;
            }
        }

        if (dup == 0)
        {
            merge.push_back(facts1.at(i));
        }
    }
    return merge;
}

bool  isValid(std::vector<std::string> a1, std::vector<std::string> a2,
	std::vector<std::string> b1, std::vector<std::string> b2) {

	for (int i = 0; i < b1.size(); i++) {
		for (int j = 0; j < b2.size(); j++) {
			if (a1.at(i) == a2.at(j) && b1.at(i) != b2.at(j)) {
				return false;
			}
		}
	}
	return true;
}

//checks if string is in fact's vector of parameters. If so, returns the index
int isIn(std::string s, Fact* fact) {
	for (int x = 0; x < fact->param.size(); x++) {
		if (s == fact->param.at(x)) {
			return x;
		}
	}
	return NULL;
}

