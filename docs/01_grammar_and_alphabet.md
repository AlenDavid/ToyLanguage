# Toy grammar and alphabet

```text

G |== <DEFS>


# commands
<DEFS> |== <DEFS>";"<DEFS>
<DEFS> |== <DEFS>";"
<DEFS> |== <DEF> <ID> "=" <VALUE>";"
<DEFS> |== <DEF> <ID> <PARAMS> <BLOCK>";"

<DEF> |== def
<ID> |== <TEXT>
<VALUE> |== <E>
<VALUE> |== """<TEXT>"""

<PARAMS> |== "(" ")"
<PARAMS> |== "(" <PARAM> ")"
<PARAM> |== <TEXT> ","
<PARAM> |== <TEXT>

<BLOCK> |== "{" <DEFS> "}"";"

# controls
<DEFS> |== <IF> <COND> <BLOCK> ";"
<IF> |== if

<DEFS> |== <FOR> <COND> <BLOCK> ";"
<IF> |== for

<COND> |== <E> > <E>

# expressions
<TEXT> |== [a-zA-Z0-9]+

<E> |== <E> + <T>
<E> |== <E> - <T>
<E> |== <T>
<T> |== <T> * <F>
<T> |== <T> / <F>
<T> |== <T>\%<F>
<T> |== <F>
<F> |== -<F>
<F> |== <X>

<X> |== [0-9]^+.([0-9]^+)
<X> |== [0-9]^+.([0-9]^+)
<X> |== (<E>)
```
