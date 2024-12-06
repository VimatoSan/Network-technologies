#include "utils.h"
#include <boost/json.hpp>
#include <sstream>

namespace winforms::utils {

    std::string double_to_str(double value) {
        std::ostringstream oss;
        oss.imbue(std::locale("C"));
        oss << value;
        return oss.str();
    }


    std::string get_str_from_json(const boost::json::object& json, const std::string& key) {
        return std::string(json.at(key).as_string().c_str());
    }


    int convert_pressure(int pascals) {
        return pascals * 0.0075;
    }
}