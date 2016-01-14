// PredictNext.cpp : Defines the entry point for the console application.
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

	client.generateNextWords(args.get(L"model"), args.get(L"order"), args.get(L"max"), args.phrase())
		.then([=](json::value v)
		{
			const json::array & candidates = v[L"candidates"].as_array();
			for (auto candidate = candidates.cbegin(); candidate != candidates.cend(); candidate++)
			{
				wcout << candidate->at(L"word").as_string();
				wcout << L" (" << candidate->at(L"probability").as_number().to_double() << ")" << endl;
			}
		})
		.wait();

	return 0;
}

