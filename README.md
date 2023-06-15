# limba
Compile-time control flow obfuscation using MBA (Mixed Boolean-Arithmetic). This project is derived from another project I am working on named ``limbo``, which is why the project files use this name. Keep in mind that this is more of a proof-of-concept rather than something ready to use in production code.

## Description
Control flow between function calls is fairly easy to trace in normal binaries.
<details>
<summary>Pre-obfuscation pictures</summary>
 
![before_xref](/assets/before_xref.png)  
![before_call](/assets/before_call.png)  

</details>

limba will generate boilerplate code that obfuscates the actual jump address using MBA. The MBA rewrite rules and address offset is randomized each compilation to hinder analysis. It is important to note the actual function body is **NOT** obfuscated and is still present in the binary, only function calls will be obfuscated. 
<details>
<summary>Post-obfuscation pictures</summary>

![after_xref](/assets/after_xref.png)  
![after_call](/assets/after_call.png)  
![after_mba](/assets/after_mba.png)  

</details>


## Support
- The project has been developed using Clang(-CL) so this is the recommended compiler, GCC isn't actively supported and may (or may not) work. MSVC is **not** supported as I found no way to make it not emit a direct pointer to the original function
- The codebase requires C++20 to compile. Compile time is a major constraint so I used a some C++20 features that reduced compilation time in my testing. The code can be modified to support C++17 (see ``emulation.h``)

## Usage
The included example uses ``premake`` for project configuration. You can generate the project files by running ``premake5 vs2022`` in the root folder. Alternatively, the relevant header files can be copied into your own project.

Sample usage:
```cpp
LIMBO_OBFUSCATED_FUNC(<ret_type>, <function_name>, <func_params>)
{
	// function body

	return <ret_value>
}
```

## Limitations
- Only a small set of MBA rules have been included
- Opaque predicates are needed to avoid trivial optimization
- Since a protected function will only have one "wrapper" function, it's easy to figure out and patch the protection with dynamic analysis and scripting
- Compilation time may be negatively impacted if the protection is used on a large number of functions

## Acknowledgements
Great open-source projects that were a great help during development
- https://github.com/JustasMasiulis/xorstr
- https://github.com/llxiaoyuan/oxorany
- https://github.com/Deniskore/nand_nor
