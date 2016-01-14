// ListModels.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "..\..\..\Common\ClientLibrary\ServiceClient.h"
#include "args.h"

using namespace std;

using namespace Microsoft::ProjectOxford::WebLM;

int wmain(int argc, wchar_t *argv[], wchar_t *envp[])
{
	args args(argc, argv);

	if (!args.valid())
	{
		args.showUsage();
		return -1;
	}

	ServiceClient client(args.get(L"key"));

	client.listModels()
		.then([=](json::value v) {
			const json::array & models = v[L"models"].as_array();
			for (auto i = models.cbegin(); i != models.cend(); i++)
			{
				const json::value & model = (*i);
				wcout << L"corpus:              " << model.at(L"corpus").as_string() << endl;
				wcout << L"model:               " << model.at(L"model").as_string() << endl;
				wcout << L"maxOrder:            " << model.at(L"maxOrder").as_number().to_int32() << endl;
				wcout << L"supportedOperations:";
				const json::array & operations = model.at(L"supportedOperations").as_array();
				for (auto j = operations.cbegin(); j != operations.cend(); j++)
				{
					wcout << L" " << j->as_string();
				}
				wcout << endl << endl;
			}
		})
		.wait();

	return 0;
}
