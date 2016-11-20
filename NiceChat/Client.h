#pragma once
#include <mutex>
#include <string>
using namespace std;


class Client
{
private:
	Client();
	~Client();
	static std::mutex mtx;
public:
	static Client* GetInstance();
	bool TryLogin(string login, string pass, string *err_message);
	bool TryRegistrate(
		string name,
		string last_name,
		string login,
		string pass,
		string *err_message
	);
	//void ConnectTo();
	void LeaveChat();
};