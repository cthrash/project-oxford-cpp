#include "stdafx.h"
#include <string>
#include "ServiceClient.h"

using namespace Microsoft::ProjectOxford::WebLM;

static const wchar_t * const rootUrl = L"https://api.projectoxford.ai/text/weblm/v1.0/";

ServiceClient::ServiceClient(const wstring subscriptionKey) : _subscriptionKey(subscriptionKey)
{
	_client = new http_client(rootUrl);
}

ServiceClient::~ServiceClient()
{
	delete _client;
}

pplx::task<json::value> ServiceClient::listModels()
{
	http_request req(methods::GET);
	req.set_request_uri(L"/models");
	req.headers().add(L"Ocp-Apim-Subscription-Key", _subscriptionKey);

	return _client->request(req)
		.then([=](http_response response) -> pplx::task<json::value>
		{
			if (response.status_code() == status_codes::OK)
			{
				return response.extract_json();
			}

			throw new std::exception();// response.reason_phrase().front());
		});
}

pplx::task<json::value> ServiceClient::post(uri_builder & path, json::value & body)
{
	http_request req(methods::POST);
	req.headers().add(L"Content-Type", L"application/json");
	req.headers().add(L"Ocp-Apim-Subscription-Key", _subscriptionKey);
	req.set_request_uri(path.to_uri());
	req.set_body(body);
	
	return _client->request(req)
		.then([=](http_response response) -> pplx::task<json::value>
	{
		if (response.status_code() == status_codes::OK)
		{
			return response.extract_json();
		}

		throw new std::exception();// response.reason_phrase().front());
	});
}

pplx::task<json::value> ServiceClient::breakIntoWords(
	const wstring & model,
	const wstring & order,
	const wstring & maxCandidates,
	const wstring & text)
{
	uri_builder path(L"/breakIntoWords");
	path.append_query(L"model", model);
	path.append_query(L"order", order);
	path.append_query(L"maxNumOfCandidatesReturned", maxCandidates);
	path.append_query(L"text", text);
	return post(path, json::value());
}

pplx::task<json::value> ServiceClient::generateNextWords(
	const wstring & model,
	const wstring & order,
	const wstring & maxCandidates,
	const wstring & context)
{
	uri_builder path(L"/generateNextWords");
	path.append_query(L"model", model);
	path.append_query(L"order", order);
	path.append_query(L"maxNumOfCandidatesReturned", maxCandidates);
	path.append_query(L"words", context);
	return post(path, json::value());
}

pplx::task<json::value> ServiceClient::calculateConditionalProbability(
	const wstring & model,
	const wstring & order,
	const wstring & context,
	const wstring & word)
{
	vector<pair<wstring, wstring>> queries;
	queries.push_back(pair<wstring, wstring>(context, word));
	return calculateConditionalProbability(model, order, queries);
}

pplx::task<json::value> ServiceClient::calculateConditionalProbability(
	const wstring & model,
	const wstring & order,
	const vector<pair<wstring, wstring>> & queries)
{
	uri_builder path(L"/calculateConditionalProbability");
	path.append_query(L"model", model);
	path.append_query(L"order", order);
	vector<json::value> array;
	for (auto query = queries.cbegin(); query != queries.cend(); query++)
	{
		json::value item;
		item[L"words"] = json::value::string(query->first);
		item[L"word"] = json::value::string(query->second);
		array.push_back(item);
	}
	json::value body;
	body[L"queries"] = json::value::array(array);
	return post(path, body);
}


pplx::task<json::value> ServiceClient::calculateJointProbability(
	const wstring & model,
	const wstring & order,
	const wstring & phrase)
{
	vector<wstring> phrases;
	phrases.push_back(phrase);
	return calculateJointProbability(model, order, phrases);
}

pplx::task<json::value> ServiceClient::calculateJointProbability(
	const wstring & model,
	const wstring & order,
	const vector<wstring> & phrases)
{
	uri_builder path(L"/calculateJointProbability");
	path.append_query(L"model", model);
	path.append_query(L"order", order);
	vector<json::value> array;
	for (auto phrase = phrases.cbegin(); phrase != phrases.cend(); phrase++)
	{
		array.push_back(json::value::string(*phrase));
	}
	json::value body;
	body[L"queries"] = json::value::array(array);
	return post(path, body);
}
