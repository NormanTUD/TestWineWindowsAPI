# TestWineWindowsAPI
This is a very hacky test of the Windows-API on Linux with Wine.

# How it's supposed to work
This is supposed to download the MS-API pages automatically and extract examples, so that they can
be compiled with mingw on Linux. It automatically runs them with Wine and reports errors once they
are run.

Results are saved in the `fails.txt` and `success.txt`.

# TODO

Not all code runs. Sometimes, variables are missing. Sometimes, there's no `main` defined and so on.
I have to modify the `modify.pl` so that all example-codes will be proper `C++`-programs.
