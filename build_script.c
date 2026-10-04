/*
File:   build_script.c
Author: Taylor Robbins
Date:   10\04\2026
Description: 
	** This is a C build script that depends on PigBuild and holds
	** the logic to build the main program into a "build" folder
	** in the repository root. Run build.sh or build.bat to compile
	** this program into a "builder" executable and run it to then
	** compile the main program.
	**
	** NOTE: Both shell scripts will try to find a "pig_build" folder
	**       living next to this repository. If it's not found they
	**       will git clone PigBuild from github into a new
	**       "pig_build" folder. To prevent this you can download
	**       PigBuild into this repos folder as a "pig_build" subfolder
	**       and that will be used instead.
*/

#include "pig_build.h"

#define MAIN_C_PATH          "[ROOT]/src/main.c"
#define WASM_OUTPUT_FILENAME "main.wasm"
#define C_SCRIPTING_FOLDER   "[ROOT]/c_scripting"
#define PIG_CORE_FOLDER      "[ROOT]/core"

Str DownloadCScriptingIfNeeded();
Str DownloadPigCoreIfNeeded();

int main()
{
	PigBuildDebugMode = false;
	RecompileIfNeeded(StrArray_Empty);
	
	Str cScriptingFolder = DownloadCScriptingIfNeeded();
	Str pigCoreFolder = DownloadPigCoreIfNeeded();
	
	WriteLine("Building " WASM_OUTPUT_FILENAME "...");
	
	CliArgs compileArgs = EMPTY;
	AddFullFilePathsArg(&compileArgs);
	AddOptimizationLevelArgNt(&compileArgs, "0");
	AddArg(&compileArgs, CLANG_DEBUG_INFO_DEFAULT);
	AddIncludeDirArgLit(&compileArgs, "[ROOT]/src");
	AddIncludeDirArgLit(&compileArgs, C_SCRIPTING_FOLDER "/symbol_set");
	AddIncludeDirArgLit(&compileArgs, PIG_CORE_FOLDER "/src");
	// AddWarningLevelArgNt(&compileArgs, "all");
	AddArgNt(&compileArgs, CLANG_M_FLAG, "bulk-memory");
	AddArgNt(&compileArgs, CLANG_TARGET_ARCHITECTURE, "wasm32");
	AddArgNt(&compileArgs, CLANG_INCLUDE_DIR, "[ROOT]/core/src/wasm/std/include");
	AddArg(&compileArgs,   CLANG_NO_ENTRYPOINT);
	AddArg(&compileArgs,   CLANG_ALLOW_UNDEFINED);
	AddArg(&compileArgs,   CLANG_NO_STD_LIBRARIES);
	AddArg(&compileArgs,   CLANG_NO_STD_INCLUDES);
	AddArgNt(&compileArgs, CLANG_EXPORT_SYMBOL, "__heap_base");
	AddArgNt(&compileArgs, CLANG_LANGUAGE, "c");
	
	AddArgNt(&compileArgs, CLI_QUOTED_ARG, MAIN_C_PATH);
	AddArgNt(&compileArgs, CLI_QUOTED_ARG, "[ROOT]/src/commands1.c");
	AddArgNt(&compileArgs, CLI_QUOTED_ARG, "[ROOT]/src/commands2.c");
	AddArgNt(&compileArgs, CLANG_OUTPUT_FILE, WASM_OUTPUT_FILENAME);
	
	// AddArg(&compileArgs, CLANG_PRECOMPILE_ONLY);
	// AddArg(&compileArgs, CLANG_PRECOMPILE_EMIT_DEFINES);
	// AddArgNt(&compileArgs, CLANG_OUTPUT_FILE, "preprocessor_macros.txt");
	
	StrArray compileTags = EMPTY;
	AddTag(&compileTags, T_BUILDING_ON_OS);
	AddTag(&compileTags, T_PROGRAM);
	AddTag(&compileTags, T_WASM);
	AddTag(&compileTags, T_CLANG);
	
	RunCliProgramAndExitOnFailureTags(
		StrLit("clang"),
		compileTags,
		&compileArgs,
		StrLit("Failed to compile main.c into main.wasm")
	);
	AssertFileExist(StrLit(WASM_OUTPUT_FILENAME), true);
	
	#if 1
	Str watFilename = ChangePathExtension(StrLit(WASM_OUTPUT_FILENAME), StrLit(".wat"), false);
	PrintLine("Converting " WASM_OUTPUT_FILENAME " to %.*s...", StrPrint(watFilename));
	CliArgs wasm2WatFlags = EMPTY;
	AddArgNt(&wasm2WatFlags, CLI_QUOTED_ARG, WASM_OUTPUT_FILENAME);
	AddArgStr(&wasm2WatFlags, "-o \"[VAL]\"", watFilename);
	RunCliProgramAndExitOnFailure(
		StrLit("wasm2wat"),
		&wasm2WatFlags,
		FormatStr("Failed to convert " WASM_OUTPUT_FILENAME " to %.*s", StrPrint(watFilename))
	);
	#endif
	
	return 0;
}

Str DownloadCScriptingIfNeeded()
{
	//TODO: Can we lock this to a specific version?
	Str cScriptingUrl = StrLit("https://git.mr4th.com/mr4th-public/c-scripting/archive/main.zip");
	Str cScriptingZipPath = StrLit("c_scripting_main.zip");
	Str cScriptingZipRootFolder = StrLit("c-scripting");
	Str cScriptingFolderPath = StrLit(C_SCRIPTING_FOLDER);
	Str cScriptingFolderPath_ResolvedRoot = ResolveRootTo(cScriptingFolderPath, StrLit(".."));
	if (!DoesFileExist(cScriptingZipPath) || !DoesFolderExist(cScriptingFolderPath_ResolvedRoot))
	{
		PrintLine("Downloading C Scripting from \"%.*s\"", StrPrint(cScriptingUrl));
		DownloadAndExtractArchive(
			cScriptingUrl,
			cScriptingZipPath,
			97053, 0x87F78B24B32DA00B,
			cScriptingFolderPath_ResolvedRoot,
			cScriptingZipRootFolder
		);
	}
	
	return cScriptingFolderPath;
}

Str DownloadPigCoreIfNeeded()
{
	//TODO: We should lock ourselves to a specific version, this is going to break the minute we do another commit to master because we do a size and hash check below after downloading the .zip
	Str pigCoreUrl = StrLit("https://github.com/PiggybankStudios/PigCore/archive/refs/heads/master.zip");
	Str pigCoreZipPath = StrLit("pig_core_master.zip");
	Str pigCoreZipRootFolder = StrLit("PigCore-master");
	Str pigCoreFolderPath = StrLit(PIG_CORE_FOLDER);
	Str pigCoreFolderPath_ResolvedRoot = ResolveRootTo(pigCoreFolderPath, StrLit(".."));
	if (!DoesFileExist(pigCoreZipPath) || !DoesFolderExist(pigCoreFolderPath_ResolvedRoot))
	{
		PrintLine("Downloading PigCore from \"%.*s\"", StrPrint(pigCoreUrl));
		DownloadAndExtractArchive(
			pigCoreUrl,
			pigCoreZipPath,
			60888124, 0x624D7E64C72E29BF,
			pigCoreFolderPath_ResolvedRoot,
			pigCoreZipRootFolder
		);
	}
	
	return pigCoreFolderPath;
}
