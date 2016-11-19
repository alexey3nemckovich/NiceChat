#include "stdafx.h"
#include "Client.h"
using namespace std;


Client::Client()
{

}


Client::~Client()
{

}


Client* Client::GetClient()
{
	static Client* client;
	if (client == NULL)
	{
		mtx.lock();
		if (client == NULL)
		{
			client = new Client();
		}
		mtx.unlock();
	}
	return client;
}


bool Client::TryLogin(string login, string pass, string *err_message)
{
	throw new runtime_error("unsupported exception");
}


bool Client::TryRegistrate(
	string login,
	string first_name,
	string second_name,
	string pass,
	string *err_message
)
{
	throw new runtime_error("unsupported exception");
}


void Client::LeaveChat()
{

}