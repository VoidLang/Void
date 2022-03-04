# The Void Programming Language 
Void is an open-source virtual-machine-based programming language written in C++ <br>
**Note: Void is still under development and is not ready yet for production use.**<br>
Feel free to [report any bugs](https://github.com/VoidLang/Void/issues) you've encountered.

## Features
Void is a type safe programming language that compiles to bytecode.<br>
There is no need to directly include your dependencies' source code, you can just <br>include the header files Void can generate for you.

## Get started

**Installation**<br>
Firstly, you will need to install Void on your platform.
``` 
soon
```

**A HelloWorld program**<br>
This is a simple example for getting started.
```java
public class Program {
    public static void main(String[] args) {
        Console.println("Hello, World!");
    }
}
```

**Compiling the project**<br>
Compile the source code to Void executable.
```
void -compile <project folder> -out <output file>
```

**Executing the program**<br>
Run the compiled Void executable.
```
void -run <void executable> [program arguments]
```

## Documentation
For further information you may read the following pages.
- [Syntax](Syntax.md)
- [Types](Types.md)
- [Modifiers](Modifiers.md)
- [Headers](Headers.md)
- [Naming conventions](Conventions.md)
- [Bytecode](Bytecode.md)
- [Unsafe actions](Unsafe.md)
- [External libraries](Libraries.md)

## Getting help
If you need help, feel free to join our official [discord server](https://discord.gg/HdgnHqqGAW).

## Contribution
If you are interested in contributing to the Void project, please read through the [contribution](Contribution.md) page.
