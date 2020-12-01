T: EQU 1
SECTION TEXT ;linha dos dados
        LOAD        B
        MuL        H
        IF T
        JMP L1
        DIV        DOIS
        STORE        R
L1: STOP

SECTION DATA
    B:    const 10
    H:    const 4
    R:    SPACE 5 
    DOIS:    CONST    2