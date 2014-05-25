#ifndef GBOOK_ABOOK_MANAGER_H
#define GBOOK_ABOOK_MANAGER_H

#include <istream>

#include "user.h"
#include "storage_manager.h"

namespace gbook {
    class abook_manager : public storage_manager {
    public:
        virtual void add(user & u)          =0;
        virtual void update(user u)         =0;
        virtual void remove(std::string id) =0;

        void flush();
        void load(std::istream & input);
    private:
    };
}

#endif // GBOOK_ABOOK_MANAGER_H
