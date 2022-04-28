#main Main
cdef Main
cmod public
cbegin
    mdef main
    mmod public static
    mparam [Lvoid.lang.String
    mbegin
        #link start 0
        #link finish 1

        currentTime
        lstore start

        #link index 2
        ipush 0
        istore index

        :begin
            iload index
            ipush 1000
            ifieq end

            iload index
            iinc
            istore index

            goto begin
        :end
        
        currentTime
        lstore finish

        lload finish
        lload start
        lrem
        lprintln
    mend
cend
