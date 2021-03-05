if("cwrap" in libutopia)
{
	let cwrap=(name,ret,args)=>libutopia[name.substr(7)]=libutopia.cwrap(name, ret, args)

	cwrap("Utopia_string_alloc", "number", []);
	cwrap("Utopia_string_read", "string", ["number"]);
	cwrap("Utopia_string_free", "void", ["number"]);

	cwrap("Utopia_Program_alloc", "number", []);
	cwrap("Utopia_Program_fromFile", "void", ["number", "string"]);
	cwrap("Utopia_Program_fromString", "void", ["number", "string", "string"]);
	cwrap("Utopia_Program_redirectOutput", "void", ["number", "function", "number"]);
	cwrap("Utopia_Program_redirectOutputToString", "void", ["number", "number"]);
	cwrap("Utopia_Program_redirectWarnings", "void", ["number", "function", "number"]);
	cwrap("Utopia_Program_redirectWarningsToString", "void", ["number", "function", "number"]);
	cwrap("Utopia_Program_execute", "void", ["number"]);
	cwrap("Utopia_Program_free", "void", ["number"]);

	cwrap("Utopia_SourceLocation_toString", "number", ["number"]);
	cwrap("Utopia_SourceLocation_file", "string", ["number"]);
	cwrap("Utopia_SourceLocation_line", "number", ["number"]);
	cwrap("Utopia_SourceLocation_character", "number", ["number"]);

	cwrap("Utopia_Warning_toString", "number", ["number"]);
	cwrap("Utopia_Warning_what", "number", ["number"]);
	cwrap("Utopia_Warning_where", "number", ["number"]);

	delete libutopia.cwrap;
}
