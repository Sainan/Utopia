<?php
// On Linux, the libutopia.so needs to be in /usr/lib (or another folder in the search path of the dynamic linker).
$libutopia = \FFI::cdef(str_replace("IMPORT ", "", file_get_contents("src/libutopia.h")), defined("PHP_WINDOWS_VERSION_MAJOR") ? "utopia-lib.dll" : "libutopia.so");
$str = $libutopia->Utopia_string_alloc();
$p = $libutopia->Utopia_Program_alloc();
$libutopia->Utopia_Program_redirectOutputToString($p, $str);
$libutopia->Utopia_Program_fromFile($p, "examples/calc.uto");
$libutopia->Utopia_Program_execute($p);
$libutopia->Utopia_Program_free($p);
echo $libutopia->Utopia_string_read($str);
$libutopia->Utopia_string_free($str);
