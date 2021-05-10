# Build-Time Components

This folder contains component manafests for build-time components that cannot be downloaded via nuget.
Each component has its own folder and corresponding *cgmanifest.json*. This is to make it easier to find
the files and parse them in shell automation without needing to depend on additional tools for parsing
the json.

The format for *cgmanifest.json* files can be found under 
[https://docs.opensource.microsoft.com/tools/cg/cgmanifest.html](CGManifest File) as part of the
[https://docs.opensource.microsoft.com](Microsoft Open Source Docs). It is *important* that the
files are formatted according to this specification so that open source dependencies are correctly
registered.