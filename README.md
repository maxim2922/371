# 371
comp 371

TO INSTALL THE IRRKLANG AUDIO LIBRARY TO THE PROJECT, FOLLOW THESE STEPS:

NOTE: THIS GUIDE IS FOR VISUAL STUDIO USERS

1. Download irrKlang 1.6.0 from this link:
	http://www.ambiera.at/downloads/irrKlang-32bit-1.6.0.zip

2. Extract the folder, open it, and copy the 'irrKlang-1.6.0' folder into the project's ThirdParty folder

3. Go to (Project)/ThirdParty/irrKlang-1.6.0/bin/win32-visualStudio
	and copy these to (Project)/Framework/Bin
	- ikpFlac.dll
	- ikpMP3.dll
	- irrKlang.dll

4. In visual studio, right click the project and click on properties to get to Project Properties
	Go to Project Properties/C C++/General
	* Additional Include Directories: add the "include" folder in ThirdParty/irrKlang-1.6.0/include
5. Project Properties/Linker/Input
	* Additional Dependencies: add irrKlang.lib
6. Project Properties/Linker/General
	* Additional Library Directories: add ThirdParty/lib/Win32-visualStudio
7. Project Properties/Build Events/Post-Build Event
	* Copy this into the Command Line: XCOPY "$(SolutionDir)"\lib*.DLL "$(TargetDir)" /D /K /Y
	* Use in Build: No
