#pragma once
#include <string.h>
#include <string>
#include <cwchar>
#include <map>
#include <vector>

using namespace std;

class args
{
	struct flags
	{
		wchar_t * flag;
		wchar_t * description;
		wchar_t * default;
		bool required;
	}
	options[3] =
	{
		{ L"key", L"API key", L"", true },
		{ L"model", L"Name of the language model", L"Title", false },
		{ L"order", L"Order of language model", L"5", false }
	};

	public:

		args(int argc, wchar_t * argv[])
		{
			wstring phrase;
			_valid = true;
			int arg = 1;

			while (arg < argc && _valid)
			{
				if (L'-' == argv[arg][0])
				{
					const wchar_t * const flag = argv[arg++] + 1;

					if (!wcscmp(flag, L"-"))
					{
						if (!phrase.empty())
						{
							_phrases.push_back(phrase);
							phrase.erase();
						}
						continue;
					}

					bool found = false;
					for (auto option : options)
					{
						if (!wcscmp(flag, option.flag))
						{
							if (arg < argc)
							{
								found = true;
								_opts.insert(pair<wstring, wstring>(flag, argv[arg]));
								arg++;
								break;
							}
							else
							{
								fprintf(stderr, "Option missing value (-%S)\n", flag);
							}
						}						
					}
					if (!found)
					{
						fprintf(stderr, "Unknown flag (-%S)\n", flag);
						_valid = false;
					}
				}
				else
				{
					if (!phrase.empty())
					{
						phrase.append(L" ");
					}
					phrase.append(argv[arg++]);
				}
			}

			if (phrase.size() > 0)
			{
				_phrases.push_back(phrase);

			}
			for (auto option : options)
			{
				if (_opts.find(option.flag) == _opts.end())
				{
					if (option.required)
					{
						fprintf(stderr, "Option (-%S) is required\n", option.flag);
						_valid = false;
					}
					else
					{
						_opts.insert(pair<wstring, wstring>(option.flag, option.default));
					}
				}
			}

			if (_phrases.empty())
			{
				fprintf(stderr, "No phrases specified\n");
				_valid = false;
			}
		}

	public:

		bool valid() const
		{
			return _valid;
		}

		void showUsage() const
		{
			wcout << L"Gets the joint probability of a sequence of words" << endl;
			wcout << L"You may specify multiple phrases by separating with --" << endl;
			wcout << L"usage: GetJointProbability ";
			for (auto option : options)
			{
				wcout << L"-" << option.flag << L" {" << option.flag << L"} ";
			}
			wcout << L"{phrase} [-- {phrase}]*" << endl;
			for (auto option : options)
			{
				wcout << L"\t-" << option.flag << L"\t - " << option.description;
				if (option.required)
				{
					wcout << L" (Required)" << endl;
				}
				else
				{
					wcout << L" (Optional, default=\"" << option.default << "\")" << endl;
				}
			}
		}

		const wstring & get(wstring key) const
		{
			return _opts.find(key)->second;
		}

		const vector<wstring> & phrases() const
		{
			return _phrases;
		}

	private:

		bool _valid;
		map<wstring, wstring> _opts;
		vector<wstring> _phrases;
};