# Utopia Language Syntax

## Legend

How brackets are used:

- `<required>`
- `[optional]`

Types:

- `string`
- `int` (is string-castable)

Terms:

- `<type>-rval`: The value is non-variable
- `value`: Variable name or rval of any type
- `<type>-castable`: Of the type itself or of a type that can be converted into it

---

## Comments

```
// I'm a comment

# I'm a comment

/*
   I'm a
  very big
   comment
           */
```

## Strings

```
"Hello, world!"

CRLF|Hello,
    |world!

LF|Hello,
  |world!

CR|Hello,
  |world!
```

## Output

Currently supported:

- `echo|print <value>`

Future considerations:

- `echo("{}", <value>)`

## Variable declaration

Currently supported:

- `<name> = <value>`

Future considerations:

- Declaration prefices (let, var, auto, string, int, etc.)
- C++ style: `[prefix] <name> { <value> }`
- Disallow changes: `const <name> = <value>`
- Disallow type changes: `strong <name> = <value>`

## String concatenation

Currently supported:

- `<string> <string-rval>`
- `<string> + <string-castable>` or `<string-castable> + <string>`

Future considerations:

- `<string> <string>`
- `<string-castable> . <string-castable>`

## Arithmetics

Currently supported:

- `<int> + <int>`
- `<int> - <int>`
- `<int> * <int>`
- `<int> / <int>`

Future considerations:

- Bitwise AND: `<int> & <int>`
- Bitwise OR: `<int> | <int>`
- Bitwise XOR: `<int> ^ <int>`
- Power: `<int> ^^ <int>`
- Power: `<int> pow <int>`
- `sqrt <int>`

## Functions

Currently supported:

- `<name> = fn { [code] }`
- `fn <name> { [code] }`
