#ifndef EGSERVER_DATABASE_H
#define EGSERVER_DATABASE_H

#include "client/dbclient.h"

namespace EGServer {
    class Database {
        public:
            Database(void);
            void InitDatabase(void);
        private:
            mongo::DBClientConnection db;
    };
};

#endif
