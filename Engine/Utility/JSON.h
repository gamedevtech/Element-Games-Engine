#ifndef JSON_PARSER_H
#define JSON_H

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
        class JSON{
            public:
                JSON(std::string _filename){
                    filename = _filename;
                    boost::property_tree::json_parser::read_json(filename, pt);
                }
                ~JSON(void){
                    //
                }

                bool GetBool(std::string path){
                    return pt.get<bool>(path);
                }

                int GetInt(std::string path){
                    return pt.get<int>(path);
                }

                float GetFloat(std::string path){
                    return pt.get<float>(path);
                }

                std::string GetString(std::string path){
                    return pt.get<std::string>(path);
                }

                void Put(std::string path, bool value){
                    pt.put(path, value);
                }

                void Put(std::string path, int value){
                    pt.put(path, value);
                }

                void Put(std::string path, float value){
                    pt.put(path, value);
                }

                void Put(std::string path, std::string value){
                    pt.put(path, value);
                }
            private:
                std::string filename;
                boost::property_tree::ptree pt;
        };
    }
}

#endif
