<?php
// Syntax: php compile-utopia.php [interpreter|compiler|lib|wasmlib|test|benchmark]

if(empty($argv))
{
	die("CLI only!");
}

$mode = $argv[1] ?? "interpreter";
$out = $argv[2] ?? "utopia-{$mode}";
$main_only = isset($argv[2]);

$wasm = false;
if($mode == "wasmlib")
{
	$wasm = true;
	$mode = "lib";
}

if(!is_file("src/main_{$mode}.cpp"))
{
	die("Invalid mode: {$mode}".PHP_EOL);
}

$clang = $wasm
	? "em++ -O3 -s DISABLE_EXCEPTION_CATCHING=0 -s WASM=1 -s MODULARIZE=1 -s EXPORT_NAME=libutopia -s EXTRA_EXPORTED_RUNTIME_METHODS=[\"cwrap\"] --post-js src/libutopia-postjs.js"
	: "clang -Ofast";
$clang .= " -std=c++17 -fvisibility=hidden -flto";

if($mode == "test" && defined("PHP_WINDOWS_VERSION_MAJOR"))
{
	$clang .= " -frtti"; // Microsoft should be ashamed for using typeid
}
else
{
	$clang .= " -fno-rtti";
}

if(!is_dir("obj"))
{
	mkdir("obj");
}
$objects = [];

if($wasm)
{
	$out = "libutopia.js";
}
else if($mode == "lib")
{
	if(defined("PHP_WINDOWS_VERSION_MAJOR")) 
	{
		$out .= ".dll";
	}
	else
	{
		$clang .= " -fPIC";
		if($out == "utopia-lib")
		{
			$out = "libutopia";
		}
		$out .= ".so";
	}
}
else
{
	if(defined("PHP_WINDOWS_VERSION_MAJOR")) 
	{
		$out .= ".exe";
	}
}

$files = [];
function populateFiles($dir)
{
	global $mode, $files;
	foreach(scandir($dir) as $file)
	{
		if(substr($file, 0, 1) == ".")
		{
			continue;
		}
		if(is_dir($file))
		{
			populateFiles($dir.$file."/");
		}
		else if(substr($file, -4) == ".cpp" && (substr($file, 0, 5) != "main_" || $file == "main_{$mode}.cpp"))
		{
			array_push($files, substr($file, 0, -4));
		}
	}
}
populateFiles("src/");

$start = microtime(true);

echo "Compiling...".PHP_EOL;
foreach($files as $file)
{
	if(!$main_only || substr($file, 0, 5) == "main_")
	{
		echo $file.PHP_EOL;
		passthru("$clang -c src/$file.cpp -o obj/$file.o");
	}
	array_push($objects, escapeshellarg("obj/$file.o"));
}

echo "Linking...".PHP_EOL;
$link = "$clang -fuse-ld=";
if(defined("PHP_WINDOWS_VERSION_MAJOR"))
{
	$link .= "lld-link";
}
else
{
	if(shell_exec("which lld"))
	{
		$link .= "l";
	}
	$link .= "ld -lstdc++";
}
if($mode == "lib" && !$wasm)
{
	$link .= " -shared";
}
passthru("$link -o $out ".join(" ", $objects));

echo "Got binary in ".(microtime(true) - $start)." seconds.".PHP_EOL;

if(!$wasm)
{
	echo "Stripping...".PHP_EOL;
	if(defined("PHP_WINDOWS_VERSION_MAJOR"))
	{
		passthru("llvm-strip -s $out");
	}
	else
	{
		passthru("strip -s $out");
	}
}
