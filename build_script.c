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
#define EXE_OUTPUT_FILENAME  "main.exe"
#define C_SCRIPTING_FOLDER   "[ROOT]/c_scripting"
#define PIG_CORE_FOLDER      "[ROOT]/core"

#define USE_MSVC  0

Str DownloadCScriptingIfNeeded();
Str DownloadPigCoreIfNeeded();

int main()
{
	PigBuildDebugMode = false;
	RecompileIfNeeded(StrArray_Empty);
	bool isMsvcInitialized = (USE_MSVC ? WasMsvcDevBatchRun() : false);
	
	Str cScriptingFolder = DownloadCScriptingIfNeeded();
	Str pigCoreFolder = DownloadPigCoreIfNeeded();
	
	PrintLine("Building %s...", USE_MSVC ? EXE_OUTPUT_FILENAME : WASM_OUTPUT_FILENAME);
	
	// +==============================+
	// |        Compiler Args         |
	// +==============================+
	CliArgs compileArgs = EMPTY;
	AddNoLogoArg(&compileArgs);
	AddFullFilePathsArg(&compileArgs);
	// AddOptimizationLevelArgNt(&compileArgs, USE_MSVC ? "d" : "0");
	AddOptimizationLevelArgNt(&compileArgs, "2");
	AddTaggedArg(&compileArgs, T_CLANG, CLANG_DEBUG_INFO_DEFAULT);
	AddTaggedArg(&compileArgs, T_MSVC_CL, CL_DEBUG_INFO);
	AddIncludeDirArgLit(&compileArgs, "[ROOT]/src");
	AddIncludeDirArgLit(&compileArgs, C_SCRIPTING_FOLDER "/symbol_set");
	AddIncludeDirArgLit(&compileArgs, PIG_CORE_FOLDER "/src");
	// AddWarningLevelArgNt(&compileArgs, "all");
	AddTaggedArgNt(&compileArgs, T_MSVC_CL, CL_LANG_VERSION, "c11");
	AddTaggedArgNt(&compileArgs, T_MSVC_CL, CL_CONFIGURE_EXCEPTION_HANDLING, "a-");
	AddTaggedArgNt(&compileArgs, T_MSVC_CL, CL_DISABLE_WARNING, "5105");
	AddTaggedArgNt(&compileArgs, T_MSVC_CL, "-G[VAL]", "R-"); //TODO: Make a #define for this?
	AddTaggedArgNt(&compileArgs, T_CLANG T_WASM, CLANG_M_FLAG, "bulk-memory");
	AddTaggedArgNt(&compileArgs, T_CLANG T_WASM, CLANG_TARGET_ARCHITECTURE, "wasm32");
	AddTaggedArgNt(&compileArgs, T_CLANG T_WASM, CLANG_INCLUDE_DIR, "[ROOT]/core/src/wasm/std/include");
	AddTaggedArg(&compileArgs,   T_CLANG T_WASM, CLANG_NO_ENTRYPOINT);
	AddTaggedArg(&compileArgs,   T_CLANG T_WASM, CLANG_ALLOW_UNDEFINED);
	AddTaggedArg(&compileArgs,   T_CLANG T_WASM, CLANG_NO_STD_LIBRARIES);
	AddTaggedArg(&compileArgs,   T_CLANG T_WASM, CLANG_NO_STD_INCLUDES);
	AddTaggedArgNt(&compileArgs, T_CLANG T_WASM, CLANG_EXPORT_SYMBOL, "__heap_base");
	AddTaggedArgNt(&compileArgs, T_CLANG T_WASM, CLANG_LANGUAGE, "c");
	
	AddArgNt(&compileArgs, CLI_QUOTED_ARG, MAIN_C_PATH);
	AddArgNt(&compileArgs, CLI_QUOTED_ARG, "[ROOT]/src/commands1.c");
	AddArgNt(&compileArgs, CLI_QUOTED_ARG, "[ROOT]/src/commands2.c");
	if (USE_MSVC) { AddTaggedArgNt(&compileArgs, T_MSVC_CL, CL_BINARY_FILE, EXE_OUTPUT_FILENAME); }
	else { AddTaggedArgNt(&compileArgs, T_CLANG, CLANG_OUTPUT_FILE, WASM_OUTPUT_FILENAME); }
	
	// AddTaggedArg(&compileArgs,   T_CLANG, CLANG_PRECOMPILE_ONLY);
	// AddTaggedArg(&compileArgs,   T_CLANG, CLANG_PRECOMPILE_EMIT_DEFINES);
	// AddTaggedArgNt(&compileArgs, T_CLANG, CLANG_OUTPUT_FILE, "preprocessor_macros.txt");
	
	// +==============================+
	// |       Linker Arguments       |
	// +==============================+
	AddTaggedArg(&compileArgs, T_MSVC_CL, "/link");
	AddTaggedArg(&compileArgs, T_MSVC_CL, LINK_DISABLE_INCREMENTAL);
	
	// +==============================+
	// |             Tags             |
	// +==============================+
	StrArray compileTags = EMPTY;
	AddTag(&compileTags, T_PROGRAM);
	AddTag(&compileTags, T_LANG_C);
	if (USE_MSVC)
	{
		AddTag(&compileTags, T_MSVC_CL);
		AddTag(&compileTags, T_BUILDING_ON_OS);
	}
	else
	{
		AddTag(&compileTags, T_CLANG);
		AddTag(&compileTags, T_WASM);
	}
	
	// +==============================+
	// |           Compile            |
	// +==============================+
	if (USE_MSVC) { InitializeMsvcIf(StrLit(PIG_BUILD_ROOT), &isMsvcInitialized); }
	RunCliProgramAndExitOnFailureTags(
		USE_MSVC ? StrLit("cl") : StrLit("clang"),
		compileTags,
		&compileArgs,
		FormatStr("Failed to compile main.c into %s", USE_MSVC ? EXE_OUTPUT_FILENAME : WASM_OUTPUT_FILENAME)
	);
	AssertFileExist(USE_MSVC ? StrLit(EXE_OUTPUT_FILENAME) : StrLit(WASM_OUTPUT_FILENAME), true);
	
	#if 1
	if (!USE_MSVC)
	{
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
	}
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
