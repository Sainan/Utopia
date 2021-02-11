<?php
$clang = "clang -std=c++17 -fno-rtti -flto -Ofast";

$mode = $argv[1] ?? "interpreter";
$out = $argv[2] ?? "utopia-{$mode}";
$main_only = isset($argv[2]);

if(!is_dir("obj"))
{
	mkdir("obj");
}
$objects = [];

if(defined("PHP_WINDOWS_VERSION_MAJOR")) 
{
	$out .= ".exe";
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
passthru("$link -o $out ".join(" ", $objects));

echo "Got binary in ".(microtime(true) - $start)." seconds.".PHP_EOL;

echo "Stripping...".PHP_EOL;
passthru("strip -s $out");
