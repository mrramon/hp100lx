if exist anldiv.obj goto gotdiv
lib c:\c\lib\slibce *anldiv *anlmul *anlrem;
:gotdiv
nmk OPTS=real
e2m hexcalc
