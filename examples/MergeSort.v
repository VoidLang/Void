#main Main
cdef Main
cbegin
    mdef main
    mmod static
    mparam [Lvoid.lang.String
    mbegin
        ; create an array to perform the sorting algorithm on
        #link array 1
        invokestatic Main createArray
        astore array

        println "--- original ---"
        aload array
        invokestatic Main debugArray [I

        aload array
        invokestatic Main shuffle [I

        println "--- shuffled ---"
        aload array
        invokestatic Main debugArray [I

        println "--- sorted ---"
    mend

    ; create the array to demonstrate the merge sorting algorithm with
    mdef createArray
    mmod static
    mreturn [I
    mbegin
        ; create the array
        #link array 0
        ipush 5
        newarray I
        astore array

        ; fill up the array with the numbers [7, 11, 13, 17, 21]
        aload array
        ipush 0
        ipush 7
        iastore

        aload array
        ipush 1
        ipush 11
        iastore

        aload array
        ipush 2
        ipush 13
        iastore

        aload array
        ipush 3
        ipush 17
        iastore

        aload array
        ipush 4
        ipush 21
        iastore

        ; array has been made
        aload array
        areturn
    mend

    ; shuffle the elements of the array using the Fisher-Yates algorithm
    mdef shuffle
    mmod static
    mparam [I
    mbegin
        #link array 0

        ; set the index to n-1
        #link index 1
        aload array
        arraylength
        idecr
        istore index

        ; set up the random generation seed
        currentTime
        seed

        ; setup a reverse loop from n-1 to 1
        :begin
            #link number 2
            ; generate a random number with 0 <= number <= index
            ; convert the index to a float in oder to make it multiplyable with the random value
            iload index
            i2f
            random
            fmul
            f2i
            istore number

            #link backup 3
            ; exchange array[number] and array[index]
            ; backup = array[number]
            ; array[number] = array[index]
            ; array[index] = backup
            
            ; store array[number] to backup
            aload array
            iload number
            iaload
            istore backup

            ; set array[number] = array[index]
            #link temp 4
            aload array
            iload index
            iaload
            istore temp

            aload array
            iload number
            iload temp
            iastore

            ; set array[index] = backup
            aload array
            iload index
            iload backup
            iastore

            ; decrement the index
            iload index
            idecr
            istore index

            ; exit the loop if the index is less than 1
            iload index
            ipush 1
            ifi< end

            ; continue loop
            goto begin
        :end
    mend

    ; perform the merge sorting algorithm
    mdef mergeSort
    mmod static
    mparam [I
    mbegin
        #link array 0

        ; make sure the array has more than 1 elements
        aload array
        arraylength
        ipush 1
        ifi> check_end
        return
        :check_end
    mend

    ; debug the content of the array
    mdef debugArray
    mmod static
    mparam [I
    mbegin
        #link array 0

        ; setup index
        #link index 1
        ipush 0
        istore index

        print "Numbers: "

        ; loop through the elements of the array
        :begin
            ; exit the loop if the index has exceeded the array length
            iload index
            aload array ; get the length of the array
            arraylength
            ifi>= end ; check if index >= length

            ; print the element of the array at the current index
            aload array
            iload index
            iaload
            iprint

            ; print a comma if the array has more elements
            iload index
            iinc
            aload array
            arraylength
            ifi< print_comma
            goto print_end
            :print_comma
                print ", "
            :print_end

            ; increment the index
            iload index
            iinc
            istore index

            ; do it again
            goto begin    
        :end

        println "."
    mend
cend