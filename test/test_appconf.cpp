#include "test_appconf.hpp"

AppConf m_appconf = AppConf("/config");

/**
 * @brief Set the Up object
 * 
 */
void setUp(void)
{

    LittleFS.begin(); // Start the Flash Files System
}

/**
 * @brief clear ram
 * 
 */
void tearDown(void)
{
    LittleFS.end();
}

void test_createfile()
{
    String filename = "wifi.json";
    m_appconf.createConfFile(filename);
    TEST_ASSERT_EQUAL(true, m_appconf.isFileExist(filename));
}

void test_isfileexist()
{
    String filename = "wifi.json";
    m_appconf.createConfFile(filename);
    TEST_ASSERT_EQUAL(true, m_appconf.isFileExist(filename));
}

void test_deletefile()
{
    String filename = "wifi.json";
    m_appconf.createConfFile(filename);
    m_appconf.deleteConfigFile(filename);
    TEST_ASSERT_EQUAL(false, m_appconf.isFileExist(filename));
}

void test_deleteconfigfiles()
{
    String filename1 = "file1.json";
    String filename2 = "file1.json";
    bool isFilesExist = false;
    m_appconf.createConfFile(filename1);
    m_appconf.createConfFile(filename2);
    m_appconf.deleteAllConfigFiles();
    if (m_appconf.isFileExist(filename1) or m_appconf.isFileExist(filename2))
    {
        isFilesExist = true;
    }

    TEST_ASSERT_EQUAL(false, isFilesExist);
}

void test_setvalue()
{
    String filename = "system.json";
    m_appconf.createConfFile(filename);
    m_appconf.setLPValue(filename, "/system/devname", "testboard");
    auto boardname = m_appconf.getLPValue(filename, "/system/devname");
    TEST_ASSERT_EQUAL_STRING("testboard", boardname.c_str());
}

void test_getDefaultValue()
{
    String filename = "system.json";
    m_appconf.createConfFile(filename);

    auto boardname = m_appconf.getLPValue(filename, "/system/devname", "testboard");
    TEST_ASSERT_EQUAL_STRING("testboard", boardname.c_str());
}

void test_removeValue()
{
    String filename = "system.json";
    m_appconf.createConfFile(filename);
    m_appconf.setLPValue(filename, "/system/devname", "testboard_test");
    m_appconf.removeLPValue(filename, "/system/devname");
    auto boardname = m_appconf.getLPValue(filename, "/system/devname", "testboard");
    TEST_ASSERT_EQUAL_STRING("testboard", boardname.c_str());
}

void test_setrootdir()
{
    AppConf aconf;
    aconf.setConfigRootDir("/config2");
    auto confpath = aconf.getConfigRootDir();
    TEST_ASSERT_EQUAL_STRING("/config2", confpath.c_str());
}

void test_getchildlist()
{
    bool pass_test = false;
    String wifisetting_file = "wifi.json";
    const String SSID1 = "ap1";
    const String PASS1 = "pass1";
    const String SSID2 = "ap2";
    const String PASS2 = "pass2";

    m_appconf.createConfFile(wifisetting_file);
    m_appconf.setLPValue(wifisetting_file, "/wifilist/" + SSID1, PASS1);
    m_appconf.setLPValue(wifisetting_file, "/wifilist/" + SSID2, PASS2);

    std::list<std::pair<String, String>> wifiparams;

    std::list<String> wifilist = m_appconf.getLPChildList(wifisetting_file, "/wifilist");
    for (auto &wifiname : wifilist)
    {
        String longpath = "/wifilist/" + wifiname;
        String wifipass = m_appconf.getLPValue(wifisetting_file, longpath);
        wifiparams.push_back(std::pair<String, String>(wifiname, wifipass));
    }

    String wifi_ssid_1 = wifiparams.front().first;
    String wifi_pass_1 = wifiparams.front().second;
    String wifi_ssid_2 = wifiparams.back().first;
    String wifi_pass_2 = wifiparams.back().second;

    if (wifi_ssid_1 == SSID1 and PASS1 == wifi_pass_1 and wifi_ssid_2 == SSID2 and wifi_pass_2 == PASS2)
    {
        pass_test = true;
    }
    TEST_ASSERT_EQUAL(true, pass_test);
}

void test_getdynamicbuffersize()
{
    const unsigned int default_buffer_size = 2048;
    auto buffersize = m_appconf.getDynamicBufferSize();
    TEST_ASSERT_EQUAL(default_buffer_size, buffersize);
}

void test_setdynamicbuffersize()
{
    const unsigned int new_buffer_size = 1024;
    m_appconf.setDynamicBufferSize(new_buffer_size);
    TEST_ASSERT_EQUAL(new_buffer_size, m_appconf.getDynamicBufferSize());
}

void dummy_test()
{
    TEST_ASSERT_EQUAL(0, 0);
}

void test_appconf()
{
    RUN_TEST(test_createfile);
    RUN_TEST(test_isfileexist);
    RUN_TEST(test_deletefile);
    RUN_TEST(test_deleteconfigfiles);
    RUN_TEST(test_setvalue);
    RUN_TEST(test_getDefaultValue);
    RUN_TEST(test_removeValue);
    RUN_TEST(test_setrootdir);
    RUN_TEST(test_getchildlist);
    RUN_TEST(test_getdynamicbuffersize);
    RUN_TEST(test_setdynamicbuffersize);
    //last test not display. Add dummy test.
    RUN_TEST(dummy_test);
}
