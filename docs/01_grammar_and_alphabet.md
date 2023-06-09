# Toy grammar and alphabet

```text
G |== <T_DEPS>

# constant top-level commands
<T_DEPS> |== <T_DEPS> <T_DEPS>
<T_DEPS> |== <T_DEPS>

<T_DEPS> |== <DEF_FN>
<T_DEPS> |== <DEF_VAR>

# functions

<DEF_FN> |== <DEF> <ID> <PARAMS> <BLOCK>
<DEF_VAR> |== <DEF> <ID> "=" <VALUE> ";"

<DEF> |== def
<ID> |== [a-zA-Z][a-zA-Z0-9]*

<PARAMS> |== "(" ")"
<PARAMS> |== "(" <PARAM> ")"
<PARAM> |== <TEXT> ","
<PARAM> |== <TEXT>

<BLOCK> |== "{" <DEFS> ";" "}"
<BLOCK> |== "{" <DEFS> ";"<RETURN> <VALUE> ";" "}"
<RETURN> |== return

<DEFS> |== <DEFS>";"<DEFS>
<DEFS> |== <DEFS>";"
<DEFS> |== <DEF> <ID> "=" <VALUE>";"

<VALUE> |== <E>
<VALUE> |== """<TEXT>"""


# controls
<DEFS> |== <IF> "(" <COND> ")" <BLOCK>
<IF> |== if

<DEFS> |== <FOR> "(" <COND> ")" <BLOCK>
<IF> |== for

<COND> |== <E>

# expressions
<TEXT> |== [a-zA-Z0-9]+

<E> |== <E> > <E>
<E> |== <E> + <T>
<E> |== <E> - <T>
<E> |== <T>
<T> |== <T> * <F>
<T> |== <T> / <F>
<T> |== <F>
<F> |== - <F>
<F> |== <X>

<X> |== [0-9]^+.([0-9]^+)
<X> |== (<E>)
```
