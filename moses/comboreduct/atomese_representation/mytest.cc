/** atomese_representation.cc ---
 *
 * Copyright (C) 2018 OpenCog Foundation
 *
 * Author: Yidnekachew Wondimu <searchyidne@gmail.com>
 * and Bitseat Tadesse
 *
 */
#include <opencog/atoms/base/Link.h>
#include <opencog/atoms/base/Node.h>
#include <opencog/atomspace/AtomSpace.h>
#include "atomese_representation.h"
#include <opencog/guile/SchemeEval.h>


using namespace opencog;
using namespace std;

AtomSpace *as;
SchemeEval *eval;

Handle interpreter(const Handle &repr){


    Handle check = repr->getOutgoingSet()[1];
    HandleSeq result = check->getOutgoingSet();
    HandleSeq rowseq;

    for(const Handle &h : result){
        Handle nextresult = h->getOutgoingSet()[0];
        rowseq.push_back(nextresult);
    }

    Handle inner_set_link = repr->getOutgoingSet()[1];

    HandleSeq inner_list_link;
    HandleSeq outputs;

    for (const Handle &h : inner_set_link->getOutgoingSet()){
        inner_list_link.push_back(h->getOutgoingSet()[1]);
    }


    unsigned long j=0;
    for(const Handle &handle : inner_list_link){

        unsigned long i = handle->getOutgoingSet().size();
        HandleSeq inputs;
        for(Handle h : handle->getOutgoingSet()){
            if(i!=1){
                inputs.push_back(h);

            }

            i--;
        }
        as = new AtomSpace();
        eval = new SchemeEval(as);
        eval->eval("(use-modules (opencog exec))");
        SchemeEval evaluator;

        Handle plus_linked = createLink(inputs, PLUS_LINK);

        Handle plus_linked_atom = as->add_atom(plus_linked);

        Handle answer = evaluator.apply("cog-execute!", plus_linked_atom);
        Handle ss = createLink(LIST_LINK,
                               rowseq[j++],
                               answer);
        outputs.push_back(ss);


    }

    Handle final_output = createLink(outputs, SET_LINK);

    cout << final_output->to_short_string();
}


int main() {

    Handle repr = atomese::load_atomese("/home/bitseat/as-moses/moses/comboreduct/atomese_representation/test1.csv");

       SchemeEval evaluator;

        try {

            throw evaluator.apply("cog-execute!", createLink(PLUS_LINK,   createNode(SCHEMA_NODE, "i1"),
                                                  createNode(SCHEMA_NODE, "i2")));

            }
            catch (const opencog::SyntaxException &e) {

                Handle h = interpreter(repr);
                //cout << "Caught exception \"" << e.what() << "\n";
            }

    return 0;

        }

