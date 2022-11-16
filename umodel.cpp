#include "event.h"
#include "umodel.h"
#include "kedge.h"
#include "define.h"

/*********************************************************************
 Update model implementation
**********************************************************************/

UpdateModel::UpdateModel()
{

}

UpdateModel::~UpdateModel()
{

}

Events UpdateModel::get_eventslist()
{
    return events_list;
}

BE UpdateModel::get_edges()
{
    return edges;
}

Edges UpdateModel::get_edgeslist()
{
    return edges_list;
}

vector<Event*> UpdateModel::get_event()
{
    vector<Event*> result;
    for(int i = 0; i < pId.size(); i++){
        result.push_back(&events_list.at(i));
    }
    return result;
}

vector<int> UpdateModel::get_id()
{
    return pId;
}

void UpdateModel::set_eventslist(Events el)
{
    events_list = el;
}

void UpdateModel::set_edges(BE el)
{
    edges = el;
}

void UpdateModel::set_edgeslist(Edges el)
{
    edges_list = el;
}

void UpdateModel::set_pId(vector<int> id)
{
    pId = id;
}

void UpdateModel::set_edgeslist_fix_updates(Edges el) //set the edgeslist and fix the hashmap mapEdgelabelNextEvent in each state as well
{
    for(Edges::iterator i = el.begin(); i != el.end(); i++){
        add_edge(*i);
    }
}

void UpdateModel::set_eventslist_fix_updates(Events el) //set the eventslist and clear the hashmap mapEdgelabelNextEvent in each state as well
{
    for(Events::iterator i = el.begin(); i != el.end(); i++){
        (*i).reset_map();
    }
    events_list = el;
}

bool UpdateModel::isValid(AgentSet ag){
    for (int i = 0; i < pId.size(); i++){
        Event e = events_list.at(i);
        EdgeList el = e.get_out();
        EdgeList::iterator it;
        for(it = el.begin(); it != el.end(); it++){
            int label = edges_list.at(*it).get_label();
            ag.erase(label);
            if(ag.size() == 0){
                return true;
            }
        }
    }
    return false;
}

EventList UpdateModel::reachableEvents(int fromEventID, Agents ags) const
{
    Event src = events_list.at(fromEventID);
    EventList out;

    EventList newlyadd;
    newlyadd.insert(fromEventID);
    while (newlyadd.size() != 0)
    {
        int curId = *newlyadd.begin();
        newlyadd.erase(newlyadd.begin());
        Event curEvent = events_list.at(curId);
        EdgeList el = curEvent.get_out();
        EdgeList::iterator it;
        for(it = el.begin(); it != el.end(); it++)
        {
            Kedge e = edges_list.at(*it);
            Agent ag = e.get_label();
            //check edge's agent
            Agents::iterator it1;
            it1 = std::find(ags.begin(), ags.end(), ag);
            if(it1 == ags.end())
            {
                continue;
            }
            //check the event_to is in outlist or not yet
            int to = e.get_to();
            EventList::iterator ielt;
            ielt = out.find(to);
            if(ielt == out.end())
            {
                out.insert(to);
                newlyadd.insert(to);
            }
        }
    }
    return out;
    
}

void UpdateModel::add_event(Event event)
{
    events_list.push_back(event);
}

void UpdateModel::print_report()
{
	cout << "The number of events is: " << events_list.size() << endl;
	cout << "The number of edges is: " << edges_list.size() << endl;
    cout << "Designated event(s) is(are): " << events_list.at(pId[0]).get_id();
    if (pId.size() > 1){
        for (int i = 1; i < pId.size(); i++){
            cout << ", " << events_list.at(i).get_id();
        }
    }
    cout << endl;

	Events::iterator it;
	for(it = events_list.begin(); it != events_list.end(); it++)
	{
        it->print_info();
        cout << endl;
//        cout << "print states into...." << endl;
//        EdgeList::iterator iee;
//        for(iee = (*it)->get_in()->begin(); iee!= (*it)->get_in()->end(); iee++){
//            cout << "id of edge is " << (*iee)->get_id() << endl;
//        }
	}

    Edges::iterator ite;
    for(ite = edges_list.begin(); ite!=edges_list.end();ite++)
    {
		ite->print_info();
	}

    BE::iterator it3;
    vector<int>::iterator it2;

    for(it3 = edges.begin(); it3 != edges.end(); it3++)
        for(it2 = it3->begin(); it2 != it3->end(); it2++)
        {
            cout << "Edge (" << edges_list.at(*it2).get_exist() << ") with id " << edges_list.at(*it2).get_id() << " has label " << edges_list.at(*it2).get_label()
            << " and connects worlds " << edges_list.at(*it2).get_from() << " --> "
            << edges_list.at(*it2).get_to() << endl;
        }
    cout << endl << endl;
}

void UpdateModel::add_edge2event(int stIndex, int edgeIndex, bool isOut)
{
	Event* st = &events_list.at(stIndex);
	if(isOut == true){
		st->add_out(edgeIndex);
	}else{
		st->add_in(edgeIndex);
	}
}

void UpdateModel::add_edge(Kedge k)
{
	edges_list.push_back(k);
    Event* evFrom = &events_list.at(k.get_from());
    Event* evTo = &events_list.at(k.get_to());
    Agent label = k.get_label();
    //add edge to stFrom
    evFrom->add_out(k.get_id());
    evFrom->add_to_map(label, k.get_to());
    //add edge to stTo
    evTo->add_in(k.get_id());
}

void UpdateModel::add_to_edges_list(Kedge k) //add to edges list only, without add to states
{
    edges_list.push_back(k);
}

void UpdateModel::add_true_event(int id)
{
    pId.push_back(id);
}