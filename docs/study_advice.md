+ https://windsting.github.io/a-tour-of-cpp-2nd-cn/ch01.html
    - 面向有后端经验的。旨在为资深程序员提供一个概念，去了解现代C++的主要构成。 它涵盖了绝大多数语言特性及多数标准库组件。 

+ `std::cout << "Hello, World!
"`
操作符<<(“输向”)把它的第二个参数写入第一个。 在本例中，字符串文本(string literal)"Hello, World!
" 被写入到标准输出流std::cout上    
+ `cout << "the square of " << x << " is " << square(x) << "
";`
链式调用
因为 << 返回的还是 ostream&（即 cout 自身的引用）
等价于：
```cpp
cout << "the square of ";
cout << x;
```
