;no operands (with or without space or tab)
.dh
.asciz 
.entry	

;unrecognized
.dd $2
command $2

;insructions
.asciz abc123
.asciz "abc123
.db a
.db 128
.db -129
.dh 32768
.dh -32769
.dw 2147483648
.dw -2147483649
.db ,1
.db 2,
.db 1,,2
.db ++1

;labels
LABEL: 
LABEL$: add 
1LABEL: add
LABELLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL: add
add:
label: add
label: add

