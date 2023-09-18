#include <string>
#include <cstring>
#include <vector>
#include <iostream>
#include <algorithm>

std::vector<std::string> splitString(std::string Message) {
  std::vector<std::string> splitCmds;
  std::string Temp = "";

  for (size_t i = 0; i < Message.length() && Message[i] != ':'; i++){
    if (Message[i] == ' ') {
      splitCmds.push_back(Temp);
      Temp = "";
    }
    else
      Temp += Message[i];
  }
  splitCmds.push_back(Temp);
  return (splitCmds);
}

bool checkMessage(std::vector<std::string> splitMessage) {
  (void)splitMessage;
  return true;
}

void  executeMessage(std::vector<std::string> splitMessage) {
  (void)splitMessage;
}

int parsing(std::string message /*, Client &User*/) {
  std::vector<std::string> splitCmds;
  std::string              temp;
  size_t                   count = std::count(message.begin(), message.end(), ':');
  //isRegistered(User);

  std::cout << "Count: " << count << std::endl;
  if (count == 0)
    count = 1;
  if (message == "")
    return 0;
  while (count > 0) {
    if (message[0] == ':')
      temp = message.substr(1, message.find(':', 1));
    else
      temp = message;
    splitCmds = splitString(temp);
    message.erase(0, message.find(':', 1));
    for (std::vector<std::string>::iterator It = splitCmds.begin(); It != splitCmds.end(); It++) {
      std::cout << *It << " | "; 
    }
    std::cout << std::endl;
    checkMessage(splitCmds);
    executeMessage(splitCmds);
    count--;
  }
  return 0;
}