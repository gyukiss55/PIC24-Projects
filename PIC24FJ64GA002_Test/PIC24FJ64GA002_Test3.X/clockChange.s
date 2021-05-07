;Place the new oscillator selection in W0
;OSCCONH (high byte) Unlock Sequence
MOV #OSCCONH,w1
MOV #0x78, w2
MOV #0x9A, w3
MOV.b w2, [w1]
MOV.b w3, [w1]
;Set new oscillator selection
MOV.b WREG, OSCCONH
;OSCCONL (low byte) unlock sequence
MOV #OSCCONL,w1
MOV.b #0x01, w0
MOV #0x46, w2
MOV #0x57, w3
MOV.b w2, [w1]
MOV.b w3, [w1]
;Start oscillator switch operation
MOV.b w0, [w1]


