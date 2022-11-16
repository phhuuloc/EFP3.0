#include "event.h"
#include "kedge.h"
#include "define.h"
#include <map>
#include <list>
#include <string>
#include <iostream>

Event::Event(){
    // pre = BFNode();
    // sub = Literals();
    pre_sub = Fluents_BF_list();
    edge_out = EdgeList();
    edge_in = EdgeList();
    mapEdgelabelNextEvent = map<int, set<int> >();
}

Event::~Event(){

}

EdgeList Event::get_in()
{
	return edge_in;
}

EdgeList Event::get_out()
{
	return edge_out;
}

Fluents_BF_list Event::get_pre_sub()
{
    return pre_sub;
}

int Event::get_id()
{
	return id;
}

string Event::get_name()
{
    return eventName;
}

bool Event::get_exist()
{
	return exists;
}

void Event::set_in(EdgeList in)
{
    edge_in = in;
}

void Event::set_out(EdgeList out)
{
    edge_out = out;
}

void Event::set_pre_sub(const Fluents_BF_list ps)
{
    // Fluents_BF_list::const_iterator itf;
    // for (itf = ps.begin(); itf != ps.end(); itf++){
    //     Fluents_BF add = Fluents_BF();
    //     add.set_bn_body(*itf->get_body());

    //     Literals addLit;
    //     Literals::iterator itLit;
    //     for (itLit = itf->get_head()->begin(); itLit != itf->get_head()->end(); itLit++){
    //         Literal x = *itLit;
    //         addLit.insert(x);
    //         cout << "Literal: " << x << endl;
    //     }
    //     add.set_l_head(addLit);

    //     pre_sub.push_back(add);
    // }
    pre_sub = ps;
}

// void Event::set_pre(BFNode p)
// {
//     pre = p;
// }

// void Event::set_sub(Literals f)
// {
//     sub = f;
// } 

void Event::set_name(string name)
{
    eventName = name;
}

void Event::set_id(int ident)
{
	id = ident;
}

void Event::set_exist(bool ex)
{
	exists = ex;
}

map<int, set<int> > Event::get_preparedMap(){
    return mapEdgelabelNextEvent;
}

void Event::reset_map(){
    mapEdgelabelNextEvent = map<int, set<int> >();
}

void Event::add_to_map(int ag, int des){
    
    set<int> isAdding;
    if(mapEdgelabelNextEvent.find(ag) != mapEdgelabelNextEvent.end()){
        isAdding = mapEdgelabelNextEvent[ag];
    }else{
        isAdding = set<int>();
    }
    isAdding.insert(des);
    mapEdgelabelNextEvent[ag] = isAdding;
}

void Event::add_out(int e)
{
	edge_out.push_back(e);
}

void Event::add_in(int e)
{
	edge_in.push_back(e);
}

void Event::print_info()
{	
	cout << "Event " << eventName << " with id " << id << " specs:" << endl;
    //cout << "Does exist: " << exists << endl;
	cout << "--The number of edges out: " << edge_out.size() << endl;
	EdgeList::iterator ite;
	for(ite = edge_out.begin();ite!=edge_out.end();ite++)
	{
		cout << *ite << " ";
	}
	cout << endl;
	cout << "--The number of edges in: " << edge_in.size() << endl;
	for(ite = edge_in.begin();ite!=edge_in.end();ite++)
	{
		cout << *ite << " ";
	}
	cout << endl;

    Fluents_BF_list::iterator itf;

    for(itf = pre_sub.begin(); itf != pre_sub.end(); itf++)
    {
        cout << "Precondition of event " << id << " is: ";
        const BFNode* tempBFN = itf->get_body();
        // cout << "NODE TYPE " << tempBFN->node_type << endl;
	    tempBFN->print();

        cout << endl;

        Literals::iterator it;
        const Literals* tempLit = itf->get_head();
        cout << "Subsitution of event " << id << " is: ";
        for(it = tempLit->begin(); it != tempLit->end(); it++)
            cout << *it << ", ";
        cout << endl;
    }


	
}