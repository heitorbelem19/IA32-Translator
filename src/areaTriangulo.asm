triangulo: equ 1
SECTION TEXT    ;linha dos dados

        LOAD        B
        MuL        H
        IF TRIANGULO
        DIV        DOIS
        STORE        R
        OUTPUT    R
        STOP
SECTION DATA
    B:    SPACE
    H:    SPACE
    R:    SPACE
    DOIS:    CONST    2