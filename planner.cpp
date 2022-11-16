// planner.cpp: implementation of the planner class.
//////////////////////////////////////////////////////////////////////
#include "planner.h"
#include <math.h>
#include <stdio.h>
#include "kstate.h"
#include "kedge.h"
#include "kripke.h"
#include "define.h"
#include "event.h"
#include "umodel.h"
#include "planninggraph.h"

/*************************************************************************
* Planner Implementation
*************************************************************************/
// Constructor & destructor
Planner::Planner(Reader* reader, Timer* timer)
{
	m_timer = timer;
	m_reader = reader;

	m_semantics = PC;
	m_task = SEARCH;
	m_algorithm = GREEDY;
	m_detect_essential = true;
	m_output_decisive = false;
	m_series_kripkes = false;
    debug = false;
    m_heuristic = NOTUSE;
    useHeuristic = false;
}

Planner::~Planner()
{

}

bool Planner::main()
{


#ifdef PRINT_DOMAIN
	m_reader->print();
	exit(0);
#endif


	// build the domain
	cout << "Building domain..." << endl;
	m_timer->start(BUILD_TIMER);
	build_domain();
	build_goal();
	//print_read_init();

	cout << "Building initial Kripke..." << endl;
    if(build_init_k() == false){
        cout << "Cannot build initial Kripke!!! Planner stops!!!" << endl;
        return 1;
    };
	m_timer->end(BUILD_TIMER);

	cout << "The number of pointed Kripkes in the initial Kripke is " << m_init_cstates.size() << endl;
    if(debug){
        for(int run = 0; run < m_init_cstates.size(); run++){
            cout << "--------------Printing Kripke report for initial Kripke :-----------" << endl << endl;
            m_init_cstates.at(run).print_report();
            cout << "----------------------------------" << endl << endl;
        }
    }
    
 	cout << "Solving..." << endl;
    
    
	m_timer->start(SEARCH_TIMER);

	CKripke* sol;
	if (useHeuristic) {
		sol = searchwithheur();
	}
	else {
		sol = search();
	}
	m_timer->end(SEARCH_TIMER);
    if(sol != NULL){
        vector<string> solution = sol->get_path();
        vector<string>::iterator isol;
        cout << "----> PLAN is: " << endl;
        for (isol = solution.begin();isol != solution.end();isol++)
        {
            cout << *isol << " ";
        }
        cout << endl;
		// cout << "Final state loop: " << sol->ks[0]->get_pstate()->loopIn() << endl;
		// cout << "Final Kripke ";
		// sol->get_result()[0]->print_report();
        // sol->get_result();
    }else {
        cout << "----> THERE IS NO PLAN FOR SUCH GOALS!!!! " << endl;
    }	
    if(debug){
        print_interal_domain();
        cout << endl;
    }
	print_statistics();

	return true;
};

CKripke* Planner::search()
{
	CKripke* out;
	CKripke* ck = new CKripke(this);
	ck->add_path("start");
	vector<Kripke>::iterator itk;
	for (itk = m_init_cstates.begin(); itk != m_init_cstates.end(); itk++) {
		Kripke* k = new Kripke(&*itk);
		ck->add_kripke(k);
	}
	//Kripke* k = new Kripke(&m_init_cstate);
	//ck->set_result(k);
	if (ck->satisfy()) {
        cout << "Congratulation. Goal found!................ :)" << endl;
		return ck;
	}

	CKripkeQueue* queue = new CKripkeQueue();
	queue->push(ck);
	if (queue->size() != 0) {
		CKripke* n = queue->front();
		queue->pop();
		out = search(n, queue);
		return out;
	}
	else {
		return NULL;
	}
}

CKripke* Planner::search(CKripke* cur, CKripkeQueue* queue)
{
	KActionList::iterator it;
	CKripke* out;
    //cout << " the size of queue is " << queue->size() << endl;
	for (it = ontic.begin(); it != ontic.end(); it++)
	{
//        if(debug){
//            cout << " current is working on " << it->get_name() << " at " ;
//            vector<string> fordebug = cur->get_path();
//            for(vector<string>::iterator i = fordebug.begin(); i!= fordebug.end(); i++){
//                cout << *i  << "-" ;
//            }
//            cout << endl;
//        }
        if (it->get_name().compare(cur->get_path().back()) == 0 && it->get_type() != DYNAMIC){
			continue;
		}


		CKripke* next = cur->next_ckripke(*it);
		if (next != NULL) {
            //cout << it->get_name() << " is executable " << endl;
			if (next->satisfy())
			{
                cout << "Congratulation. Goal found!..... :)" << endl;
				return next;
			}
			else {
				//cout << "size of path is " << next->get_path().size() << endl;
                //cout << " current adding on " << it->get_name() << endl;
				queue->push(next);
			}
		}
        else{
            //cout << it->get_name() << " is NOT executable " << endl;
        }
	}
	if (queue->size() != 0) {
		CKripke* n = queue->front();
		queue->pop();
		out = search(n, queue);
		return out;
	}
	else {
 		return NULL;
	}
}

/* search with heuristic */

CKripke* Planner::searchwithheur()
{
	CKripke* out;
	CKripke* ck = new CKripke(this);
	ck->add_path("start");
    vector<Kripke>::iterator itk;
    for (itk = m_init_cstates.begin(); itk != m_init_cstates.end(); itk++) {
        Kripke* k = new Kripke(&*itk);
        ck->add_kripke(k);
    }
    
	//Kripke* k = new Kripke(&m_init_cstate);
	//ck->set_result(k);
	if (ck->satisfy()) {
        cout << "Congratulation. Goal found!..... :)" << endl;
		return ck;
	}
	Priority_Queue* queue = new Priority_Queue(this);
	queue->insert(ck, debug);
    if (queue->front != NULL) {
        CKripke* n = queue->pop();
        if(debug==true){
            vector<string> fordebug = n->get_path();
            for(vector<string>::iterator i = fordebug.begin(); i!= fordebug.end(); i++){
                cout << *i  << "-" ;
            }
            cout << endl;
        }
        out = searchwithheur(n, queue);
        
        return out;
    }
    else {
        return NULL;
    }
	//CKripke* n = queue->pop();
	//out = searchwithheur(n, queue);
	//return out;
}

CKripke* Planner::searchwithheur(CKripke* cur, Priority_Queue* queue)
{
	KActionList::iterator it;
	CKripke* out;
	for (it = ontic.begin(); it != ontic.end(); it++)
	{
		if (it->get_name().compare(cur->get_path().back()) == 0 && it->get_type() != DYNAMIC){
			continue;
		}

		CKripke* next = cur->next_ckripke(*it);
		if (next != NULL) {
			if (next->satisfy())
			{
                cout << "Congratulation. Goal found!..... :)" << endl;
                return next;
			}
			else {
				//cout << "size of path is " << next->get_path().size() << endl;
                bool tiep;
                if(queue->front == NULL){
                    tiep = true;
                }
                else{
                    tiep = false;
                }
                if(debug){
                    cout << ">>>>>adding ckripke of executing ";
                    vector<string> p = next->get_path();
                    for(vector<string>::iterator ip = p.begin(); ip!= p.end(); ip++){
                        cout << *ip << "--";
                    }
                    cout << endl;
                }
				queue->insert(next,debug);
			}
		}
	}
    if (queue->front != NULL) {
        CKripke* n = queue->pop();
        if(debug){
            vector<string> p = n->get_path();
            for(vector<string>::iterator ip = p.begin(); ip!= p.end(); ip++){
                cout << *ip << "--";
            }
            cout << endl;
        }
        out = searchwithheur(n, queue);
        return out;
    }
    else {
        return NULL;
    }
	//CKripke* n = queue->pop();
	//if (n == NULL) {
	//	return NULL;
	//}
	//out = searchwithheur(n, queue);
	//return out;
}
//---------------------

bool Planner::build_domain()
{
	int i;
	StringList::const_iterator itf;
	typedef map<string, Literal>::value_type value_type;
	typedef map<string, Agent>::value_type value_type2;
	PropositionList::const_iterator it_prop;
	KAction* k_act;
	StaticLaw stat;
	Literals hd, bd;

	cout << "Building agent list..." << endl;
	StringList::iterator ita;
	i = 0;
	for (ita = m_reader->m_agents.begin(); ita != m_reader->m_agents.end(); ita++)
	{
		a_map.insert(value_type2(*ita, i));
		m_agents.insert(i++);
	}


	cout << "Building fluent literals" << endl;
	// build fluent literals;
	i = 0;
	for (itf = m_reader->m_fluents.begin();
		itf != m_reader->m_fluents.end(); itf++) {
		m_map.insert(value_type(*itf, i));
		m_literals.insert(i++);
        if(debug) {
            cout << "Print literal " << *itf << " is " << (i - 1) << endl;
        }
		m_map.insert(value_type(NEGATION_SYMBOL + *itf, i));
		m_literals.insert(i++);

	}

	// build action list
	cout << "Building action list" << endl;
	for (it_prop = m_reader->m_propositions.begin();
		it_prop != m_reader->m_propositions.end(); it_prop++) {
		hd = ground((*it_prop).get_effect());
		bd = ground((*it_prop).get_precondition());

		BFNode b;
		switch ((*it_prop).n_type) {
		case STATIC: //tiep: check this if still valid or not
			stat.set_head(hd);
			stat.set_body(bd);
			m_statics.push_back(stat);
			break;
		case DYNAMIC:
			k_act = add_ontic((*it_prop).act, DYNAMIC);
			b = (*it_prop).bel_form;
			update_node(&b);
			k_act->add_effect(ground(&(it_prop->effect)), b);
			break;
		case EXECUTABILITY:
			k_act = add_ontic((*it_prop).act, DYNAMIC);
			b = (*it_prop).bel_form;
			update_node(&b);
			k_act->add_execs(b);
			break;


			//    case IMPOSSIBILITY:
			//      //cout << "Adding action "<< (*it_prop).act_name << " to ontic (Impossibility)" << endl;
			//      //p_act = add_ontic((*it_prop).act_name);		//Ben: add_action -> add_ontic
			//      //p_act->add_imposs(bd);
			//      break;


		case DETERMINATION:
			k_act = add_ontic((*it_prop).act, DETERMINATION);
			k_act->add_sensedFluents(ground(&(it_prop->effect)));
			break;
		case ANNOUNCEMENT:					//Ben
			k_act = add_ontic((*it_prop).act, ANNOUNCEMENT);
			k_act->add_ann_ff(*convert(it_prop->flu_form));
			//k_act->add_agent(-1);
			break;
		case OBSERVANCE:
			k_act = add_ontic((*it_prop).act, IMPOSSIBILITY);
			k_act->add_observes(a_map.find((*it_prop).agentPro)->second, *convert((*it_prop).flu_form));
			//k_act->add_cond(convert((*it_prop).flu_form));
			break;
		case AWARENESS:
			k_act = add_ontic((*it_prop).act, IMPOSSIBILITY);
			k_act->add_awares(a_map.find((*it_prop).agentPro)->second, *convert((*it_prop).flu_form));
			break;
		default:
			break;
		}
	}
	cout << "Building InitRequire class" << endl;
	Nodes::const_iterator itn;
	ini = InitRequire();
	FluentFormula* phi1 = new FluentFormula;
	FluentFormula* phi2 = new FluentFormula;
	bool firstCheckFor12 = true;
	bool firstCheckFor13 = true;

	for (itn = m_reader->k_init.begin();itn != m_reader->k_init.end();itn++)
	{
		if (itn->node_type == fluForm) //initially phi
		{
			if ((itn->flu_form).size() == 0) {
				cout << "-------ERROR: INITIAL DECLARATION INCONSISTENT------- " << endl;
				exit(1);
			}
			if (firstCheckFor12) {
				phi1 = convert(itn->flu_form);
				firstCheckFor12 = false;
			}
			else {
				phi1 = and2ff(phi1, convert(itn->flu_form));
			}
			continue;
		}


		if (itn->node_type == CForm && itn->bfnode1->node_type == fluForm) //initially C(phi)
		{
			if (firstCheckFor13) {
				phi2 = convert(itn->bfnode1->flu_form);
				firstCheckFor13 = false;
			}
			else {
				phi2 = and2ff(phi2, convert(itn->bfnode1->flu_form));
			}
			continue;
		}
		/* Deleted because new version of initial state
		if(itn->node_type == CForm && itn->bfnode1->node_type == BForm) //initially C(B(i,phi))
		{
		if(a_map.find(itn->bfnode1->agentPro) == a_map.end())
		{
		cout << "-------ERROR: AGENT NOT DECLARED------- " << endl;
		exit(1);
		}

		Agent ag = a_map.find(itn->bfnode1->agentPro)->second;
		FluentFormula ff = *(convert(itn->bfnode1->bfnode1->flu_form));
		ini.add_correct(ag,ff);
		continue;
		}*/

		if (itn->node_type == CForm && itn->bfnode1->node_type == BForm) //initially C(B(i,phi))
		{
			if (a_map.find(itn->bfnode1->agentPro) == a_map.end())
			{
				cout << "-------ERROR: AGENT NOT DECLARED------- " << endl;
				exit(1);
			}

			//Tiep Le: comment below for combine C(B(i,phi)) with C(phi) since they should be considered the same
			//            Agent ag = a_map.find(itn->bfnode1->agentPro)->second;
			//            FluentFormula ff = *(convert(itn->bfnode1->bfnode1->flu_form));
			//            ini.add_correct(ag,ff);
			//            continue;
			//Tiep Le: fix for combine C(B(i,phi)) with C(phi) since they should be considered the same
			if (firstCheckFor13) {
				phi2 = convert(itn->bfnode1->bfnode1->flu_form);
				firstCheckFor13 = false;
			}
			else {
				phi2 = and2ff(phi2, convert(itn->bfnode1->bfnode1->flu_form));
			}
			continue;
		}
		if (itn->node_type == CForm && itn->bfnode1->node_type == propForm
			&& itn->bfnode1->bfOperator == BFOR) //initially C(B(i,phi) \/ B(i,-phi))
		{
			if (a_map.find(itn->bfnode1->bfnode1->agentPro) == a_map.end())
			{
				cout << "-------ERROR: AGENT NOT DECLARED------- " << endl;
				exit(1);
			}

			Agent ag = a_map.find(itn->bfnode1->bfnode1->agentPro)->second;
			FluentFormula ff = *(convert(itn->bfnode1->bfnode1->bfnode1->flu_form));
			ini.add_same(ag, ff);
			continue;
		}
	}
	ini.pointed = *phi1;
	ini.musthave = *phi2;
	return true;
}

void Planner::print_read_init()
{
	FluentFormula pointed = ini.pointed;
	FluentFormula musthave = ini.musthave;
	Agent_FF_list correctintepret = ini.correctintepret;
	Agent_FF_list sameintepret = ini.sameintepret;
	cout << "----------print_read_init--------------" << endl;
	cout << "printing pointed" << endl;
	print_fluform(pointed);
	cout << "print musthave" << endl;
	print_fluform(musthave);
	Agent_FF_list::iterator it;
	cout << "print correct" << endl;
	for (it = correctintepret.begin(); it != correctintepret.end();it++)
	{
		Agent ag = it->get_head();
		FluentFormula ff = it->get_body();
		cout << ag << " ---> ";
		print_fluform(ff);
	}
	cout << "print same" << endl;
	for (it = sameintepret.begin(); it != sameintepret.end();it++)
	{
		Agent ag = it->get_head();
		FluentFormula ff = it->get_body();
		cout << ag << " ---> ";
		print_fluform(ff);
	}
}
bool Planner::build_goal()
{
	Nodes::iterator it;
	Literals st;

	// build the goal
    //int n = 0;
    for (it = (m_reader->m_gd).begin(); it != (m_reader->m_gd).end(); it++) {
        //cout << "goal " << n++ << endl;
		update_node(&(*it));
		m_gd.push_back(*it);
	}
	return true;
}

bool Planner::build_init_k()
{
	m_init_cstate = Kripke();
	Literals::iterator it_lits;
	States::iterator it_st;
	States::iterator it_st2;
	AgentSet::iterator it_ag;

	/*********** States Initialization **************/
    
    States* usedSl = new States;
    vector<int> pId = vector<int>();
    
    //for special case when every agent knows about truth value of every fluents
    if(ini.musthave.size() == 1 && m_literals.size()/2 == ini.musthave.begin()->size()){
        FluentFormula::iterator it1 = ini.musthave.begin();;
        Kstate* s;
        s = new Kstate();
        Literals* lits = new Literals;
        lits->insert(it1->begin(), it1->end());
        s->set_lits(*lits);
        s->set_id(0);
        usedSl->push_back(*s);
        pId.push_back(0);
        m_init_cstate.set_stateslist(*usedSl);
        m_init_cstate.set_pId(*pId.begin());
    }
    //for general case but number of fluents should be < 16; otherwise variable of type int will overflow
    else if(m_literals.size() < 32){ //for general case
        int litSize = m_literals.size() / 2;
        int stateSize = (int)pow(2.0, litSize);
        States* sl = new States;
        Kstate* s;
        for (int i = 0; i < stateSize; i++) { //states id from 0
            s = new Kstate();
            s->set_id(i);
            sl->push_back(*s);
        }
        
        int ids = 0;
        
        for (it_st = sl->begin(); it_st != sl->end(); it_st++)
        {
            Kstate* temp = &(*it_st);
            computeLiteral(temp);
            if (ini.musthave.size() == 0 || temp->entail_fluform(ini.musthave)) { //add only states that satisfies C(phi)
                temp->set_id(ids);
                usedSl->push_back(*temp);
                ids++;
            }
            if (temp->entail_fluform(ini.pointed)) //set pointed states if this satisfies phi
            {
                pId.push_back(temp->get_id());
            }
        }
        delete sl;
        m_init_cstate.set_stateslist(*usedSl);
        m_init_cstate.set_pId(*pId.begin());
    }
    //for case where number of fluents is >= 16. Cannot build initial state in this case. Return false;
    else{
        cout << "Critical: The program can solve only problems whose the number of fluents is less than 16" << endl;
        return false;
    }
	
	int edgeId = 0;
    
	/*********** Edges Initialization **************/

	for (it_st = usedSl->begin(); it_st != usedSl->end(); it_st++)
	{
		for (it_st2 = usedSl->begin(); it_st2 != usedSl->end(); it_st2++)
		{
			for (it_ag = m_agents.begin(); it_ag != m_agents.end(); it_ag++)
			{
				Agent_FF_list::iterator itaf;
				bool shouldadd = true;
				//Tiep Le: comment below since we do not check for initially C(B(i,phi)) anymore. It should be considered in C(phi).
				//                    Agent_FF_list tempcorrect = ini.correctintepret;
				//                    for(itaf = tempcorrect.begin(); itaf != tempcorrect.end(); itaf++)
				//                    {
				//                        int checkingag = itaf->get_head();
				//                        if(checkingag != *it_ag){
				//                            continue;
				//                        }
				//                        FluentFormula checkingff = itaf->get_body();
				//                        if(!it_st->entail_fluform(checkingff) || !it_st2->entail_fluform(checkingff))
				//                        {
				//                            shouldadd = false;
				//                            break;
				//                        }
				//                    }
				//                    if(!shouldadd){
				//                        shouldadd = true;
				//                        continue;
				//                    }
				for (itaf = ini.sameintepret.begin(); itaf != ini.sameintepret.end(); itaf++)
				{
					int checkingag = itaf->get_head();
					if (checkingag != *it_ag) {
						continue;
					}
					FluentFormula checkingff = itaf->get_body();
					if (!isSameSatisfyFF(*it_st, *it_st2, checkingff))
					{
						shouldadd = false;
						break;
					}
				}
				if (!shouldadd) {
					shouldadd = true;
					continue;
				}
				//add this edge
				Kedge* newEd = new Kedge();
				int fromnew = it_st->get_id();
				newEd->set_from(fromnew);
				int tonew = it_st2->get_id();
				newEd->set_to(tonew);
				newEd->set_id(edgeId);
				newEd->set_label(*it_ag);
				m_init_cstate.add_edge(*newEd);
                //m_init_cstate.add_edge2state(fromnew, edgeId, true); //do NOT need this any more because they are implemented within add_edge
                //m_init_cstate.add_edge2state(tonew, edgeId, false);
				edgeId++;
			}
		}
	}

	//generate a set of kripke for initial e-state
	m_init_cstates = KripkeList();
	vector<int>::iterator iterPId;
	for (iterPId = pId.begin(); iterPId != pId.end(); iterPId++) {
		m_init_cstate.set_pId(*iterPId);
		Kripke m = Kripke(&m_init_cstate);
		m.set_pId(*iterPId);
		m_init_cstates.push_back(m);
	}

	return true;
}


FluentFormula* Planner::and2ff(FluentFormula* flu1, FluentFormula* flu2)
{
	FluentFormula::iterator it1;
	FluentFormula::iterator it2;
	Fluents ns;
	FluentFormula* output = new FluentFormula;

	for (it2 = flu1->begin(); it2 != flu1->end(); it2++)
	{
		for (it1 = flu2->begin(); it1 != flu2->end(); it1++)
		{
			if (is_consistent(*it1, *it2))
			{
				ns = *it2;
				ns.insert(it1->begin(), it1->end());
				output->insert(ns);
			}
		}
	}

	return output;
}

bool Planner::is_consistent(Fluents f1, Fluents f2)
{
	Fluents::iterator it;
	Fluent nl;

	for (it = f2.begin(); it != f2.end(); it++)
	{
		if ((*it) % 2 == 0)
		{
			nl = (*it) + 1;
		}
		else
		{
			nl = (*it) - 1;
		}

		if (f1.find(nl) != f1.end())
			return false;
	}

	return true;
}


void Planner::computeLiteral(Kstate* s) {
	int at = m_literals.size() - 2;
	int div, rem;
	div = s->get_id();
	Literals* lits = new Literals;
	while (at >= 0) {
		rem = div % 2;
		lits->insert(at + rem);
		div = div / 2;
		at = at - 2;
	}
	s->set_lits(*lits);

}

FluentFormula* Planner::convert(StringList2 list)
{
	StringList2::iterator it4;
	StringList::iterator it5;
	unsigned short k;
	Fluents temp;
	FluentFormula* output = new FluentFormula;

	for (it4 = list.begin(); it4 != list.end(); it4++)
	{
		for (it5 = it4->begin(); it5 != it4->end(); it5++)
		{
			k = m_map.find(*it5)->second;
			temp.insert(k);
		}

		output->insert(temp);
		temp.clear();
	}
	return output;
}




// grounding functions
Literals Planner::ground(const StringList* x) const
{
	StringList::iterator it;
	Literals y;

	for (it = x->begin(); it != x->end(); it++) {
		y.insert(ground(*it));
	}

	return y;
}

Literal Planner::ground(const string& x) const
{
	map<string, Literal>::const_iterator p = m_map.find(x);

	if (p != m_map.end()) {
		return (p->second);
	}

	cout << "ERROR: Literal " << x << " is undeclared." << endl;
	cout << "Check the fluent declaration." << endl << endl;

	exit(1);
}

StringList Planner::convert(const Literals& x) const
{
	StringList y;
	Literals::iterator it;

	for (it = x.begin(); it != x.end(); it++) {
		y.insert(convert(*it));
	}

	return y;
}

string Planner::convert(const Literal& x) const
{
	unsigned short int i = x / 2;
	StringList::iterator it;

	if (i >= m_reader->m_fluents.size())
		return NULL;

	it = m_reader->m_fluents.begin();

	for (i = 0; i < x / 2; i++) {
		it++;
	}

	if (x % 2 == 0)
		return *it;

	return (NEGATION_SYMBOL + (*it));
}

Agents Planner::convert(StringList list)
{
	Agents output;
	StringList::iterator it;

	for (it = list.begin(); it != list.end(); it++)
	{
		output.push_back(a_map.find(*it)->second);
	}

	return output;
}


/* printing functions */
void Planner::print(const Literal& l) const
{
	cout << convert(l);
}

void Planner::print(const Literals& x) const
{
	Literals::iterator it;
	bool comma = false;
	for (it = x.begin(); it != x.end(); it++) {
		if (comma)
			cout << ",";
		print(*it);
		comma = true;
	}
}

void Planner::print_interal_domain()
{
	KActionList::iterator it;


	StaticLaws::const_iterator its;

	cout << "Static Laws: " << endl;

	for (its = m_statics.begin(); its != m_statics.end(); ++its) {
		cout << "    ";
		print(*its->get_head());
		cout << " <- ";
		print(*its->get_body());
		cout << endl;
	}

	cout << "Ontic (normal) Actions" << endl;
	cout << "-------------------" << ontic.size() << "---------" << endl;
	for (it = ontic.begin();it != ontic.end(); it++) {
		if (it->get_type() == DYNAMIC) {
			//print executable
			Nodes::const_iterator itn;
			cout << it->get_name() << " " << it->get_type() << endl;
			for (itn = it->get_execs()->begin(); itn != it->get_execs()->end();itn++) {
				cout << it->get_name() << " is executable if ";
				itn->print(m_map, a_map);
				cout << endl;
			}
			//print action causes .... if ....
			Fluents_BF_list::const_iterator itfb;
			for (itfb = (*it->get_effects()).begin(); itfb != (*it->get_effects()).end(); itfb++) {
				cout << it->get_name() << " has effect ";
				print_literals(*itfb->get_head());
				cout << ", and has belief formula ";
				itfb->get_body()->print(m_map, a_map);
				//          cout << "agent is ";
				//          print_literals(it->get_agents());
				cout << endl;
			}
		}
		else if (it->get_type() == DETERMINATION) {
			cout << it->get_name() << " " << it->get_type() << " has sensed fluents ";
			Literals::iterator it1;
			Literals temp = it->get_sensedFluents();
			for (it1 = temp.begin(); it1 != temp.end(); it1++) {
				cout << *it1 << " ";
			}
		}
		else if (it->get_type() == ANNOUNCEMENT) {
			cout << it->get_name() << " " << it->get_type() << " has announce fluents ";
			print_fluform(*(it->get_ann_ff().begin()));
		}

		cout << endl;
	}





	//  // print init cstate
	//  cout <<  "INIT" << endl;
	//  cout <<  "----------------------------" << endl;
	//  m_init_cstate.print();
	//  cout << endl;
	//
	//  // print goal state
	//  cout <<  "GOAL " << endl;
	//  cout <<  "----------------------------" << endl;
	//  print(m_goal);
	//  cout << endl;

	// print statistics
	StringList2::const_iterator itst;
	// unsigned int i = 0;

	cout << "STATISTICS " << endl;
	cout << "----------------------------" << endl;
	cout << "Total actions: " << sensing.size() + ann.size() + ontic.size() << endl;
	cout << "\tOntic actions: " << ontic.size() << endl;
	//cout << "\tSensing actions: " << sensing.size() << endl;
	//cout << "\tAnnouncement actions: " << ann.size() << endl;
	cout << "Total fluents: " << m_literals.size() / 2 << endl;
	cout << "Unknown fluents: " << endl;
	// for (itst = m_reader->m_init.begin();
	// 	itst != m_reader->m_init.end(); itst++) {
	// 	cout << "\tState " << i++ << ": ";
	// 	cout << m_literals.size() / 2 - (*itst).size();
	// 	cout << endl;
	// }
	cout << endl;
}


//void Planner::print_summary() const
//{
//  cout <<  "SUMMARY" << endl;
//  cout <<  "---------------------" << endl;
//  cout <<  "Number of fluents: " << m_literals.size()/2 << endl;
//  cout <<  "Number of actions: " << m_actions.size() << endl;
//  cout <<  "Number of static laws: " << m_statics.size() << endl;
//}

void Planner::print_statistics() const
{
	double total = m_timer->time(READ_TIMER) +
		m_timer->time(BUILD_TIMER) + m_timer->time(SEARCH_TIMER);

	cout << "STATISTICS" << endl;
	cout << "---------------------" << endl;
	printf("Total time: %.3f (sec) \n", total);
#ifdef PRINT_TIME_DETAILS
	printf("  Reading: %.3f (sec) [%.2f %%]\n",
		m_timer->time(READ_TIMER),
		100.0 * m_timer->time(READ_TIMER) / total);
	printf("  Preprocessing: %.3f (sec) [%.2f %%]\n",
		m_timer->time(BUILD_TIMER),
		100.0 * m_timer->time(BUILD_TIMER) / total);
	printf("  Search: %.3f (sec) [%.2f %%]\n",
		m_timer->time(SEARCH_TIMER),
		100.0 * m_timer->time(SEARCH_TIMER) / total);
#endif
	//
	//  printf("Total states allocated: %d\n", m_states.size());
	//  printf("Total cstate(s): %d\n", m_cstates.size());
	//  printf("Total cstate(s) remaining in the queue: %d\n", m_queue.size());
}

KAction* Planner::add_ontic(const MyAction act, PropositionType pt)
{
	map<string, int>::iterator it;
	string str = act.get_name();
	KAction a(this, act, pt);

	it = m_action_map.find(str);

	if (it == m_action_map.end()) {
		// create a new entry
		ontic.push_back(a);
		m_action_map.insert(map<string, int>::value_type(str, ontic.size() - 1));
		return &(ontic[ontic.size() - 1]);
	}
	if (pt != IMPOSSIBILITY) {
		ontic[it->second].set_type(pt);
	}

	return &(ontic[it->second]);
}

Literals Planner::negate(const Literals& x) const
{
	Literals::const_iterator it;
	Literals y; // output literals
	Literal l;

	for (it = x.begin(); it != x.end(); ++it) {
		NEGATE(*it, l);
		y.insert(l);
	}
	return y;
}

void Planner::print_fluform(StringList2 in) {
	StringList2::const_iterator it1;
	StringList::const_iterator it2;
	cout << "[";
	for (it1 = in.begin(); it1 != in.end();it1++) {
		cout << "[";
		for (it2 = it1->begin(); it2 != it1->end(); it2++) {
			cout << *it2 << " ";
		}
		cout << "]";
	}
	cout << "]" << endl;
}

void Planner::print_fluform(FluentFormula in) {
	FluentFormula::iterator it1;
	Fluents::iterator it2;
	cout << "[";
	for (it1 = in.begin(); it1 != in.end();it1++) {
		cout << "[";
		for (it2 = it1->begin(); it2 != it1->end(); it2++) {
			cout << *it2 << " ";
		}
		cout << "]";
	}
	cout << "]" << endl;
}


void Planner::print_literals(const Literals& ls) const
{
	Literals::iterator it;
	bool begin = true;
	for (it = ls.begin();it != ls.end();it++) {
		if (begin) {
			cout << *it;
			begin = false;
		}
		else {
			cout << "," << *it;
		}
	}
	//cout << endl;
}
void Planner::print_ontic(KAction* k) {

}

bool Planner::check_Agents(Agent ag, Agents list)
{
	Agents::iterator it;

	for (it = list.begin(); it != list.end(); it++)
		if (ag == *it)
			return true;

	return false;
}

bool Planner::is_executable(Kripke k, KAction a) const
{
	Nodes::const_iterator it;
	for (it = a.get_execs()->begin(); it != a.get_execs()->end(); it++)
	{
        
		if (!k.entail(*it))
		{
			return false;
		}
	}
	return true;
}

bool Planner::is_executable1(Kripke k, KAction a) const
{
	Nodes::const_iterator it;
	for (it = a.get_execs()->begin(); it != a.get_execs()->end(); it++)
	{
		if (!k.entail1(*it))
		{
			return false;
		}
	}
	return true;
}


//%%%%%%%%%%%---------- Execute announcement action-----------------%%%%%%%%%%%%%
Kripke* Planner::execute_ann(Kripke k, KAction a)
{
	Planner p = a.get_planner();
	// int oldpId = k.get_id();
	bool not_truthful = false;
	bool lying = false, misleading = false , truthful = false;

	//check executable
	if (!is_executable(k, a)) {
		return NULL;
	}
	//check special excutable condition for announce action.
	if (!k.entail_ff_list(a.get_ann_ff()))
	{	
		// cout << "Lying" << endl;
		not_truthful = true;
		// return NULL;
	}

	AgentSet fd = get_fd(a, k);
	AgentSet od = get_od(a, k);
	AgentSet pd = get_pd(a, k);

	// cout << "Action name: " << a.get_name() << endl;
	AgentSet executors = get_executor(a);
	Agents executors_vector(executors.begin(), executors.end());
	AgentSet::iterator itera;
	Agents::iterator itagstr;


	list<FluentFormula> annFlu = a.get_ann_ff();
	list<FluentFormula>::iterator itTemp;

	for(itera = executors.begin();itera != executors.end(); itera++)
	{
		for (itTemp = annFlu.begin(); itTemp != annFlu.end(); itTemp++){
			BFNode* tempVarPhiNode = new BFNode();
			BFNode* tempNegVarPhiNode = new BFNode();
			tempVarPhiNode->node_type = fluForm;
			tempNegVarPhiNode->node_type = fluForm;

			FluentFormula* ff = new FluentFormula();

			FluentFormula::iterator ffTemp;
			for (ffTemp = itTemp->begin(); ffTemp != itTemp->end(); ffTemp++){
				Fluents::iterator flTemp;
				for (flTemp = ffTemp->begin(); flTemp != ffTemp->end(); flTemp++){
					Fluent negTemp;
					NEGATE(*flTemp, negTemp);

					Fluents tmp;
					tmp.insert(negTemp);

					ff->insert(tmp);

				}
			}
			tempVarPhiNode->set_flu(&(*itTemp));

			tempNegVarPhiNode->set_flu(ff);

			BFNode* BVarphi = new BFNode();
			BFNode* BNegVarphi = new BFNode();
			BVarphi->node_type = BForm;
			BNegVarphi->node_type = BForm;

			BVarphi->agentPro2 = *itera;
			BNegVarphi->agentPro2 = *itera;

			BVarphi->bfnode1 = tempVarPhiNode;
			BNegVarphi->bfnode1 = tempNegVarPhiNode;

			if (k.entail(*BVarphi)){
				truthful = true;
			} else if (k.entail(*BNegVarphi)){
				lying = true;
			} else {
				misleading = true;
			}
			break;
		}
		// executors_vector.insert(&(*itera));
		// cout << "Executor: " << *itera << endl;;
	}

	
	UpdateModel* upmodel = new UpdateModel();

	// Nodes* varPhi = new Nodes();
	// Nodes* negVarPhi = new Nodes();


	// for (itTemp = annFlu.begin(); itTemp != annFlu.end(); itTemp++) {
	// 	BFNode* tempVarPhiNode = new BFNode();
	// 	BFNode* tempNegVarPhiNode = new BFNode();
	// 	tempVarPhiNode->node_type = fluForm;
	// 	tempNegVarPhiNode->node_type = fluForm;

	// 	FluentFormula* ff = new FluentFormula();
	// 	StringList strLNegVarPhi;
	// 	StringList strLVarPhi;
	// 	FluentFormula::iterator ffTemp;
	// 	for (ffTemp = itTemp->begin(); ffTemp != itTemp->end(); ffTemp++){
	// 		Fluents::iterator flTemp;
	// 		for (flTemp = ffTemp->begin(); flTemp != ffTemp->end(); flTemp++){
	// 			Fluent negTemp;
	// 			NEGATE(*flTemp, negTemp);

	// 			Fluents tmp;
	// 			tmp.insert(negTemp);

	// 			ff->insert(tmp);

	// 			string toStringNegVarPhi = to_string(negTemp);
	// 			strLNegVarPhi.insert(toStringNegVarPhi);

	// 			string toStringVarPhi = to_string(*flTemp);
	// 			strLVarPhi.insert(toStringVarPhi);
	// 		}
	// 	}
	// 	tempNegVarPhiNode->set_flu(&(*itTemp));
	// 	tempNegVarPhiNode->flu_form.insert(strLVarPhi);

	// 	tempNegVarPhiNode->set_flu(ff);
	// 	tempNegVarPhiNode->flu_form.insert(strLNegVarPhi);

	// 	varPhi->push_back(*tempVarPhiNode);
	// 	negVarPhi->push_back(*tempNegVarPhiNode);
	// }


	if (lying){//-----------------------------------------LYING----------------------------------------
		//Create events
		Event* sigma = new Event();
		sigma->set_id(0);
		sigma->set_name("Sigma");

		Event* zeta = new Event();
		zeta->set_id(1);
		zeta->set_name("Zeta");

		Event* tau = new Event();
		tau->set_id(2);
		tau->set_name("Tau");


		Event* chi = new Event();
		chi->set_id(3);
		chi->set_name("Chi");

		Event* mu = new Event();
		mu->set_id(4);
		mu->set_name("Mu");

		Event* epsilon = new Event();
		epsilon->set_id(5);
		epsilon->set_name("Epsilon");


		//Precondition and subsitution for Sigma
		Fluents_BF_list pre_sub_sigma = Fluents_BF_list();
		//Transform into FluentsFormula single fluent
		for (itTemp = annFlu.begin(); itTemp != annFlu.end(); itTemp++) {
			Fluents_BF addSigma = Fluents_BF();
			BFNode* addNodeSigma = new BFNode();
			addNodeSigma->node_type = EForm;
			addNodeSigma->agents = executors_vector;
			for (itagstr = executors_vector.begin(); itagstr != executors_vector.end(); itagstr++){
				for (auto itmap = p.a_map.begin(); itmap != p.a_map.end(); itmap++){
					if (itmap->second == *itagstr){
						addNodeSigma->groupAgent.insert(itmap->first);
						break;
					}
				}
			}

			BFNode* tempNode = new BFNode();
			tempNode->node_type = fluForm;

			FluentFormula* ff = new FluentFormula();
			StringList toStrL;
			FluentFormula::iterator ffTemp;
			for (ffTemp = itTemp->begin(); ffTemp != itTemp->end(); ffTemp++){
				Fluents::iterator flTemp;
				for (flTemp = ffTemp->begin(); flTemp != ffTemp->end(); flTemp++){
					Fluent negTemp;
					NEGATE(*flTemp, negTemp);

					Fluents tmp;
					tmp.insert(negTemp);

					ff->insert(tmp);

					string toString = to_string(negTemp);
					toStrL.insert(toString);
				}
			}
			tempNode->set_flu(ff);
			tempNode->flu_form.insert(toStrL);

			addNodeSigma->bfnode1 = tempNode;

			addSigma.set_bn_body(*addNodeSigma);

			pre_sub_sigma.push_back(addSigma);
		}

		sigma->set_pre_sub(pre_sub_sigma);

		//Precondition and subsitution for Zeta/Chi
		Fluents_BF_list pre_sub_zeta_chi = Fluents_BF_list();
		for (itTemp = annFlu.begin(); itTemp != annFlu.end(); itTemp++) {
			Fluents_BF addZetaChi = Fluents_BF();
			BFNode* addNodeZetaChi = new BFNode();
			addNodeZetaChi->node_type = fluForm;

			FluentFormula* ff = new FluentFormula();
			StringList toStrL;
			FluentFormula::iterator ffTemp;
			for (ffTemp = itTemp->begin(); ffTemp != itTemp->end(); ffTemp++){
				Fluents::iterator flTemp;
				for (flTemp = ffTemp->begin(); flTemp != ffTemp->end(); flTemp++){
					Fluent negTemp;
					NEGATE(*flTemp, negTemp);

					Fluents tmp;
					tmp.insert(negTemp);

					ff->insert(tmp);

					string toString = to_string(negTemp);
					toStrL.insert(toString);
				}
			}
			addNodeZetaChi->set_flu(ff);
			addNodeZetaChi->flu_form.insert(toStrL);

			addZetaChi.set_bn_body(*addNodeZetaChi);

			pre_sub_zeta_chi.push_back(addZetaChi);
		}
		zeta->set_pre_sub(pre_sub_zeta_chi);
		chi->set_pre_sub(pre_sub_zeta_chi);

		//Precondition and subsitution for Tau/Mu
		Fluents_BF_list pre_sub_tau_mu = Fluents_BF_list();
		for (itTemp = annFlu.begin(); itTemp != annFlu.end(); itTemp++) {
			Fluents_BF addTauMu = Fluents_BF();
			BFNode* addNodeTauMu = new BFNode();
			addNodeTauMu->node_type = fluForm;		

			addNodeTauMu->set_flu(&(*itTemp));

			StringList toStrL;
			FluentFormula::iterator ffTemp;
			for (ffTemp = itTemp->begin(); ffTemp != itTemp->end(); ffTemp++){
				Fluents::iterator flTemp;
				for (flTemp = ffTemp->begin(); flTemp != ffTemp->end(); flTemp++){
					string toString = to_string(*flTemp);
					toStrL.insert(toString);
				}
			}
			addNodeTauMu->flu_form.insert(toStrL);

			addTauMu.set_bn_body(*addNodeTauMu);

			pre_sub_tau_mu.push_back(addTauMu);
		}
		tau->set_pre_sub(pre_sub_tau_mu);
		mu->set_pre_sub(pre_sub_tau_mu);

		//Create edges
		Edges edlist;

		AgentSet::iterator itag;
		int edgeID = 0;

		//Edges for full observers
		for (itag = fd.begin(); itag != fd.end(); itag++){
			//Edge from Sigma to Sigma
			Kedge* sigmaToSigma = new Kedge();
			sigmaToSigma->set_id(edgeID);
			sigmaToSigma->set_from(0);
			sigmaToSigma->set_to(0);
			sigmaToSigma->set_label(*itag);
			sigmaToSigma->set_exist(true);
			sigmaToSigma->set_inUpdateModel(true);

			if (executors.find(*itag) == executors.end()){
				Nodes* edge_con_sigma_sigma = new Nodes();

				for (itTemp = annFlu.begin(); itTemp != annFlu.end(); itTemp++) {
					BFNode* agentNode = new BFNode();
					agentNode->node_type = BForm;
					agentNode->agentPro2 = *itag;
					for (auto itmap = p.a_map.begin(); itmap != p.a_map.end(); itmap++){
						if (itmap->second == *itag){
							agentNode->agentPro = itmap->first;
							break;
						}
					}
					

					BFNode* alphaNode = new BFNode();
					alphaNode->agents = executors_vector;
					alphaNode->node_type = EForm;
					for (itagstr = executors_vector.begin(); itagstr != executors_vector.end(); itagstr++){
						for (auto itmap = p.a_map.begin(); itmap != p.a_map.end(); itmap++){
							if (itmap->second == *itagstr){
								alphaNode->groupAgent.insert(itmap->first);
								break;
							}
						}
					}

					BFNode* tempNode = new BFNode();
					tempNode->node_type = fluForm;

					FluentFormula* ff = new FluentFormula();
					StringList toStrL;
					FluentFormula::iterator ffTemp;
					for (ffTemp = itTemp->begin(); ffTemp != itTemp->end(); ffTemp++){
						Fluents::iterator flTemp;
						for (flTemp = ffTemp->begin(); flTemp != ffTemp->end(); flTemp++){
							Fluent negTemp;
							NEGATE(*flTemp, negTemp);

							Fluents tmp;
							tmp.insert(negTemp);

							ff->insert(tmp);

							string toString = to_string(negTemp);
							toStrL.insert(toString);
						}
					}
					tempNode->set_flu(ff);
					tempNode->flu_form.insert(toStrL);

					alphaNode->bfnode1 = tempNode;
					agentNode->bfnode1 = alphaNode;

					edge_con_sigma_sigma->push_back(*agentNode);
				}

				sigmaToSigma->set_edge_condition(edge_con_sigma_sigma);
			}

			sigma->add_out(edgeID);
			sigma->add_in(edgeID);

			edlist.push_back(sigmaToSigma);
			edgeID++;


			//Edge from Sigma to Zeta
			Kedge* sigmaToZeta = new Kedge();
			sigmaToZeta->set_id(edgeID);
			sigmaToZeta->set_from(0);
			sigmaToZeta->set_to(1);
			sigmaToZeta->set_label(*itag);
			sigmaToZeta->set_exist(true);
			sigmaToZeta->set_inUpdateModel(true);
			Nodes* edge_con_sigma_zeta = new Nodes();

			for (itTemp = annFlu.begin(); itTemp != annFlu.end(); itTemp++) {
				BFNode* agentNode = new BFNode();
				agentNode->node_type = propForm;
				agentNode->bfOperator = BFOR;

				BFNode* agentBFNode1 = new BFNode();
				agentBFNode1->node_type = BForm;
				agentBFNode1->agentPro2 = *itag;
				for (auto itmap = p.a_map.begin(); itmap != p.a_map.end(); itmap++){
					if (itmap->second == *itag){
						agentBFNode1->agentPro = itmap->first;
						break;
					}
				}

				BFNode* alphaNode = new BFNode();
				alphaNode->agents = executors_vector;
				alphaNode->node_type = EForm;
				for (itagstr = executors_vector.begin(); itagstr != executors_vector.end(); itagstr++){
					for (auto itmap = p.a_map.begin(); itmap != p.a_map.end(); itmap++){
						if (itmap->second == *itagstr){
							alphaNode->groupAgent.insert(itmap->first);
							break;
						}
					}
				}

				BFNode* tempNode = new BFNode();
				tempNode->node_type = fluForm;

				FluentFormula* ff = new FluentFormula();
				StringList toStrL;
				FluentFormula::iterator ffTemp;
				for (ffTemp = itTemp->begin(); ffTemp != itTemp->end(); ffTemp++){
					Fluents::iterator flTemp;
					for (flTemp = ffTemp->begin(); flTemp != ffTemp->end(); flTemp++){
						Fluent negTemp;
						NEGATE(*flTemp, negTemp);

						Fluents tmp;
						tmp.insert(negTemp);

						ff->insert(tmp);

						string toString = to_string(negTemp);
						toStrL.insert(toString);
					}
				}
				tempNode->set_flu(ff);
				tempNode->flu_form.insert(toStrL);

				alphaNode->bfnode1 = tempNode;
				agentBFNode1->bfnode1 = alphaNode;

				BFNode* agentBFNode2 = new BFNode();
				agentBFNode2->node_type = BForm;
				agentBFNode2->agentPro2 = *itag;
				agentBFNode2->bfnode1 = tempNode;
				for (auto itmap = p.a_map.begin(); itmap != p.a_map.end(); itmap++){
					if (itmap->second == *itag){
						agentBFNode2->agentPro = itmap->first;
						break;
					}
				}
				

				agentNode->bfnode1 = agentBFNode1;
				agentNode->bfnode2 = agentBFNode2;

				edge_con_sigma_zeta->push_back(*agentNode);
			}
			sigmaToZeta->set_edge_condition(edge_con_sigma_zeta);

			sigma->add_out(edgeID);
			zeta->add_in(edgeID);

			edlist.push_back(sigmaToZeta);
			edgeID++;


			//Edge from Sigma to Tau
			Kedge* sigmaToTau = new Kedge();
			sigmaToTau->set_id(edgeID);
			sigmaToTau->set_from(0);
			sigmaToTau->set_to(2);
			sigmaToTau->set_label(*itag);
			sigmaToTau->set_exist(true);
			sigmaToTau->set_inUpdateModel(true);
			Nodes* edge_con_sigma_tau = new Nodes();
			
			for (itTemp = annFlu.begin(); itTemp != annFlu.end(); itTemp++) {
				BFNode* agentNode = new BFNode();
				agentNode->node_type = propForm;
				agentNode->bfOperator = BFNOT;

				BFNode* agentBFNode1 = new BFNode();
				agentBFNode1->node_type = BForm;
				agentBFNode1->agentPro2 = *itag;
				for (auto itmap = p.a_map.begin(); itmap != p.a_map.end(); itmap++){
					if (itmap->second == *itag){
						agentBFNode1->agentPro = itmap->first;
						break;
					}
				}

				BFNode* tempNode = new BFNode();
				tempNode->node_type = fluForm;

				FluentFormula* ff = new FluentFormula();
				StringList toStrL;
				FluentFormula::iterator ffTemp;
				for (ffTemp = itTemp->begin(); ffTemp != itTemp->end(); ffTemp++){
					Fluents::iterator flTemp;
					for (flTemp = ffTemp->begin(); flTemp != ffTemp->end(); flTemp++){
						Fluent negTemp;
						NEGATE(*flTemp, negTemp);

						Fluents tmp;
						tmp.insert(negTemp);

						ff->insert(tmp);

						string toString = to_string(negTemp);
						toStrL.insert(toString);
					}
				}
				tempNode->set_flu(ff);
				tempNode->flu_form.insert(toStrL);

				agentBFNode1->bfnode1 = tempNode;

				agentNode->bfnode1 = agentBFNode1;

				edge_con_sigma_tau->push_back(*agentNode);
			}
			sigmaToTau->set_edge_condition(edge_con_sigma_tau);

			sigma->add_out(edgeID);
			tau->add_in(edgeID);

			edlist.push_back(sigmaToTau);
			edgeID++;


			//Edge from Zeta to Zeta
			Kedge* zetaToZeta = new Kedge();
			zetaToZeta->set_id(edgeID);
			zetaToZeta->set_from(1);
			zetaToZeta->set_to(1);
			zetaToZeta->set_label(*itag);
			zetaToZeta->set_exist(true);
			zetaToZeta->set_inUpdateModel(true);
			zetaToZeta->set_edge_condition(edge_con_sigma_zeta);

			zeta->add_out(edgeID);
			zeta->add_in(edgeID);

			edlist.push_back(zetaToZeta);
			edgeID++;


			//Edge from Zeta to Tau
			Kedge* zetaToTau = new Kedge();
			zetaToTau->set_id(edgeID);
			zetaToTau->set_from(1);
			zetaToTau->set_to(2);
			zetaToTau->set_label(*itag);
			zetaToTau->set_exist(true);
			zetaToTau->set_inUpdateModel(true);
			zetaToTau->set_edge_condition(edge_con_sigma_tau);

			zeta->add_out(edgeID);
			tau->add_in(edgeID);

			edlist.push_back(zetaToTau);
			edgeID++;


			//Edge from Tau to Zeta
			Kedge* tauToZeta = new Kedge();
			tauToZeta->set_id(edgeID);
			tauToZeta->set_from(2);
			tauToZeta->set_to(1);
			tauToZeta->set_label(*itag);
			tauToZeta->set_exist(true);
			tauToZeta->set_inUpdateModel(true);
			tauToZeta->set_edge_condition(edge_con_sigma_zeta);

			tau->add_out(edgeID);
			zeta->add_in(edgeID);

			edlist.push_back(tauToZeta);
			edgeID++;


			//Edge from Tau to Tau
			Kedge* tauToTau = new Kedge();
			tauToTau->set_id(edgeID);
			tauToTau->set_from(2);
			tauToTau->set_to(2);
			tauToTau->set_label(*itag);
			tauToTau->set_exist(true);
			tauToTau->set_inUpdateModel(true);

			tau->add_out(edgeID);
			tau->add_in(edgeID);

			edlist.push_back(tauToTau);
			edgeID++;


			//Edge from Chi to Chi
			Kedge* chiToChi = new Kedge();
			chiToChi->set_id(edgeID);
			chiToChi->set_from(3);
			chiToChi->set_to(3);
			chiToChi->set_label(*itag);
			chiToChi->set_exist(true);
			chiToChi->set_inUpdateModel(true);

			chi->add_out(edgeID);
			chi->add_in(edgeID);

			edlist.push_back(chiToChi);
			edgeID++;


			//Edge from Mu to Mu
			Kedge* muToMu = new Kedge();
			muToMu->set_id(edgeID);
			muToMu->set_from(4);
			muToMu->set_to(4);
			muToMu->set_label(*itag);
			muToMu->set_exist(true);
			muToMu->set_inUpdateModel(true);

			mu->add_out(edgeID);
			mu->add_in(edgeID);

			edlist.push_back(muToMu);
			edgeID++;


			//Edge from Chi to Mu
			Kedge* chiToMu = new Kedge();
			chiToMu->set_id(edgeID);
			chiToMu->set_from(3);
			chiToMu->set_to(4);
			chiToMu->set_label(*itag);
			chiToMu->set_exist(true);
			chiToMu->set_inUpdateModel(true);
			Nodes* edge_con_chi_mu = new Nodes();

			for (itTemp = annFlu.begin(); itTemp != annFlu.end(); itTemp++) {
				BFNode* agentBFNode1 = new BFNode();
				agentBFNode1->node_type = BForm;
				agentBFNode1->agentPro2 = *itag;
				for (auto itmap = p.a_map.begin(); itmap != p.a_map.end(); itmap++){
					if (itmap->second == *itag){
						agentBFNode1->agentPro = itmap->first;
						break;
					}
				}

				BFNode* tempNode = new BFNode();
				tempNode->node_type = fluForm;

				tempNode->set_flu(&(*itTemp));

				StringList toStrL;
				FluentFormula::iterator ffTemp;
				for (ffTemp = itTemp->begin(); ffTemp != itTemp->end(); ffTemp++){
					Fluents::iterator flTemp;
					for (flTemp = ffTemp->begin(); flTemp != ffTemp->end(); flTemp++){
						string toString = to_string(*flTemp);
						toStrL.insert(toString);
					}
				}
				tempNode->flu_form.insert(toStrL);

				agentBFNode1->bfnode1 = tempNode;

				edge_con_chi_mu->push_back(*agentBFNode1);
			}
			chiToMu->set_edge_condition(edge_con_chi_mu);

			chi->add_out(edgeID);
			mu->add_in(edgeID);

			edlist.push_back(chiToMu);
			edgeID++;


			//Edge from Mu to Chi
			Kedge* muToChi = new Kedge();
			muToChi->set_id(edgeID);
			muToChi->set_from(4);
			muToChi->set_to(3);
			muToChi->set_label(*itag);
			muToChi->set_exist(true);
			muToChi->set_inUpdateModel(true);
			Nodes* edge_con_mu_chi = new Nodes();

			for (itTemp = annFlu.begin(); itTemp != annFlu.end(); itTemp++) {
				BFNode* agentBFNode1 = new BFNode();
				agentBFNode1->node_type = BForm;
				agentBFNode1->agentPro2 = *itag;
				for (auto itmap = p.a_map.begin(); itmap != p.a_map.end(); itmap++){
					if (itmap->second == *itag){
						agentBFNode1->agentPro = itmap->first;
						break;
					}
				}

				BFNode* tempNode = new BFNode();
				tempNode->node_type = fluForm;

				FluentFormula* ff = new FluentFormula();
				StringList toStrL;
				FluentFormula::iterator ffTemp;
				for (ffTemp = itTemp->begin(); ffTemp != itTemp->end(); ffTemp++){
					Fluents::iterator flTemp;
					for (flTemp = ffTemp->begin(); flTemp != ffTemp->end(); flTemp++){
						Fluent negTemp;
						NEGATE(*flTemp, negTemp);

						Fluents tmp;
						tmp.insert(negTemp);

						ff->insert(tmp);

						string toString = to_string(negTemp);
						toStrL.insert(toString);
					}
				}
				tempNode->set_flu(ff);
				tempNode->flu_form.insert(toStrL);

				agentBFNode1->bfnode1 = tempNode;

				edge_con_mu_chi->push_back(*agentBFNode1);
			}
			muToChi->set_edge_condition(edge_con_mu_chi);

			mu->add_out(edgeID);
			chi->add_in(edgeID);

			edlist.push_back(muToChi);
			edgeID++;


			//Edge from Epsilon to Epsilon
			Kedge* epsilonToEpsilon = new Kedge();
			epsilonToEpsilon->set_id(edgeID);
			epsilonToEpsilon->set_from(5);
			epsilonToEpsilon->set_to(5);
			epsilonToEpsilon->set_label(*itag);
			epsilonToEpsilon->set_exist(true);
			epsilonToEpsilon->set_inUpdateModel(true);

			epsilon->add_out(edgeID);
			epsilon->add_in(edgeID);

			edlist.push_back(epsilonToEpsilon);
			edgeID++;
		}

		//Edges for partial observers
		for (itag = pd.begin(); itag != pd.end(); itag++){
			//Edge from Sigma to Chi
			Kedge* sigmaToChi = new Kedge();
			sigmaToChi->set_id(edgeID);
			sigmaToChi->set_from(0);
			sigmaToChi->set_to(3);
			sigmaToChi->set_label(*itag);
			sigmaToChi->set_exist(true);
			sigmaToChi->set_inUpdateModel(true);

			sigma->add_out(edgeID);
			chi->add_in(edgeID);

			edlist.push_back(sigmaToChi);
			edgeID++;


			//Edge from Sigma to Mu
			Kedge* sigmaToMu = new Kedge();
			sigmaToMu->set_id(edgeID);
			sigmaToMu->set_from(0);
			sigmaToMu->set_to(4);
			sigmaToMu->set_label(*itag);
			sigmaToMu->set_exist(true);
			sigmaToMu->set_inUpdateModel(true);

			sigma->add_out(edgeID);
			mu->add_in(edgeID);

			edlist.push_back(sigmaToMu);
			edgeID++;


			//Edge from Zeta to Chi
			Kedge* zetaToChi = new Kedge();
			zetaToChi->set_id(edgeID);
			zetaToChi->set_from(1);
			zetaToChi->set_to(3);
			zetaToChi->set_label(*itag);
			zetaToChi->set_exist(true);
			zetaToChi->set_inUpdateModel(true);

			zeta->add_out(edgeID);
			chi->add_in(edgeID);

			edlist.push_back(zetaToChi);
			edgeID++;


			//Edge from Zeta to Mu
			Kedge* zetaToMu = new Kedge();
			zetaToMu->set_id(edgeID);
			zetaToMu->set_from(1);
			zetaToMu->set_to(4);
			zetaToMu->set_label(*itag);
			zetaToMu->set_exist(true);
			zetaToMu->set_inUpdateModel(true);

			zeta->add_out(edgeID);
			mu->add_in(edgeID);

			edlist.push_back(zetaToMu);
			edgeID++;


			//Edge from Tau to Chi
			Kedge* tauToChi = new Kedge();
			tauToChi->set_id(edgeID);
			tauToChi->set_from(2);
			tauToChi->set_to(3);
			tauToChi->set_label(*itag);
			tauToChi->set_exist(true);
			tauToChi->set_inUpdateModel(true);

			tau->add_out(edgeID);
			chi->add_in(edgeID);

			edlist.push_back(tauToChi);
			edgeID++;


			//Edge from Tau to Mu
			Kedge* tauToMu = new Kedge();
			tauToMu->set_id(edgeID);
			tauToMu->set_from(2);
			tauToMu->set_to(4);
			tauToMu->set_label(*itag);
			tauToMu->set_exist(true);
			tauToMu->set_inUpdateModel(true);

			tau->add_out(edgeID);
			mu->add_in(edgeID);

			edlist.push_back(tauToMu);
			edgeID++;


			//Edge from Chi to Chi
			Kedge* chiToChi = new Kedge();
			chiToChi->set_id(edgeID);
			chiToChi->set_from(3);
			chiToChi->set_to(3);
			chiToChi->set_label(*itag);
			chiToChi->set_exist(true);
			chiToChi->set_inUpdateModel(true);

			chi->add_out(edgeID);
			chi->add_in(edgeID);

			edlist.push_back(chiToChi);
			edgeID++;


			//Edge from Chi to Mu
			Kedge* chiToMu = new Kedge();
			chiToMu->set_id(edgeID);
			chiToMu->set_from(3);
			chiToMu->set_to(4);
			chiToMu->set_label(*itag);
			chiToMu->set_exist(true);
			chiToMu->set_inUpdateModel(true);

			chi->add_out(edgeID);
			mu->add_in(edgeID);

			edlist.push_back(chiToMu);
			edgeID++;


			//Edge from Mu to Chi
			Kedge* muToChi = new Kedge();
			muToChi->set_id(edgeID);
			muToChi->set_from(4);
			muToChi->set_to(3);
			muToChi->set_label(*itag);
			muToChi->set_exist(true);
			muToChi->set_inUpdateModel(true);

			mu->add_out(edgeID);
			chi->add_in(edgeID);

			edlist.push_back(muToChi);
			edgeID++;


			//Edge from Mu to Mu
			Kedge* muToMu = new Kedge();
			muToMu->set_id(edgeID);
			muToMu->set_from(4);
			muToMu->set_to(4);
			muToMu->set_label(*itag);
			muToMu->set_exist(true);
			muToMu->set_inUpdateModel(true);

			mu->add_out(edgeID);
			mu->add_in(edgeID);

			edlist.push_back(muToMu);
			edgeID++;


			//Edge from Epsilon to Epsilon
			Kedge* epsilonToEpsilon = new Kedge();
			epsilonToEpsilon->set_id(edgeID);
			epsilonToEpsilon->set_from(5);
			epsilonToEpsilon->set_to(5);
			epsilonToEpsilon->set_label(*itag);
			epsilonToEpsilon->set_exist(true);
			epsilonToEpsilon->set_inUpdateModel(true);

			epsilon->add_out(edgeID);
			epsilon->add_in(edgeID);

			edlist.push_back(epsilonToEpsilon);
			edgeID++;
		}

		//Edges for oblivous
		for (itag = od.begin(); itag != od.end(); itag++){
			//Edge from Sigma to Epsilon
			Kedge* sigmaToEpsilon = new Kedge();
			sigmaToEpsilon->set_id(edgeID);
			sigmaToEpsilon->set_from(0);
			sigmaToEpsilon->set_to(5);
			sigmaToEpsilon->set_label(*itag);
			sigmaToEpsilon->set_exist(true);
			sigmaToEpsilon->set_inUpdateModel(true);

			sigma->add_out(edgeID);
			epsilon->add_in(edgeID);

			edlist.push_back(sigmaToEpsilon);
			edgeID++;


			//Edge from Zeta to Epsilon
			Kedge* zetaToEpsilon = new Kedge();
			zetaToEpsilon->set_id(edgeID);
			zetaToEpsilon->set_from(1);
			zetaToEpsilon->set_to(5);
			zetaToEpsilon->set_label(*itag);
			zetaToEpsilon->set_exist(true);
			zetaToEpsilon->set_inUpdateModel(true);

			zeta->add_out(edgeID);
			epsilon->add_in(edgeID);

			edlist.push_back(zetaToEpsilon);
			edgeID++;


			//Edge from Tau to Epsilon
			Kedge* tauToEpsilon = new Kedge();
			tauToEpsilon->set_id(edgeID);
			tauToEpsilon->set_from(2);
			tauToEpsilon->set_to(5);
			tauToEpsilon->set_label(*itag);
			tauToEpsilon->set_exist(true);
			tauToEpsilon->set_inUpdateModel(true);

			tau->add_out(edgeID);
			epsilon->add_in(edgeID);

			edlist.push_back(tauToEpsilon);
			edgeID++;


			//Edge from Chi to Epsilon
			Kedge* chiToEpsilon = new Kedge();
			chiToEpsilon->set_id(edgeID);
			chiToEpsilon->set_from(3);
			chiToEpsilon->set_to(5);
			chiToEpsilon->set_label(*itag);
			chiToEpsilon->set_exist(true);
			chiToEpsilon->set_inUpdateModel(true);

			chi->add_out(edgeID);
			epsilon->add_in(edgeID);

			edlist.push_back(chiToEpsilon);
			edgeID++;


			//Edge from Mu to Epsilon
			Kedge* muToEpsilon = new Kedge();
			muToEpsilon->set_id(edgeID);
			muToEpsilon->set_from(4);
			muToEpsilon->set_to(5);
			muToEpsilon->set_label(*itag);
			muToEpsilon->set_exist(true);
			muToEpsilon->set_inUpdateModel(true);

			mu->add_out(edgeID);
			epsilon->add_in(edgeID);

			edlist.push_back(muToEpsilon);
			edgeID++;


			//Edge from Epsilon to Epsilon
			Kedge* epsilonToEpsilon = new Kedge();
			epsilonToEpsilon->set_id(edgeID);
			epsilonToEpsilon->set_from(5);
			epsilonToEpsilon->set_to(5);
			epsilonToEpsilon->set_label(*itag);
			epsilonToEpsilon->set_exist(true);
			epsilonToEpsilon->set_inUpdateModel(true);

			epsilon->add_out(edgeID);
			epsilon->add_in(edgeID);

			edlist.push_back(epsilonToEpsilon);
			edgeID++;
		}
		

		Events evlist;
		evlist.push_back(*sigma);
		evlist.push_back(*zeta);
		evlist.push_back(*tau);
		evlist.push_back(*chi);
		evlist.push_back(*mu);
		evlist.push_back(*epsilon);
		
		upmodel->set_eventslist(evlist);
		upmodel->set_edgeslist(edlist);
		upmodel->add_true_event(sigma->get_id());

		// return NULL;
	} else if (misleading){//----------------------------------------MISLEADING-----------------------------------------------------------------
		
		//Create events
		Event* sigma = new Event();
		sigma->set_id(0);
		sigma->set_name("Sigma");

		Event* zeta = new Event();
		zeta->set_id(1);
		zeta->set_name("Zeta");

		Event* tau = new Event();
		tau->set_id(2);
		tau->set_name("Tau");


		Event* chi = new Event();
		chi->set_id(3);
		chi->set_name("Chi");

		Event* mu = new Event();
		mu->set_id(4);
		mu->set_name("Mu");

		Event* epsilon = new Event();
		epsilon->set_id(5);
		epsilon->set_name("Epsilon");


		//Precondition and subsitution for Sigma
		Fluents_BF_list pre_sub_sigma = Fluents_BF_list();
		//Transform into FluentsFormula single fluent
		for (itTemp = annFlu.begin(); itTemp != annFlu.end(); itTemp++) {
			Fluents_BF addSigma = Fluents_BF();
			BFNode* addNodeSigma = new BFNode();
			BFNode* tempNodeSigma = new BFNode();
			BFNode* BVarphiNode = new BFNode();
			BFNode* BNegVarphiNode = new BFNode();

			addNodeSigma->node_type = propForm;
			addNodeSigma->bfOperator = BFNOT;
			tempNodeSigma->node_type = propForm;
			tempNodeSigma->bfOperator = BFOR;

			BVarphiNode->node_type = EForm;
			BVarphiNode->agents = executors_vector;
			BNegVarphiNode->node_type = EForm;
			BNegVarphiNode->agents = executors_vector;

			for (itagstr = executors_vector.begin(); itagstr != executors_vector.end(); itagstr++){
				for (auto itmap = p.a_map.begin(); itmap != p.a_map.end(); itmap++){
					if (itmap->second == *itagstr){
						BVarphiNode->groupAgent.insert(itmap->first);
						BNegVarphiNode->groupAgent.insert(itmap->first);
						break;
					}
				}
			}

			BFNode* tempNode = new BFNode();
			tempNode->node_type = fluForm;

			BFNode* tempNode2 = new BFNode();
			tempNode2->node_type = fluForm;
			tempNode2->set_flu(&(*itTemp));

			FluentFormula* ff = new FluentFormula();
			StringList toStrLNeg, toStrL;
			FluentFormula::iterator ffTemp;
			for (ffTemp = itTemp->begin(); ffTemp != itTemp->end(); ffTemp++){
				Fluents::iterator flTemp;
				for (flTemp = ffTemp->begin(); flTemp != ffTemp->end(); flTemp++){
					Fluent negTemp;
					NEGATE(*flTemp, negTemp);

					Fluents tmp;
					tmp.insert(negTemp);

					ff->insert(tmp);

					string toString = to_string(negTemp);
					toStrLNeg.insert(toString);

					toString = to_string(*flTemp);
					toStrL.insert(toString);
				}
			}
			tempNode->set_flu(ff);
			tempNode->flu_form.insert(toStrLNeg);

			tempNode2->flu_form.insert(toStrL);

			tempNodeSigma->bfnode1 = tempNode;
			tempNodeSigma->bfnode2 = tempNode2;

			addNodeSigma->bfnode1 = tempNodeSigma;

			addSigma.set_bn_body(*addNodeSigma);

			pre_sub_sigma.push_back(addSigma);
		}

		sigma->set_pre_sub(pre_sub_sigma);

		//Precondition and subsitution for Zeta/Chi
		Fluents_BF_list pre_sub_zeta_chi = Fluents_BF_list();
		for (itTemp = annFlu.begin(); itTemp != annFlu.end(); itTemp++) {
			Fluents_BF addZetaChi = Fluents_BF();
			BFNode* addNodeZetaChi = new BFNode();
			addNodeZetaChi->node_type = fluForm;

			FluentFormula* ff = new FluentFormula();
			StringList toStrL;
			FluentFormula::iterator ffTemp;
			for (ffTemp = itTemp->begin(); ffTemp != itTemp->end(); ffTemp++){
				Fluents::iterator flTemp;
				for (flTemp = ffTemp->begin(); flTemp != ffTemp->end(); flTemp++){
					Fluent negTemp;
					NEGATE(*flTemp, negTemp);

					Fluents tmp;
					tmp.insert(negTemp);

					ff->insert(tmp);

					string toString = to_string(negTemp);
					toStrL.insert(toString);
				}
			}
			addNodeZetaChi->set_flu(ff);
			addNodeZetaChi->flu_form.insert(toStrL);

			addZetaChi.set_bn_body(*addNodeZetaChi);

			pre_sub_zeta_chi.push_back(addZetaChi);
		}
		zeta->set_pre_sub(pre_sub_zeta_chi);
		chi->set_pre_sub(pre_sub_zeta_chi);

		//Precondition and subsitution for Tau/Mu
		Fluents_BF_list pre_sub_tau_mu = Fluents_BF_list();
		for (itTemp = annFlu.begin(); itTemp != annFlu.end(); itTemp++) {
			Fluents_BF addTauMu = Fluents_BF();
			BFNode* addNodeTauMu = new BFNode();
			addNodeTauMu->node_type = fluForm;		

			addNodeTauMu->set_flu(&(*itTemp));

			StringList toStrL;
			FluentFormula::iterator ffTemp;
			for (ffTemp = itTemp->begin(); ffTemp != itTemp->end(); ffTemp++){
				Fluents::iterator flTemp;
				for (flTemp = ffTemp->begin(); flTemp != ffTemp->end(); flTemp++){
					string toString = to_string(*flTemp);
					toStrL.insert(toString);
				}
			}
			addNodeTauMu->flu_form.insert(toStrL);

			addTauMu.set_bn_body(*addNodeTauMu);

			pre_sub_tau_mu.push_back(addTauMu);
		}
		tau->set_pre_sub(pre_sub_tau_mu);
		mu->set_pre_sub(pre_sub_tau_mu);

		//Create edges
		Edges edlist;

		AgentSet::iterator itag;
		int edgeID = 0;

		//Edges for full observers
		for (itag = fd.begin(); itag != fd.end(); itag++){
			//Edge from Sigma to Sigma
			Kedge* sigmaToSigma = new Kedge();
			sigmaToSigma->set_id(edgeID);
			sigmaToSigma->set_from(0);
			sigmaToSigma->set_to(0);
			sigmaToSigma->set_label(*itag);
			sigmaToSigma->set_exist(true);
			sigmaToSigma->set_inUpdateModel(true);

			if (executors.find(*itag) == executors.end()){
				Nodes* edge_con_sigma_sigma = new Nodes();

				for (itTemp = annFlu.begin(); itTemp != annFlu.end(); itTemp++) {
					BFNode* agentNode = new BFNode();
					agentNode->node_type = BForm;
					agentNode->agentPro2 = *itag;
					for (auto itmap = p.a_map.begin(); itmap != p.a_map.end(); itmap++){
						if (itmap->second == *itag){
							agentNode->agentPro = itmap->first;
							break;
						}
					}
					

					BFNode* BNegVarphiNode = new BFNode();
					BNegVarphiNode->agents = executors_vector;
					BNegVarphiNode->node_type = EForm;

					BFNode* BVarphiNode = new BFNode();
					BVarphiNode->agents = executors_vector;
					BVarphiNode->node_type = EForm;

					for (itagstr = executors_vector.begin(); itagstr != executors_vector.end(); itagstr++){
						for (auto itmap = p.a_map.begin(); itmap != p.a_map.end(); itmap++){
							if (itmap->second == *itagstr){
								BNegVarphiNode->groupAgent.insert(itmap->first);
								BVarphiNode->groupAgent.insert(itmap->first);
								break;
							}
						}
					}

					BFNode* negVarphiNode = new BFNode();
					negVarphiNode->node_type = fluForm;

					BFNode* varphiNode = new BFNode();
					varphiNode->node_type = fluForm;

					FluentFormula* ff = new FluentFormula();
					StringList toStrLNeg, toStrL;
					FluentFormula::iterator ffTemp;
					for (ffTemp = itTemp->begin(); ffTemp != itTemp->end(); ffTemp++){
						Fluents::iterator flTemp;
						for (flTemp = ffTemp->begin(); flTemp != ffTemp->end(); flTemp++){
							Fluent negTemp;
							NEGATE(*flTemp, negTemp);

							Fluents tmp;
							tmp.insert(negTemp);

							ff->insert(tmp);

							string toString = to_string(negTemp);
							toStrLNeg.insert(toString);

							toString = to_string(*flTemp);
							toStrL.insert(toString);
						}
					}
					negVarphiNode->set_flu(ff);
					negVarphiNode->flu_form.insert(toStrLNeg);

					varphiNode->set_flu(&(*itTemp));
					varphiNode->flu_form.insert(toStrL);

					BNegVarphiNode->bfnode1 = negVarphiNode;
					BVarphiNode->bfnode1 = varphiNode;

					BFNode* tempNode = new BFNode();
					tempNode->node_type = propForm;
					tempNode->bfOperator = BFOR;
					tempNode->bfnode1 = BNegVarphiNode;
					tempNode->bfnode2 = BVarphiNode;

					BFNode* alphaNode = new BFNode();
					alphaNode->node_type = propForm;
					alphaNode->bfOperator = BFNOT;
					alphaNode->bfnode1 = tempNode;

					agentNode->bfnode1 = alphaNode;

					edge_con_sigma_sigma->push_back(*agentNode);
				}

				sigmaToSigma->set_edge_condition(edge_con_sigma_sigma);
			}

			sigma->add_out(edgeID);
			sigma->add_in(edgeID);

			edlist.push_back(sigmaToSigma);
			edgeID++;


			//Edge from Sigma to Zeta
			Kedge* sigmaToZeta = new Kedge();
			sigmaToZeta->set_id(edgeID);
			sigmaToZeta->set_from(0);
			sigmaToZeta->set_to(1);
			sigmaToZeta->set_label(*itag);
			sigmaToZeta->set_exist(true);
			sigmaToZeta->set_inUpdateModel(true);

			Nodes* edge_con_sigma_zeta = new Nodes();

			if (executors.find(*itag) == executors.end()){
				for (itTemp = annFlu.begin(); itTemp != annFlu.end(); itTemp++) {
					BFNode* agentNode = new BFNode();
					agentNode->node_type = propForm;
					agentNode->bfOperator = BFOR;

					BFNode* agentBFNode1 = new BFNode();
					agentBFNode1->node_type = BForm;
					agentBFNode1->agentPro2 = *itag;
					for (auto itmap = p.a_map.begin(); itmap != p.a_map.end(); itmap++){
						if (itmap->second == *itag){
							agentBFNode1->agentPro = itmap->first;
							break;
						}
					}

					BFNode* BNegVarphiNode = new BFNode();
					BFNode* BVarphiNode = new BFNode();
					BNegVarphiNode->agents = executors_vector;
					BNegVarphiNode->node_type = EForm;
					BVarphiNode->agents = executors_vector;
					BVarphiNode->node_type = EForm;
					for (itagstr = executors_vector.begin(); itagstr != executors_vector.end(); itagstr++){
						for (auto itmap = p.a_map.begin(); itmap != p.a_map.end(); itmap++){
							if (itmap->second == *itagstr){
								BNegVarphiNode->groupAgent.insert(itmap->first);
								BVarphiNode->groupAgent.insert(itmap->first);
								break;
							}
						}
					}

					BFNode* negVarphiNode = new BFNode();
					negVarphiNode->node_type = fluForm;

					BFNode* varphiNode = new BFNode();
					varphiNode->node_type = fluForm;

					FluentFormula* ff = new FluentFormula();
					StringList toStrLNeg, toStrL;;
					FluentFormula::iterator ffTemp;
					for (ffTemp = itTemp->begin(); ffTemp != itTemp->end(); ffTemp++){
						Fluents::iterator flTemp;
						for (flTemp = ffTemp->begin(); flTemp != ffTemp->end(); flTemp++){
							Fluent negTemp;
							NEGATE(*flTemp, negTemp);

							Fluents tmp;
							tmp.insert(negTemp);

							ff->insert(tmp);

							string toString = to_string(negTemp);
							toStrLNeg.insert(toString);

							toString = to_string(*flTemp);
							toStrL.insert(toString);
						}
					}
					negVarphiNode->set_flu(ff);
					negVarphiNode->flu_form.insert(toStrLNeg);

					varphiNode->set_flu(&(*itTemp));
					varphiNode->flu_form.insert(toStrL);

					BNegVarphiNode->bfnode1 = negVarphiNode;
					BVarphiNode->bfnode1 = varphiNode;

					BFNode* tempOrNode = new BFNode();
					tempOrNode->node_type = propForm;
					tempOrNode->bfOperator = BFOR;
					tempOrNode->bfnode1 = BNegVarphiNode;
					tempOrNode->bfnode2 = BVarphiNode;

					BFNode* tempNotNode = new BFNode();
					tempNotNode->node_type = propForm;
					tempNotNode->bfOperator = BFNOT;
					tempNotNode->bfnode1 = tempOrNode;


					agentBFNode1->bfnode1 = tempNotNode;

					BFNode* agentBFNode2 = new BFNode();
					agentBFNode2->node_type = BForm;
					agentBFNode2->agentPro2 = *itag;
					agentBFNode2->bfnode1 = negVarphiNode;
					for (auto itmap = p.a_map.begin(); itmap != p.a_map.end(); itmap++){
						if (itmap->second == *itag){
							agentBFNode2->agentPro = itmap->first;
							break;
						}
					}
					

					agentNode->bfnode1 = agentBFNode1;
					agentNode->bfnode2 = agentBFNode2;

					edge_con_sigma_zeta->push_back(*agentNode);
				}
				sigmaToZeta->set_edge_condition(edge_con_sigma_zeta);
			}
			

			sigma->add_out(edgeID);
			zeta->add_in(edgeID);

			edlist.push_back(sigmaToZeta);
			edgeID++;


			//Edge from Sigma to Tau
			Kedge* sigmaToTau = new Kedge();
			sigmaToTau->set_id(edgeID);
			sigmaToTau->set_from(0);
			sigmaToTau->set_to(2);
			sigmaToTau->set_label(*itag);
			sigmaToTau->set_exist(true);
			sigmaToTau->set_inUpdateModel(true);
			Nodes* edge_con_sigma_tau = new Nodes();
			
			for (itTemp = annFlu.begin(); itTemp != annFlu.end(); itTemp++) {
				BFNode* agentNode = new BFNode();
				agentNode->node_type = propForm;
				agentNode->bfOperator = BFNOT;

				BFNode* agentBFNode1 = new BFNode();
				agentBFNode1->node_type = BForm;
				agentBFNode1->agentPro2 = *itag;
				for (auto itmap = p.a_map.begin(); itmap != p.a_map.end(); itmap++){
					if (itmap->second == *itag){
						agentBFNode1->agentPro = itmap->first;
						break;
					}
				}

				BFNode* tempNode = new BFNode();
				tempNode->node_type = fluForm;

				FluentFormula* ff = new FluentFormula();
				StringList toStrL;
				FluentFormula::iterator ffTemp;
				for (ffTemp = itTemp->begin(); ffTemp != itTemp->end(); ffTemp++){
					Fluents::iterator flTemp;
					for (flTemp = ffTemp->begin(); flTemp != ffTemp->end(); flTemp++){
						Fluent negTemp;
						NEGATE(*flTemp, negTemp);

						Fluents tmp;
						tmp.insert(negTemp);

						ff->insert(tmp);

						string toString = to_string(negTemp);
						toStrL.insert(toString);
					}
				}
				tempNode->set_flu(ff);
				tempNode->flu_form.insert(toStrL);

				agentBFNode1->bfnode1 = tempNode;

				agentNode->bfnode1 = agentBFNode1;

				edge_con_sigma_tau->push_back(*agentNode);
			}
			sigmaToTau->set_edge_condition(edge_con_sigma_tau);

			sigma->add_out(edgeID);
			tau->add_in(edgeID);

			edlist.push_back(sigmaToTau);
			edgeID++;


			//Edge from Zeta to Zeta
			Kedge* zetaToZeta = new Kedge();
			zetaToZeta->set_id(edgeID);
			zetaToZeta->set_from(1);
			zetaToZeta->set_to(1);
			zetaToZeta->set_label(*itag);
			zetaToZeta->set_exist(true);
			zetaToZeta->set_inUpdateModel(true);

			if (executors.find(*itag) == executors.end()){
				zetaToZeta->set_edge_condition(edge_con_sigma_zeta);
			}

			zeta->add_out(edgeID);
			zeta->add_in(edgeID);

			edlist.push_back(zetaToZeta);
			edgeID++;


			//Edge from Zeta to Tau
			Kedge* zetaToTau = new Kedge();
			zetaToTau->set_id(edgeID);
			zetaToTau->set_from(1);
			zetaToTau->set_to(2);
			zetaToTau->set_label(*itag);
			zetaToTau->set_exist(true);
			zetaToTau->set_inUpdateModel(true);

			if (executors.find(*itag) == executors.end()){
				zetaToTau->set_edge_condition(edge_con_sigma_tau);

				zeta->add_out(edgeID);
				tau->add_in(edgeID);

				edlist.push_back(zetaToTau);
				edgeID++;
			}

			


			//Edge from Tau to Zeta
			Kedge* tauToZeta = new Kedge();
			tauToZeta->set_id(edgeID);
			tauToZeta->set_from(2);
			tauToZeta->set_to(1);
			tauToZeta->set_label(*itag);
			tauToZeta->set_exist(true);
			tauToZeta->set_inUpdateModel(true);
			tauToZeta->set_edge_condition(edge_con_sigma_zeta);

			tau->add_out(edgeID);
			zeta->add_in(edgeID);

			edlist.push_back(tauToZeta);
			edgeID++;


			//Edge from Tau to Tau
			Kedge* tauToTau = new Kedge();
			tauToTau->set_id(edgeID);
			tauToTau->set_from(2);
			tauToTau->set_to(2);
			tauToTau->set_label(*itag);
			tauToTau->set_exist(true);
			tauToTau->set_inUpdateModel(true);

			tau->add_out(edgeID);
			tau->add_in(edgeID);

			edlist.push_back(tauToTau);
			edgeID++;


			//Edge from Chi to Chi
			Kedge* chiToChi = new Kedge();
			chiToChi->set_id(edgeID);
			chiToChi->set_from(3);
			chiToChi->set_to(3);
			chiToChi->set_label(*itag);
			chiToChi->set_exist(true);
			chiToChi->set_inUpdateModel(true);

			chi->add_out(edgeID);
			chi->add_in(edgeID);

			edlist.push_back(chiToChi);
			edgeID++;


			//Edge from Mu to Mu
			Kedge* muToMu = new Kedge();
			muToMu->set_id(edgeID);
			muToMu->set_from(4);
			muToMu->set_to(4);
			muToMu->set_label(*itag);
			muToMu->set_exist(true);
			muToMu->set_inUpdateModel(true);

			mu->add_out(edgeID);
			mu->add_in(edgeID);

			edlist.push_back(muToMu);
			edgeID++;


			//Edge from Chi to Mu
			Kedge* chiToMu = new Kedge();
			chiToMu->set_id(edgeID);
			chiToMu->set_from(3);
			chiToMu->set_to(4);
			chiToMu->set_label(*itag);
			chiToMu->set_exist(true);
			chiToMu->set_inUpdateModel(true);
			Nodes* edge_con_chi_mu = new Nodes();

			for (itTemp = annFlu.begin(); itTemp != annFlu.end(); itTemp++) {
				BFNode* agentBFNode1 = new BFNode();
				agentBFNode1->node_type = BForm;
				agentBFNode1->agentPro2 = *itag;
				for (auto itmap = p.a_map.begin(); itmap != p.a_map.end(); itmap++){
					if (itmap->second == *itag){
						agentBFNode1->agentPro = itmap->first;
						break;
					}
				}

				BFNode* tempNode = new BFNode();
				tempNode->node_type = fluForm;

				tempNode->set_flu(&(*itTemp));

				StringList toStrL;
				FluentFormula::iterator ffTemp;
				for (ffTemp = itTemp->begin(); ffTemp != itTemp->end(); ffTemp++){
					Fluents::iterator flTemp;
					for (flTemp = ffTemp->begin(); flTemp != ffTemp->end(); flTemp++){
						string toString = to_string(*flTemp);
						toStrL.insert(toString);
					}
				}
				tempNode->flu_form.insert(toStrL);

				agentBFNode1->bfnode1 = tempNode;

				edge_con_chi_mu->push_back(*agentBFNode1);
			}
			chiToMu->set_edge_condition(edge_con_chi_mu);

			chi->add_out(edgeID);
			mu->add_in(edgeID);

			edlist.push_back(chiToMu);
			edgeID++;


			//Edge from Mu to Chi
			Kedge* muToChi = new Kedge();
			muToChi->set_id(edgeID);
			muToChi->set_from(4);
			muToChi->set_to(3);
			muToChi->set_label(*itag);
			muToChi->set_exist(true);
			muToChi->set_inUpdateModel(true);
			Nodes* edge_con_mu_chi = new Nodes();

			for (itTemp = annFlu.begin(); itTemp != annFlu.end(); itTemp++) {
				BFNode* agentBFNode1 = new BFNode();
				agentBFNode1->node_type = BForm;
				agentBFNode1->agentPro2 = *itag;
				for (auto itmap = p.a_map.begin(); itmap != p.a_map.end(); itmap++){
					if (itmap->second == *itag){
						agentBFNode1->agentPro = itmap->first;
						break;
					}
				}

				BFNode* tempNode = new BFNode();
				tempNode->node_type = fluForm;

				FluentFormula* ff = new FluentFormula();
				StringList toStrL;
				FluentFormula::iterator ffTemp;
				for (ffTemp = itTemp->begin(); ffTemp != itTemp->end(); ffTemp++){
					Fluents::iterator flTemp;
					for (flTemp = ffTemp->begin(); flTemp != ffTemp->end(); flTemp++){
						Fluent negTemp;
						NEGATE(*flTemp, negTemp);

						Fluents tmp;
						tmp.insert(negTemp);

						ff->insert(tmp);

						string toString = to_string(negTemp);
						toStrL.insert(toString);
					}
				}
				tempNode->set_flu(ff);
				tempNode->flu_form.insert(toStrL);

				agentBFNode1->bfnode1 = tempNode;

				edge_con_mu_chi->push_back(*agentBFNode1);
			}
			muToChi->set_edge_condition(edge_con_mu_chi);

			mu->add_out(edgeID);
			chi->add_in(edgeID);

			edlist.push_back(muToChi);
			edgeID++;


			//Edge from Epsilon to Epsilon
			Kedge* epsilonToEpsilon = new Kedge();
			epsilonToEpsilon->set_id(edgeID);
			epsilonToEpsilon->set_from(5);
			epsilonToEpsilon->set_to(5);
			epsilonToEpsilon->set_label(*itag);
			epsilonToEpsilon->set_exist(true);
			epsilonToEpsilon->set_inUpdateModel(true);

			epsilon->add_out(edgeID);
			epsilon->add_in(edgeID);

			edlist.push_back(epsilonToEpsilon);
			edgeID++;
		}

		//Edges for partial observers
		for (itag = pd.begin(); itag != pd.end(); itag++){
			//Edge from Sigma to Sigma
			Kedge* sigmaToSigma = new Kedge();
			sigmaToSigma->set_id(edgeID);
			sigmaToSigma->set_from(0);
			sigmaToSigma->set_to(0);
			sigmaToSigma->set_label(*itag);
			sigmaToSigma->set_exist(true);
			sigmaToSigma->set_inUpdateModel(true);

			
			Nodes* edge_con_sigma_sigma = new Nodes();

			for (itTemp = annFlu.begin(); itTemp != annFlu.end(); itTemp++) {
				BFNode* agentNode = new BFNode();
				agentNode->node_type = BForm;
				agentNode->agentPro2 = *itag;
				for (auto itmap = p.a_map.begin(); itmap != p.a_map.end(); itmap++){
					if (itmap->second == *itag){
						agentNode->agentPro = itmap->first;
						break;
					}
				}
					

				BFNode* BNegVarphiNode = new BFNode();
				BNegVarphiNode->agents = executors_vector;
				BNegVarphiNode->node_type = EForm;

				BFNode* BVarphiNode = new BFNode();
				BVarphiNode->agents = executors_vector;
				BVarphiNode->node_type = EForm;

				for (itagstr = executors_vector.begin(); itagstr != executors_vector.end(); itagstr++){
					for (auto itmap = p.a_map.begin(); itmap != p.a_map.end(); itmap++){
						if (itmap->second == *itagstr){
							BNegVarphiNode->groupAgent.insert(itmap->first);
							BVarphiNode->groupAgent.insert(itmap->first);
							break;
						}
					}
				}

				BFNode* negVarphiNode = new BFNode();
				negVarphiNode->node_type = fluForm;

				BFNode* varphiNode = new BFNode();
				varphiNode->node_type = fluForm;

				FluentFormula* ff = new FluentFormula();
				StringList toStrLNeg, toStrL;
				FluentFormula::iterator ffTemp;
				for (ffTemp = itTemp->begin(); ffTemp != itTemp->end(); ffTemp++){
					Fluents::iterator flTemp;
					for (flTemp = ffTemp->begin(); flTemp != ffTemp->end(); flTemp++){
						Fluent negTemp;
						NEGATE(*flTemp, negTemp);

						Fluents tmp;
						tmp.insert(negTemp);

						ff->insert(tmp);

						string toString = to_string(negTemp);
						toStrLNeg.insert(toString);

						toString = to_string(*flTemp);
						toStrL.insert(toString);
					}
				}
				negVarphiNode->set_flu(ff);
				negVarphiNode->flu_form.insert(toStrLNeg);

				varphiNode->set_flu(&(*itTemp));
				varphiNode->flu_form.insert(toStrL);

				BNegVarphiNode->bfnode1 = negVarphiNode;
				BVarphiNode->bfnode1 = varphiNode;

				BFNode* tempNode = new BFNode();
				tempNode->node_type = propForm;
				tempNode->bfOperator = BFOR;
				tempNode->bfnode1 = BNegVarphiNode;
				tempNode->bfnode2 = BVarphiNode;

				BFNode* alphaNode = new BFNode();
				alphaNode->node_type = propForm;
				alphaNode->bfOperator = BFNOT;
				alphaNode->bfnode1 = tempNode;

				agentNode->bfnode1 = alphaNode;

				edge_con_sigma_sigma->push_back(*agentNode);
			}

			sigmaToSigma->set_edge_condition(edge_con_sigma_sigma);


			sigma->add_out(edgeID);
			sigma->add_in(edgeID);

			edlist.push_back(sigmaToSigma);
			edgeID++;


			//Edge from Sigma to Chi
			Kedge* sigmaToChi = new Kedge();
			sigmaToChi->set_id(edgeID);
			sigmaToChi->set_from(0);
			sigmaToChi->set_to(3);
			sigmaToChi->set_label(*itag);
			sigmaToChi->set_exist(true);
			sigmaToChi->set_inUpdateModel(true);

			sigma->add_out(edgeID);
			chi->add_in(edgeID);

			edlist.push_back(sigmaToChi);
			edgeID++;


			//Edge from Sigma to Mu
			Kedge* sigmaToMu = new Kedge();
			sigmaToMu->set_id(edgeID);
			sigmaToMu->set_from(0);
			sigmaToMu->set_to(4);
			sigmaToMu->set_label(*itag);
			sigmaToMu->set_exist(true);
			sigmaToMu->set_inUpdateModel(true);

			sigma->add_out(edgeID);
			mu->add_in(edgeID);

			edlist.push_back(sigmaToMu);
			edgeID++;


			//Edge from Zeta to Chi
			Kedge* zetaToChi = new Kedge();
			zetaToChi->set_id(edgeID);
			zetaToChi->set_from(1);
			zetaToChi->set_to(3);
			zetaToChi->set_label(*itag);
			zetaToChi->set_exist(true);
			zetaToChi->set_inUpdateModel(true);

			zeta->add_out(edgeID);
			chi->add_in(edgeID);

			edlist.push_back(zetaToChi);
			edgeID++;


			//Edge from Zeta to Mu
			Kedge* zetaToMu = new Kedge();
			zetaToMu->set_id(edgeID);
			zetaToMu->set_from(1);
			zetaToMu->set_to(4);
			zetaToMu->set_label(*itag);
			zetaToMu->set_exist(true);
			zetaToMu->set_inUpdateModel(true);

			zeta->add_out(edgeID);
			mu->add_in(edgeID);

			edlist.push_back(zetaToMu);
			edgeID++;


			//Edge from Tau to Chi
			Kedge* tauToChi = new Kedge();
			tauToChi->set_id(edgeID);
			tauToChi->set_from(2);
			tauToChi->set_to(3);
			tauToChi->set_label(*itag);
			tauToChi->set_exist(true);
			tauToChi->set_inUpdateModel(true);

			tau->add_out(edgeID);
			chi->add_in(edgeID);

			edlist.push_back(tauToChi);
			edgeID++;


			//Edge from Tau to Mu
			Kedge* tauToMu = new Kedge();
			tauToMu->set_id(edgeID);
			tauToMu->set_from(2);
			tauToMu->set_to(4);
			tauToMu->set_label(*itag);
			tauToMu->set_exist(true);
			tauToMu->set_inUpdateModel(true);

			tau->add_out(edgeID);
			mu->add_in(edgeID);

			edlist.push_back(tauToMu);
			edgeID++;


			//Edge from Chi to Chi
			Kedge* chiToChi = new Kedge();
			chiToChi->set_id(edgeID);
			chiToChi->set_from(3);
			chiToChi->set_to(3);
			chiToChi->set_label(*itag);
			chiToChi->set_exist(true);
			chiToChi->set_inUpdateModel(true);

			chi->add_out(edgeID);
			chi->add_in(edgeID);

			edlist.push_back(chiToChi);
			edgeID++;


			//Edge from Chi to Mu
			Kedge* chiToMu = new Kedge();
			chiToMu->set_id(edgeID);
			chiToMu->set_from(3);
			chiToMu->set_to(4);
			chiToMu->set_label(*itag);
			chiToMu->set_exist(true);
			chiToMu->set_inUpdateModel(true);

			chi->add_out(edgeID);
			mu->add_in(edgeID);

			edlist.push_back(chiToMu);
			edgeID++;


			//Edge from Mu to Chi
			Kedge* muToChi = new Kedge();
			muToChi->set_id(edgeID);
			muToChi->set_from(4);
			muToChi->set_to(3);
			muToChi->set_label(*itag);
			muToChi->set_exist(true);
			muToChi->set_inUpdateModel(true);

			mu->add_out(edgeID);
			chi->add_in(edgeID);

			edlist.push_back(muToChi);
			edgeID++;


			//Edge from Mu to Mu
			Kedge* muToMu = new Kedge();
			muToMu->set_id(edgeID);
			muToMu->set_from(4);
			muToMu->set_to(4);
			muToMu->set_label(*itag);
			muToMu->set_exist(true);
			muToMu->set_inUpdateModel(true);

			mu->add_out(edgeID);
			mu->add_in(edgeID);

			edlist.push_back(muToMu);
			edgeID++;


			//Edge from Epsilon to Epsilon
			Kedge* epsilonToEpsilon = new Kedge();
			epsilonToEpsilon->set_id(edgeID);
			epsilonToEpsilon->set_from(5);
			epsilonToEpsilon->set_to(5);
			epsilonToEpsilon->set_label(*itag);
			epsilonToEpsilon->set_exist(true);
			epsilonToEpsilon->set_inUpdateModel(true);

			epsilon->add_out(edgeID);
			epsilon->add_in(edgeID);

			edlist.push_back(epsilonToEpsilon);
			edgeID++;
		}

		//Edges for oblivous
		for (itag = od.begin(); itag != od.end(); itag++){
			//Edge from Sigma to Epsilon
			Kedge* sigmaToEpsilon = new Kedge();
			sigmaToEpsilon->set_id(edgeID);
			sigmaToEpsilon->set_from(0);
			sigmaToEpsilon->set_to(5);
			sigmaToEpsilon->set_label(*itag);
			sigmaToEpsilon->set_exist(true);
			sigmaToEpsilon->set_inUpdateModel(true);

			sigma->add_out(edgeID);
			epsilon->add_in(edgeID);

			edlist.push_back(sigmaToEpsilon);
			edgeID++;


			//Edge from Zeta to Epsilon
			Kedge* zetaToEpsilon = new Kedge();
			zetaToEpsilon->set_id(edgeID);
			zetaToEpsilon->set_from(1);
			zetaToEpsilon->set_to(5);
			zetaToEpsilon->set_label(*itag);
			zetaToEpsilon->set_exist(true);
			zetaToEpsilon->set_inUpdateModel(true);

			zeta->add_out(edgeID);
			epsilon->add_in(edgeID);

			edlist.push_back(zetaToEpsilon);
			edgeID++;


			//Edge from Tau to Epsilon
			Kedge* tauToEpsilon = new Kedge();
			tauToEpsilon->set_id(edgeID);
			tauToEpsilon->set_from(2);
			tauToEpsilon->set_to(5);
			tauToEpsilon->set_label(*itag);
			tauToEpsilon->set_exist(true);
			tauToEpsilon->set_inUpdateModel(true);

			tau->add_out(edgeID);
			epsilon->add_in(edgeID);

			edlist.push_back(tauToEpsilon);
			edgeID++;


			//Edge from Chi to Epsilon
			Kedge* chiToEpsilon = new Kedge();
			chiToEpsilon->set_id(edgeID);
			chiToEpsilon->set_from(3);
			chiToEpsilon->set_to(5);
			chiToEpsilon->set_label(*itag);
			chiToEpsilon->set_exist(true);
			chiToEpsilon->set_inUpdateModel(true);

			chi->add_out(edgeID);
			epsilon->add_in(edgeID);

			edlist.push_back(chiToEpsilon);
			edgeID++;


			//Edge from Mu to Epsilon
			Kedge* muToEpsilon = new Kedge();
			muToEpsilon->set_id(edgeID);
			muToEpsilon->set_from(4);
			muToEpsilon->set_to(5);
			muToEpsilon->set_label(*itag);
			muToEpsilon->set_exist(true);
			muToEpsilon->set_inUpdateModel(true);

			mu->add_out(edgeID);
			epsilon->add_in(edgeID);

			edlist.push_back(muToEpsilon);
			edgeID++;


			//Edge from Epsilon to Epsilon
			Kedge* epsilonToEpsilon = new Kedge();
			epsilonToEpsilon->set_id(edgeID);
			epsilonToEpsilon->set_from(5);
			epsilonToEpsilon->set_to(5);
			epsilonToEpsilon->set_label(*itag);
			epsilonToEpsilon->set_exist(true);
			epsilonToEpsilon->set_inUpdateModel(true);

			epsilon->add_out(edgeID);
			epsilon->add_in(edgeID);

			edlist.push_back(epsilonToEpsilon);
			edgeID++;
		}
		

		Events evlist;
		evlist.push_back(*sigma);
		evlist.push_back(*zeta);
		evlist.push_back(*tau);
		evlist.push_back(*chi);
		evlist.push_back(*mu);
		evlist.push_back(*epsilon);
		
		upmodel->set_eventslist(evlist);
		upmodel->set_edgeslist(edlist);
		upmodel->add_true_event(sigma->get_id());


	} else if (truthful && !not_truthful){

		/*****************Correction of false beliefs**********************/
		//We check all the fully observant
		AgentSet::iterator it_fd;
		list<FluentFormula>::iterator it_ff;
			
		//-----------
		//Add a copy of the list because a.get_ann_ff() is a const list
		list<FluentFormula> copyList = a.get_ann_ff();

		Edges to_add;
		int id_edge = k.get_edgeslist().size();

		for (it_fd = fd.begin();it_fd != fd.end();it_fd++) {
			for (it_ff = copyList.begin(); it_ff != copyList.end(); it_ff++) {
				k.correctFalseBeliefs(*it_ff, *it_fd, id_edge, to_add);
			}
		}
		
		//Add all the found edges
		Edges::iterator it_ed;
		for (it_ed = to_add.begin();it_ed != to_add.end();it_ed++) {
			k.add_edge(*it_ed);
		}

		/******************************************************************/

		Event* sigma = new Event();
		sigma->set_id(0);
		sigma->set_name("Sigma");

		Event* tau = new Event();
		tau->set_id(1);
		tau->set_name("Tau");

		Event* epsilon = new Event();
		epsilon->set_id(2);
		epsilon->set_name("Epsilon");

		Fluents_BF_list pre_sub_sigma = Fluents_BF_list();

		//Transform into FluentsFormula single fluent
		for (itTemp = annFlu.begin(); itTemp != annFlu.end(); itTemp++) {
			Fluents_BF addSigma = Fluents_BF();
			BFNode* addNodeSigma = new BFNode();
			addNodeSigma->node_type = fluForm;		

			addNodeSigma->set_flu(&(*itTemp));

			StringList toStrL;
			FluentFormula::iterator ffTemp;
			for (ffTemp = itTemp->begin(); ffTemp != itTemp->end(); ffTemp++){
				Fluents::iterator flTemp;
				for (flTemp = ffTemp->begin(); flTemp != ffTemp->end(); flTemp++){
					string toString = to_string(*flTemp);
					toStrL.insert(toString);
				}
			}
			addNodeSigma->flu_form.insert(toStrL);

			addSigma.set_bn_body(*addNodeSigma);

			pre_sub_sigma.push_back(addSigma);
		}

		sigma->set_pre_sub(pre_sub_sigma);


		Fluents_BF_list pre_sub_tau = Fluents_BF_list();

		for (itTemp = annFlu.begin(); itTemp != annFlu.end(); itTemp++) {
			Fluents_BF addTau = Fluents_BF();
			BFNode* addNodeTau = new BFNode();
			addNodeTau->node_type = fluForm;

			FluentFormula* ff = new FluentFormula();
			StringList toStrL;
			FluentFormula::iterator ffTemp;
			for (ffTemp = itTemp->begin(); ffTemp != itTemp->end(); ffTemp++){
				Fluents::iterator flTemp;
				for (flTemp = ffTemp->begin(); flTemp != ffTemp->end(); flTemp++){
					Fluent negTemp;
					NEGATE(*flTemp, negTemp);

					Fluents tmp;
					tmp.insert(negTemp);

					ff->insert(tmp);

					string toString = to_string(negTemp);
					toStrL.insert(toString);
				}
			}
			addNodeTau->set_flu(ff);
			addNodeTau->flu_form.insert(toStrL);

			addTau.set_bn_body(*addNodeTau);

			pre_sub_tau.push_back(addTau);
		}

		tau->set_pre_sub(pre_sub_tau);


		Edges edlist;

		AgentSet::iterator itag;
		int edgeID = 0;

		for (itag = m_agents.begin(); itag != m_agents.end(); itag++){
			FluentFormula* deltaFF = new FluentFormula();
			FluentFormula* thetaFF = new FluentFormula();
			bool hasDelta = false;
			bool hasTheta = false;
			bool needDeltaFF = false;
			bool needThetaFF = false;

			//Edge from Sigma to Sigma
			Kedge* sigmaToSigma = new Kedge();
			sigmaToSigma->set_id(edgeID);
			sigmaToSigma->set_from(0);
			sigmaToSigma->set_to(0);
			sigmaToSigma->set_label(*itag);
			sigmaToSigma->set_exist(true);
			sigmaToSigma->set_inUpdateModel(true);

			//Edge from Tau to Tau
			Kedge* tauToTau = new Kedge();
			tauToTau->set_from(1);
			tauToTau->set_to(1);
			tauToTau->set_label(*itag);
			tauToTau->set_exist(true);
			tauToTau->set_inUpdateModel(true);

			//Edge from Sigma to Tau
			Kedge* sigmaToTau = new Kedge();
			sigmaToTau->set_from(0);
			sigmaToTau->set_to(1);
			sigmaToTau->set_label(*itag);
			sigmaToTau->set_exist(true);
			sigmaToTau->set_inUpdateModel(true);

			//Edge from Tau to Sigma
			Kedge* tauToSigma = new Kedge();
			tauToSigma->set_from(1);
			tauToSigma->set_to(1);
			tauToSigma->set_label(*itag);
			tauToSigma->set_exist(true);
			tauToSigma->set_inUpdateModel(true);

			//Edge from Sigma to Epsilon
			Kedge* sigmaToEpsilon = new Kedge();
			sigmaToEpsilon->set_from(0);
			sigmaToEpsilon->set_to(2);
			sigmaToEpsilon->set_label(*itag);
			sigmaToEpsilon->set_exist(true);
			sigmaToEpsilon->set_inUpdateModel(true);

			//Edge from Tau to Epsilon
			Kedge* tauToEpsilon = new Kedge();
			tauToEpsilon->set_from(1);
			tauToEpsilon->set_to(2);
			tauToEpsilon->set_label(*itag);
			tauToEpsilon->set_exist(true);
			tauToEpsilon->set_inUpdateModel(true);

			//Edge from Epsilon to Epsilon
			Kedge* epsilonToEpsilon = new Kedge();
			epsilonToEpsilon->set_from(2);
			epsilonToEpsilon->set_to(2);
			epsilonToEpsilon->set_label(*itag);
			epsilonToEpsilon->set_exist(true);
			epsilonToEpsilon->set_inUpdateModel(true);


			Agent_FF_list::iterator iaTemp;
			Agent_FF_list temp = a.get_m_observes();
			//Get Delta
			for (iaTemp = temp.begin(); iaTemp != temp.end(); iaTemp++){
				if (*itag == iaTemp->get_head()){
					if (iaTemp->get_body().size() != 0){
						needDeltaFF = true;
						FluentFormula ff = iaTemp->get_body();
						*deltaFF = ff;
					}
					hasDelta = true;
					break;
				}
			}

			temp = a.get_m_awares();
			//Get Theta
			for (iaTemp = temp.begin(); iaTemp != temp.end(); iaTemp++){
				if (*itag == iaTemp->get_head()){
					if (iaTemp->get_body().size() != 0){
						needThetaFF = true;
						FluentFormula ff = iaTemp->get_body();
						*thetaFF = ff;
					}
					hasTheta = true;
					break;
				}
			}

			Nodes* edge_con_sigma_sigma_and_tau_tau = new Nodes();
			Nodes* edge_con_sigma_tau_and_tau_sigma = new Nodes();
			Nodes* edge_con_to_epsilon = new Nodes();
			BFNode* nodeSigmaSigmaAndTauTau = new BFNode();
			BFNode* nodeSigmaTauAndTauSigma = new BFNode();
			BFNode* nodeToEpsilon = new BFNode();

			if (hasDelta){ //Check if exist delta_i 
				if (needDeltaFF){ //Check if delta_i != /top
					if (hasTheta){ //Check if exist theta_i
						if (needThetaFF){ //Check if theta_i != /top
							BFNode* tempNodeDelta = new BFNode();
							tempNodeDelta->node_type = fluForm;
							tempNodeDelta->fluform = deltaFF;

							BFNode* tempNodeTheta = new BFNode();
							tempNodeTheta->node_type = fluForm;
							tempNodeTheta->fluform = thetaFF;

							nodeSigmaSigmaAndTauTau->node_type = propForm;
							nodeSigmaSigmaAndTauTau->bfOperator = BFOR;
							nodeSigmaSigmaAndTauTau->bfnode1 = tempNodeDelta;
							nodeSigmaSigmaAndTauTau->bfnode2 = tempNodeTheta;

							BFNode* tempNodeNegDelta = new BFNode();
							tempNodeNegDelta->node_type = propForm;
							tempNodeNegDelta->bfOperator = BFNOT;
							tempNodeNegDelta->bfnode1 = tempNodeDelta;

							BFNode* tempNodeNegTheta = new BFNode();
							tempNodeNegTheta->node_type = propForm;
							tempNodeNegTheta->bfOperator = BFNOT;
							tempNodeNegTheta->bfnode1 = tempNodeTheta;

							nodeSigmaTauAndTauSigma->node_type = propForm;
							nodeSigmaTauAndTauSigma->bfOperator = BFAND;
							nodeSigmaTauAndTauSigma->bfnode1 = tempNodeNegDelta;
							nodeSigmaTauAndTauSigma->bfnode2 = tempNodeTheta;

							nodeToEpsilon->node_type = propForm;
							nodeToEpsilon->bfOperator = BFAND;
							nodeToEpsilon->bfnode1 = tempNodeNegDelta;
							nodeToEpsilon->bfnode2 = tempNodeNegTheta;

							edge_con_sigma_sigma_and_tau_tau->push_back(*nodeSigmaSigmaAndTauTau);
							edge_con_sigma_tau_and_tau_sigma->push_back(*nodeSigmaTauAndTauSigma);
							edge_con_to_epsilon->push_back(*nodeToEpsilon);

							sigmaToSigma->set_edge_condition(edge_con_sigma_sigma_and_tau_tau);
							sigma->add_out(edgeID);
							sigma->add_in(edgeID);

							edlist.push_back(sigmaToSigma);
							edgeID++;

							tauToTau->set_id(edgeID);
							tauToTau->set_edge_condition(edge_con_sigma_sigma_and_tau_tau);
							tau->add_out(edgeID);
							tau->add_in(edgeID);

							edlist.push_back(tauToTau);
							edgeID++;

							sigmaToTau->set_id(edgeID);
							sigmaToTau->set_edge_condition(edge_con_sigma_tau_and_tau_sigma);
							sigma->add_out(edgeID);
							tau->add_in(edgeID);

							edlist.push_back(sigmaToTau);
							edgeID++;

							tauToSigma->set_id(edgeID);
							tauToSigma->set_edge_condition(edge_con_sigma_tau_and_tau_sigma);
							tau->add_out(edgeID);
							sigma->add_in(edgeID);

							edlist.push_back(tauToSigma);
							edgeID++;

							sigmaToEpsilon->set_id(edgeID);
							sigmaToEpsilon->set_edge_condition(edge_con_to_epsilon);
							sigma->add_out(edgeID);
							epsilon->add_in(edgeID);

							edlist.push_back(sigmaToEpsilon);
							edgeID++;

							tauToEpsilon->set_id(edgeID);
							tauToEpsilon->set_edge_condition(edge_con_to_epsilon);
							tau->add_out(edgeID);
							epsilon->add_in(edgeID);

							edlist.push_back(tauToEpsilon);
							edgeID++;

						} else { // theta_i == \top
							BFNode* tempNodeDelta = new BFNode();
							tempNodeDelta->node_type = fluForm;
							tempNodeDelta->fluform = deltaFF;

							nodeSigmaTauAndTauSigma->node_type = propForm;
							nodeSigmaTauAndTauSigma->bfOperator = BFNOT;
							nodeSigmaTauAndTauSigma->bfnode1 = tempNodeDelta;

							edge_con_sigma_tau_and_tau_sigma->push_back(*nodeSigmaTauAndTauSigma);

							sigma->add_out(edgeID);
							sigma->add_in(edgeID);

							edlist.push_back(sigmaToSigma);
							edgeID++;

							tauToTau->set_id(edgeID);
							tau->add_out(edgeID);
							tau->add_in(edgeID);

							edlist.push_back(tauToTau);
							edgeID++;

							sigmaToTau->set_id(edgeID);
							sigmaToTau->set_edge_condition(edge_con_sigma_tau_and_tau_sigma);
							sigma->add_out(edgeID);
							tau->add_in(edgeID);

							edlist.push_back(sigmaToTau);
							edgeID++;

							tauToSigma->set_id(edgeID);
							tauToSigma->set_edge_condition(edge_con_sigma_tau_and_tau_sigma);
							tau->add_out(edgeID);
							sigma->add_in(edgeID);

							edlist.push_back(tauToSigma);
							edgeID++;

						}
						
					} else { // Does not exist theta_i
						nodeSigmaSigmaAndTauTau->node_type = fluForm;
						nodeSigmaSigmaAndTauTau->fluform = deltaFF;


						nodeToEpsilon->node_type = propForm;
						nodeToEpsilon->bfOperator = BFNOT;
						nodeToEpsilon->bfnode1 = nodeSigmaSigmaAndTauTau;

						edge_con_sigma_sigma_and_tau_tau->push_back(*nodeSigmaSigmaAndTauTau);
						edge_con_to_epsilon->push_back(*nodeToEpsilon);

						sigmaToSigma->set_edge_condition(edge_con_sigma_sigma_and_tau_tau);
						sigma->add_out(edgeID);
						sigma->add_in(edgeID);

						edlist.push_back(sigmaToSigma);
						edgeID++;
						
						tauToTau->set_id(edgeID);
						tauToTau->set_edge_condition(edge_con_sigma_sigma_and_tau_tau);
						tau->add_out(edgeID);
						tau->add_in(edgeID);

						edlist.push_back(tauToTau);
						edgeID++;

						sigmaToEpsilon->set_id(edgeID);
						sigmaToEpsilon->set_edge_condition(edge_con_to_epsilon);
						sigma->add_out(edgeID);
						epsilon->add_in(edgeID);

						edlist.push_back(sigmaToEpsilon);
						edgeID++;

						tauToEpsilon->set_id(edgeID);
						tauToEpsilon->set_edge_condition(edge_con_to_epsilon);
						tau->add_out(edgeID);
						epsilon->add_in(edgeID);

						edlist.push_back(tauToEpsilon);
						edgeID++;

					}
							
				} else { // delta_i == /top
					sigma->add_out(edgeID);
					sigma->add_in(edgeID);

					edlist.push_back(sigmaToSigma);
					edgeID++;

					tauToTau->set_id(edgeID);
					tau->add_out(edgeID);
					tau->add_in(edgeID);

					edlist.push_back(tauToTau);
					edgeID++;
				} 
				
			} else{ // Does not exist delta_i
				if (hasTheta){ //Check if exist theta_i
					if (needThetaFF){ // Check if theta_i != /top
						nodeSigmaSigmaAndTauTau->node_type = fluForm;
						nodeSigmaSigmaAndTauTau->fluform = thetaFF;

						nodeSigmaTauAndTauSigma->node_type = fluForm;
						nodeSigmaTauAndTauSigma->fluform = thetaFF;

						nodeToEpsilon->node_type = propForm;
						nodeToEpsilon->bfOperator = BFNOT;
						nodeToEpsilon->bfnode1 = nodeSigmaSigmaAndTauTau;

						edge_con_sigma_sigma_and_tau_tau->push_back(*nodeSigmaSigmaAndTauTau);
						edge_con_sigma_tau_and_tau_sigma->push_back(*nodeSigmaTauAndTauSigma);
						edge_con_to_epsilon->push_back(*nodeToEpsilon);

						sigmaToSigma->set_edge_condition(edge_con_sigma_sigma_and_tau_tau);
						sigma->add_out(edgeID);
						sigma->add_in(edgeID);

						edlist.push_back(sigmaToSigma);
						edgeID++;

						tauToTau->set_id(edgeID);
						tauToTau->set_edge_condition(edge_con_sigma_sigma_and_tau_tau);
						tau->add_out(edgeID);
						tau->add_in(edgeID);

						edlist.push_back(tauToTau);
						edgeID++;

						sigmaToTau->set_id(edgeID);
						sigmaToTau->set_edge_condition(edge_con_sigma_tau_and_tau_sigma);
						sigma->add_out(edgeID);
						tau->add_in(edgeID);

						edlist.push_back(sigmaToTau);
						edgeID++;

						tauToSigma->set_id(edgeID);
						tauToSigma->set_edge_condition(edge_con_sigma_tau_and_tau_sigma);
						tau->add_out(edgeID);
						sigma->add_in(edgeID);

						edlist.push_back(tauToSigma);
						edgeID++;

						sigmaToEpsilon->set_id(edgeID);
						sigmaToEpsilon->set_edge_condition(edge_con_to_epsilon);
						sigma->add_out(edgeID);
						epsilon->add_in(edgeID);

						edlist.push_back(sigmaToEpsilon);
						edgeID++;

						tauToEpsilon->set_id(edgeID);
						tauToEpsilon->set_edge_condition(edge_con_to_epsilon);
						tau->add_out(edgeID);
						epsilon->add_in(edgeID);

						edlist.push_back(tauToEpsilon);
						edgeID++;
					} else{ //Theta_i == /top

						sigma->add_out(edgeID);
						sigma->add_in(edgeID);

						edlist.push_back(sigmaToSigma);
						edgeID++;

						tauToTau->set_id(edgeID);
						tau->add_out(edgeID);
						tau->add_in(edgeID);

						edlist.push_back(tauToTau);
						edgeID++;

						sigmaToTau->set_id(edgeID);
						sigma->add_out(edgeID);
						tau->add_in(edgeID);

						edlist.push_back(sigmaToTau);
						edgeID++;

						tauToSigma->set_id(edgeID);
						tau->add_out(edgeID);
						sigma->add_in(edgeID);

						edlist.push_back(tauToSigma);
						edgeID++;
					}
				} else {
					sigmaToEpsilon->set_id(edgeID);
					sigma->add_out(edgeID);
					epsilon->add_in(edgeID);

					edlist.push_back(sigmaToEpsilon);
					edgeID++;

					tauToEpsilon->set_id(edgeID);
					tau->add_out(edgeID);
					epsilon->add_in(edgeID);

					edlist.push_back(tauToEpsilon);
					edgeID++;
				}
			}

			epsilonToEpsilon->set_id(edgeID);
			epsilon->add_out(edgeID);
			epsilon->add_in(edgeID);

			edlist.push_back(epsilonToEpsilon);
			edgeID++;

		}
		
		/*//Edges for full observers
		for(itag = fd.begin(); itag != fd.end(); itag++){
			Kedge* sigmaEdge = new Kedge();
			sigmaEdge->set_id(edgeID);
			sigmaEdge->set_from(0);
			sigmaEdge->set_to(0);
			sigmaEdge->set_label(*itag);
			sigmaEdge->set_exist(true);
			sigmaEdge->set_inUpdateModel(true);

			sigma->add_out(edgeID);
			sigma->add_in(edgeID);

			edlist.push_back(sigmaEdge);
			edgeID++;



			Kedge* tauEdge = new Kedge();
			tauEdge->set_id(edgeID);
			tauEdge->set_from(1);
			tauEdge->set_to(1);
			tauEdge->set_label(*itag);
			tauEdge->set_exist(true);
			tauEdge->set_inUpdateModel(true);

			tau->add_out(edgeID);
			tau->add_in(edgeID);

			edlist.push_back(tauEdge);
			edgeID++;



			Kedge* epsilonEdge = new Kedge();
			epsilonEdge->set_id(edgeID);
			epsilonEdge->set_from(2);
			epsilonEdge->set_to(2);
			epsilonEdge->set_label(*itag);
			epsilonEdge->set_exist(true);
			epsilonEdge->set_inUpdateModel(true);

			epsilon->add_out(edgeID);
			epsilon->add_in(edgeID);

			edlist.push_back(epsilonEdge);
			edgeID++;
		}

		//Edges for partial observers
		for(itag = pd.begin(); itag != pd.end(); itag++){
			Kedge* sigmaEdge = new Kedge();
			sigmaEdge->set_id(edgeID);
			sigmaEdge->set_from(0);
			sigmaEdge->set_to(0);
			sigmaEdge->set_label(*itag);
			sigmaEdge->set_exist(true);
			sigmaEdge->set_inUpdateModel(true);

			sigma->add_out(edgeID);
			sigma->add_in(edgeID);

			edlist.push_back(sigmaEdge);
			edgeID++;



			Kedge* tauEdge = new Kedge();
			tauEdge->set_id(edgeID);
			tauEdge->set_from(1);
			tauEdge->set_to(1);
			tauEdge->set_label(*itag);
			tauEdge->set_exist(true);
			tauEdge->set_inUpdateModel(true);

			tau->add_out(edgeID);
			tau->add_in(edgeID);

			edlist.push_back(tauEdge);
			edgeID++;


			Kedge* sigmaTauEdge = new Kedge();
			sigmaTauEdge->set_id(edgeID);
			sigmaTauEdge->set_from(0);
			sigmaTauEdge->set_to(1);
			sigmaTauEdge->set_label(*itag);
			sigmaTauEdge->set_exist(true);
			sigmaTauEdge->set_inUpdateModel(true);

			sigma->add_out(edgeID);
			tau->add_in(edgeID);

			edlist.push_back(sigmaTauEdge);
			edgeID++;



			Kedge* tauSigmaEdge = new Kedge();
			tauSigmaEdge->set_id(edgeID);
			tauSigmaEdge->set_from(1);
			tauSigmaEdge->set_to(0);
			tauSigmaEdge->set_label(*itag);
			tauSigmaEdge->set_exist(true);
			tauSigmaEdge->set_inUpdateModel(true);

			tau->add_out(edgeID);
			sigma->add_in(edgeID);

			edlist.push_back(tauSigmaEdge);
			edgeID++;



			Kedge* epsilonEdge = new Kedge();
			epsilonEdge->set_id(edgeID);
			epsilonEdge->set_from(2);
			epsilonEdge->set_to(2);
			epsilonEdge->set_label(*itag);
			epsilonEdge->set_exist(true);
			epsilonEdge->set_inUpdateModel(true);

			epsilon->add_out(edgeID);
			epsilon->add_in(edgeID);

			edlist.push_back(epsilonEdge);
			edgeID++;
		}

		//Edges for oblivous
		for(itag = od.begin(); itag != od.end(); itag++){
			Kedge* sigmaEdge = new Kedge();
			sigmaEdge->set_id(edgeID);
			sigmaEdge->set_from(0);
			sigmaEdge->set_to(2);
			sigmaEdge->set_label(*itag);
			sigmaEdge->set_exist(true);
			sigmaEdge->set_inUpdateModel(true);

			sigma->add_out(edgeID);
			epsilon->add_in(edgeID);

			edlist.push_back(sigmaEdge);
			edgeID++;


			Kedge* tauEdge = new Kedge();
			tauEdge->set_id(edgeID);
			tauEdge->set_from(1);
			tauEdge->set_to(2);
			tauEdge->set_label(*itag);
			tauEdge->set_exist(true);
			tauEdge->set_inUpdateModel(true);

			tau->add_out(edgeID);
			epsilon->add_in(edgeID);

			edlist.push_back(tauEdge);
			edgeID++;


			Kedge* epsilonEdge = new Kedge();
			epsilonEdge->set_id(edgeID);
			epsilonEdge->set_from(2);
			epsilonEdge->set_to(2);
			epsilonEdge->set_label(*itag);
			epsilonEdge->set_exist(true);
			epsilonEdge->set_inUpdateModel(true);

			epsilon->add_out(edgeID);
			epsilon->add_in(edgeID);

			edlist.push_back(epsilonEdge);

			edgeID++;
		}*/

		Events evlist;
		evlist.push_back(*sigma);
		evlist.push_back(*tau);
		evlist.push_back(*epsilon);
		
		upmodel->set_eventslist(evlist);
		upmodel->set_edgeslist(edlist);
		upmodel->add_true_event(sigma->get_id());
	} else {
		return NULL;
	}

	// if (lying){
		// cout << "Update model for action " << a.get_name() << endl;
	// 	upmodel->print_report();
	// }
	// cout << "Update model for action " << a.get_name() << endl;
	// upmodel->print_report();


	// if (lying){
	// 	cout << "Lying: " << a.get_name() << endl;
	// } else if (misleading) {
	// 	cout << "Misleading: " << a.get_name() << endl;
	// } else if (truthful){
	// 	cout << "Truthful: " << a.get_name() << endl;
	// }

	Kripke* m1 = cross_product(k, *upmodel);
	// m1->print_report();
	// cout << "Action name: " << a.get_name() << endl;
	Kripke* m2 = update_kripke(*m1);
	// m2->print_report();
	/*
	if(!(m2->isValid(m_agents)))
	{
	return NULL;
	}
	*/
	return m2;
}

//%%%%%%%%%%%---------- Execute sensing action-----------------%%%%%%%%%%%%%
Kripke* Planner::execute_sensing(Kripke k, KAction a)
{
	// int oldpId = k.get_id();

	if (!is_executable(k, a)) {
		return NULL;
	}
	AgentSet fd = get_fd(a, k);
	AgentSet od = get_od(a, k);
	AgentSet pd = get_pd(a, k);

	// cout << "Action name: " << a.get_name() << endl;
	// AgentSet executors = get_executor(a);
	// AgentSet::iterator itera;
	// for(itera = executors.begin();itera != executors.end(); itera++)
	// {
	// 	cout << "Executor: " << *itera << endl;;
	// }

	/*
	AgentSet::iterator itera;
	for(itera = fd.begin();itera != fd.end(); itera++)
	{
	cout << "fd: " << *itera;
	}
	cout << endl;
	for(itera = pd.begin();itera != pd.end(); itera++)
	{
	cout << "pd: " << *itera;
	}
	cout << endl;
	for(itera = od.begin();itera != od.end(); itera++)
	{
	cout << "od: " << *itera;
	}
	cout << endl;
	*/

	Literals sensedFlu = a.get_sensedFluents();

	/*****************Correction of false beliefs**********************/
	
	//We check all the fully observant
	AgentSet::iterator it_fd;
	Literals::const_iterator it_li;

	//Transform into FluentsFormula single fluent
	list<FluentFormula> toEntail;

	for (it_li = sensedFlu.begin(); it_li != sensedFlu.end(); it_li++) {
		Fluents tmp;
		tmp.insert(*it_li);
		FluentFormula ff;
		ff.insert(tmp);
		toEntail.push_back(ff);
	}
	list<FluentFormula>::iterator it_lff;

	Edges to_add;
	int id_edge = k.get_edgeslist().size();

	for (it_fd = fd.begin();it_fd != fd.end();it_fd++) {
		for (it_lff = toEntail.begin(); it_lff != toEntail.end(); it_lff++) {
			k.correctFalseBeliefs(*it_lff, *it_fd, id_edge, to_add);
		}
	}

	//Add all the found edges
	Edges::iterator it_ed;
	for (it_ed = to_add.begin();it_ed != to_add.end();it_ed++) {
		k.add_edge(*it_ed);
	}
    
	/******************************************************************/

	vector<Kripke*> result;
	result.push_back(&k);
	// int index = 0;

	Literals::const_iterator itTemp;

	for (itTemp = sensedFlu.begin(); itTemp != sensedFlu.end(); itTemp++) {
		// result[index] = new Kripke();
		Event* sigma = new Event();
		sigma->set_id(0);
		sigma->set_name("Sigma");
		// sigma->set_pre_sub(*a.get_effects());

		Event* tau = new Event();
		tau->set_id(1);
		tau->set_name("Tau");

		Event* epsilon = new Event();
		epsilon->set_id(2);
		epsilon->set_name("Epsilon");


		Fluents_BF_list pre_sub_sigma = Fluents_BF_list();

		Fluents_BF addSigma = Fluents_BF();
		BFNode* addNodeSigma = new BFNode();
		addNodeSigma->node_type = fluForm;		

		Fluents* tmp = new Fluents();
		tmp->insert(*itTemp);

		// cout << "Sigma " << *itTemp << endl;

		FluentFormula* ff = new FluentFormula();
		ff->insert(*tmp);
		addNodeSigma->set_flu(ff);

		// cout << "Size Sigma" << addNodeSigma->fluform->size() << endl;

		string toString = to_string(*itTemp);
		StringList toStrL;
		toStrL.insert(toString);
		addNodeSigma->flu_form.insert(toStrL);

		addSigma.set_bn_body(*addNodeSigma);

		pre_sub_sigma.push_back(addSigma);


		sigma->set_pre_sub(pre_sub_sigma);


		Fluents_BF_list pre_sub_tau = Fluents_BF_list();


		Fluents_BF addTau = Fluents_BF();
		BFNode* addNodeTau = new BFNode();
		addNodeTau->node_type = fluForm;

		Fluent* negTemp = new Fluent();
		NEGATE(*itTemp, *negTemp);

		Fluents* tmp2 = new Fluents();
		tmp2->insert(*negTemp);

		// cout << "Tau " << negTemp << endl;

		FluentFormula* ff2 = new FluentFormula();
		ff2->insert(*tmp2);
		addNodeTau->set_flu(ff2);

		string toString2 = to_string(*negTemp);
		StringList toStrL2;
		toStrL2.insert(toString2);
		addNodeTau->flu_form.insert(toStrL2);


		addTau.set_bn_body(*addNodeTau);

		pre_sub_tau.push_back(addTau);


		tau->set_pre_sub(pre_sub_tau);


		Edges edlist;

		AgentSet::iterator itag;
		int edgeID = 0;

		for (itag = m_agents.begin(); itag != m_agents.end(); itag++){
			FluentFormula* deltaFF = new FluentFormula();
			FluentFormula* thetaFF = new FluentFormula();
			bool hasDelta = false;
			bool hasTheta = false;
			bool needDeltaFF = false;
			bool needThetaFF = false;

			//Edge from Sigma to Sigma
			Kedge* sigmaToSigma = new Kedge();
			sigmaToSigma->set_id(edgeID);
			sigmaToSigma->set_from(0);
			sigmaToSigma->set_to(0);
			sigmaToSigma->set_label(*itag);
			sigmaToSigma->set_exist(true);
			sigmaToSigma->set_inUpdateModel(true);

			//Edge from Tau to Tau
			Kedge* tauToTau = new Kedge();
			tauToTau->set_from(1);
			tauToTau->set_to(1);
			tauToTau->set_label(*itag);
			tauToTau->set_exist(true);
			tauToTau->set_inUpdateModel(true);

			//Edge from Sigma to Tau
			Kedge* sigmaToTau = new Kedge();
			sigmaToTau->set_from(0);
			sigmaToTau->set_to(1);
			sigmaToTau->set_label(*itag);
			sigmaToTau->set_exist(true);
			sigmaToTau->set_inUpdateModel(true);

			//Edge from Tau to Sigma
			Kedge* tauToSigma = new Kedge();
			tauToSigma->set_from(1);
			tauToSigma->set_to(1);
			tauToSigma->set_label(*itag);
			tauToSigma->set_exist(true);
			tauToSigma->set_inUpdateModel(true);

			//Edge from Sigma to Epsilon
			Kedge* sigmaToEpsilon = new Kedge();
			sigmaToEpsilon->set_from(0);
			sigmaToEpsilon->set_to(2);
			sigmaToEpsilon->set_label(*itag);
			sigmaToEpsilon->set_exist(true);
			sigmaToEpsilon->set_inUpdateModel(true);

			//Edge from Tau to Epsilon
			Kedge* tauToEpsilon = new Kedge();
			tauToEpsilon->set_from(1);
			tauToEpsilon->set_to(2);
			tauToEpsilon->set_label(*itag);
			tauToEpsilon->set_exist(true);
			tauToEpsilon->set_inUpdateModel(true);

			//Edge from Epsilon to Epsilon
			Kedge* epsilonToEpsilon = new Kedge();
			epsilonToEpsilon->set_from(2);
			epsilonToEpsilon->set_to(2);
			epsilonToEpsilon->set_label(*itag);
			epsilonToEpsilon->set_exist(true);
			epsilonToEpsilon->set_inUpdateModel(true);


			Agent_FF_list::iterator iaTemp;
			Agent_FF_list temp = a.get_m_observes();
			//Get Delta
			for (iaTemp = temp.begin(); iaTemp != temp.end(); iaTemp++){
				if (*itag == iaTemp->get_head()){
					if (iaTemp->get_body().size() != 0){
						needDeltaFF = true;
						FluentFormula ff = iaTemp->get_body();
						*deltaFF = ff;
					}
					hasDelta = true;
					break;
				}
			}

			temp = a.get_m_awares();
			//Get Theta
			for (iaTemp = temp.begin(); iaTemp != temp.end(); iaTemp++){
				if (*itag == iaTemp->get_head()){
					if (iaTemp->get_body().size() != 0){
						needThetaFF = true;
						FluentFormula ff = iaTemp->get_body();
						*thetaFF = ff;
					}
					hasTheta = true;
					break;
				}
			}

			Nodes* edge_con_sigma_sigma_and_tau_tau = new Nodes();
			Nodes* edge_con_sigma_tau_and_tau_sigma = new Nodes();
			Nodes* edge_con_to_epsilon = new Nodes();
			BFNode* nodeSigmaSigmaAndTauTau = new BFNode();
			BFNode* nodeSigmaTauAndTauSigma = new BFNode();
			BFNode* nodeToEpsilon = new BFNode();

			if (hasDelta){ //Check if exist delta_i 
				if (needDeltaFF){ //Check if delta_i != /top
					if (hasTheta){ //Check if exist theta_i
						if (needThetaFF){ //Check if theta_i != /top
							BFNode* tempNodeDelta = new BFNode();
							tempNodeDelta->node_type = fluForm;
							tempNodeDelta->fluform = deltaFF;

							BFNode* tempNodeTheta = new BFNode();
							tempNodeTheta->node_type = fluForm;
							tempNodeTheta->fluform = thetaFF;

							nodeSigmaSigmaAndTauTau->node_type = propForm;
							nodeSigmaSigmaAndTauTau->bfOperator = BFOR;
							nodeSigmaSigmaAndTauTau->bfnode1 = tempNodeDelta;
							nodeSigmaSigmaAndTauTau->bfnode2 = tempNodeTheta;

							BFNode* tempNodeNegDelta = new BFNode();
							tempNodeNegDelta->node_type = propForm;
							tempNodeNegDelta->bfOperator = BFNOT;
							tempNodeNegDelta->bfnode1 = tempNodeDelta;

							BFNode* tempNodeNegTheta = new BFNode();
							tempNodeNegTheta->node_type = propForm;
							tempNodeNegTheta->bfOperator = BFNOT;
							tempNodeNegTheta->bfnode1 = tempNodeTheta;

							nodeSigmaTauAndTauSigma->node_type = propForm;
							nodeSigmaTauAndTauSigma->bfOperator = BFAND;
							nodeSigmaTauAndTauSigma->bfnode1 = tempNodeNegDelta;
							nodeSigmaTauAndTauSigma->bfnode2 = tempNodeTheta;

							nodeToEpsilon->node_type = propForm;
							nodeToEpsilon->bfOperator = BFAND;
							nodeToEpsilon->bfnode1 = tempNodeNegDelta;
							nodeToEpsilon->bfnode2 = tempNodeNegTheta;

							edge_con_sigma_sigma_and_tau_tau->push_back(*nodeSigmaSigmaAndTauTau);
							edge_con_sigma_tau_and_tau_sigma->push_back(*nodeSigmaTauAndTauSigma);
							edge_con_to_epsilon->push_back(*nodeToEpsilon);

							sigmaToSigma->set_edge_condition(edge_con_sigma_sigma_and_tau_tau);
							sigma->add_out(edgeID);
							sigma->add_in(edgeID);

							edlist.push_back(sigmaToSigma);
							edgeID++;

							tauToTau->set_id(edgeID);
							tauToTau->set_edge_condition(edge_con_sigma_sigma_and_tau_tau);
							tau->add_out(edgeID);
							tau->add_in(edgeID);

							edlist.push_back(tauToTau);
							edgeID++;

							sigmaToTau->set_id(edgeID);
							sigmaToTau->set_edge_condition(edge_con_sigma_tau_and_tau_sigma);
							sigma->add_out(edgeID);
							tau->add_in(edgeID);

							edlist.push_back(sigmaToTau);
							edgeID++;

							tauToSigma->set_id(edgeID);
							tauToSigma->set_edge_condition(edge_con_sigma_tau_and_tau_sigma);
							tau->add_out(edgeID);
							sigma->add_in(edgeID);

							edlist.push_back(tauToSigma);
							edgeID++;

							sigmaToEpsilon->set_id(edgeID);
							sigmaToEpsilon->set_edge_condition(edge_con_to_epsilon);
							sigma->add_out(edgeID);
							epsilon->add_in(edgeID);

							edlist.push_back(sigmaToEpsilon);
							edgeID++;

							tauToEpsilon->set_id(edgeID);
							tauToEpsilon->set_edge_condition(edge_con_to_epsilon);
							tau->add_out(edgeID);
							epsilon->add_in(edgeID);

							edlist.push_back(tauToEpsilon);
							edgeID++;

						} else { // theta_i == \top
							BFNode* tempNodeDelta = new BFNode();
							tempNodeDelta->node_type = fluForm;
							tempNodeDelta->fluform = deltaFF;

							nodeSigmaTauAndTauSigma->node_type = propForm;
							nodeSigmaTauAndTauSigma->bfOperator = BFNOT;
							nodeSigmaTauAndTauSigma->bfnode1 = tempNodeDelta;

							edge_con_sigma_tau_and_tau_sigma->push_back(*nodeSigmaTauAndTauSigma);

							sigma->add_out(edgeID);
							sigma->add_in(edgeID);

							edlist.push_back(sigmaToSigma);
							edgeID++;

							tauToTau->set_id(edgeID);
							tau->add_out(edgeID);
							tau->add_in(edgeID);

							edlist.push_back(tauToTau);
							edgeID++;

							sigmaToTau->set_id(edgeID);
							sigmaToTau->set_edge_condition(edge_con_sigma_tau_and_tau_sigma);
							sigma->add_out(edgeID);
							tau->add_in(edgeID);

							edlist.push_back(sigmaToTau);
							edgeID++;

							tauToSigma->set_id(edgeID);
							tauToSigma->set_edge_condition(edge_con_sigma_tau_and_tau_sigma);
							tau->add_out(edgeID);
							sigma->add_in(edgeID);

							edlist.push_back(tauToSigma);
							edgeID++;

						}
						
					} else { // Does not exist theta_i
						nodeSigmaSigmaAndTauTau->node_type = fluForm;
						nodeSigmaSigmaAndTauTau->fluform = deltaFF;


						nodeToEpsilon->node_type = propForm;
						nodeToEpsilon->bfOperator = BFNOT;
						nodeToEpsilon->bfnode1 = nodeSigmaSigmaAndTauTau;

						edge_con_sigma_sigma_and_tau_tau->push_back(*nodeSigmaSigmaAndTauTau);
						edge_con_to_epsilon->push_back(*nodeToEpsilon);

						sigmaToSigma->set_edge_condition(edge_con_sigma_sigma_and_tau_tau);
						sigma->add_out(edgeID);
						sigma->add_in(edgeID);

						edlist.push_back(sigmaToSigma);
						edgeID++;
						
						tauToTau->set_id(edgeID);
						tauToTau->set_edge_condition(edge_con_sigma_sigma_and_tau_tau);
						tau->add_out(edgeID);
						tau->add_in(edgeID);

						edlist.push_back(tauToTau);
						edgeID++;

						sigmaToEpsilon->set_id(edgeID);
						sigmaToEpsilon->set_edge_condition(edge_con_to_epsilon);
						sigma->add_out(edgeID);
						epsilon->add_in(edgeID);

						edlist.push_back(sigmaToEpsilon);
						edgeID++;

						tauToEpsilon->set_id(edgeID);
						tauToEpsilon->set_edge_condition(edge_con_to_epsilon);
						tau->add_out(edgeID);
						epsilon->add_in(edgeID);

						edlist.push_back(tauToEpsilon);
						edgeID++;

					}
							
				} else { // delta_i == /top
					sigma->add_out(edgeID);
					sigma->add_in(edgeID);

					edlist.push_back(sigmaToSigma);
					edgeID++;

					tauToTau->set_id(edgeID);
					tau->add_out(edgeID);
					tau->add_in(edgeID);

					edlist.push_back(tauToTau);
					edgeID++;
				} 
				
			} else{ // Does not exist delta_i
				if (hasTheta){ //Check if exist theta_i
					if (needThetaFF){ // Check if theta_i != /top
						nodeSigmaSigmaAndTauTau->node_type = fluForm;
						nodeSigmaSigmaAndTauTau->fluform = thetaFF;

						nodeSigmaTauAndTauSigma->node_type = fluForm;
						nodeSigmaTauAndTauSigma->fluform = thetaFF;

						nodeToEpsilon->node_type = propForm;
						nodeToEpsilon->bfOperator = BFNOT;
						nodeToEpsilon->bfnode1 = nodeSigmaSigmaAndTauTau;

						edge_con_sigma_sigma_and_tau_tau->push_back(*nodeSigmaSigmaAndTauTau);
						edge_con_sigma_tau_and_tau_sigma->push_back(*nodeSigmaTauAndTauSigma);
						edge_con_to_epsilon->push_back(*nodeToEpsilon);

						sigmaToSigma->set_edge_condition(edge_con_sigma_sigma_and_tau_tau);
						sigma->add_out(edgeID);
						sigma->add_in(edgeID);

						edlist.push_back(sigmaToSigma);
						edgeID++;

						tauToTau->set_id(edgeID);
						tauToTau->set_edge_condition(edge_con_sigma_sigma_and_tau_tau);
						tau->add_out(edgeID);
						tau->add_in(edgeID);

						edlist.push_back(tauToTau);
						edgeID++;

						sigmaToTau->set_id(edgeID);
						sigmaToTau->set_edge_condition(edge_con_sigma_tau_and_tau_sigma);
						sigma->add_out(edgeID);
						tau->add_in(edgeID);

						edlist.push_back(sigmaToTau);
						edgeID++;

						tauToSigma->set_id(edgeID);
						tauToSigma->set_edge_condition(edge_con_sigma_tau_and_tau_sigma);
						tau->add_out(edgeID);
						sigma->add_in(edgeID);

						edlist.push_back(tauToSigma);
						edgeID++;

						sigmaToEpsilon->set_id(edgeID);
						sigmaToEpsilon->set_edge_condition(edge_con_to_epsilon);
						sigma->add_out(edgeID);
						epsilon->add_in(edgeID);

						edlist.push_back(sigmaToEpsilon);
						edgeID++;

						tauToEpsilon->set_id(edgeID);
						tauToEpsilon->set_edge_condition(edge_con_to_epsilon);
						tau->add_out(edgeID);
						epsilon->add_in(edgeID);

						edlist.push_back(tauToEpsilon);
						edgeID++;
					} else{ //Theta_i == /top

						sigma->add_out(edgeID);
						sigma->add_in(edgeID);

						edlist.push_back(sigmaToSigma);
						edgeID++;

						tauToTau->set_id(edgeID);
						tau->add_out(edgeID);
						tau->add_in(edgeID);

						edlist.push_back(tauToTau);
						edgeID++;

						sigmaToTau->set_id(edgeID);
						sigma->add_out(edgeID);
						tau->add_in(edgeID);

						edlist.push_back(sigmaToTau);
						edgeID++;

						tauToSigma->set_id(edgeID);
						tau->add_out(edgeID);
						sigma->add_in(edgeID);

						edlist.push_back(tauToSigma);
						edgeID++;
					}
				} else {
					sigmaToEpsilon->set_id(edgeID);
					sigma->add_out(edgeID);
					epsilon->add_in(edgeID);

					edlist.push_back(sigmaToEpsilon);
					edgeID++;

					tauToEpsilon->set_id(edgeID);
					tau->add_out(edgeID);
					epsilon->add_in(edgeID);

					edlist.push_back(tauToEpsilon);
					edgeID++;
				}
			}

			epsilonToEpsilon->set_id(edgeID);
			epsilon->add_out(edgeID);
			epsilon->add_in(edgeID);

			edlist.push_back(epsilonToEpsilon);
			edgeID++;

		}
		

		/*for(itag = fd.begin(); itag != fd.end(); itag++){
			Kedge* sigmaEdge = new Kedge();
			sigmaEdge->set_id(edgeID);
			sigmaEdge->set_from(0);
			sigmaEdge->set_to(0);
			sigmaEdge->set_label(*itag);
			sigmaEdge->set_exist(true);
			sigmaEdge->set_inUpdateModel(true);

			sigma->add_out(edgeID);
			sigma->add_in(edgeID);

			edlist.push_back(sigmaEdge);
			edgeID++;



			Kedge* tauEdge = new Kedge();
			tauEdge->set_id(edgeID);
			tauEdge->set_from(1);
			tauEdge->set_to(1);
			tauEdge->set_label(*itag);
			tauEdge->set_exist(true);
			tauEdge->set_inUpdateModel(true);

			tau->add_out(edgeID);
			tau->add_in(edgeID);

			edlist.push_back(tauEdge);
			edgeID++;



			Kedge* epsilonEdge = new Kedge();
			epsilonEdge->set_id(edgeID);
			epsilonEdge->set_from(2);
			epsilonEdge->set_to(2);
			epsilonEdge->set_label(*itag);
			epsilonEdge->set_exist(true);
			epsilonEdge->set_inUpdateModel(true);

			epsilon->add_out(edgeID);
			epsilon->add_in(edgeID);

			edlist.push_back(epsilonEdge);
			edgeID++;
		}

		for(itag = pd.begin(); itag != pd.end(); itag++){
			Kedge* sigmaEdge = new Kedge();
			sigmaEdge->set_id(edgeID);
			sigmaEdge->set_from(0);
			sigmaEdge->set_to(0);
			sigmaEdge->set_label(*itag);
			sigmaEdge->set_exist(true);
			sigmaEdge->set_inUpdateModel(true);

			sigma->add_out(edgeID);
			sigma->add_in(edgeID);

			edlist.push_back(sigmaEdge);
			edgeID++;



			Kedge* tauEdge = new Kedge();
			tauEdge->set_id(edgeID);
			tauEdge->set_from(1);
			tauEdge->set_to(1);
			tauEdge->set_label(*itag);
			tauEdge->set_exist(true);
			tauEdge->set_inUpdateModel(true);

			tau->add_out(edgeID);
			tau->add_in(edgeID);

			edlist.push_back(tauEdge);
			edgeID++;


			Kedge* sigmaTauEdge = new Kedge();
			sigmaTauEdge->set_id(edgeID);
			sigmaTauEdge->set_from(0);
			sigmaTauEdge->set_to(1);
			sigmaTauEdge->set_label(*itag);
			sigmaTauEdge->set_exist(true);
			sigmaTauEdge->set_inUpdateModel(true);

			sigma->add_out(edgeID);
			tau->add_in(edgeID);

			edlist.push_back(sigmaTauEdge);
			edgeID++;



			Kedge* tauSigmaEdge = new Kedge();
			tauSigmaEdge->set_id(edgeID);
			tauSigmaEdge->set_from(1);
			tauSigmaEdge->set_to(0);
			tauSigmaEdge->set_label(*itag);
			tauSigmaEdge->set_exist(true);
			tauSigmaEdge->set_inUpdateModel(true);

			tau->add_out(edgeID);
			sigma->add_in(edgeID);

			edlist.push_back(tauSigmaEdge);
			edgeID++;



			Kedge* epsilonEdge = new Kedge();
			epsilonEdge->set_id(edgeID);
			epsilonEdge->set_from(2);
			epsilonEdge->set_to(2);
			epsilonEdge->set_label(*itag);
			epsilonEdge->set_exist(true);
			epsilonEdge->set_inUpdateModel(true);

			epsilon->add_out(edgeID);
			epsilon->add_in(edgeID);

			edlist.push_back(epsilonEdge);
			edgeID++;
		}

		for(itag = od.begin(); itag != od.end(); itag++){
			Kedge* sigmaEdge = new Kedge();
			sigmaEdge->set_id(edgeID);
			sigmaEdge->set_from(0);
			sigmaEdge->set_to(2);
			sigmaEdge->set_label(*itag);
			sigmaEdge->set_exist(true);
			sigmaEdge->set_inUpdateModel(true);

			sigma->add_out(edgeID);
			epsilon->add_in(edgeID);

			edlist.push_back(sigmaEdge);
			edgeID++;


			Kedge* tauEdge = new Kedge();
			tauEdge->set_id(edgeID);
			tauEdge->set_from(1);
			tauEdge->set_to(2);
			tauEdge->set_label(*itag);
			tauEdge->set_exist(true);
			tauEdge->set_inUpdateModel(true);

			tau->add_out(edgeID);
			epsilon->add_in(edgeID);

			edlist.push_back(tauEdge);
			edgeID++;


			Kedge* epsilonEdge = new Kedge();
			epsilonEdge->set_id(edgeID);
			epsilonEdge->set_from(2);
			epsilonEdge->set_to(2);
			epsilonEdge->set_label(*itag);
			epsilonEdge->set_exist(true);
			epsilonEdge->set_inUpdateModel(true);

			epsilon->add_out(edgeID);
			epsilon->add_in(edgeID);

			edlist.push_back(epsilonEdge);

			edgeID++;
		}*/


		Events evlist;
		evlist.push_back(*sigma);
		evlist.push_back(*tau);
		evlist.push_back(*epsilon);
		

		UpdateModel* upmodel = new UpdateModel();
		upmodel->set_eventslist(evlist);
		upmodel->set_edgeslist(edlist);
		upmodel->add_true_event(sigma->get_id());
		upmodel->add_true_event(tau->get_id());

		// cout << "Update model for action " << a.get_name() << endl;

		// if (a.get_name().compare("check_1[a]") == 0)
		// upmodel->print_report();

		// cout << "size of result before update: " << result.size() << endl;

		Kripke* mTemp = result.back();

		// cout << "Pre Kripke: " << endl;
		// mTemp->print_report();

		Kripke* m1 = cross_product(mTemp, *upmodel);
		// cout << "After cross-product: " << endl;
		// m1->print_report();
		Kripke* m2 = update_kripke(*m1);
		// cout << "Next Kripke: " << endl;
		// m2->print_report();
    	result.push_back(m2);
		// cout << "size of result after update: " << result.size() << endl;
	}
	// m2->print_report();
	/*
	if(!(m2->isValid(m_agents)))
	{
	return NULL;
	}
	*/
	// cout << "size of result after action: " << result.size() << endl;

	return result.back();


}

bool Planner::isSameSatisfyFF(Kstate k1, Kstate k2, FluentFormula check) const
{
	if (k1.entail_fluform(check) && !k2.entail_fluform(check))
	{
		return false;
	}
	else if (!k1.entail_fluform(check) && k2.entail_fluform(check))
	{
		return false;
	}
	return true;
}
bool Planner::isSameInterpret(Kstate k1, Kstate k2, Literals check) const
{
	Literals::iterator it;
	for (it = check.begin();it != check.end();it++)
	{
		if (k1.entail_lit(*it) && !k2.entail_lit(*it)) {
			return false;
		}
		else if (!k1.entail_lit(*it) && k2.entail_lit(*it))
		{
			return false;
		}
	}
	return true;
}
Kripke* Planner::execute_ontic(Kripke k, KAction a)
{
	// int oldpId = k.get_id();

	if (!is_executable(k, a)) {
		return NULL;
	}
    
    //cout << "begin computing fd, pd, od" << endl;
	// AgentSet fd = get_fd(a, k);
	// AgentSet pd = get_pd(a, k);
	// AgentSet od = get_od(a, k);

	// // cout << "Action name: " << a.get_name() << endl;
	// AgentSet executors = get_executor(a);
	// AgentSet::iterator itera;
	// for(itera = executors.begin();itera != executors.end(); itera++)
	// {
	// 	cout << "Executor: " << *itera << endl;;
	// }


	/*
	AgentSet::iterator itera;
	for(itera = fd.begin();itera != fd.end(); itera++)
	{
	cout << "-----fd: " << *itera;
	}
	cout << endl;
	for(itera = pd.begin();itera != pd.end(); itera++)
	{
	cout << "----pd: " << *itera;
	}
	cout << endl;
	for(itera = od.begin();itera != od.end(); itera++)
	{
	cout << "----od: " << *itera;
	}
	cout << endl;
	*/

	Literals lits = compute_effect(&a, &k);

	Event* sigma = new Event();
	sigma->set_id(0);
	sigma->set_name("Sigma");

	const Fluents_BF_list* temp = a.get_effects();

	sigma->set_pre_sub(*temp);


	Event* epsilon = new Event();
	epsilon->set_id(1);
	epsilon->set_name("Epsilon");

	Edges edlist;

	AgentSet::iterator itag;
	int edgeID = 0;

	for (itag = m_agents.begin(); itag != m_agents.end(); itag++){
		//Edge from Sigma to Sigma
		Kedge* sigmaToSigma = new Kedge();
		sigmaToSigma->set_id(edgeID);
		sigmaToSigma->set_from(0);
		sigmaToSigma->set_to(0);
		sigmaToSigma->set_label(*itag);
		sigmaToSigma->set_exist(true);
		sigmaToSigma->set_inUpdateModel(true);
		Nodes* edge_con_sigma_sigma = new Nodes();
		bool need_edge_con = true;
		bool exist = false;

		//Edge from Sigma to Epsilon
		Kedge* sigmaToEpsilon = new Kedge();
		sigmaToEpsilon->set_from(0);
		sigmaToEpsilon->set_to(1);
		sigmaToEpsilon->set_label(*itag);
		sigmaToEpsilon->set_exist(true);
		sigmaToEpsilon->set_inUpdateModel(true);
		Nodes* edge_con_sigma_epsilon = new Nodes();

		Agent_FF_list::iterator iaTemp;
		Agent_FF_list temp = a.get_m_observes();
		for (iaTemp = temp.begin(); iaTemp != temp.end(); iaTemp++){
			if (*itag == iaTemp->get_head()){
				if (iaTemp->get_body().size() == 0){
					need_edge_con = false;
				} else {
					FluentFormula* tempFF = new FluentFormula();
					FluentFormula ff = iaTemp->get_body();

					*tempFF = ff;

					BFNode* nodeSigmaSigma = new BFNode();
					nodeSigmaSigma->node_type = fluForm;
					nodeSigmaSigma->fluform = tempFF;

					edge_con_sigma_sigma->push_back(*nodeSigmaSigma);



					BFNode* nodeSigmaEpsilon = new BFNode();
					nodeSigmaEpsilon->node_type = propForm;
					nodeSigmaEpsilon->bfOperator = BFNOT;
					nodeSigmaEpsilon->bfnode1 = nodeSigmaSigma;

					edge_con_sigma_epsilon->push_back(*nodeSigmaEpsilon);
				}
				exist = true;
				break;
			}
		}

		if (exist) {
			if (need_edge_con){
				sigmaToSigma->set_edge_condition(edge_con_sigma_sigma);
				sigmaToEpsilon->set_edge_condition(edge_con_sigma_epsilon);
			}
			sigma->add_out(edgeID);
			sigma->add_in(edgeID);
			
			edlist.push_back(sigmaToSigma);
			

			if (need_edge_con) {
				edgeID++;
				sigmaToEpsilon->set_id(edgeID);

				sigma->add_out(edgeID);
				epsilon->add_in(edgeID);

				edlist.push_back(sigmaToEpsilon);
			}
		} else {
			sigmaToEpsilon->set_id(edgeID);

			sigma->add_out(edgeID);
			epsilon->add_in(edgeID);

			edlist.push_back(sigmaToEpsilon);
		}

		edgeID++;

		//Edge from Epsilon to Epsilon
		Kedge* epsilonEdge = new Kedge();
		epsilonEdge->set_id(edgeID);
		epsilonEdge->set_from(1);
		epsilonEdge->set_to(1);
		epsilonEdge->set_label(*itag);
		epsilonEdge->set_exist(true);
		epsilonEdge->set_inUpdateModel(true);

		epsilon->add_out(edgeID);
		epsilon->add_in(edgeID);

		edlist.push_back(epsilonEdge);

		edgeID++;
	}
	

	// for(itag = fd.begin(); itag != fd.end(); itag++){
	// 	Kedge* sigmaEdge = new Kedge();
	// 	sigmaEdge->set_id(edgeID);
	// 	sigmaEdge->set_from(0);
	// 	sigmaEdge->set_to(0);
	// 	sigmaEdge->set_label(*itag);
	// 	sigmaEdge->set_exist(true);
	// 	sigmaEdge->set_inUpdateModel(true);

	// 	sigma->add_out(edgeID);
	// 	sigma->add_in(edgeID);

	// 	edlist.push_back(sigmaEdge);

	// 	edgeID++;

	// 	Kedge* epsilonEdge = new Kedge();
	// 	epsilonEdge->set_id(edgeID);
	// 	epsilonEdge->set_from(1);
	// 	epsilonEdge->set_to(1);
	// 	epsilonEdge->set_label(*itag);
	// 	epsilonEdge->set_exist(true);
	// 	epsilonEdge->set_inUpdateModel(true);

	// 	epsilon->add_out(edgeID);
	// 	epsilon->add_in(edgeID);

	// 	edlist.push_back(epsilonEdge);

	// 	edgeID++;
	// }

	// for(itag = od.begin(); itag != od.end(); itag++){
	// 	Kedge* sigmaEdge = new Kedge();
	// 	sigmaEdge->set_id(edgeID);
	// 	sigmaEdge->set_from(0);
	// 	sigmaEdge->set_to(1);
	// 	sigmaEdge->set_label(*itag);
	// 	sigmaEdge->set_exist(true);
	// 	sigmaEdge->set_inUpdateModel(true);

	// 	sigma->add_out(edgeID);
	// 	epsilon->add_in(edgeID);

	// 	edlist.push_back(sigmaEdge);

	// 	edgeID++;

	// 	Kedge* epsilonEdge = new Kedge();
	// 	epsilonEdge->set_id(edgeID);
	// 	epsilonEdge->set_from(1);
	// 	epsilonEdge->set_to(1);
	// 	epsilonEdge->set_label(*itag);
	// 	epsilonEdge->set_exist(true);
	// 	epsilonEdge->set_inUpdateModel(true);

	// 	epsilon->add_out(edgeID);
	// 	epsilon->add_in(edgeID);

	// 	edlist.push_back(epsilonEdge);

	// 	edgeID++;
	// }


	Events evlist;
	evlist.push_back(*sigma);
	evlist.push_back(*epsilon);
	

	UpdateModel* upmodel = new UpdateModel();
	upmodel->set_eventslist(evlist);
	upmodel->set_edgeslist(edlist);
	upmodel->add_true_event(sigma->get_id());

	// cout << "Update model for action " << a.get_name() << endl;

	// if (a.get_name().compare("left[a]") == 0)
		// upmodel->print_report();

	// Kripke* m = new Kripke(&k);

	Kripke* m = cross_product(k, *upmodel);
	// if (a.get_name().compare("left[a]") == 0)
	// 	m->print_report();

	Kripke* m1 = update_kripke(*m);
	// m1->print_report();
	/*
	if(!(m1->isValid(m_agents)))
	{
	return NULL;
	}
	*/
	return m1;

}

Kripke* Planner::cross_product(Kripke k, UpdateModel u)
{
	Kripke* result = new Kripke();
	States newStateList;
	Edges newEdgeList;

	States states_list = k.get_stateslist();
	Events events_list = u.get_eventslist();
	Edges k_edges = k.get_edgeslist();
	Edges u_edges = u.get_edgeslist();

	States::iterator itState;
	Events::iterator itEvent;
	Edges::iterator itEdgeState;
	Edges::iterator itEdgeEvent;

	int stateID = 0;
	//Create states
	for (itEvent = events_list.begin(); itEvent != events_list.end(); itEvent++){
		for (itState = states_list.begin(); itState != states_list.end(); itState++){
			Fluents_BF_list::iterator itFBFList;
			Fluents_BF_list flBFListTemp = itEvent->get_pre_sub();
			Literals *litChanged = new Literals();
			bool isEntail = false;
			for (itFBFList = flBFListTemp.begin(); itFBFList != flBFListTemp.end(); itFBFList++){
				if(itState->entail(*itFBFList->get_body(), k)){
					Literals tempLits = *itFBFList->get_head();
					litChanged->insert(tempLits.begin(), tempLits.end());
					isEntail = true;
				}
			}
			if (flBFListTemp.size() == 0){
				isEntail = true;
			}
			if (!isEntail) {
				stateID++;
				continue;
			}

			Kstate* newKstate = comp_nextstate(*itState, *litChanged);
			EdgeList* newEdgeIn = new EdgeList();
			EdgeList* newEdgeOut = new EdgeList();
			newKstate->set_id(stateID);
			newKstate->set_in(*newEdgeIn);
			newKstate->set_out(*newEdgeOut);

			// cout << "New state " << stateID << " come from state " << itState->get_id() << " and event " << itEvent->get_name() << endl;
			
			vector<int> eventTrue = u.get_id();
			vector<int>::iterator eventTrueIt = find(eventTrue.begin(), eventTrue.end(), itEvent->get_id());
			if ((k.get_id() == itState->get_id()) && (eventTrueIt != eventTrue.end())){
				result->set_pId(stateID);
			}
			stateID++;
			newStateList.push_back(*newKstate);
		}
	}

	//Create edges
	int edgeID = 0;
	int count = 0;
	int matchN = 0;
	for (itEdgeEvent = u_edges.begin(); itEdgeEvent != u_edges.end(); itEdgeEvent ++){
		for (itEdgeState = k_edges.begin(); itEdgeState != k_edges.end(); itEdgeState++){
			// cout << "Edge pair " << count++ << endl;
			int fromWorld = -1;
			int toWorld = -1;
			
			if (itEdgeState->get_label() != itEdgeEvent->get_label()){
				continue;
			}

			Kstate* tempFromState;
			Kstate* tempToState;

		
			int stateN = 0;
			for (itState = newStateList.begin(); itState != newStateList.end(); itState++){
				// cout << "State " << stateN++ << ",with ID " << itState->get_id() << endl;

				if (itState->get_id() == (itEdgeEvent->get_from()*states_list.size()) + itEdgeState->get_from()){
					fromWorld = itState->get_id();
					tempFromState = &(*itState);
				}
				if (itState->get_id() == (itEdgeEvent->get_to()*states_list.size()) + itEdgeState->get_to()){
					toWorld = itState->get_id();
					tempToState = &(*itState);
				}
			}
			if (fromWorld == -1 || toWorld == -1){
				continue;
			}

			bool isEntail = true;
			Nodes* edge_con = itEdgeEvent->get_edge_condition();
			int tempIndex = fromWorld % states_list.size();
			Kstate originalState = states_list[tempIndex];
			if (edge_con != NULL){
				Nodes::iterator itBFNode;
				for (itBFNode = edge_con->begin(); itBFNode != edge_con->end(); itBFNode++){
					if (!originalState.entail(*itBFNode, k)){
						isEntail = false;
						break;
					}
				}
			}

			// if (fromWorld == 2 && toWorld == 2 && itEdgeState->get_label() == 0){
			// 	cout << "CHECK   agent " << itEdgeState->get_label() << " result " << isEntail << endl;
			// }

			if (!isEntail){
				continue;
			}

			Kedge* newEdge = new Kedge();
			newEdge->set_id(edgeID);
			newEdge->set_from(fromWorld);
			newEdge->set_to(toWorld);
			newEdge->set_label(itEdgeState->get_label());
			newEdge->set_exist(true);

			tempFromState->add_to_map(itEdgeState->get_label(), toWorld);
			tempFromState->add_out(edgeID);
			tempToState->add_in(edgeID);

			newEdgeList.push_back(*newEdge);

			edgeID++;
		}
	}

	result->set_stateslist(newStateList);
	result->set_edgeslist(newEdgeList);
	// cout << "Real ID " << result->get_id() << endl;
	return result;
}

bool Planner::check_lits(Literal a, Literals list)
{
	Literals::iterator it;

	for (it = list.begin(); it != list.end(); it++)
		if (a == *it)
			return true;

	return false;
}

//udpate_kripke removes edges and states that are not reachable from pointed and prepares the hashmap ``mapEdgelabelNextState" for each state
Kripke* Planner::update_kripke(Kripke k) {
	Kripke* out = new Kripke();
	AgentSet ags = m_agents;
	Agents agl;
	AgentSet::iterator ita;
    
	for (ita = ags.begin();ita != ags.end();ita++)
	{
		agl.push_back(*ita);
	}
    
    
	StateList sl = k.reachableStates(k.get_id(), agl);
	sl.insert(k.get_id());
	if (sl.size() == k.get_stateslist().size())
	{
		// out = new Kripke();
        out->set_stateslist_fix_states(k.get_stateslist());
		// out->set_edgeslist(k.get_edgeslist());
        out->set_edgeslist_fix_states(k.get_edgeslist());
        out->set_pId(k.get_id());
		return out;
	}
	States::iterator its;
	States slk = k.get_stateslist();
	States res;
	int id = 0;
	map<int, int> savedId;
	for (its = slk.begin();its != slk.end();its++)
	{
		int old = its->get_id();
		if (sl.find(old) != sl.end())
		{
			Kstate* n = its->copy();
			n->set_id(id);
			savedId.insert(map<int, int>::value_type(old, id));
			id = id + 1;
			n->set_in(EdgeList());
			n->set_out(EdgeList());
			res.push_back(*n);
		}
	}
	out->set_stateslist(res);
	out->set_pId(savedId.find(k.get_id())->second);

	Edges edg;
	Edges edgold = k.get_edgeslist();
	Edges::iterator ite;
	int start = 0;
	for (ite = edgold.begin();ite != edgold.end();ite++)
	{
		int from = ite->get_from();
		int to = ite->get_to();


		if (sl.find(from) == sl.end() || sl.find(to) == sl.end())
		{
			continue;
		}
		Kedge* newEd = new Kedge();

		int fromnew = savedId.find(from)->second;
		newEd->set_from(fromnew);
		int tonew = savedId.find(to)->second;
		newEd->set_to(tonew);
		newEd->set_id(start);
		newEd->set_label(ite->get_label());
		
		out->add_edge(*newEd);
        //out->add_edge2state(fromnew, start, true); // do NOT need this any more because they are implemented in add_edge
        //m->get_stateslist().at(fromnew).get_out().push_back(start);
        //out->add_edge2state(tonew, start, false);
        //m->get_stateslist().at(tonew).get_in().push_back(start);
		start = start + 1;
	}
	return out;
}

void Planner::update_node(BFNode* node)
{
	switch (node->node_type)
	{
	case fluForm: //base case
				  //node->agentPro2 = a_map.find(node->agentPro)->second;
		node->fluform = convert(node->flu_form);
		break;

	case BForm:
		node->agentPro2 = a_map.find(node->agentPro)->second;
		update_node(node->bfnode1);
		break;

	case propForm:
		update_node(node->bfnode1);
		if (node->bfOperator == BFNOT || node->bfOperator == NONE)
		{
			break;
		}
		update_node(node->bfnode2);
		break;

	case EForm:
		node->agents = convert(node->groupAgent);
		update_node(node->bfnode1);
		break;

	case CForm:
		node->agents = convert(node->groupAgent);
		update_node(node->bfnode1);
		break;

	case BFEmpty:
		break;

	default:
		break;
	}
}


AgentSet Planner::get_fd(const KAction& act, Kripke k) const
{
	AgentSet out = AgentSet();
	Agent_FF_list::iterator ia;
	Agent_FF_list temp = act.get_m_observes();
	for (ia = temp.begin();ia != temp.end(); ia++) {
		if (ia->get_body().size() == 0) {
			out.insert(ia->get_head());
			continue;
		}
		FluentFormula ff = ia->get_body();
		BFNode temp;
		temp.node_type = fluForm;
		temp.fluform = &ff;
		if (k.entail(temp))
		{
			out.insert(ia->get_head());
		}
	}
	return out;
}

AgentSet Planner::get_pd(const KAction& act, Kripke k) const
{
	AgentSet out = AgentSet();
	Agent_FF_list::iterator ia;
	Agent_FF_list temp = act.get_m_awares();
	for (ia = temp.begin();ia != temp.end(); ia++) {
		if (ia->get_body().size() == 0) {
			out.insert(ia->get_head());
			continue;
		}
		FluentFormula ff = ia->get_body();
		BFNode temp;
		temp.node_type = fluForm;
		temp.fluform = &ff;
		if (k.entail(temp))
		{
			out.insert(ia->get_head());
		}
	}
	return out;
}

AgentSet Planner::get_od(const KAction& act, Kripke k) const
{
	AgentSet ags = m_agents;
	AgentSet pd = get_pd(act, k);
	AgentSet fd = get_fd(act, k);
	AgentSet::iterator it;
	if (pd.size() != 0) {
		for (it = pd.begin();it != pd.end();it++) {
			ags.erase(*it);
		}
	}
	if (fd.size() != 0) {
		for (it = fd.begin();it != fd.end();it++) {
			ags.erase(*it);
		}
	}
	return ags;
}

AgentSet Planner::get_executor(const KAction& act) const
{
	AgentSet out = AgentSet();
 	Planner p = act.get_planner();
	StringList executor = act.get_act().executors;
	StringList::const_iterator itstrl;

	for (itstrl = executor.begin(); itstrl != executor.end(); itstrl++){
		Agent temp = p.a_map.find(*itstrl)->second;
		out.insert(temp);
	}
	return out;
}
//////////////////////////

Literals Planner::compute_effect(KAction* a, Kripke* k)
{
	//cout << "computing the effects of " << a->get_name() << endl;
	Literals out;
	Fluents_BF_list::const_iterator it;
	Literals::const_iterator it2;

	for (it = a->get_effects()->begin(); it != a->get_effects()->end(); it++)
	{
		if (k->entail(*(it->get_body())))
		{
			for (it2 = it->get_head()->begin();
				it2 != it->get_head()->end(); it2++)
			{
				out.insert(*it2);
			}
		}
	}
    //cout << "effect size is " << out.size() << endl;
	return out;
}

//required KAction is executable here already
Kstate* Planner::comp_nextstate(Kstate prev, Literals lits)
{
	Kstate* next = prev.copy();
	Literals lit = next->get_lits();
	Literals::const_iterator it;
	for (it = lits.begin();it != lits.end();it++)
	{
		if ((*it) % 2 == 1) // this literal is negation
		{
			lit.erase((*it) - 1);
			lit.insert(*it);
		}
		else { // this literal is not a negation
			lit.erase((*it) + 1);
			lit.insert(*it);
		}
	}
	next->set_lits(lit);
	return next;
}

EdgeList Planner::updateEdgesListId(EdgeList edgs, map<int, int> maps)
{
	EdgeList out;
	EdgeList::iterator it;
	for (it = edgs.begin();it != edgs.end();it++)
	{
		if (maps.find(*it) != maps.end()) {
			out.push_back(maps.find(*it)->second);
		}
	}
	return out;
}

Nodes Planner::get_goal()
{
	return m_gd;
}

/*******************
 CKripke class
 ******************/
CKripke::CKripke(Planner* pl)
{
	planner = pl;
	old = NULL;
	path = vector<string>();
	ks = vector<Kripke*>();
}

CKripke::~CKripke()
{

};

void CKripke::set_result(Kripke* ik)
{
	k = ik;
};


void CKripke::add_path(string name)
{
	path.push_back(name);
}

vector<Kripke*> CKripke::get_result() {
	return ks;
}

vector<string> CKripke::get_path() {
	return path;
}

bool CKripke::satisfy()
{
	Nodes::iterator it;
	vector<Kripke*>::iterator itk;
	for (itk = ks.begin(); itk != ks.end(); itk++) {
		for (it = planner->m_gd.begin(); it != planner->m_gd.end(); it++)
		{
			// it->print();
			// cout << endl;
			if (!(*itk)->entail(*it))
			{
				return false;
			}
		}
	}
	return true;

}

int CKripke::maxSubGoal()
{
	Nodes::iterator it;
	vector<Kripke*>::iterator itk;
	int result = ks.size()*planner->m_gd.size();
	for (itk = ks.begin(); itk != ks.end(); itk++) {
		for (it = planner->m_gd.begin(); it != planner->m_gd.end(); it++)
		{
			if (!(*itk)->entail(*it))
			{
				result--;
			}
		}
	}
	// cout << "Max SubGoal: " << result << endl;
	return result;
}

int CKripke::loopInRealWorld(){
	int result = 0;
	vector<Kripke*>::iterator itk;
	for (itk = ks.begin(); itk != ks.end(); itk++) {
		States temp_state_list = (*itk)->get_stateslist();
		States::iterator itStage;
		Kstate realWorld;
		for (itStage = temp_state_list.begin(); itStage != temp_state_list.end(); itStage++){
			if (itStage->get_id() == (*itk)->get_id()){
				realWorld = *itStage;
				break;
			}
		}
		result += realWorld.loopIn();
		// cout << "Sub: " << realWorld.loopIn() << endl;
	}
	// cout << "#Loop is: " << result << endl;
	return result;
}

int CKripke::lengthOfPlanningGraph() //return the maximal length of Planning Graphs; each of which is a PlanningGraph of a Kripkek in ks
{
    int out = 0;
    vector<Kripke*>::iterator itk;
    for (itk = ks.begin(); itk != ks.end(); itk++) {
        PlanningGraph pg = PlanningGraph(planner);
        //(*itk)->print_report();
        int l = pg.constructEPG(*itk);
        if(l == -1){
            return -1; //one of kripke in vector<Kripke*> ks cannot get to the goal -> this CKripke should be dropped;
        }else {
            if(l > out){
                out = l;
            }
        }
    }
    return out;
    
}
//return how many sub-goals are satisfied already
int CKripke::satisfywithprior()
{
	int out = 0;
	Nodes::iterator it;
	for (it = planner->m_gd.begin(); it != planner->m_gd.end(); it++)
	{
		if (k->entail(*it))
		{
			out++;
		}
	}
	return out;
}

void CKripke::set_path(vector<string> st)
{
	path = st;
}

void CKripke::set_old(CKripke* o)
{
	old = o;
}
CKripke* CKripke::get_old()
{
	return old;
}
CKripke* CKripke::next_ckripke(KAction a)
{
	CKripke* n = new CKripke(planner);
	n->set_path(path);
	n->add_path(a.get_name());
	n->set_old(this);
	Kripke* nk;
	vector<Kripke*>::iterator itk;
	for (itk = ks.begin(); itk != ks.end(); itk++) {
		if (a.get_type() == DYNAMIC)
		{
			//cout << a.get_name() << " ontic" << endl;
			nk = planner->execute_ontic(*itk, a);
		}
		else if (a.get_type() == DETERMINATION)
		{
			//cout << a.get_name() << " sensing" << endl;
			nk = planner->execute_sensing(*itk, a);

		}
		else { //ANNOUNCEMENT
			   //cout << a.get_name() << " announce" << endl;
			nk = planner->execute_ann(*itk, a);
		}
		if (nk == NULL) {
			return NULL; //is not executable
		}
		else {
			n->add_kripke(nk);
		}
	}
	/*
	vector<string>::iterator isol1;
	vector<string> pp = n->get_path();
	for(isol1=pp.begin();isol1!= pp.end();isol1++)
	{
	cout << *isol1 << " ";
	}
	cout << endl;
	*/
	return n;
}

void CKripke::add_kripke(Kripke* k) {
	ks.push_back(k);
}
