#include "funcs.h"

using namespace std;
using namespace gbook;

bool gbook::is_user_in(user & u, user_list & l) {
    return
        find_if(
            begin(l),
            end(l),
            [&u] (user & u_in_l) { return u.custom5 == u_in_l.custom5; }
        )
            !=
        end(l);
}

bool gbook::is_user_in(user & u, user_ptr_list & l) {
    return
        find_if(
            begin(l),
            end(l),
            [&u] (user * u_in_l) { return u.custom5 == u_in_l->custom5; }
        )
            !=
        end(l);
}

bool gbook::is_user_in(user * u, user_ptr_list & l) {
    return
        find_if(
            begin(l),
            end(l),
            [u] (user * u_in_l) { return u->custom5 == u_in_l->custom5; }
        )
            !=
        end(l);
}

user_list::iterator gbook::find_user_in(user & u, user_list & l) {
    return find_if(
        begin(l),
        end(l),
        [&u] (user & u_in_l) { return u.custom5 == u_in_l.custom5; }
    );
}

user_ptr_list::iterator gbook::find_user_in(user & u, user_ptr_list & l) {
    return find_if(
        begin(l),
        end(l),
        [&u] (user * u_in_l) { return u.custom5 == u_in_l->custom5; }
    );
}

user_ptr_list::iterator gbook::find_user_in(user * u, user_ptr_list & l) {
    return find_if(
        begin(l),
        end(l),
        [u] (user * u_in_l) { return u->custom5 == u_in_l->custom5; }
    );
}
