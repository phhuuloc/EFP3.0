#ifndef UMODEL_H
#define UMODEL_H
#include "define.h"
#include "event.h"
#include "kedge.h"

typedef vector<Event> Events;
typedef set<int> EventList; // vector of events's id
typedef vector< vector<int> > BE;

class UpdateModel{
    protected:
        Events events_list;
        Edges edges_list;
        BE edges;
        vector<int> pId;
    
    private:

    public:
        UpdateModel();
        ~UpdateModel();

        Events get_eventslist();
        BE get_edges();
        Edges get_edgeslist();
        vector<Event*> get_event();
        vector<int> get_id();
        void set_eventslist(Events);
        void set_edges(BE);
        void set_pId(vector<int>);
        void set_edgeslist(Edges);
        void set_edgeslist_fix_updates(Edges);
        void set_eventslist_fix_updates(Events);
        void add_event(Event);
        void print_report();
        void add_edge2event(int evIndex, int edgeIndex, bool isOut);
        void add_edge(Kedge);
        void add_true_event(int);
        void add_to_edges_list(Kedge k); //add to edges list only, without add to events
        EventList reachableEvents(int, Agents) const;
        bool isValid(AgentSet);
};

#endif
