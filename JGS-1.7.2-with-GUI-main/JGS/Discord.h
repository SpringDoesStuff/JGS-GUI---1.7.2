#pragma once

#include <CkHttpRequestW.h>
#include <CkHttpW.h>
#include <CkHttpResponseW.h>

class Discord
{
public:
	static void UpdateStatus(std::string InStatus);
};

void Discord::UpdateStatus(std::string InStatus)
{
	CkHttpRequestW req;
	CkHttpW http;

	bool success;

	//  Any string unlocks the component for the 1st 30 days.
	success = http.UnlockComponent(L"Anything for 30-day trial");
	if (success != true) {
		wprintf(L"LogJGSDiscord: %s\n", http.lastErrorText());
		return;
	}

	std::string JsonString = "{\"content\": null,\"embeds\": [{\"title\": \"Server status update\",\"description\": \"";
	JsonString += InStatus;
	JsonString += "\", \"color\": 16711680, \"timestamp\" : \"";
	JsonString += " ";
	JsonString += "\"}], \"attachments\": [] }";

	std::wstring JsonWString(JsonString.begin(), JsonString.end());
	const wchar_t* Json = JsonWString.c_str();

	CkHttpResponseW* resp = 0;
	resp = http.PostJson2(L"https://discord.com/api/webhooks/981118326556225536/7uDRluim2J0FKv0eqpGwtjgrbAOuxtChoS_SSdslDjCbOAUSX1ztFHj7MDfyzLtZl3sd", L"application/json", Json);
	if (resp == 0) {
		wprintf(L"LogJGSDiscord: %s\n", http.lastErrorText());
	}
	else {
		//  Display the JSON response.
		wprintf(L"LogJGSDiscord: %s\n", resp->bodyStr());
		delete resp;
	}
}