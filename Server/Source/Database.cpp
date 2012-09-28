#include "../Database.h"

namespace EGServer {
    Database::Database(void) {
        db.connect("localhost");
    }

    void Database::InitDatabase(void) {
        std::list<std::string> collection_names = db.getCollectionNames("spacesim");
        std::list<std::string>::iterator coll_iter = collection_names.begin();
        bool has_users = false;
        while(coll_iter != collection_names.end()) {
            if ((*coll_iter) == "spacesim.users") {
                has_users = true;
            }
            ++coll_iter;
        }

        if (!has_users) {
            db.createCollection("spacesim.users");
            mongo::BSONObjBuilder obj;
            obj.append("username", "admin");
            obj.append("password", "admin");
            obj.append("email", "palodequeso@gmail.com");
            db.insert("spacesim.users", obj.obj());
        }
    }
};
