#include "stdafx.h"
#define CATCH_CONFIG_RUNNER
#include "catch.hpp"
#include <iostream>
#include <vector>
#include <string>
using namespace std;

class Subscriber;

class PrintingAgency{
	vector<Subscriber*> subscribers;
	vector<string> produnctions; 
public:
	void registerSubscriber(Subscriber* subscriber);
	void unregisterSubscriber(Subscriber* subscriber);
	void notifySubscribers(const string& productionName);
	void addProduction(string name);
};

class Subscriber{
	vector<string> subscriptions;
	vector<boolean> available;
public:
	void addSubscription(const string& productionName);
	void removeSubscription(const string& productionName);
	boolean isSubscribed(const string& productionName);
	boolean isSubscriptionAvailable(const string& productionName);
	void onSuscribedProductionAvailable(const string& productionName);
};


void PrintingAgency::registerSubscriber(Subscriber* subscriber){
	subscribers.push_back(subscriber);
}

void PrintingAgency::unregisterSubscriber(Subscriber* subscriber){
	auto it = find(subscribers.begin(), subscribers.end(), subscriber);
	subscribers.erase(it);
}

void PrintingAgency::notifySubscribers(const string& productionName){
	for (auto s : subscribers){
		if (s->isSubscribed(productionName))
			s->onSuscribedProductionAvailable(productionName);
	}
}

void PrintingAgency::addProduction(string name){
	produnctions.push_back(name);
	notifySubscribers(name);
}


void Subscriber::addSubscription(const string& productionName){
	subscriptions.push_back(productionName);
	available.push_back(false);
}

void Subscriber::removeSubscription(const string& productionName){
	auto it = find(subscriptions.begin(), subscriptions.end(), productionName);
	subscriptions.erase(it);
}

boolean Subscriber::isSubscribed(const string& productionName){
	return find(subscriptions.begin(), subscriptions.end(), productionName) != subscriptions.end();
}

boolean Subscriber::isSubscriptionAvailable(const string& productionName){
	auto it = find(subscriptions.begin(), subscriptions.end(), productionName);
	return it != subscriptions.end() && (available[distance(subscriptions.begin(), it)]);
}

void Subscriber::onSuscribedProductionAvailable(const string& productionName){
	cout << productionName << " is available now\n";
	auto it = find(subscriptions.begin(), subscriptions.end(), productionName);
	if (it != subscriptions.end())
		available[distance(subscriptions.begin(), it)] = true;
}


int main(int argc, char* argv[])
{
	Catch::Session().run(argc, argv);
	system("PAUSE");
	return 0;
}

TEST_CASE("Subscriber subscriptions"){
	Subscriber s;
	s.addSubscription("Title 1");
	s.addSubscription("Title 2");
	CHECK(s.isSubscribed("Title 1") == true);
	CHECK(s.isSubscribed("Title 2") == true);
	CHECK(s.isSubscribed("Title 3") == false);
	s.removeSubscription("Title 1");
	CHECK(s.isSubscribed("Title 1") == false);
}

TEST_CASE("Main"){
	string s1 = "Production 1";
	string s2 = "Production 2";
	string s3 = "Production 3";
	PrintingAgency agency;
	// Subscriber 1
	Subscriber* s = new Subscriber();
	s->addSubscription(s1);
	agency.registerSubscriber(s);

	CHECK(s->isSubscribed(s1));
	CHECK_FALSE(s->isSubscribed(s2));

	CHECK_FALSE(s->isSubscriptionAvailable(s1));
	agency.addProduction(s1);
	CHECK(s->isSubscriptionAvailable(s1));

	s->addSubscription(s2);
	CHECK(s->isSubscribed(s2));
	agency.addProduction(s2);
	CHECK(s->isSubscriptionAvailable(s2));

	// Subscriber 2
	Subscriber* sub2 = new Subscriber();
	s->addSubscription(s3);
	sub2->addSubscription(s3);
	CHECK(s->isSubscribed(s3));
	CHECK(sub2->isSubscribed(s3));
	CHECK_FALSE(s->isSubscriptionAvailable(s3));
	CHECK_FALSE(sub2->isSubscriptionAvailable(s3));
	agency.registerSubscriber(sub2);
	agency.addProduction(s3);
	CHECK(s->isSubscriptionAvailable(s3));
	CHECK(sub2->isSubscriptionAvailable(s3));
}