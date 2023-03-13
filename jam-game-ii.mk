##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=jam-game-ii
ConfigurationName      :=Debug
WorkspaceConfiguration :=Debug
WorkspacePath          :=C:/Users/Poga/Desktop/painis/tram
ProjectPath            :=C:/Users/Poga/Desktop/painis/jam-game-ii
IntermediateDirectory  :=../tram/build-$(WorkspaceConfiguration)/__/jam-game-ii
OutDir                 :=$(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=Poga
Date                   :=13/03/2023
CodeLitePath           :="C:/Program Files/CodeLite"
MakeDirCommand         :=mkdir
LinkerName             :=C:/mingw64/bin/g++.exe
SharedObjectLinkerName :=C:/mingw64/bin/g++.exe -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputDirectory        :=C:/Users/Poga/Desktop/painis/tram/build-$(WorkspaceConfiguration)/bin
OutputFile             :=..\tram\build-$(WorkspaceConfiguration)\bin\$(ProjectName).exe
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :=$(IntermediateDirectory)/ObjectsList.txt
PCHCompileFlags        :=
RcCmpOptions           := 
RcCompilerName         :=C:/mingw64/bin/windres.exe
LinkOptions            :=  -std=c++20
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)C:\Users\Poga\Desktop\painis\tram-sdk\src $(IncludeSwitch)C:\Users\Poga\Desktop\painis\tram-sdk\libraries 
IncludePCH             := 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)tramsdk $(LibrarySwitch)BulletSoftBody $(LibrarySwitch)BulletDynamics $(LibrarySwitch)BulletCollision $(LibrarySwitch)LinearMath $(LibrarySwitch)glfw3 $(LibrarySwitch)OpenAL32 $(LibrarySwitch)gdi32 $(LibrarySwitch)opengl32 
ArLibs                 :=  "tramsdk" "BulletSoftBody" "BulletDynamics" "BulletCollision" "LinearMath" "glfw3" "OpenAL32" "gdi32" "opengl32" 
LibPath                := $(LibraryPathSwitch). $(LibraryPathSwitch)C:\Users\Poga\Desktop\painis\tram-sdk $(LibraryPathSwitch)C:\Users\Poga\Desktop\painis\tram-sdk\libraries\binaries\win64 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overridden using an environment variable
##
AR       := C:/mingw64/bin/ar.exe rcu
CXX      := C:/mingw64/bin/g++.exe
CC       := C:/mingw64/bin/gcc.exe
CXXFLAGS :=  -g -O0 -Wall -std=c++20 $(Preprocessors)
CFLAGS   :=  -g -O0 -Wall -std=c++20 $(Preprocessors)
ASFLAGS  := 
AS       := C:/mingw64/bin/as.exe


##
## User defined environment variables
##
CodeLiteDir:=C:\Program Files\CodeLite
WXWIN:=C:\Users\Poga\Desktop\miskaste\wxwidgets
WXCFG:=gcc_dll\mswu
Objects0=$(IntermediateDirectory)/src_entities_trigger.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_camera.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_mongus.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_main.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_entities_pickup.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_entities_crab.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_entities_switch.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_entities_door.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_levelswitch.cpp$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: MakeIntermediateDirs $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@if not exist "$(IntermediateDirectory)" $(MakeDirCommand) "$(IntermediateDirectory)"
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

MakeIntermediateDirs:
	@if not exist "$(IntermediateDirectory)" $(MakeDirCommand) "$(IntermediateDirectory)"
	@if not exist "$(OutputDirectory)" $(MakeDirCommand) "$(OutputDirectory)"

$(IntermediateDirectory)/.d:
	@if not exist "$(IntermediateDirectory)" $(MakeDirCommand) "$(IntermediateDirectory)"

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/src_entities_trigger.cpp$(ObjectSuffix): src/entities/trigger.cpp $(IntermediateDirectory)/src_entities_trigger.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Poga/Desktop/painis/jam-game-ii/src/entities/trigger.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_entities_trigger.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_entities_trigger.cpp$(DependSuffix): src/entities/trigger.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_entities_trigger.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_entities_trigger.cpp$(DependSuffix) -MM src/entities/trigger.cpp

$(IntermediateDirectory)/src_entities_trigger.cpp$(PreprocessSuffix): src/entities/trigger.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_entities_trigger.cpp$(PreprocessSuffix) src/entities/trigger.cpp

$(IntermediateDirectory)/src_camera.cpp$(ObjectSuffix): src/camera.cpp $(IntermediateDirectory)/src_camera.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Poga/Desktop/painis/jam-game-ii/src/camera.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_camera.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_camera.cpp$(DependSuffix): src/camera.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_camera.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_camera.cpp$(DependSuffix) -MM src/camera.cpp

$(IntermediateDirectory)/src_camera.cpp$(PreprocessSuffix): src/camera.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_camera.cpp$(PreprocessSuffix) src/camera.cpp

$(IntermediateDirectory)/src_mongus.cpp$(ObjectSuffix): src/mongus.cpp $(IntermediateDirectory)/src_mongus.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Poga/Desktop/painis/jam-game-ii/src/mongus.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_mongus.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_mongus.cpp$(DependSuffix): src/mongus.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_mongus.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_mongus.cpp$(DependSuffix) -MM src/mongus.cpp

$(IntermediateDirectory)/src_mongus.cpp$(PreprocessSuffix): src/mongus.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_mongus.cpp$(PreprocessSuffix) src/mongus.cpp

$(IntermediateDirectory)/src_main.cpp$(ObjectSuffix): src/main.cpp $(IntermediateDirectory)/src_main.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Poga/Desktop/painis/jam-game-ii/src/main.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_main.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_main.cpp$(DependSuffix): src/main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_main.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_main.cpp$(DependSuffix) -MM src/main.cpp

$(IntermediateDirectory)/src_main.cpp$(PreprocessSuffix): src/main.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_main.cpp$(PreprocessSuffix) src/main.cpp

$(IntermediateDirectory)/src_entities_pickup.cpp$(ObjectSuffix): src/entities/pickup.cpp $(IntermediateDirectory)/src_entities_pickup.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Poga/Desktop/painis/jam-game-ii/src/entities/pickup.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_entities_pickup.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_entities_pickup.cpp$(DependSuffix): src/entities/pickup.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_entities_pickup.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_entities_pickup.cpp$(DependSuffix) -MM src/entities/pickup.cpp

$(IntermediateDirectory)/src_entities_pickup.cpp$(PreprocessSuffix): src/entities/pickup.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_entities_pickup.cpp$(PreprocessSuffix) src/entities/pickup.cpp

$(IntermediateDirectory)/src_entities_crab.cpp$(ObjectSuffix): src/entities/crab.cpp $(IntermediateDirectory)/src_entities_crab.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Poga/Desktop/painis/jam-game-ii/src/entities/crab.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_entities_crab.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_entities_crab.cpp$(DependSuffix): src/entities/crab.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_entities_crab.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_entities_crab.cpp$(DependSuffix) -MM src/entities/crab.cpp

$(IntermediateDirectory)/src_entities_crab.cpp$(PreprocessSuffix): src/entities/crab.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_entities_crab.cpp$(PreprocessSuffix) src/entities/crab.cpp

$(IntermediateDirectory)/src_entities_switch.cpp$(ObjectSuffix): src/entities/switch.cpp $(IntermediateDirectory)/src_entities_switch.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Poga/Desktop/painis/jam-game-ii/src/entities/switch.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_entities_switch.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_entities_switch.cpp$(DependSuffix): src/entities/switch.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_entities_switch.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_entities_switch.cpp$(DependSuffix) -MM src/entities/switch.cpp

$(IntermediateDirectory)/src_entities_switch.cpp$(PreprocessSuffix): src/entities/switch.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_entities_switch.cpp$(PreprocessSuffix) src/entities/switch.cpp

$(IntermediateDirectory)/src_entities_door.cpp$(ObjectSuffix): src/entities/door.cpp $(IntermediateDirectory)/src_entities_door.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Poga/Desktop/painis/jam-game-ii/src/entities/door.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_entities_door.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_entities_door.cpp$(DependSuffix): src/entities/door.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_entities_door.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_entities_door.cpp$(DependSuffix) -MM src/entities/door.cpp

$(IntermediateDirectory)/src_entities_door.cpp$(PreprocessSuffix): src/entities/door.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_entities_door.cpp$(PreprocessSuffix) src/entities/door.cpp

$(IntermediateDirectory)/src_levelswitch.cpp$(ObjectSuffix): src/levelswitch.cpp $(IntermediateDirectory)/src_levelswitch.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Poga/Desktop/painis/jam-game-ii/src/levelswitch.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_levelswitch.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_levelswitch.cpp$(DependSuffix): src/levelswitch.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_levelswitch.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_levelswitch.cpp$(DependSuffix) -MM src/levelswitch.cpp

$(IntermediateDirectory)/src_levelswitch.cpp$(PreprocessSuffix): src/levelswitch.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_levelswitch.cpp$(PreprocessSuffix) src/levelswitch.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r $(IntermediateDirectory)


