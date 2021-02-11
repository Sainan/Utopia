# Utopia

*The perfect language that doesn't exist.*

For me, the perfect language can be used to cross-compile programs written in itself to independent native binaries for all platforms, all while supporting thousands of built-in APIs for every purpose, and as such can be used to create any type of software.

Well, that would be a lot of effort, to say the least. Let's go back to the real world.

A good compromise between interpreted and compiled can be made by parsing source files into intermediary "opcodes", which can then either be interpreted or compiled... or, you could take the interpreter, hard-code your program's opcodes, and compile that.

So, this is my proof-of-concept of exactly that.

## Usage

### Interpreter

If you have PHP-CLI and clang installed, you can compile the interpreter with:

> `php compile-utopia.php interpreter`

And then use it to interpret any ul-file:

- Windows: `utopia-interpreter.exe <ul-file>`
- Linux: `./utopia-interpreter <ul-file>`

You may provide `examples/hello.ul` in place of `<ul-file>`.

### Compiler

If you have PHP-CLI and clang installed, you can compile a ul-file like so:

> `php compile-ul.php <ul-file>`

You may provide `examples/hello.ul` in place of `<ul-file>`.

This will create an executable binary file that is native to your system with the same name as the ul-file.

## Conclusion

This works surprisingly better than expected. Clang is such a chad, I think it pretty much optimized the interpreter away from the lazy "compiling the interpreter to get a compiled binary" approach. These are the file sizes I saw on Linux:

```
Interpreter          34,8 KB
Compiler             30,8 KB
Compiled hello.ul    22,8 KB
```

And as the interpreter and compiler increase in size, the "compiled hello.ul" stays identical!

In theory, this now only needs devised a syntax that is somehow consistent, explicit, and minimal all at the same time, be implemented along with thousands of built-in APIs, and finally be used in the real world.

That is still no easy task, but at least it seems achievable in comparison to my idea of "a truly perfect language," and with how good clang is at optimizing, maybe this "truly perfect language" isn't even necessary, and maybe my frustration with existing languages and their tooling will make me actually take Utopia beyond proof-of-concept.
