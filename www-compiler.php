<?php
if(array_key_exists("utofile", $_FILES))
{
	if(substr($_FILES["utofile"]["name"], -4) != ".uto")
	{
		die("file name must end on .uto\n");
	}
	if(defined("PHP_WINDOWS_VERSION_MAJOR"))
	{
		shell_exec("utopia-compiler.exe ".escapeshellarg($_FILES["utofile"]["tmp_name"]));
	}
	else
	{
		shell_exec("./utopia-compiler ".escapeshellarg($_FILES["utofile"]["tmp_name"]));
	}
	shell_exec("php compile-utopia.php compiled ".escapeshellarg($_FILES["utofile"]["tmp_name"]));
	$ext = "";
	if(defined("PHP_WINDOWS_VERSION_MAJOR"))
	{
		$ext = ".exe";
	}
	header("Content-Type: application/octet-stream");
	header("Content-Transfer-Encoding: Binary");
	header("Content-disposition: attachment; filename=\"".substr($_FILES["utofile"]["name"], 0, -4)."$ext\"");
	readfile($_FILES["utofile"]["tmp_name"].$ext);
	if($ext)
	{
		unlink($_FILES["utofile"]["tmp_name"].$ext);
	}
	exit;
}
?>
<p>www-compiler ready to native-compile your uto-files on <?=PHP_OS;?></p>
<form method="POST" action="www-compiler.php" enctype="multipart/form-data">
	<input type="file" name="utofile" />
	<input type="submit" />
</form>
