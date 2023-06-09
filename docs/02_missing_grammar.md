# Grammar that's current missing from the compiler

```text
# top level variables
<T_DEPS> |== <DEF_VAR>


# controls
<DEFS> |== <IF> "(" <COND> ")" <BLOCK>
<DEFS> |== <FOR> "(" <COND> ")" <BLOCK>

# data
<E> |== <E> > <E>
<E> |== <E> + <T>
<E> |== <E> - <T>
<E> |== <T>

<T> |== <T> * <F>
<T> |== <T> / <F>

<T> |== <F>
<F> |== - <F>

<F> |== <X>

```
