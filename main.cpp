#include <iostream>
#include <string>

int parsing(std::string Message);

int main() {
  const char *correctOwnCmds[] = {"LOGIN phipno", "NICK phipno", "USER phipno", "JOIN channel1", "OP admin", "PRVMSG phipno Text is great jaja", NULL};
  // const char *kickCmds[] = {"KICK channel1 phipno", "KICK channel1,channel2 phipno,phipno", "KICK channel1,channel2 phipno", "KICK channel1 phipno,admin,thisAndThat"};
  const char *inviteCmds[] = {"INVITE phipno channel1", NULL};
  const char *topicCmds[] = {"TOPIC channel1 texteverything and so on", NULL};
  const char *ModeCmds[] = {"MODE channel1 -k", "MODE channel1 +o phipno", NULL};

  parsing("LOGIN phipno");
  parsing("PRVMSG phipno yes");
  parsing("PRVMSG phipno hello sods dsd");

  for (int i = 0; correctOwnCmds[i] != NULL; i++) {
    parsing(correctOwnCmds[i]);
  }
  // for (int i = 0; kickCmds[i] != NULL; i++) {
  //   parsing(kickCmds[i]);
  // }
  for (int i = 0; inviteCmds[i] != NULL; i++) {
    parsing(inviteCmds[i]);
  }
  for (int i = 0; topicCmds[i] != NULL; i++) {
    parsing(topicCmds[i]);
  }
  for (int i = 0; ModeCmds[i] != NULL; i++) {
    parsing(ModeCmds[i]);
  }
}