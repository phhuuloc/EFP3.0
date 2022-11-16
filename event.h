#ifndef EVENT_H
#define EVENT_H
#include "kedge.h"
#include "define.h"
#include <map>
#include <list>
#include <string>
#include <iostream>


using namespace std;

class Kedge;

class Event{
    protected:
        int id;
        string eventName;
        EdgeList edge_out;
        EdgeList edge_in;
        Fluents_BF_list pre_sub;
        bool exists;
        
        map<int, set<int> > mapEdgelabelNextEvent; //is used to get the next states get to via the edges of label agent.

    public:
        Event();
        ~Event();

        EdgeList get_in();
	    EdgeList get_out();
        Fluents_BF_list get_pre_sub();
        // BFNode get_pre();
        // Literals get_sub();
        string get_name();

        int get_id();
	    bool get_exist();
        map<int, set<int> > get_preparedMap();

        void set_in(EdgeList);
	    void set_out(EdgeList);
        void set_pre_sub(const Fluents_BF_list);
        void set_name(string);

        void set_id(int);
        void set_exist(bool);
        void add_out(int);
        void add_in(int);
        void print_info();
        void add_to_map(int, int); //is used to add to mapEdgelabelNextState
        void reset_map(); // is used to resetset mapEdgelabelNextState = map<int, set<int> >();
};

#endif
