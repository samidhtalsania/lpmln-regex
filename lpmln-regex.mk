##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=lpmln-regex
ConfigurationName      :=Debug
WorkspacePath          := "/home/samidh/Documents/lpmln"
ProjectPath            := "/home/samidh/Documents/lpmln/lpmln-regex"
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=samidh
Date                   :=25/05/16
CodeLitePath           :="/home/samidh/.codelite"
LinkerName             :=/usr/bin/g++
SharedObjectLinkerName :=/usr/bin/g++ -shared -fPIC
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
OutputFile             :=$(IntermediateDirectory)/$(ProjectName)
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="lpmln-regex.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  
IncludePath            := $(IncludeSwitch)/usr/include/boost/  $(IncludeSwitch). $(IncludeSwitch). 
IncludePCH             := 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)boost_regex 
ArLibs                 :=  "boost_regex" 
LibPath                :=$(LibraryPathSwitch)/usr/lib/x86_64-linux-gnu/  $(LibraryPathSwitch). $(LibraryPathSwitch)/usr/lib/x86_64-linux-gnu/ 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := /usr/bin/ar rcu
CXX      := /usr/bin/g++
CC       := /usr/bin/gcc
CXXFLAGS :=  -g -O0 -std=c++14 -std=c++11 -Wall $(Preprocessors)
CFLAGS   :=  -g -O0 -Wall $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/main.cpp$(ObjectSuffix) $(IntermediateDirectory)/Predicate.cpp$(ObjectSuffix) $(IntermediateDirectory)/Domain.cpp$(ObjectSuffix) $(IntermediateDirectory)/Completion.cpp$(ObjectSuffix) $(IntermediateDirectory)/FactCompletion.cpp$(ObjectSuffix) $(IntermediateDirectory)/RuleCompletion.cpp$(ObjectSuffix) $(IntermediateDirectory)/Variable.cpp$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

MakeIntermediateDirs:
	@test -d ./Debug || $(MakeDirCommand) ./Debug


$(IntermediateDirectory)/.d:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/main.cpp$(ObjectSuffix): main.cpp $(IntermediateDirectory)/main.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/samidh/Documents/lpmln/lpmln-regex/main.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/main.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/main.cpp$(DependSuffix): main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/main.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/main.cpp$(DependSuffix) -MM "main.cpp"

$(IntermediateDirectory)/main.cpp$(PreprocessSuffix): main.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/main.cpp$(PreprocessSuffix) "main.cpp"

$(IntermediateDirectory)/Predicate.cpp$(ObjectSuffix): Predicate.cpp $(IntermediateDirectory)/Predicate.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/samidh/Documents/lpmln/lpmln-regex/Predicate.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Predicate.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Predicate.cpp$(DependSuffix): Predicate.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Predicate.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Predicate.cpp$(DependSuffix) -MM "Predicate.cpp"

$(IntermediateDirectory)/Predicate.cpp$(PreprocessSuffix): Predicate.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Predicate.cpp$(PreprocessSuffix) "Predicate.cpp"

$(IntermediateDirectory)/Domain.cpp$(ObjectSuffix): Domain.cpp $(IntermediateDirectory)/Domain.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/samidh/Documents/lpmln/lpmln-regex/Domain.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Domain.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Domain.cpp$(DependSuffix): Domain.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Domain.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Domain.cpp$(DependSuffix) -MM "Domain.cpp"

$(IntermediateDirectory)/Domain.cpp$(PreprocessSuffix): Domain.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Domain.cpp$(PreprocessSuffix) "Domain.cpp"

$(IntermediateDirectory)/Completion.cpp$(ObjectSuffix): Completion.cpp $(IntermediateDirectory)/Completion.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/samidh/Documents/lpmln/lpmln-regex/Completion.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Completion.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Completion.cpp$(DependSuffix): Completion.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Completion.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Completion.cpp$(DependSuffix) -MM "Completion.cpp"

$(IntermediateDirectory)/Completion.cpp$(PreprocessSuffix): Completion.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Completion.cpp$(PreprocessSuffix) "Completion.cpp"

$(IntermediateDirectory)/FactCompletion.cpp$(ObjectSuffix): FactCompletion.cpp $(IntermediateDirectory)/FactCompletion.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/samidh/Documents/lpmln/lpmln-regex/FactCompletion.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/FactCompletion.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/FactCompletion.cpp$(DependSuffix): FactCompletion.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/FactCompletion.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/FactCompletion.cpp$(DependSuffix) -MM "FactCompletion.cpp"

$(IntermediateDirectory)/FactCompletion.cpp$(PreprocessSuffix): FactCompletion.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/FactCompletion.cpp$(PreprocessSuffix) "FactCompletion.cpp"

$(IntermediateDirectory)/RuleCompletion.cpp$(ObjectSuffix): RuleCompletion.cpp $(IntermediateDirectory)/RuleCompletion.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/samidh/Documents/lpmln/lpmln-regex/RuleCompletion.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/RuleCompletion.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/RuleCompletion.cpp$(DependSuffix): RuleCompletion.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/RuleCompletion.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/RuleCompletion.cpp$(DependSuffix) -MM "RuleCompletion.cpp"

$(IntermediateDirectory)/RuleCompletion.cpp$(PreprocessSuffix): RuleCompletion.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/RuleCompletion.cpp$(PreprocessSuffix) "RuleCompletion.cpp"

$(IntermediateDirectory)/Variable.cpp$(ObjectSuffix): Variable.cpp $(IntermediateDirectory)/Variable.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/samidh/Documents/lpmln/lpmln-regex/Variable.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Variable.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Variable.cpp$(DependSuffix): Variable.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Variable.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Variable.cpp$(DependSuffix) -MM "Variable.cpp"

$(IntermediateDirectory)/Variable.cpp$(PreprocessSuffix): Variable.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Variable.cpp$(PreprocessSuffix) "Variable.cpp"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


