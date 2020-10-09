//-----------------------------------------------------------------------------
// Flags       : clang-format SMTGSequencer
// Project     : VST SDK
//
// Category    : Validator
// Filename    : validator.cpp
// Created by  : Steinberg, 04/2005
// Description : VST 3 Plug-in Validator class
//
//-----------------------------------------------------------------------------
// LICENSE
// (c) 2020, Steinberg Media Technologies GmbH, All Rights Reserved
//-----------------------------------------------------------------------------
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistributions of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//   * Redistributions in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//   * Neither the name of the Steinberg Media Technologies nor the names of its
//     contributors may be used to endorse or promote products derived from this
//     software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
// IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
// OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
// OF THE POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------

#include "validator.h"
#include "testsuite.h"
#include "public.sdk/source/vst/hosting/plugprovider.h"
#include "public.sdk/source/vst/testsuite/vststructsizecheck.h"
#include "public.sdk/source/vst/utility/stringconvert.h"
#include "base/source/fcommandline.h"
#include "pluginterfaces/vst/ivstaudioprocessor.h"
#include "pluginterfaces/vst/ivstcomponent.h"
#include "pluginterfaces/vst/ivsteditcontroller.h"
#include "pluginterfaces/vst/ivsthostapplication.h"
#include "pluginterfaces/vst/ivstunits.h"

#if SMTG_OS_WINDOWS
#include <conio.h>
#include <windows.h>
#endif

#include <array>
#include <cstdio>
#include <iostream>

//------------------------------------------------------------------------
namespace Steinberg {
FUnknown* gStandardPluginContext = nullptr;

//------------------------------------------------------------------------
namespace Vst {
namespace {

constexpr auto SEPARATOR = "-------------------------------------------------------------\n";
constexpr auto VALIDATOR_INFO = kVstVersionString
    " Plug-in Validator\n"
    "Program by Steinberg (Built on " __DATE__ ")\n";

//------------------------------------------------------------------------
bool filterClassCategory (FIDString category, FIDString classCategory)
{
	return strcmp (category, classCategory) == 0;
}

//------------------------------------------------------------------------
void printAllInstalledPlugins (std::ostream* os)
{
	if (!os)
		return;

	*os << "Searching installed Plug-ins...\n";
	os->flush ();

	auto paths = VST3::Hosting::Module::getModulePaths ();
	if (paths.empty ())
	{
		*os << "No Plug-ins found.\n";
		return;
	}
	for (const auto& path : paths)
	{
		*os << path << "\n";
	}
}

//------------------------------------------------------------------------
void printAllSnapshots (std::ostream* os)
{
	if (!os)
		return;
	*os << "Searching installed Plug-ins...\n";
	auto paths = VST3::Hosting::Module::getModulePaths ();
	if (paths.empty ())
	{
		*os << "No Plug-ins found.\n";
		return;
	}
	for (const auto& path : paths)
	{
		auto snapshots = VST3::Hosting::Module::getSnapshots (path);
		if (snapshots.empty ())
		{
			*os << "No snapshots in " << path << "\n";
			continue;
		}
		for (auto& snapshot : snapshots)
		{
			for (auto& desc : snapshot.images)
			{
				*os << "Snapshot : " << desc.path << "[" << desc.scaleFactor << "x]\n";
			}
		}
	}
}

//------------------------------------------------------------------------
void printFactoryInfo (const VST3::Hosting::PluginFactory& factory, std::ostream* os)
{
	if (os)
	{
		*os << "* Scanning classes...\n\n";

		auto factoryInfo = factory.info ();

		*os << "  Factory Info:\n\tvendor = " << factoryInfo.vendor ()
		    << "\n\turl = " << factoryInfo.url () << "\n\temail = " << factoryInfo.email ()
		    << "\n\n";

		//---print all included plug-ins---------------
		uint32 i = 0;
		for (auto& classInfo : factory.classInfos ())
		{
			*os << "  Class Info " << i << ":\n\tname = " << classInfo.name ()
			    << "\n\tcategory = " << classInfo.category ()
			    << "\n\tsubCategories = " << classInfo.subCategoriesString ()
			    << "\n\tversion = " << classInfo.version ()
			    << "\n\tsdkVersion = " << classInfo.sdkVersion ()
			    << "\n\tcid = " << classInfo.ID ().toString () << "\n\n";
			++i;
		}
	}
}

//------------------------------------------------------------------------
void checkModuleSnapshots (const VST3::Hosting::Module::Ptr& module, std::ostream* infoStream)
{
	if (infoStream)
		*infoStream << "* Checking snapshots...\n\n";

	auto snapshots = VST3::Hosting::Module::getSnapshots (module->getPath ());
	if (snapshots.empty ())
	{
		if (infoStream)
			*infoStream << "Warning: No snapshots in Bundle.\n\n";
	}
	else
	{
		for (auto& classInfo : module->getFactory ().classInfos ())
		{
			if (!filterClassCategory (kVstAudioEffectClass, classInfo.category ().data ()))
				continue;
			bool found = false;
			for (auto& snapshot : snapshots)
			{
				if (snapshot.uid == classInfo.ID ())
				{
					found = true;
					if (infoStream)
					{
						*infoStream << "Found snapshots for '" << classInfo.name () << "'\n";
						for (auto& image : snapshot.images)
							*infoStream << " - " << image.path << " [" << image.scaleFactor
							            << "x]\n";
						*infoStream << "\n";
					}
					break;
				}
			}
			if (!found)
			{
				if (infoStream)
				{
					*infoStream << "Warning: No snapshot for '" << classInfo.name ()
					            << "' in Bundle.\n\n";
				}
			}
		}
	}
}

//------------------------------------------------------------------------
//-- Options
constexpr auto optHelp = "help";
constexpr auto optVersion = "version";
constexpr auto optLocalInstance = "l";
constexpr auto optSuiteName = "suite";
constexpr auto optExtensiveTests = "e";
constexpr auto optQuiet = "q";
constexpr auto optTestComponentPath = "test-component";
constexpr auto optListInstalledPlugIns = "list";
constexpr auto optListPlugInSnapshots = "snapshots";
constexpr auto optCID = "cid";

//------------------------------------------------------------------------
} // anonymous

//------------------------------------------------------------------------
// Validator
//------------------------------------------------------------------------
Validator::Validator (int argc, char* argv[]) : argc (argc), argv (argv)
{
	infoStream = &std::cout;
	errorStream = &std::cout;

	mPlugInterfaceSupport = owned (NEW PlugInterfaceSupport);

	gStandardPluginContext = this->unknownCast ();
	TestingPluginContext::set (gStandardPluginContext);
}

//------------------------------------------------------------------------
Validator::~Validator ()
{
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API Validator::queryInterface (const char* _iid, void** obj)
{
	QUERY_INTERFACE (_iid, obj, IHostApplication::iid, IHostApplication)
	QUERY_INTERFACE (_iid, obj, ITestResult::iid, ITestResult)

	if (mPlugInterfaceSupport && mPlugInterfaceSupport->queryInterface (_iid, obj) == kResultTrue)
		return ::Steinberg::kResultOk;

	return FObject::queryInterface (_iid, obj);
}

//------------------------------------------------------------------------
void PLUGIN_API Validator::addErrorMessage (const tchar* msg)
{
	if (errorStream)
	{
		auto str = VST3::StringConvert::convert (msg);
		*errorStream << "ERROR: " << str << "\n";
	}
}

//------------------------------------------------------------------------
void PLUGIN_API Validator::addMessage (const tchar* msg)
{
	if (infoStream)
	{
		auto str = VST3::StringConvert::convert (msg);
		*infoStream << "Info:  " << str << "\n";
	}
}

//------------------------------------------------------------------------
tresult PLUGIN_API Validator::getName (String128 name)
{
	VST3::StringConvert::convert ("vstvalidator", name, 127);
	return kResultTrue;
}

//------------------------------------------------------------------------
tresult PLUGIN_API Validator::createInstance (TUID cid, TUID iid, void** obj)
{
	FUID classID = FUID::fromTUID (cid);
	FUID interfaceID = FUID::fromTUID (iid);
	if (classID == IMessage::iid && interfaceID == IMessage::iid)
	{
		*obj = new HostMessage;
		return kResultTrue;
	}
	else if (classID == IAttributeList::iid && interfaceID == IAttributeList::iid)
	{
		*obj = new HostAttributeList;
		return kResultTrue;
	}
	*obj = nullptr;
	return kResultFalse;
}

//------------------------------------------------------------------------
int Validator::run ()
{
	// defaults
	bool useGlobalInstance = true;
	bool useExtensiveTests = false;
	std::string testSuiteName;

	// parse command line
	CommandLine::Descriptions desc;
	CommandLine::VariablesMap valueMap;
	CommandLine::FilesVector files;

	using Description = CommandLine::Description;
	desc.addOptions (
	    kVstVersionString " Plug-in Validator",
	    {{optHelp, "Print help", Description::kBool},
	     {optVersion, "Print version", Description::kBool},
	     {optLocalInstance, "Use local instance per test", Description::kBool},
	     {optSuiteName, "[name] Only run a special test suite", Description::kString},
	     {optExtensiveTests, "Run extensive tests [may take a long time]", Description::kBool},
	     {optQuiet, "Only print errors", Description::kBool},
	     {optCID, "Only test processor with specified class ID", Description::kString},
	     {optTestComponentPath,
	      "[path] Path to an additional component which includes custom tests",
	      Description::kString},
	     {optListInstalledPlugIns, "Show all installed Plug-Ins", Description::kBool},
	     {optListPlugInSnapshots, "List snapshots from all installed Plug-Ins",
	      Description::kBool}});

	CommandLine::parse (argc, argv, desc, valueMap, &files);
	if (valueMap.count (optVersion))
	{
		std::cout << VALIDATOR_INFO;
		return 0;
	}
	else if (valueMap.count (optListInstalledPlugIns))
	{
		printAllInstalledPlugins (infoStream);
		return 0;
	}
	else if (valueMap.count (optListPlugInSnapshots))
	{
		printAllSnapshots (infoStream);
		return 0;
	}
	else if (valueMap.hasError () || valueMap.count (optHelp) || files.empty ())
	{
		std::cout << "\n" << desc << "\n";
		std::cout << "Usage: vstvalidator [options] vst3module\n\n";
		return 1;
	}

	if (valueMap.count (optLocalInstance))
		useGlobalInstance = false;
	if (valueMap.count (optExtensiveTests))
		useExtensiveTests = true;
	if (valueMap.count (optQuiet))
		infoStream = nullptr;
	if (valueMap.count (optSuiteName))
		testSuiteName = valueMap[optSuiteName];
	VST3::Optional<VST3::UID> testProcessor;
	if (valueMap.count (optCID))
		testProcessor = VST3::UID::fromString (valueMap[optCID]);
	std::string customTestComponentPath;
	if (valueMap.count (optTestComponentPath))
		customTestComponentPath = valueMap[optTestComponentPath];

	auto globalFailure = false;
	for (auto& path : files)
	{

#if SMTG_OS_WINDOWS
// TODO: Impl
#else
		// if path is not absolute, create one
		if (path[0] != '/')
		{
			auto realPath = realpath (path.data (), NULL);
			if (realPath)
			{
				path.assign (realPath);
				free (realPath);
			}
		}
		if (customTestComponentPath.empty () == false && customTestComponentPath[0] != '/')
		{
			auto realPath = realpath (customTestComponentPath.data (), NULL);
			if (realPath)
			{
				customTestComponentPath.assign (realPath);
				free (realPath);
			}
		}
#endif

		//---load VST module-----------------
		if (infoStream)
			*infoStream << "* Loading module...\n\n\t" << path << "\n\n";

		std::string error;
		auto module = Module::create (path, error);
		if (!module)
		{
			*errorStream << "Invalid Module!\n";
			if (!error.empty ())
				*errorStream << error << "\n";
			return -1;
		}

		testModule (module, {useGlobalInstance, useExtensiveTests, customTestComponentPath,
		                     testSuiteName, std::move (testProcessor)});

		if (numTestsFailed > 0)
			globalFailure = true;
	}

	return globalFailure ? -1 : 0;
}

//------------------------------------------------------------------------
void Validator::testModule (Module::Ptr module, const ModuleTestConfig& config)
{
	using TestFactoryMap = std::map<std::string, IPtr<ITestFactory>>;
	using PlugProviderVector = std::vector<IPtr<PlugProvider>>;

	numTestsFailed = numTestsPassed = 0;

	auto factory = module->getFactory ();
	printFactoryInfo (module->getFactory (), infoStream);

	//---check for snapshots-----------------
	checkModuleSnapshots (module, infoStream);

	Module::Ptr testModule;
	PlugProviderVector plugProviders;
	TestFactoryMap testFactories;
	auto testSuite = owned (new TestSuite ("Tests"));

	//---create tests---------------
	if (infoStream)
		*infoStream << "* Creating tests...\n\n";
	for (auto& classInfo : factory.classInfos ())
	{
		if (filterClassCategory (kVstAudioEffectClass, classInfo.category ().data ()))
		{
			if (!config.testProcessor || *config.testProcessor == classInfo.ID ())
			{
				auto plugProvider =
				    owned (new PlugProvider (factory, classInfo, config.useGlobalInstance));
				if (plugProvider)
				{
					auto tests = createTests (plugProvider, classInfo.name ().data (),
					                          config.useExtensiveTests);
					testSuite->addTestSuite (classInfo.name ().data (), tests);
					plugProviders.emplace_back (plugProvider);
				}
			}
		}
		else if (filterClassCategory (kTestClass, classInfo.category ().data ()))
		{ // gather test factories supplied by the plug-in
			if (auto testFactory = factory.createInstance<ITestFactory> (classInfo.ID ()))
			{
				testFactories.insert (std::make_pair (classInfo.name ().data (), testFactory));
			}
		}
	}

	// now check testModule if supplied
	if (config.customTestComponentPath.empty () == false)
	{
		std::string error;
		testModule = Module::create (config.customTestComponentPath, error);
		if (testModule)
		{
			const auto& _factory = testModule->getFactory ();
			for (const auto& classInfo : _factory.classInfos ())
			{
				if (filterClassCategory (kTestClass, classInfo.category ().data ()))
				{ // gather test factories supplied by the plug-in
					if (auto testFactory = _factory.createInstance<ITestFactory> (classInfo.ID ()))
					{
						testFactories.insert (
						    std::make_pair (classInfo.name ().data (), testFactory));
					}
				}
			}
		}
		else if (errorStream)
		{
			*errorStream << "Could not create custom test component ["
			             << config.customTestComponentPath << "]\n";
		}
	}
	if (infoStream && !testFactories.empty ())
		*infoStream << "* Creating Plug-in supplied tests...\n\n";
	// create plug-in supplied tests
	for (const auto& item : testFactories)
	{
		for (const auto& plugProvider : plugProviders)
		{
			IPtr<TestSuite> plugTestSuite = owned (new TestSuite (item.first.data ()));
			if (item.second->createTests (plugProvider->unknownCast (), plugTestSuite) ==
			    kResultTrue)
			{
				testSuite->addTestSuite (plugTestSuite->getName ().data (), plugTestSuite);
			}
		}
	}
	testFactories.clear ();

	//---run tests---------------------------
	if (infoStream)
		*infoStream << "* Running tests...\n\n";

	runTestSuite (testSuite,
	              config.testSuiteName.empty () ? nullptr : config.testSuiteName.data ());

	if (infoStream)
	{
		*infoStream << SEPARATOR;
		*infoStream << "Result: " << numTestsPassed << " tests passed, " << numTestsFailed
		            << " tests failed\n";
		*infoStream << SEPARATOR;
	}
}

//------------------------------------------------------------------------
namespace { // anonymous

//------------------------------------------------------------------------
template <typename T, typename... Args>
void createTest (ITestSuite* parent, ITestPlugProvider* plugProvider, Args&&... arguments)
{
	auto test = owned (new T (plugProvider, std::forward<Args> (arguments)...));
	parent->addTest (test->getName (), test);
}

//------------------------------------------------------------------------
void createSpeakerArrangementTest (ITestSuite* parent, ITestPlugProvider* plugProvider,
                                   SymbolicSampleSizes sampleSize, SpeakerArrangement inSpArr,
                                   SpeakerArrangement outSpArr)
{
	createTest<SpeakerArrangementTest> (parent, plugProvider, sampleSize, inSpArr, outSpArr);
}

//------------------------------------------------------------------------
void createPrecisionTests (ITestSuite* parent, ITestPlugProvider* plugProvider,
                           SymbolicSampleSizes sampleSize, bool extensive)
{
	createTest<ProcessTest> (parent, plugProvider, sampleSize);
	createTest<ProcessThreadTest> (parent, plugProvider, sampleSize);
	createTest<SilenceFlagsTest> (parent, plugProvider, sampleSize);
	createTest<SilenceProcessingTest> (parent, plugProvider, sampleSize);
	createTest<FlushParamTest> (parent, plugProvider, sampleSize);
	createTest<FlushParamTest2> (parent, plugProvider, sampleSize);
	createTest<FlushParamTest3> (parent, plugProvider, sampleSize);
	createTest<VariableBlockSizeTest> (parent, plugProvider, sampleSize);
	createTest<ProcessFormatTest> (parent, plugProvider, sampleSize);
	createTest<BypassPersistenceTest> (parent, plugProvider, sampleSize);

	if (extensive)
	{
		constexpr std::array<SpeakerArrangement, 15> saArray = {
		    {SpeakerArr::kMono, SpeakerArr::kStereo, SpeakerArr::kStereoSurround,
		     SpeakerArr::kStereoCenter, SpeakerArr::kStereoSide, SpeakerArr::kStereoCLfe,
		     SpeakerArr::k30Cine, SpeakerArr::k30Music, SpeakerArr::k31Cine, SpeakerArr::k31Music,
		     SpeakerArr::k40Cine, SpeakerArr::k40Music, SpeakerArr::k41Cine, SpeakerArr::k41Music,
		     SpeakerArr::k50}};

		for (auto inArr : saArray)
		{
			for (auto outArr : saArray)
			{
				createSpeakerArrangementTest (parent, plugProvider, sampleSize, inArr, outArr);
			}
		}

		constexpr std::array<int32, 3> autoRates = {{100, 50, 1}};
		constexpr std::array<int32, 4> numParams = {{1, 2, 10, -1}};
		for (auto inp : numParams)
		{
			for (auto iar : autoRates)
			{
				createTest<AutomationTest> (parent, plugProvider, sampleSize, iar, inp, false);
				createTest<AutomationTest> (parent, plugProvider, sampleSize, iar, inp, true);
			}
		}
	}
	else
	{
		createSpeakerArrangementTest (parent, plugProvider, sampleSize, SpeakerArr::kStereo,
		                              SpeakerArr::kStereo);
		createSpeakerArrangementTest (parent, plugProvider, sampleSize, SpeakerArr::kMono,
		                              SpeakerArr::kMono);

		createTest<AutomationTest> (parent, plugProvider, sampleSize, 100, 1, false);
		createTest<AutomationTest> (parent, plugProvider, sampleSize, 100, 1, true);
	}
}

//------------------------------------------------------------------------
} // anonymous

//------------------------------------------------------------------------
IPtr<TestSuite> Validator::createTests (ITestPlugProvider* plugProvider,
                                        const ConstString& plugName, bool extensive)
{
	IPtr<TestSuite> plugTestSuite = owned (new TestSuite (plugName));

	IPtr<TestSuite> generalTests = owned (new TestSuite ("General Tests"));
	// todo: add tests here!
	createTest<EditorClassesTest> (generalTests, plugProvider);
	createTest<ScanBussesTest> (generalTests, plugProvider);
	createTest<ScanParametersTest> (generalTests, plugProvider);
	createTest<MidiMappingTest> (generalTests, plugProvider);
	createTest<MidiLearnTest> (generalTests, plugProvider);
	createTest<UnitInfoTest> (generalTests, plugProvider);
	createTest<ProgramInfoTest> (generalTests, plugProvider);
	createTest<TerminateInitializeTest> (generalTests, plugProvider);
	createTest<UnitStructureTest> (generalTests, plugProvider);
	createTest<ValidStateTransitionTest> (generalTests, plugProvider);
	//	createTest<InvalidStateTransitionTest> (generalTests, plugProvider);
	//	createTest<RepeatIdenticalStateTransitionTest> (generalTests, plugProvider);

	createTest<BusConsistencyTest> (generalTests, plugProvider);
	//	createTest<BusInvalidIndexTest> (generalTests, plugProvider);
	createTest<BusActivationTest> (generalTests, plugProvider);

	createTest<CheckAudioBusArrangementTest> (generalTests, plugProvider);
	createTest<SideChainArrangementTest> (generalTests, plugProvider);

	createTest<SuspendResumeTest> (generalTests, plugProvider, kSample32);

	createTest<NoteExpressionTest> (generalTests, plugProvider);
	createTest<KeyswitchTest> (generalTests, plugProvider);
	createTest<ProcessContextRequirementsTest> (generalTests, plugProvider);

	plugTestSuite->addTestSuite (generalTests->getName ().data (), generalTests);

	IPtr<TestSuite> singlePrecisionTests =
	    owned (new TestSuite ("Single Precision (32 bit) Tests"));
	createPrecisionTests (singlePrecisionTests, plugProvider, kSample32, extensive);
	plugTestSuite->addTestSuite (singlePrecisionTests->getName ().data (), singlePrecisionTests);

	IPtr<TestSuite> doublePrecisionTests =
	    owned (new TestSuite ("Double Precision (64 bit) Tests"));
	createPrecisionTests (doublePrecisionTests, plugProvider, kSample64, extensive);
	plugTestSuite->addTestSuite (doublePrecisionTests->getName ().data (), doublePrecisionTests);

	return plugTestSuite;
}

//------------------------------------------------------------------------
void Validator::addTest (ITestSuite* _testSuite, TestBase* testItem)
{
	_testSuite->addTest (testItem->getName (), testItem);
	testItem->release ();
}

//------------------------------------------------------------------------
void Validator::runTestSuite (TestSuite* suite, FIDString nameFilter)
{
	std::string name;
	if (nameFilter == nullptr || suite->getName () == nameFilter)
	{
		nameFilter = nullptr; // make sure if suiteName is the namefilter that sub suite will run
		ITest* testItem = nullptr;
		// first run all tests in the suite
		for (int32 i = 0; i < suite->getTestCount (); i++)
		{
			if (suite->getTest (i, testItem, name) == kResultTrue)
			{
				if (infoStream)
					*infoStream << "[" << name << "]\n";

				if (testItem->setup ())
				{
					bool success = testItem->run (this);
					if (success)
					{
						if (infoStream)
							*infoStream << "[Succeeded]\n";
						numTestsPassed++;
					}
					else
					{
						if (infoStream)
							*infoStream << "[XXXXXXX Failed]\n";
						if (errorStream && errorStream != infoStream)
						{
							*errorStream << "Test [" << name << "] ";
							*errorStream << "Failed\n";
						}
						numTestsFailed++;
					}

					if (!testItem->teardown ())
					{
						if (infoStream)
							*infoStream << "Failed to teardown test!\n";
						if (errorStream && errorStream != infoStream)
						{
							*errorStream << "[" << name << "] ";
							*errorStream << "Failed to teardown test!\n";
						}
					}
				}
				else
				{
					testItem->teardown ();
					if (infoStream)
						*infoStream << "Failed to setup test!\n";
					if (errorStream && errorStream != infoStream)
					{
						*errorStream << "[" << name << "] ";
						*errorStream << "Failed to setup test!\n";
					}
				}
				if (infoStream)
					*infoStream << "\n";
			}
		}
	}
	// next run sub suites
	int32 subTestSuiteIndex = 0;
	ITestSuite* subSuite = nullptr;
	while (suite->getTestSuite (subTestSuiteIndex++, subSuite, name) == kResultTrue)
	{
		auto* ts = FCast<TestSuite> (subSuite);
		if (ts)
		{
			if (infoStream)
			{
				*infoStream << SEPARATOR;
				*infoStream << "TestSuite : " << ts->getName ().data () << "\n";
				*infoStream << SEPARATOR << "\n";
			}
			runTestSuite (ts, nameFilter);
		}
	}
}
}
} // namespaces
