#main Main
cdef Main
cmod public
cbegin
    mdef main
    mmod public static
    mparam [Lvoid.lang.String
    mbegin
        #link index 0
        ipush 0
        istore index

        :begin
            iload index
            ipush 10
            ifieq end

            print "num > "
            iload index
            iprintln

            iload index
            iinc
            istore index
            goto begin
        :end
    mend
cend