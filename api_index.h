const String key_pin_value PROGMEM = R"=====(
    $PIN_VALUE$
    )=====";

const String json_response_switch PROGMEM = R"=====(
    { pinValue: $PIN_VALUE$ }
    )=====";

const String api_ophost_register = "http://ophost.local/register";