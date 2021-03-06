#ifndef JSON_PARSER_H
#define JSON_PARSER_H

#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

//http://www.boost.org/doc/libs/1_41_0/doc/html/boost_propertytree/accessing.html
/* Example:
 * If your json read '{ "screen": { "width": 800 } }'
 * Then you could use the path "screen.width" to get the 800 out
 * int screen_width = json_parser.GetInt("screen.width");

 Read about get_optional
 and put
*/

namespace EG{
    namespace Utility{
        class JSONValue {
            public:
                enum JSONType{
                    INT = 0,
                    FLOAT = 1,
                    STRING = 2,
                    ARRAY = 3,
                    OBJECT = 4
                };

                JSONValue(void);
                ~JSONValue(void);
            private:
                unsigned int type;
                int int_value;
        };

        class JSON{
            public:
                JSON(std::string _filename = "");
                ~JSON(void);

                std::string GetJSON(void);

                bool GetBool(std::string path);
                int GetInt(std::string path);
                float GetFloat(std::string path);
                std::string GetString(std::string path);

                void Put(std::string path, bool value);
                void Put(std::string path, int value);
                void Put(std::string path, float value);
                void Put(std::string path, std::string value);
            private:
                std::string filename;
                boost::property_tree::ptree pt;
        };
    }
}

#endif
