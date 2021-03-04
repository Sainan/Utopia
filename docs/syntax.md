# Utopia Language Syntax

## Legend

- `<required>`
- `[optional]`

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

- `echo|print <value|variable>`

Future considerations:

- `echo("{}", <value|variable>)`

## Variable declaration

Currently supported:

- `<name> = <value|variable>`

Future considerations:

- Declaration prefices (let, var, auto, string, int, etc.)
- C++ style: `[prefix] <name> { <value> }`
- Disallow changes: `const <name> = "..."`
- Disallow type changes: `strong <name> = "..."`

## String concatenation

Currently supported:

- `<string> <string>`
- `<string-var> + <string-var>`

Future considerations:

- `<string-var> <string-var>`
- `<string-var> . <string-var>`

## Arithmetics

Currently supported:

- `<int-var> + <int-var>`
- `<int-var> - <int-var>`
- `<int-var> * <int-var>`
- `<int-var> / <int-var>`

Future considerations:

- Bitwise AND: `<int> & <int>`
- Bitwise OR: `<int> | <int>`
- Bitwise XOR: `<int> ^ <int>`
- Power: `<int> ^^ <int>`
- Power: `<int> pow <int>`
- `sqrt <int>`

## Functions

Currently supported:

- `<name> = fn { ... }`
- `fn <name> { ... }`
