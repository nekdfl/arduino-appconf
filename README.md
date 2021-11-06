Description
=======

Every project I have is the same architecture and appconf is one of the components I use every time.
This is an attempt at making a public library. If you have any ideas for improvement - you are welcome!

Examples of work can be found in the test folder.
Important! Before using the class methods, you need to call the Littlefs.begin() method.

Example:
=======

>     LittleFS.begin(); //start file system
>     AppConf m_appconf = AppConf("/config"); //config must be stored in "/config " directory.
>     String filename = "system.json"; // just name of file with or without extension
>     m_appconf.createConfFile(filename); // create new config file (you can do it once at settings init)
>     m_appconf.setLPValue(filename, "/system/devname", "testboard"); // set value. First argument use "path style" to settings. Last element - is your setting name (key). Second argument is value.
>     auto boardname = m_appconf.getLPValue(filename, "/system/devname"); // read value from file

