#include "../JSON.h"

namespace EG{
    namespace Utility{
        JSON::JSON(std::string _filename) {
            filename = _filename;
            if (filename != "") {
                boost::property_tree::json_parser::read_json(filename, pt);
            }
        }

        JSON::~JSON(void) {
            //
        }

        std::string JSON::GetJSON(void) {
            std::stringstream out;
            boost::property_tree::write_json(out, pt);
            std::cout << "Get JSON: " << out.str() << std::endl;
            return out.str();
        }

        bool JSON::GetBool(std::string path){
            return pt.get<bool>(path);
        }

        int JSON::GetInt(std::string path){
            return pt.get<int>(path);
        }

        float JSON::GetFloat(std::string path){
            return pt.get<float>(path);
        }

        std::string JSON::GetString(std::string path){
            return pt.get<std::string>(path);
        }

        void JSON::Put(std::string path, bool value){
            pt.put<bool>(path, value);
        }

        void JSON::Put(std::string path, int value){
            pt.put<int>(path, value);
        }

        void JSON::Put(std::string path, float value){
            pt.put<float>(path, value);
        }

        void JSON::Put(std::string path, std::string value){
            pt.put<std::string>(path, value);
        }
    }
}
