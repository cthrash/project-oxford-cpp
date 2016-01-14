// GetJointProbability.cpp : Defines the entry point for the console application.
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

	client.calculateJointProbability(args.get(L"model"), args.get(L"order"), args.phrases())
		.then([=](json::value v)
		{
			const json::array & results = v[L"results"].as_array();
			for (auto result = results.cbegin(); result != results.cend(); result++)
			{
				wcout << result->at(L"words").as_string();
				wcout << L" (" << result->at(L"probability").as_number().to_double() << ")" << endl;
			}
		})
		.wait();

	return 0;
}

