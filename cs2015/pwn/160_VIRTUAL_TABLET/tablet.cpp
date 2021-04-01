#include<iostream>
#include<algorithm>
#include<cstdlib>
#include<string.h>

#define ICAO_SIZE 4
#define NAME_SIZE 16
#define MAX_USERS 20

using namespace std;

class User {

  private:
    char icaocode[ICAO_SIZE];
    char name[NAME_SIZE];

  public:
    User(string name);
    string getName();
    void display_info();
    virtual void weather();
    virtual void spawn_shell();

};

class Admin : User {
  public:
    Admin(string name) : User(name) {};
    virtual void spawn_shell();
};

string User::getName() {
  return this->name;
}

void User::display_info() {
  cout << "Name: " << this->name << " "
       << "(icao: " << this->icaocode << ") "
       << endl;
}

User::User(string name) {
  if ( name.length() > NAME_SIZE ) {
    name.erase(NAME_SIZE, string::npos);
  }
  icaocode[0] = '\0';
  strcpy(this->name, name.c_str());
}

void User::weather() {
  cout << endl;
  cout << "Please enter your ICAO code: ";
  cin >> icaocode;
  string icao(icaocode);
  icao.erase(remove(icao.begin(), icao.end(), '\n'), icao.end());
  icao.erase(remove(icao.begin(), icao.end(), '\''), icao.end());
  if (icao.length() > ICAO_SIZE) {
    cout << "BAD ICAO code" << endl;
  } else {
    cout << "\nHere is your weather forecast:\n" << endl;
    string cmd = "curl -s 'http://weather.noaa.gov/pub/data/observations/metar/decoded/" + icao + ".TXT'";
    system(cmd.c_str());
    cout << endl;
  }
}

User * users[MAX_USERS];
int users_num = 0;

void logout() {
  auto user = users[--users_num];
  users[users_num] = 0;
  delete user;
}

void User::spawn_shell() {
  cout << "Non-privileged users cannot run a shell" << endl;
}

void Admin::spawn_shell() {
  system("/bin/sh");
}

void help() {
  cout <<
    "\nCommand list:\n"
    "  exit    : Terminates the session.\n"
    "  help    : Displays this message.\n"
    "  login   : Login as a different user.\n"
    "  logout  : Logout as the current user.\n"
    "  spawn   : Spawn a shell (privileged users only).\n"
    "  switch  : Switch to another logged in user.\n"
    "  users   : Displays all logged in users.\n"
    "  weather : Displays the current weather conditions.\n"
    "  whoami  : Displays current user info.\n"
    << endl;
}

void login() {
  string username;
  cout << "username: ";
  getline(cin, username);
  auto newuser = new User(username);
  users[users_num++] = newuser;
}

void display_users() {
  for(auto user : users) {
    if (user) {
      user->display_info();
    }
  }
}

void switch_user() {
  cout << "Which user do you want to become?\nusername: ";
  string username;
  getline(cin, username);
  int i;
  for( i = 0 ; i < MAX_USERS ; i ++ ) {
    if (users[i]) {
      if (users[i]->getName() == username) {
        break;
      }
    }
  }
  if ( i < MAX_USERS ) {
    auto tmp = users[i];
    users[i] = users[users_num-1];
    users[users_num-1] = tmp;
  } else {
    cout << "User " << username << " does not exist." << endl;
  }
}

bool select_command() {

  auto current_user = users[users_num - 1];
  string command;
  cout << current_user->getName() << "@" << "iTAB$ ";
  if (!getline(cin, command)) {
    return true;
  }
  if (command.empty()) {
    return false;
  }
  if (command == "help") {
    help();
  } else if (command == "exit") {
    return true;
  } else if (command == "weather") {
    current_user->weather();
  } else if (command == "spawn") {
    current_user->spawn_shell();
  } else if (command == "switch") {
    switch_user();
  } else if (command == "users") {
    display_users();
  } else if (command == "whoami") {
    current_user->display_info();
  } else if (command == "login") {
    login();
  } else if (command == "logout") {
    logout();
  } else {
    cout << "Unknown command, type `help' to see a list of available commands" << endl;
  }
  return false;
}


int main() {
  bool complete = false;
  cout << "~~~~~~~~~~~~~~~~~~ iTAB, the FUTURE is THERE ~~~~~~~~~~~~~~~~~" << endl;
  cout << "|                      Think Interactive!                    |" << endl;
  cout << "--------------------------------------------------------------" << endl << endl;
  for( int i = 0 ; i < MAX_USERS ; i++ ) {
    users[i] = 0;
  }
  users[users_num++] = new User("guest");
  while(!complete && users_num > 0) {
    complete = select_command();
  }
  for(auto user : users) {
    if (user) {
      delete user;
    }
  }
  return 0;
}