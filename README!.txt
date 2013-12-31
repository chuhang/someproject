1. Install gloox
	1.1 download gloox-1.0.9
	1.2 open gloox.vcproj with visual studio and build, then you will have the .lib and .dll files under some directory named gloox-1.0/Debug
	1.3 open the project myfltk, Property-Configuration Properties-C/C++-General-Additional Include Directories, change the directory to where you installed gloox
	1.4 Property-Configuration Properties-Linker-Input, change the location of .lib file
	1.5 Include the .dll file to the project, I did this by copying the .dll file to the Debug directory...

2. Install FLTK
	2.1 download fltk-1.3.2
	2.2 in fltk-1.3.2/ide, select the ide you use and open the project file. Select Release and run (for getting fltk.lib...etc.), then select Debug and run (for getting fltkd.lib...etc).
	2.3 open the project myfltk, Property-Configuration Properties-C/C++-Command Line-Additional Options, change the include path

3. Final Step
	Open FreakyPeepingTool/myfltk.sln, compile and run


For now this is all what I can recall... Maybe there are other settings that I did but cannot remember, but I think the project will compile if you follow the above instructions.