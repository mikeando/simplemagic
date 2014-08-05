# Simple Magic for C
## Oh my God, its full of magic.

## What is SimpleMagic
SimpleMagic is a simple library to help add run time checks to 
polymorphic C code.

Its best illustrated with a simple example:

~~~{.c}
typedef struct MyStruct {
  SMC_ADD_MAGIC();
  ...
};

SMC_MAGIC(MyStruct, 0xcdf7d4ef2fbb1292UL);

void use_struct_from_void(void* data) {
  MyStruct * value = smc_check_type(MyStruct, data);
  ...
}

int main() {
  MyStruct s;
  smc_init_magic(MyStruct, &s);
  use_struct_from_void(&s);
}

~~~

## What SimpleMagic isn't
SimpleMagic is not designed to be anything more than a last minute debugging
check during development.  Its based on the idea that you'd like your function
to fail early if it gets passed the wrong type - not that you want to handle
different types in different ways.

Due to the way that it handles magic without putting restrictions on
the structure of your structs, it is not usable for type switching.


## How does it work?

`SMC_ADD_MACIC` adds the single field required for operation.

~~~
#define SMC_ADD_MAGIC uint64_t smc__magic;
~~~

`SMC_MAGIC(X,V)` expands to `static const uint64_t smc__magic__X = V

And finally `smc_check_type(T,V)` becomes semantically.

~~~
{
  T* temp = (T*)V;
  assert(temp!=NULL && temp->smc__magic == smc__magic__T);
  return temp;
}
~~~

## Turning it off in release builds

Disabeling it is easy. Simply define `SMC_NO_MAGIC` in your build.

However - objects built with `SMC_NO_MAGIC` will have different layout from 
those built without `SMC_NO_MAGIC`, as this flag completely removes the
`smc__magic` member defined by `SMC_ADD_MAGIC`, as well as removing the
constants defined by `SMC_MAGIC` and removing calls to `smc_magic_init` 
and `smc_check_type`.

So you can instead define `SMC_NO_MAGIC_CHECKS` which just disables
calls to `smc_check_type` - leaving the object layout unchanged.

## Undefined behaviour?

Yes, SimpleMagic can in many cases invoke undefined behaviour.  It may do this
in the case where the cast used in the check is incorrect, and the `smc__magic`
field in the target type lies at least partially outside the actual input
struct. The usual result would be a segmentation fault type run time error -
which is likely what you would have recieved later since you were going to
perform a cast to the incorrect type anyway. you can reduce your chance
of getting a segmentation fault and instead getting a proper assertion error by
placing the magic towards the front of your structs, ideally as the first member.

In my polymorphic C code I typically have a loyout for structs that looks somewhat like
this

~~~
struct MyStruct {
  SMC_ADD_MAGIC();
  MyStruct_vtable * vtable;
  void * user_data
};
~~~

