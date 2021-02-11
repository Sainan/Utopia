<?php
if(empty($argv[1]) || substr($argv[1], -3) != ".ul")
{
	die("Syntax: php compile-ul.php <ul-file>".PHP_EOL);
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

passthru("php compile-utopia.php compiled ".escapeshellarg(substr($argv[1], 0, -3)));

unlink("src/main_compiled.cpp");
