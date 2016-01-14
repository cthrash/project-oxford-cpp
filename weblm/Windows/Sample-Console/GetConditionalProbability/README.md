Web Language Model (WebLM) Sample Application
========================

GetConditionalProbability is sample command-line tool to demonstrate the use of the [Project Oxford](<https://projectoxford.ai>) C++ WebLM ClientLibrary.

This tool shows the conditional probability of a word in a given context (phrase).

```
usage: GetConditionalProbability -key {key} -model {model} -order {order} -word {word} {phrase} [-- [-word {word}] {phrase}]*
        -key     - API key (Required)
        -model   - Name of the language model (Optional, default="Title")
        -order   - Order of language model (Optional, default="5")
        -word    - Word for which the probability is computed (Required)
```

To start, you must first create a [Microsoft Azure](<https://azure.microsoft.com>) account.  
Once you've created an Azure account, you can obtain an [API key](<http://projectoxford.ai/subscription>).  For this tool, the API key must be provided as the value the to `-key` argument.

Contributing
============
We welcome contributions and are always looking for new SDKs, input, and
suggestions. Feel free to file issues on the repo and we'll address them as we can. You can also learn more about how you can help on the [Contribution
Rules & Guidelines](</CONTRIBUTING.md>).

For questions, feedback, or suggestions about Project Oxford services, reach out to us directly on our [Project Oxford Forums](<https://social.msdn.microsoft.com/forums/azure/en-US/home?forum=mlapi>).

License
=======

All Project Oxford SDKs and samples are licensed with the MIT License. For more details, see
[LICENSE](</LICENSE.md>).       

