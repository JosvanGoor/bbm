#ifndef UTILITY_SETTINGS_HPP
#define UTILITY_SETTINGS_HPP

#include <map>
#include <string>

#include "../Core.hpp"

namespace utility
{

    /*
        This class represents a simple settings file with key=value pairs.
        It offers some functions to parse values into ints floats doubles and longs
    */
    class Settings : public Object
    {
    public:
        Settings(std::string file = "settings.ini");

        //default constructor
        void set(const std::string &key, const std::string &value); //inserts or overwrites
        //safe get, never throws.
        std::string get(const std::string &key) const; //returns value or "" if not applicable.

        bool has_key(const std::string &key) const;

        //these throw exceptions on invalid formats or missing keys
        int as_int(const std::string &key) const;
        long as_long(const std::string &key) const;
        float as_float(const std::string &key) const;
        double as_double(const std::string &key) const;
        std::string as_string(const std::string &key) const;

        std::string get_file();
        void set_file(const std::string &file);
        void save_to_file() const;
        bool load_from_file();

        std::string full_settings_report(std::string line_prefix = "") const; //returns all pairs as printable string

        virtual std::string to_string() const;

    protected:
        std::string m_file;
        std::map<std::string, std::string> m_settings;
    };

}

#endif
