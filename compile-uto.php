<?php
if(empty($argv[1]) || substr($argv[1], -4) != ".uto")
{
	die("Syntax: php compile-uto.php <uto-file>".PHP_EOL);
}

passthru("php compile-utopia.php compiler");

if(defined("PHP_WINDOWS_VERSION_MAJOR")) 
{
	passthru("utopia-compiler.exe ".escapeshellarg($argv[1]));
}
else
{
	passthru("./utopia-compiler ".escapeshellarg($argv[1]));
}

passthru("php compile-utopia.php compiled ".escapeshellarg(substr($argv[1], 0, -4)));

unlink("src/main_compiled.cpp");
