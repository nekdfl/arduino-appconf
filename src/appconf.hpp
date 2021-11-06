#ifndef APPCONF_HPP
#define APPCONF_HPP

#include <Arduino.h>
#include <apputils.hpp>
#include <ArduinoJson.h>
#include <LittleFS.h>
#include <utility>

//uncomment to enable debug
// #define DEBUG_TO_SERIAL 1

//uncomment to force disable debug
// #ifdef DEBUG_TO_SERIAL
// #undef DEBUG_TO_SERIAL
// #define DEBUG_TO_SERIAL 0
// #endif
#include "appconfig.hpp"
#include <appdebug.hpp>

class AppConf
{
protected:
    /* data */
    String m_rootdir;

public:
    AppConf(const String &a_rootdir = "/config");
    ~AppConf();

    void setConfigRootDir(const String &a_rootdir);

    String getConfigRootDir();

    void deleteAllConfigFiles();

    String getLPValue(const String &a_filename, const String &a_valuepath, const String &a_defaultvalue = String(""));

    void setLPValue(const String &a_filename, const String &a_valuepath, const String &a_value);

    void removeLPValue(const String &a_filename, const String &a_valuepath);

    std::list<String> getLPChildList(const String &a_filename, const String &a_valuepath);

    void createConfFile(const String &a_filename);

    bool isFileExist(const String &a_filename);

    void deleteConfigFile(const String &a_filename);

protected:
    ArduinoJson::JsonVariant worlkLP(std::list<String> &pathlist, JsonDocument &doc);

    String readFile(const String &a_filename);

    void readJsonDocFromFile(const String &a_filename, JsonDocument &a_doc);

    void writeFile(const String &a_filename, const String &a_data);

    void writeFile(const String &a_filename, const JsonDocument &a_doc);

    void parseJson(JsonDocument &doc, const String &a_json);

private:
    inline void initRealRootDirPath()
    {
        while (m_rootdir.endsWith("/"))
        {
            m_rootdir = m_rootdir.substring(0, m_rootdir.length() - 1);
        }

        while (m_rootdir.startsWith("/"))
        {
            m_rootdir = m_rootdir.substring(1, m_rootdir.length());
        }

        m_rootdir = "/" + m_rootdir + "/";
    }

    inline void createRootDirIfNotExists()
    {
        if (!LittleFS.exists(m_rootdir))
        {

            Dir rootdir = LittleFS.openDir(m_rootdir);
            if (!rootdir.isDirectory())
            {
                debug_printf_ln("createRootDirIfNotExists");
                LittleFS.mkdir(m_rootdir);
            }
            else
            {
                debug_printf_ln("createRootDirIfNotExists is exists");
            }
        }
    }

    inline String makeFullFilePath(const String &a_filename)
    {
        String file = m_rootdir + a_filename;
        return file;
    }
};

#endif