#include <iostream>
#include <string>

int parsing(std::string message);

int main() {
  std::string empty = "";

  std::cout << parsing(empty) << std::endl;

  std::string gibberish = "hello";

  std::cout << parsing(gibberish) << std::endl;

  std::string space = "Invite @thisUsr    bla bla bla";

  std::cout << parsing(space) << std::endl;

  std::string prefix = ":Prefix:Test:hsad sadasdsaazxc zxcz czxc :nice  ";
 
  std::cout << parsing(prefix) << std::endl;

}