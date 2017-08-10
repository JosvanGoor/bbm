#include "Settings.hpp"

#include <sstream>
#include <fstream>

namespace utility
{

    Settings::Settings(std::string str)
    {
        m_file = str;
    }
    
    void Settings::set(const std::string &key, const std::string &value)
    {
        m_settings[key] = value;
    }

    std::string Settings::get(const std::string &key) const
    {
        auto it = m_settings.find(key);
        if(it == m_settings.end()) return "";
        return it->second;
    }

    int Settings::as_int(const std::string &key) const
    {
        auto it = m_settings.find(key);
        if(it == m_settings.end()) throw Exception(__PRETTY_FUNCTION__, "Failed to find value for key " + key);
        return std::stoi(it->second);
    }

    long Settings::as_long(const std::string &key) const
    {
        auto it = m_settings.find(key);
        if(it == m_settings.end()) throw Exception(__PRETTY_FUNCTION__, "Failed to find value for key " + key);
        return std::stol(it->second);
    }

    float Settings::as_float(const std::string &key) const
    {
        auto it = m_settings.find(key);
        if(it == m_settings.end()) throw Exception(__PRETTY_FUNCTION__, "Failed to find value for key " + key);
        return std::stof(it->second);
    }

    double Settings::as_double(const std::string &key) const
    {
        auto it = m_settings.find(key);
        if(it == m_settings.end()) throw Exception(__PRETTY_FUNCTION__, "Failed to find value for key " + key);
        return std::stod(it->second);
    }

    std::string Settings::as_string(const std::string &key) const
    {
        auto it = m_settings.find(key);
        if(it == m_settings.end()) throw Exception(__PRETTY_FUNCTION__, "Failed to find value for key " + key);
        return it->second;
    }

    std::string Settings::get_file()
    {
        return m_file;
    }

    void Settings::set_file(const std::string &file)
    {
        m_file = file;
    }

    void Settings::save_to_file() const
    {
        std::ofstream out(m_file);

        for(auto entry : m_settings)
        {
            if(entry.first.substr(0, 2) == std::string("__")) continue;
            out << entry.first << "=" << entry.second << "\n";
        }

        out.close();
    }

    void Settings::save_all_to_file() const
    {
        std::ofstream out(m_file);

        for(auto entry : m_settings)
        {
            out << entry.first << "=" << entry.second << "\n";
        }

        out.close();
    }

    bool Settings::load_from_file()
    {
        std::ifstream in(m_file);
        if(in.fail()) return false;

        while(!in.eof())
        {
            std::string line = get_line(in);
            size_t eq = line.find_first_of("=");
            if(eq == std::string::npos) continue; //no pair found

            m_settings[line.substr(0, eq)] = line.substr(eq+1);
        }
        return true;
    }

    std::string Settings::full_settings_report(std::string line_prefix) const
    {
        std::stringstream ss;

        for(auto entry : m_settings)
        {
            ss << line_prefix << entry.first << " = " << entry.second << "\n";
        }

        return ss.str();
    }

    std::string Settings::to_string() const
    {
        return "utility::Settings: stored " + std::to_string(m_settings.size()) + " key/value pairs.";
    }

}