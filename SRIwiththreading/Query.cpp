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

