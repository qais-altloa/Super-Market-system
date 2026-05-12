#pragma once
#include <iostream>
using namespace std;

class user {
protected:

	string username;
	string password;
public:
	user(string useername = "", string passsword = "") {
		username = useername;
		password = passsword;
	}
	void setusername(string s) { username = s; }
	void setpassword(string p) { password = p; }
	string get_username() { return username; }
	string get_password() { return password; }

	bool logain(string useername, string passsword) {
		return (username == useername && password == passsword);
	}

	virtual void displayrole() {
		cout << "User :" << username << endl;
	}

};

class admin :public user {
public:
	admin(string useername = "", string passsword = "") :user(useername, passsword) {}
	void displayrole()override {
		cout << "admin :" << username;
	}
};