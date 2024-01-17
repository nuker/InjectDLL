#  A simple DLL injector

**Through out 2023 I learned various ways to inject my payloads into remote processes I took it upon myself to write a simple DLL injector that I can use for future research**

- **If one takes away anything from this repo it should be how you allocate memory for your payload inside of the process**

**Inside of the load method, near the end I first allocate my memory region as PAGE_READWRITE (RW) after I've written my payload to memory region I allocated, I THEN set the memory page permissions to PAGE_EXECUTE_READ (RX)**
**This is done so that our payload is not RWX. Allocating memory with RWX permissions inside of a process can raise suspicion, ultimately leading to your payload being found**

 **I will be making a better one**

# Compile with visual studio
