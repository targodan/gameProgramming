# GameDevelopment

## The Coordinate System

North is the negative Z-Axis, west is the negative X-Axis.

![CoordinateSystem](https://www.corbatto.de/coordinates.svg)

## Code Convention - **draft**

The dos and don'ts of this project.

### Language

Everythin, including names and comments should be in english.

### Whitespaces and linebreaks

Indentation is done with 4 spaces.
Tabs are not permitted under any circumstances.

Between a function name and the brackets for the parameters should never be a space, same applies to if, while, for etc. calls. E. g. `if(cond)` `func(param)`.

Parameters are separated by a comma. There should always be a space after, but never before the comma. E. g. `func(param0, param1)`.

Newlines are always LF, not CRLF (`"\n"`, not `"\r\n"` like windows).

The opening curly bracket should always be on the same line as the function/for-loop/if-statement etc. they belong to.
Between the closing bracket and the opening curly bracket should be one space.
`else` statements should be in the same line as the brackets.

```cpp
void test() {
    // ...
}

if(cond) {
    // ...
} else {
    // ...
}

class Test {
    // ...
};
```

If a line gets too long due to number of parameters put each parameter in it's own line and indent it 2 times (8 spaces). The closing `);` should be indented once.

```cpp
thisIsAVeryLongFunction(
        param0,
        param1,
        param2,
        ...,
        paramN
    );
```

Long terms should look like this.

```cpp
((a + b * c) / d);
// Use extra brackets where the operator priority is not absolutely clear.
(a || b) && (b || c);
(a && b) || (b && c);
```

When declaring variables or members avoid using "," as it leads to ambiguities.
The "\*" and "&" for pointers/references belong to the type, so do it like this.

```cpp
int* intptr;
int* anotherptr;
int& ref = *intptr;
```

### Always use curly brackets!

Under (almost) no circumstances is any statement like `if` or `else` to be used without curly brackets.

```cpp
// Don't leave out these curly-brackets!
if(cond) {
    // single line statement
}
```

If you need if-else-cascades do them like this.
This is **absolutely** the only case where leaving out the curly brackets is allowed.

```cpp
if(cond1) {
    // ...
} else if(cond2) {
    // ...
} else if(cond3) {
    // ...
}
```

### Naming Convention

All names are done in camel-case `getSomeStuff` instead of ~~`get_some_stuff`~~.

Class names start with a capital letter, function names, variable names and namespaces start with a lower letter. `class Vector` but `void getX()`.

Variable names are *not* to be prefixed like this ~~`iNumber`, `pVec` or `m_member`~~.
Just call them reasonably like `number`, `vec` or `member`.

Inside a class alsways use the `this` pointer to access members.

```cpp
int getSomeInt() {
    return this->someInt;
}
```

Except for very rare exceptions member variable should be private or protected and have getters and setters if they should be accessible from outside of the class.
Setters should always start with `set` followed by the capitalized member name.
Getters should mostly start with `get` followed by the capitalized member name.
If the member is a boolean and it makes semantical sense the getter may start with `is` or `has` instead. E. g. `bool isLarge()` or `bool hasEyes()`.

Don't shy away from namespaces.
Our engine classes and functions will be in theire own namespace.
Namespaces can and should be deeper than one level.
If the depth gets too cracy, the indentation may be altered as seen in ["Files and folders"](#Files-and-folders) below.

```cpp
namespace engine {
    class EntityManager {
        // ...
    };
}
```

Enums should be in theire own namespaces in order to avoid conflicts.
In this case the namespace should be upper case and the actual enum type should be called `T`.

```cpp
namespace MsgType {
    // Ask me if you don't understand this way of declaring a type. ;)
    typedef enum {
        Type1,
        Type2
    } T;
}

// ...

MsgType::T var = MsgType::Type1;
```

### Files and folders

Each namespace (except our root namespace "engine") should have its own folder with the same name as the namespace.
Header files should only contain declarations, not definitions.
Only very small inline functions or templated classes and functions may be defined in header files.

Header files end on ".h", body files end on ".cpp".

Each class should be in its own header and body file of the same name.
See example:

```cpp
// file "engine/foo/bar/FooBar.h"

#ifndef FOOBAR_H
#define FOOBAR_H

namespace engine {
namespace foo {
namespace bar {
    class FooBar {
        void foo();
    };
}
}
}

#endif
```

```cpp
// file "engine/foo/bar/foobar.cpp"

#include "foobar.h"

// DONT'T use "using namespace" here, this is prone to conflicts!
namespace engine {
namespace foo {
namespace bar {
    void FooBar::foo() {
        // ...
    }
}
}
}
```

The first two lines and the last line of a header file always have to look like this:

```cpp
#ifndef FILENAME_H
#define FILENAME_H
// ...
#endif
```

### Important C++ keywords

Always use the `const` keyword where applicable and avoid call by value on large arguments.
If you need a crash course on all the uses of the `const` keyword just ask me. :wink:

```cpp
class Vector {
    // ...

    // const here!
    float getX() const {
        return x;
    }

    // member not const but arg is const ref in order to avoid copying.
    void add(const Vector& v) {
        // ...
    }
};
```

Please make use of the `override` keyword, it really helps detect errors early.

```cpp
class A {
    // To be overridden
    void someMethod() const = 0;
};

class B : public A {
    void someMethod() const override {}
};
```

Please make extensive use of smart pointers.
There are three types `std::unique_ptr`, `std::shared_ptr` and `std::weak_ptr`.
I found the top voted answer [here](http://stackoverflow.com/questions/106508/what-is-a-smart-pointer-and-when-should-i-use-one) to be quite well explained.

### Git Workflow

The master branch should always be clean, i. e. it should at least compile.
That means before merging please test.

Under no circumstances may the master branch be fore-pushed.

If you do work-in-progress stuff please work on your own branch.

```bash
$ git checkout -b tim
```

If you accidentally worked on the master branch and already changed things before checking out your branch here is a little fix for it.

```bash
# Say you already have 2 commits on the master branch.

# First: commit current changes
$ git add -A
$ git commit
# That makes 3 commits

# Go to your branch and update it
$ git checkout yourBranch
$ git rebase master

# Now clean up the master branch
$ git checkout master
# Go back 3 commits.
$ git reset --hard HEAD~3
```

I would advise to pull using the rebase flag.
This is less conflict prone.

```bash
$ git pull --rebase
```
