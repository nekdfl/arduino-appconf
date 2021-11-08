# Description

Every project I have is the same architecture and appconf is one of the components I use every time.
This is an attempt at making a public library. If you have any ideas for improvement - you are welcome!

Examples of work can be found in the test folder.
Important! Before using the class methods, you need to call the Littlefs.begin() method.

## Example

> Start file system 
> ```LittleFS.begin();```

> Config must be stored in "/config " directory
> ```AppConf m_appconf = AppConf("/config");```

> Just name of file with or without extension
> ```String filename = "system.json";```

> Create new config file (you can do it once at settings init)
> ```m_appconf.createConfFile(filename);```

>   Set value. First argument use "path style" to settings. 
    Last element - is your setting name (key). Second argument is value.
> ```m_appconf.setLPValue(filename, "/system/devname", "testboard");```

> Read value from file
> ```auto boardname = m_appconf.getLPValue(filename, "/system/devname");```
