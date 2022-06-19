#main Main
cdef Main
cmod public
cbegin
    mdef main
    mmod public static
    mparam [Lvoid.lang.String
    mbegin
        ; todo support start args

        #link command 0

        :begin
            input "> "
            astore command

            aload command
            spush "help"
            ifseq help

            aload command
            spush "version"
            ifseq version

            aload command
            spush "quit"
            ifseq quit

            goto begin         

            :help
                println "Commands: help, version, quit"
                goto begin

            :version
                println "1.0.1"
                goto begin

            :quit
                println "Goodbye"
                return
    mend
cend