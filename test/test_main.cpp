#include <Arduino.h>
#include <unity.h>

#include "test_appconf.hpp"
#include "appconfig.hpp"
#include "appconf.hpp"

void setup()
{
    delay(2000);

    UNITY_BEGIN(); // IMPORTANT LINE!
    test_appconf();
    UNITY_END(); // stop unit testing
}

void loop()
{
}
