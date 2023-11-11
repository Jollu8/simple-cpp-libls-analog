# Variant

Simple `boost::any` analog (C++11).

Short example:

```c++

Variant v;
v = 10;
if (v.isType<int>()){
std::cout << v.value<int>() << std::endl;
}
```

Variant supports compare operations if it supported by holded value. So it can be used in std::set like containers.

(c) 2022 Emil Jollu