# Utopia

*The perfect language that doesn't exist.*

For me, the perfect language can be used to cross-compile programs written in itself to independent native binaries for all platforms, all while supporting thousands of built-in APIs for every purpose, and as such can be used to create any type of software.

Well, that would be a lot of effort, to say the least. Let's go back to the real world.

A good compromise between interpreted and compiled can be made by parsing source files into intermediary "opcodes", which can then either be interpreted or compiled... or, you could take the interpreter, hard-code your program's opcodes, and compile that. So, this is my proof-of-concept of exactly that.

## Usage

### Interpreter

If you have PHP-CLI and clang installed, you can compile the interpreter with:

> `php compile-utopia.php interpreter`

And then use it to interpret any uto-file:

- Windows: `utopia-interpreter.exe <uto-file>`
- Linux: `./utopia-interpreter <uto-file>`

You may provide `examples/hello.uto` in place of `<uto-file>`.

### Compiler

If you have PHP-CLI and clang installed, you can compile a uto-file like so:

> `php compile-uto.php <uto-file>`

You may provide `examples/hello.uto` in place of `<uto-file>`.

This will create an executable binary file that is native to your system with the same name as the uto-file.

As for binaries for different systems (cross-compiling), I thought Clang had this sorted, but I wasted like an entire day on that shit, and figured the best solution is just setting up servers to native-compile for you:

- https://winserv.utopia.sh/www-compiler.php
- https://debserv.utopia.sh/www-compiler.php

## Conclusion

At first glance, this works surprisingly better than expected; these are the file sizes I saw on Linux with the initial version:

```
Interpreter           34,8 KB
Compiler              30,8 KB
Compiled hello.uto    22,8 KB
```

Clang could optimize away a lot of redundant code, but unforunately, the interpreter is still present in the binary, and as more opcodes are added, even if they are unused, "compiled binaries" get bigger. However, it might still be possible to help clang understand which opcodes are (un)used at a later time.

In fact, it might be possible to introduce compilation without clang and many other things "at a later time," however, I don't even think interpreted languages are necessarily imperfect, but being able to interpret **and** compile Utopia code is a nice bonus, and I think with that, the leitmotif of "the perfect language" in my mind is obvious: "it works the way you want it to."

Of course, devising a syntax and implementing it along with thousands of built-in APIs all while following that motto is going to be very challenging, but I don't see my frustration with existing languages and their tooling going away anytime soon, so, "hold my beer." ;D
