#include "appconf.hpp"

AppConf::AppConf(const String &a_rootdir) : m_rootdir(a_rootdir)
{
    initRealRootDirPath();
    createRootDirIfNotExists();
}

AppConf::~AppConf()
{
}

void AppConf::setConfigRootDir(const String &a_rootdir)
{
    m_rootdir = a_rootdir;
}

String AppConf::getConfigRootDir()
{
    return m_rootdir;
}

void AppConf::deleteAllConfigFiles()
{
    debug_printf_ln("+++++AppConf::deleteAllConfigFiles+++++");
    Dir configfiles = LittleFS.openDir(m_rootdir);
    debug_printf_ln("AppConf::deleteAllConfigFiles::root_dir %s", m_rootdir.c_str());
    while (configfiles.next())
    {

        debug_printf_ln("file name: %s", configfiles.fileName().c_str());
        LittleFS.remove(m_rootdir + configfiles.fileName());
    }

#ifdef DEBUG_TO_SERIAL
    if (configfiles.next())
    {
        debug_printf_ln("file name: %s", configfiles.fileName().c_str());
        while (configfiles.next())
        {
            debug_printf_ln("file name: %s", configfiles.fileName().c_str());
        }
    }
    else
    {
        debug_printf_ln("AppConf::deleteAllConfigFiles::root_dir is empty");
    }
#endif
}

String AppConf::getLPValue(const String &a_filename, const String &a_valuepath, const String &a_defaultvalue)
{
    auto v_fullfilepath = makeFullFilePath(a_filename);
    DynamicJsonDocument doc(2048);
    readJsonDocFromFile(v_fullfilepath, doc);
    std::list<String> pathlist = splitPath(a_valuepath, "/");

    if (pathlist.size() <= 0) // invalid input data
        return a_defaultvalue;
    else if (pathlist.size() == 1) //key on first level
    {
        debug_printf_ln("short path");
        String key = pathlist.back();
        if (doc.containsKey(key) && !doc[key].isNull() && doc[key].is<String>())
        {
            String res = doc.getMember(key);
            return res;
        }
        else
        {
            return a_defaultvalue;
        }
    }
    else
    {
        auto tmp = worlkLP(pathlist, doc);
        if (!tmp.isNull() && tmp.is<String>())
        {
            String res = tmp;
            debug_printf_ln("res: %s ", res.c_str());
            return res;
        }
    }

    debug_printf_ln("value not exists return default");
    //like else
    return a_defaultvalue;
}

void AppConf::setLPValue(const String &a_filename, const String &a_valuepath, const String &a_value)
{
    auto v_fullfilepath = makeFullFilePath(a_filename);
    DynamicJsonDocument doc(2048);
    readJsonDocFromFile(v_fullfilepath, doc);
    std::list<String> pathlist = splitPath(a_valuepath, "/");
    String key;
    if (pathlist.size() <= 0) // invalid input data
        return;
    else if (pathlist.size() == 1) //key on first level
    {
        debug_printf_ln("short path");
        key = pathlist.back();
        auto tmp = doc.getOrAddMember(key);
        tmp.set(a_value);
    }
    else if (pathlist.size() >= 2) // longpath
    {
        auto tmp = worlkLP(pathlist, doc);
        tmp.set(a_value);
    }

#ifdef DEBUG_VERBOSE
    dumpJsonToSerial(doc);
#endif
    writeFile(v_fullfilepath, doc);
}

void AppConf::removeLPValue(const String &a_filename, const String &a_valuepath)
{
    auto v_fullfilepath = makeFullFilePath(a_filename);
    DynamicJsonDocument doc(2048);
    readJsonDocFromFile(v_fullfilepath, doc);
    String key;
    std::list<String> pathlist = splitPath(a_valuepath, "/");

    if (pathlist.size() <= 0) // invalid input data
        return;
    else if (pathlist.size() == 1) //key on first level
    {
        debug_printf_ln("short path");
        key = pathlist.back();
        doc.remove(key);
    }
    else if (pathlist.size() >= 2) // longpath
    {
        auto key = pathlist.back();
        pathlist.pop_back();
        auto tmp = worlkLP(pathlist, doc);
        tmp.remove(key);
    }
#ifdef DEBUG_VERBOSE
    dumpJsonToSerial(doc);
#endif
    writeFile(v_fullfilepath, doc);
}

std::list<String> AppConf::getLPChildList(const String &a_filename, const String &a_valuepath)
{
    auto v_fullfilepath = makeFullFilePath(a_filename);
    std::list<String> reslist;
    DynamicJsonDocument doc(2048);
    readJsonDocFromFile(v_fullfilepath, doc);
    std::list<String> pathlist = splitPath(a_valuepath, "/");
    JsonObject root = worlkLP(pathlist, doc);
    for (JsonPair p : root)
    {
        reslist.push_back(p.key().c_str());
    }
    reslist.sort();
    return reslist;
}

//protected members
ArduinoJson::JsonVariant AppConf::worlkLP(std::list<String> &pathlist, JsonDocument &doc)
{
    // DEBUG_AC("+++++++worlkLP++++++");
    ArduinoJson::JsonVariant res;
    String keypath;
    debug_printf_ln("long path");
    keypath = pathlist.front();
    pathlist.pop_front();
    res = doc.getOrAddMember(keypath);
    while (!pathlist.empty())
    {
        keypath = pathlist.front();
#ifdef DEBUG_VERBOSE
        String ff = res[keypath];
        debug_printf_ln("key: %s: value %s", keypath.c_str(), ff.c_str());
#endif
        res = res.getOrAddMember(keypath);
        pathlist.pop_front();
    }

    return res;
}

String AppConf::readFile(const String &a_filename)
{
    String buf;
    debug_printf_ln("Try read file %s", a_filename.c_str());
    File vrfile = LittleFS.open(a_filename.c_str(), "r");
    if (!vrfile)
    {
        debug_printf_ln("Failed to open file for reading %s", a_filename.c_str());
        return buf;
    }

    while (vrfile.available())
        buf += vrfile.readString();

    vrfile.close();
    debug_printf_ln("readFile::file data %s", buf.c_str());
    return buf;
}

void AppConf::readJsonDocFromFile(const String &a_filename, JsonDocument &a_doc)
{
    String buf = readFile(a_filename);
    parseJson(a_doc, buf);
}

void AppConf::writeFile(const String &a_filename, const String &a_data)
{
    File vfile = LittleFS.open(a_filename.c_str(), "w");
    if (!vfile)
    {
        debug_printf_ln("Failed to open file for writing");
        return;
    }
    debug_printf_ln("writeFile::file data: %s", a_data.c_str());
    if (vfile.print(a_data.c_str()))
        debug_printf_ln("File %s was written", a_filename.c_str());
    else
        debug_printf_ln("writeFile::Write to %s failed", a_filename.c_str());
    vfile.close();
}

void AppConf::writeFile(const String &a_filename, const JsonDocument &a_doc)
{
    String buf;
    serializeJson(a_doc, buf);
    writeFile(a_filename, buf);
}

void AppConf::parseJson(JsonDocument &doc, const String &a_json)
{
    DeserializationError error = deserializeJson(doc, a_json.c_str());
    if (error)
    {
        String msg;
        msg = "deserialize error:";
        msg += error.c_str();
        debug_printf_ln("%s", msg.c_str());
        // throw std::runtime_error(msg.c_str());
        return;
    }
}

void AppConf::createConfFile(const String &a_filename)
{
    auto v_fullfilepath = makeFullFilePath(a_filename);
    const String empy_json = "{}";
    File f = LittleFS.open(v_fullfilepath, "w");
    debug_printf_ln("createConfFile::full_file_path: %s", v_fullfilepath.c_str());
    debug_printf_ln("createConfFile::file data: %s", empy_json.c_str());
    f.println(empy_json.c_str());
    f.close();
}

bool AppConf::isFileExist(const String &a_filename)
{
    auto v_fullfilepath = makeFullFilePath(a_filename);
    return LittleFS.exists(v_fullfilepath);
}

void AppConf::deleteConfigFile(const String &a_filename)
{
    auto v_fullfilepath = makeFullFilePath(a_filename);
    debug_printf_ln("AppConf::deleteConfigFile");
    debug_printf_ln("full file %s", v_fullfilepath.c_str());
    if (LittleFS.exists(v_fullfilepath))
    {
        debug_printf_ln("delete file %s", v_fullfilepath.c_str());
        LittleFS.remove(v_fullfilepath);
    }
    else
    {
        /* code */
        debug_printf_ln("file not exist");
    }
}
