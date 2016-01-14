#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <string>

using namespace std;                        // Standard Template Library
using namespace utility;                    // Common utilities like string conversions
using namespace web;                        // Common features like URIs.
using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;          // HTTP client feature

namespace Microsoft {
	namespace ProjectOxford {
		namespace WebLM {
			class ServiceClient
			{
				public:

					// Constructor.  You can obtain a subscriptionKey by getting an Azure account
					// at https://azure.microsoft.com and then going to https://www.projectoxford.ai/Subscription
					ServiceClient(const wstring subscriptionKey);
					~ServiceClient();

				public:

					// Get a list of language models available.
					// For each model, the highest order and supported methods are listed.
					pplx::task<json::value> listModels();

					// Break a catenated phrases into constituent words.
					pplx::task<json::value> breakIntoWords(
						const wstring & model,
						const wstring & order,
						const wstring & maxCandidates,
						const wstring & text);

					// Generate a list of words that could follow a phrase.
					pplx::task<json::value> generateNextWords(
						const wstring & model,
						const wstring & order,
						const wstring & maxCandidates,
						const wstring & context);

					// Calculate the conditional probability of a word, given a context.
					// Overload for single (context, word) pair.
					pplx::task<json::value> calculateConditionalProbability(
						const wstring & model,
						const wstring & order,
						const wstring & context,
						const wstring & word);
					
					// Calculate the conditional probability of a word, given a context.
					// Overload for multiple (context, word) pairs.
					pplx::task<json::value> calculateConditionalProbability(
						const wstring & model,
						const wstring & order,
						const vector<pair<wstring, wstring>> & queries);

					// Calculate the joint probability of a phrase.
					// Overload for single phrase.
					pplx::task<json::value> calculateJointProbability(
						const wstring & model,
						const wstring & order,
						const wstring & phrase);

					// Calculate the joint probability of a phrase.
					// Overload for multiple phrases.
					pplx::task<json::value> calculateJointProbability(
						const wstring & model,
						const wstring & order,
						const vector<wstring> & phrases);

				private:

					// Common HTTP post method.
					pplx::task<json::value> post(uri_builder & path, json::value & body);

				private:

					http_client * _client;
					wstring _subscriptionKey;
			};
		}
	}
}
