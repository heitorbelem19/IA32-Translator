SECTION TEXT    ;linha dos dados
        LOAD        B
        MuL        H
        DIV        DOIS
        STORE        R
        STOP
SECTION DATA
    B:    const 10
    H:    const 4
    R:    SPACE 5 
    DOIS:    CONST    2