# limba
Compile-time control flow obfuscation using MBA (Mixed Boolean-Arithmetic). This project is derived from another project I am working on named ``limbo``, which is why the project files use this name.

## Description
Control flow between function calls is fairly easy to trace in normal binaries.
<details>
<summary>Pre-obfuscation pictures</summary>
 
![image](https://github.com/ThatLing/limba/assets/24573384/edeed1ee-b6cf-43d4-8f18-651e1194d9db)  
![image](https://github.com/ThatLing/limba/assets/24573384/fe62c45a-1a0f-4d3e-8dcc-4113f9c2b5eb)  

</details>

limba will generate boilerplate code that obfuscates the actual jump address using MBA. The MBA rewrite rules and address offset is randomized each compilation to hinder analysis. It is important to note the actual function body is **NOT** obfuscated and is still present in the binary, only function calls will be obfuscated. 
<details>
<summary>Post-obfuscation pictures</summary>

![image](https://github.com/ThatLing/limba/assets/24573384/a8269a93-f61a-4e7a-a8b7-9c6fc2a75747)  
![image](https://github.com/ThatLing/limba/assets/24573384/796d3d9a-36c9-4151-ae8c-3a9798ab346e)  
![image](https://github.com/ThatLing/limba/assets/24573384/3ad4b009-e145-49c0-8fa2-0e11f7af1b23)  

</details>


## Support
- The currently supported compilers are Clang and GCC. MSVC is not supported as I found no way to make it not emit a direct pointer to the original function
- The codebase requires C++20 to compile, this was done to decrease compile times. The code can be modified to support C++17 (see ``emulation.h``)

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
- Since a protected function will only have one "wrapper" function, it's easy to figure out and patch the protection with dynamic analysis

## Acknowledgements
- https://github.com/JustasMasiulis/xorstr
- https://github.com/llxiaoyuan/oxorany
- https://github.com/Deniskore/nand_nor
