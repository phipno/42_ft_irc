#include <iostream>
#include <string>

int parsing(std::string Message);

int main() {
  std::string correctOwnCmds[] = {"LOGIN phipno", "NICK phipno", "USER phipno", "JOIN channel1", "OP admin", "PRVMSG phipno Text is great jaja"}
  std::string kickCmds[] = {"KICK channel1 phipno", "KICK channel1,channel2 phipno,phipno", "KICK channel1,channel2 phipno", "KICK channel1 phipno,admin,thisAndThat"};
  std::string inviteCmds[] = {"INVITE phipno channel1"};
  std::string topicCmds[] = {"TOPIC channel1 texteverything and so on"};
  std::string ModeCmds[] = {"MODE -k", "MODE +o phipno"};

  parsing("LOGIN phipno");
}