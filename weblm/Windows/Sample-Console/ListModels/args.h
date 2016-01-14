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
	options[1] =
	{
		{ L"key", L"API key", L"", true }
	};

	public:

		args(int argc, wchar_t * argv[])
		{
			_valid = true;
			int arg = 1;

			while (arg < argc && _valid)
			{
				if (L'-' == argv[arg][0])
				{
					const wchar_t * const flag = argv[arg++] + 1;
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
					fprintf(stderr, "Unexpected argument (%S), ignoring", argv[arg-1]);
				}
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
		}

	public:

		bool valid() const
		{
			return _valid;
		}

		void showUsage() const
		{
			wcout << L"Lists the language models currently supported" << endl;
			wcout << L"usage: ListModels ";
			for (auto option : options)
			{
				wcout << L"-" << option.flag << L" {" << option.flag << L"} ";
			}
			wcout << endl;
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

	private:

		bool _valid;
		map<wstring, wstring> _opts;
};