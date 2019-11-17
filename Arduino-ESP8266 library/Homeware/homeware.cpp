#include <homeware.h>

Homeware::Homeware(char * id, char * host, WiFiClientSecure* client){
  _client = client;
  _host = host;
  _id = id;
  strcpy(_code, id);
  strcat(_code, "-code");

  //Compose the request
  /*strcpy(_request_a, "GET /read/?id=");
  strcat(_request_a, id);
  strcpy(_request_b, " HTTP/1.1\r\n");
  strcat(_request_b, "Host: ");
  strcat(_request_b, host);
  strcat(_request_b, "\r\n");
  strcat(_request_b, "User-Agent: ARC ");
  strcat(_request_b, id);
  strcat(_request_b, "\r\n");
  strcat(_request_b, "authorization: bearer ");*/

  //client.setFingerprint(fingerprint);
  _client->setInsecure();
}

void Homeware::test(){
  int i = 0;
}

void Homeware::getToken(bool refresh){

  _token[0] = '\0';

  if (!_client->connect(_host, _httpsPort)) {
    Serial.println("connection failed");
    return;
  }

  //Send the request with refresh or authentication_code
  if (!refresh){
    _client->print(String("GET ") + "/token/" + " HTTP/1.1\r\n" +
                 "Host: " + _host + "\r\n" +
                 "User-Agent: " + _id + "\r\n" +
                 "code: " + _code + "\r\n" +
                 "grant_type: authorization_code\r\n" +
                 "Connection: close\r\n\r\n");

   _refresh[0] = '\0';
 } else {
   _client->print(String("GET ") + "/token/" + " HTTP/1.1\r\n" +
                "Host: " + _host + "\r\n" +
                "User-Agent: " + _id + "\r\n" +
                "code: " + _refresh + "\r\n" +
                "grant_type: refresh_token\r\n" +
                "Connection: close\r\n\r\n");
 }

 //Compose the request
 strcpy(_request_a, "GET /read/?id=");
 strcat(_request_a, _id);
 strcpy(_request_b, " HTTP/1.1\r\n");
 strcat(_request_b, "Host: ");
 strcat(_request_b, _host);
 strcat(_request_b, "\r\n");
 strcat(_request_b, "User-Agent: ARC ");
 strcat(_request_b, _id);
 strcat(_request_b, "\r\n");
 strcat(_request_b, "authorization: bearer ");

  //Read the response
  String line;
  int past = millis();
  while (_client->connected() || (millis() - past < 5000)) {
    char c = _client->read();
    //Serial.print(c);
    line += c;
  }

  //Verify if there is an error
  int start_char_p = line.indexOf("error\":");
  if (start_char_p > 0){
    Serial.println("Invalid grant");
  } else {
    //Find the token
    start_char_p = line.indexOf("access_token\":\"") + 15;
    for (int i = start_char_p; i < line.length(); i++){
      if (line[i] != '"'){
        //token_c +=  line[i];
        int len = strlen(_token);
        _token[len] = line[i];
        _token[len+1] = '\0';
      } else {
        break;
      }
    }

    Serial.println("Access token");
    Serial.println(_token);

    strcat(_request_b, _token);
    strcat(_request_b, "\r\nConnection: close\r\n\r\n");

    //Find the refresh token
    if (!refresh){
      start_char_p = line.indexOf("refresh_token\":\"") + 16;
      for (int i = start_char_p; i < line.length(); i++){
        if (line[i] != '"'){
          //token_c +=  line[i];
          int len = strlen(_refresh);
          _refresh[len] = line[i];
          _refresh[len+1] = '\0';
        } else {
          break;
        }
      }
    }

    Serial.println("Refresh token");
    Serial.println(_refresh);
  }

}

char* Homeware::getJSON(){
  if (!_client->connect(_host, _httpsPort)) {
    Serial.println("connection failed");
    delay(15000);
    return "connection failed";
  }
  //Save the value
  strcpy(_request, _request_a);
  strcat(_request, _request_b);

  //Send the request
  _client->print(_request);

  strcpy(_json_c, "");
  while (_client->connected()) {
    //_line = _client->readStringUntil('\n');
    if (_client->readStringUntil('\n') == "\r") {
      break;
    }
  }

  char c = 'a';
  while (_client->available()) {
    c = _client->read();

    int len = strlen(_json_c);
    _json_c[len] = c;
    _json_c[len+1] = '\0';

  }


  /*bool detected = false;
  while (_client->connected()) {

    char c = _client->read();
    _client->getString();
    //Serial.print(c);

    if (c == '{')
      detected = true;
    else if (detected && _last_c == '}' && c == ' ')
      detected = false;

     if (detected){
      int len = strlen(_json_c);
      _json_c[len] = c;
      _json_c[len+1] = '\0';
    }

    _last_c = c;

  }
  */
  return _json_c;
}

bool Homeware::sendTrait(char* trait, char* value, char* vartype){
  if (!_client->connect(_host, _httpsPort)) {
    Serial.println("connection failed");
    delay(15000);
    return false;
  }
  //Save the value
  strcpy(_request, _request_a);
  strcat(_request, "&param=");
  strcat(_request, trait);
  strcat(_request, "&value=");
  strcat(_request, value);
  strcat(_request, "&vartype=");
  strcat(_request, vartype);
  strcat(_request, _request_b);

  //Send the request
  _client->print(_request);

  //char* ref = "thermostatMode\":";
  strcpy(_ref, trait);
  strcat(_ref, "\":");

  //A creepy way to read the state ;-)
  bool detected = false;
  strcpy(_state, "");
  while (_client->connected()) {
    for (int i = 0; i < strlen(_ref)-1; i++){
      _actual[i] = _actual[i+1];
    }
    _actual[strlen(_ref)-1] = _client->read();

    if (_actual[strlen(_ref)-1] == ',')
      detected = false;

     if (detected){
      int len = strlen(_state);
      _state[len] = _actual[strlen(_ref)-1];
      _state[len+1] = '\0';
    }

    if (strcmp(_actual, _ref) == 0)
      detected = true;

  }

  if (strcmp(_state, value) == 0)
    return true;
  else
    return false;




}
