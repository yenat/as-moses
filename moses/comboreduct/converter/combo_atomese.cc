/** combo_atomese.cc ---
 *
 * Copyright (C) 2018 OpenCog Foundation
 *
 * Authors: Kasim Ebrahim <se.kasim.ebrahim@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License v3 as
 * published by the Free Software Foundation and including the exceptions
 * at http://opencog.org/wiki/Licenses
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program; if not, write to:
 * Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */


#include <opencog/atoms/base/Handle.h>
#include <opencog/atoms/base/Node.h>
#include <opencog/atoms/base/Link.h>
#include <moses/comboreduct/combo/vertex.h>


namespace opencog{namespace combo{

using namespace std;
using namespace boost;

Type atomese_argument(Handle &h, const argument &a,
                      const id::procidure_types& parent_procidure_type)
{
    switch (parent_procidure_type){
    case id::predicate:{
        Handle tmp = createNode(PREDICATE_NODE,
                                to_string(a.is_negated()? -a.idx : a.idx));
        if(a.is_negated()){
            HandleSeq handle_seq;
            handle_seq.push_back(tmp);
            h = createLink(handle_seq, NOT_LINK);
        } else{
            h = tmp;
        }
        return -1;
    }
    case id::schema:{
        Handle tmp = createNode(SCHEMA_NODE, to_string(a.is_negated()? -a.idx : a.idx));
        if(a.is_negated()){
            HandleSeq handle_seq;
            handle_seq.push_back(tmp);
            handle_seq.push_back(createNode(NUMBER_NODE, "-1"));
            h = createLink(handle_seq, TIMES_LINK);
        } else{
            h = tmp;
        }
        return -1;
    }
    default:{
        OC_ASSERT(false, "unsupported procedure type");
    }
    }
    return -1;
}

Type atomese_builtin(const builtin& b, id::procidure_types& procidure_type)
{
    switch (b){
    case id::logical_and:{
        procidure_type = id::predicate;
        return AND_LINK;
    }
    case id::logical_or:{
        procidure_type = id::predicate;
        return OR_LINK;
    }
    case id::logical_not:{
        procidure_type = id::predicate;
        return NOT_LINK;
    }
    case id::plus:{
        procidure_type = id::schema;
        return PLUS_LINK;
    }
    default:{
        OC_ASSERT(false, "unsupported");
        return -1;
    }
    }
}

Type atomese_vertex(Handle &h, const vertex &v,
                    id::procidure_types& parent_procidure_type)
{
    if(const argument* a = get<argument>(&v)){
        return atomese_argument(h, *a, parent_procidure_type);
    }
    else if(const builtin* b = get<builtin>(&v)){
        return atomese_builtin(*b, parent_procidure_type);
    }
    return -1;
}

template<typename Iter>
opencog::Handle atomese_combo_it(Iter it,
                                 id::procidure_types& parent_procidure_type)
{

    id::procidure_types procidure_type = parent_procidure_type;
    combo_tree::iterator head = it;
    Handle handle;
    Type link_type = atomese_vertex(handle, *head, procidure_type);

    if(link_type!=(unsigned short)-1){
        HandleSeq handle_seq;
        for(auto sib=head.begin(); sib!=head.end(); ++sib){
            handle_seq.push_back(atomese_combo_it(sib, procidure_type));
        }
        handle = createLink(handle_seq, link_type);
    }
    return handle;
}

Handle atomese_combo(const combo_tree& ct)
{
    Handle handle;
    combo_tree::iterator it=ct.begin();
    id::procidure_types ptype = id::procidure_types::unknown;
    handle = atomese_combo_it(it, ptype);
    return handle;
}

}}  // ~namespaces combo opencog
