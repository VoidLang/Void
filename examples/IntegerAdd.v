#main Main
cdef Main
cmod public
cbegin
    mdef main
    mmod public static
    mparam [Lvoid.lang.String
    mbegin
        ipush 10
        ipush 3
        invokestatic Main sum I I
        iprintln
    mend

    mdef sum
    mmod public static
    mparam I I
    mreturn I
    mbegin
        iload 0
        iload 1
        iadd
        ireturn
    mend
cend