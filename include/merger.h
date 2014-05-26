#ifndef GBOOK_MERGER_H
#define GBOOK_MERGER_H

#include <vector>
#include <map>

#include "typedefs.h"
#include "user.h"
#include "storage_manager.h"

namespace gbook {
    class merger {
    public:
        storage_changes & primary() {
            return primary;
        }

    private:
        storage_changes primary;
        vector<storage_changes> secondaries;
    };
}

#endif // GBOOK_MERGER_H
